#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "WarpStar.h"
#include "../Object/JampRamp.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Bomb.h"
#include "Planet.h"
#include "LoopStage.h"
#include "Common/Collider.h"
#include "Common/Capsule.h"
#include "Common/Transform.h"
#include "../Scene/GameScene.h"
#include "Stage.h"

Stage::Stage(Bike* bike, EnemyBase* enemy, Bomb* bomb, GameScene* gameScene)
	: resMng_(ResourceManager::GetInstance())
{
	gameScene_ = gameScene;

	bike_ = bike;
	enemy_ = enemy;
	bomb_ = bomb;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;

	isMakeLoopStage_ = false;

	isJamp_ = false;

	//���[�v�p�̃X�e�[�W
	/*loopStage_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE);
	loopStage_.pos = { -5000.0f, -5600.0f, 6500.0f };
	loopStage_.scl = { 1.0f,1.0f,1.0f };
	loopStage_.quaRot = Quaternion();
	loopStage_.MakeCollider(Collider::TYPE::STAGE);
	loopStage_.Update();*/

}

Stage::~Stage(void)
{

	// ���[�v�X�^�[
	for (auto star : warpStars_)
	{
		delete star;
	}
	warpStars_.clear();

	// �f��
	for (auto pair : planets_)
	{
		delete pair.second;
	}
	planets_.clear();

	//���[�v�p�̃X�e�[�W
	for (auto loop : loopStage_)
	{
		delete loop;
	}
	loopStage_.clear();

	delete jampRamp_;
}

void Stage::Init(void)
{
	MakeMainStage();
	MakeLoopStage();
	MakeWarpStar();

	jampRamp_ = new JampRamp();
	jampRamp_->Init();

	step_ = -1.0f;
}

void Stage::Update(void)
{

	// ���[�v�X�^�[
	for (const auto& s : warpStars_)
	{
		s->Update();
	}

	// �f��
	for (const auto& s : planets_)
	{
		s.second->Update();
	}

	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		ls->Update();
	}

	//�X�e�[�W�𐶐�����
	MakeLoopStage();

	//�o�C�N�ƃW�����v��̓����蔻��
	auto bikeCap = bike_->GetCapsule();
	auto jumpRampCap = jampRamp_->GetCapsule();

	VECTOR diff = VSub(jumpRampCap->GetCenter(), bikeCap->GetCenter());
	float  dis = AsoUtility::SqrMagnitudeF(diff);
	if (dis < bikeCap->GetRadius() * jumpRampCap->GetRadius())
	{
		isJamp_ = true;
		Jump();
	}
	else
	{
		isJamp_ = false;

	}

	jampRamp_->Update();

}

void Stage::Draw(void)
{

	// ���[�v�X�^�[
	for (const auto& s : warpStars_)
	{
		s->Draw();
	}

	// �f��
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		ls->Draw();
	}

	jampRamp_->Draw();

	DrawFormatString(0, 200, 0xff0000, "IsJump:%d", isJamp_);
}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// �Ώۂ̃X�e�[�W���擾����
	activePlanet_ = GetPlanet(activeName_);

	// �X�e�[�W�̓����蔻��ݒ�
	bike_->ClearCollider();
	bike_->AddCollider(activePlanet_->GetTransform().collider);
	jampRamp_->ClearCollider();
	jampRamp_->AddCollider(activePlanet_->GetTransform().collider);
	bomb_->ClearCollider();
	bomb_->AddCollider(activePlanet_->GetTransform().collider);
	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		bike_->AddCollider(ls->GetTransform().collider);
		jampRamp_->AddCollider(ls->GetTransform().collider);
		bomb_->AddCollider(ls->GetTransform().collider);
	}

	enemy_->ClearCollider();
	enemy_->AddCollider(activePlanet_->GetTransform().collider);
	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		//enemy_->AddCollider(ls->GetTransform().collider);

		std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
		for (int i = 0; i < enemys_.size(); i++)
		{
			enemys_[i]->AddCollider(ls->GetTransform().collider);
		}
	}

	step_ = TIME_STAGE_CHANGE;

}

Planet* Stage::GetPlanet(NAME type)
{
	if (planets_.count(type) == 0)
	{
		return nullPlanet;
	}

	return planets_[type];
}

bool Stage::GetIsMakeLoopStage(void)
{
	return isMakeLoopStage_;
}

void Stage::SetMakeLoopStage(bool value)
{
	isMakeLoopStage_ = value;
}

VECTOR Stage::GetForwardLoopPos(void)
{
	//�擪���[�v�X�e�[�W�̍��W���擾
	int size = (int)loopStage_.size();
	return loopStage_[size - 1]->GetPos();
}

void Stage::Jump(void)
{
	//bike_->SetSpeed(120.0f,50.0f, 3000.0f);
	bike_->Jump();
}

