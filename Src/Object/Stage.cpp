#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "WarpStar.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "Planet.h"
#include "LoopStage.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(Bike* bike, EnemyBase* enemy)
	: resMng_(ResourceManager::GetInstance())
{
	bike_ = bike;
	enemy_ = enemy;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;

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

}

void Stage::Init(void)
{
	MakeMainStage();
	MakeLoopStage();
	MakeWarpStar();



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



}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// �Ώۂ̃X�e�[�W���擾����
	activePlanet_ = GetPlanet(activeName_);

	// �X�e�[�W�̓����蔻����v���C���[�ɐݒ�
	bike_->ClearCollider();
	bike_->AddCollider(activePlanet_->GetTransform().collider);
	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		bike_->AddCollider(ls->GetTransform().collider);
	}


	// �X�e�[�W�̂����蔻���G�ɐݒ�
	enemy_->ClearCollider();
	enemy_->AddCollider(activePlanet_->GetTransform().collider);
	//���[�v�p�̃X�e�[�W
	for (const auto& ls : loopStage_)
	{
		enemy_->AddCollider(ls->GetTransform().collider);
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

void Stage::MakeMainStage(void)
{

	// �ŏ��̘f��
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	float scale = 1.0f;
	planetTrans.scl = { scale,scale,scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -5000.0f, -5600.0f, 0.0f };

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

	Transform loopTrans;
	LoopStage* stage;


	float z = bike_->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)loopStage_.size();

	if (size <= mapZ)
	{

		loopTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));



		float scale = 1.0f;
		loopTrans.scl = { scale,scale,scale };
		loopTrans.quaRot = Quaternion();
		loopTrans.pos = { -5000.0f, -5600.0f, 6500.0f * (size + 1) };

		// �����蔻��(�R���C�_)�쐬
		loopTrans.MakeCollider(Collider::TYPE::STAGE);
		loopTrans.Update();

		//���蔲���邽�߂����Ńo�C�N�ƓG�̃R���C�_�[���ǉ����Ƃ�
		for (const auto& ls : loopStage_)
		{
			bike_->AddCollider(ls->GetTransform().collider);
			enemy_->AddCollider(ls->GetTransform().collider);
		}

		stage = new LoopStage(bike_, loopTrans);
		stage->Init();
		loopStage_.push_back(stage);

	}






}

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