void Stage::MakeMainStage(void)
{

	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	float scale = 1.0f;
	planetTrans.scl = { scale * 2.5f,scale,scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { STAGE_START_POS };

	// �����蔻��(�R���C�_)�쐬
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet =
		new Planet(
			name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planets_.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeLoopStage(void)
{


	//���蔲���邽�߂����Ńo�C�N�ƓG�̃R���C�_�[���ǉ����Ƃ�
	if (gameScene_->GetIsCreateEnemy())
	{
		for (const auto& ls : loopStage_)
		{
			std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
			for (int i = 0; i < enemys_.size(); i++)
			{
				enemys_[i]->AddCollider(activePlanet_->GetTransform().collider);
				enemys_[i]->AddCollider(ls->GetTransform().collider);
			}
		}
	}

	Transform loopTrans;
	LoopStage* stage;


	float z = bike_->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)loopStage_.size();

	//���̋�����������
	if (size <= mapZ)
	{

		loopTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));



		float scale = 1.0f;
		loopTrans.scl = { scale * 2.5f,scale,scale };
		loopTrans.quaRot = Quaternion();
		loopTrans.pos = { STAGE_START_POS.x,  STAGE_START_POS.y,  STAGE_START_POS.z + 5000.0f * (size + 1) };

		// �����蔻��(�R���C�_)�쐬
		loopTrans.MakeCollider(Collider::TYPE::STAGE);
		loopTrans.Update();

		//���蔲���邽�߂����Ńv���C���[�̃R���C�_�[���ǉ����Ƃ�
		for (const auto& ls : loopStage_)
		{
			bike_->AddCollider(ls->GetTransform().collider);
			bomb_->AddCollider(ls->GetTransform().collider);
		}

		stage = new LoopStage(bike_, loopTrans);
		stage->Init();
		loopStage_.push_back(stage);
		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//���̃X�e�[�W���폜
	if (loopStage_.size() >= 6)
	{
	/*	std::queue<int> lt;

		for (size_t i = 0; i < loopStage_.size(); ++i) {
			lt.push(i);
		}

		while (!lt.empty() && loopStage_.size() >= 6)
		{
			int index = lt.front();
			lt.pop();*/

			// �X�e�[�W���폜����
			LoopStage* tailLoop = loopStage_[size-5];
			tailLoop->Destroy();

			//// �����Ɉ�v����C�e���[�^���폜
			//loopStage_.erase(
			//	std::remove(
			//		loopStage_.begin(),
			//		loopStage_.end(),
			//		tailLoop
			//	),
			//	loopStage_.end()
			//);
		
	}


	//tailLoop->Destroy();
	//std::_Vector_iterator<std::_Vector_val<std::_Simple_types<LoopStage*>>> it;


	//for (const auto& ls : loopStage_)
	//{
	//	//	it = std::remove_if(loopStage_.begin(), loopStage_.end(), [=]() {
	//	//		return ls->GetState() == LoopStage::STATE::BACK;
	//	//		});
	//}

	//// �폜��������������_���Œ�`
	//auto it = std::remove_if(loopStage_.begin(), loopStage_.end(),
	//	[](LoopStage* ls) {
	//		return ls->GetState() == LoopStage::STATE::BACK;
	//	});

	//// �x�N�^�[����v�f���폜
	//loopStage_.erase(it, loopStage_.end());

}

//// �X�e�[�W��ǉ�����֐�
//void Stage::AddStage(LoopStage* newStage)
//{
//	std::queue<LoopStage*> stageQueue;
//	int size = 0; // size��loopStage_�̃T�C�Y���w��
//	loopStage_.push_back(newStage);
//	stageQueue.push(newStage);
//	size++;
//
//	// 6�ȏ�̃X�e�[�W������ꍇ�͌Â��X�e�[�W���폜
//	if (stageQueue.size() > 5) {
//		LoopStage* oldStage = stageQueue.front();
//		stageQueue.pop();
//
//		oldStage->Destroy();
//
//		// �Â��X�e�[�W��loopStage_����폜
//		loopStage_.erase(
//			std::remove(loopStage_.begin(), loopStage_.end(), oldStage),
//			loopStage_.end()
//		);
//	}
//}
void Stage::MakeWarpStar(void)
{

	//Transform trans;
	//WarpStar* star;

	//// ���Ƃ����f���ւ̃��[�v�X�^�[
	////------------------------------------------------------------------------------
	//trans.pos = { -910.0f, 200.0f, 894.0f };
	//trans.scl = { 0.6f, 0.6f, 0.6f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(-25.0f),
	//	AsoUtility::Deg2RadF(-50.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);

 // 	star = new WarpStar(bike_, trans);
	//star->Init();
	//warpStars_.push_back(star);
	////------------------------------------------------------------------------------

}
