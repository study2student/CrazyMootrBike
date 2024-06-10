#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/EnemyBike.h"
#include "../Object/Rider/Rider.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Rider/ShortDisEnemy.h"
#include "../Object/Rider/LongDisEnemy.h"
#include "../Object/Rider/MagicEnemy.h"
#include "../Object/Planet.h"
#include "../Object/Bomb.h"
#include "../Object/Score.h"
#include "../Object/Helicopter.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	rider_ = nullptr;
	//player_ = nullptr;
	bike_ = nullptr;
	enemy_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
	enemyBike_ = nullptr;
	helicopter_ = nullptr;
	score_ = nullptr;
}

GameScene::~GameScene(void)
{
	//delete player_;
	delete enemy_;
	delete rider_;
}

void GameScene::Init(void)
{

	// �v���C���[
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = std::make_shared<Bike>();
	bike_->Init();

	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	//�X�R�A
	score_ = std::make_shared<Score>();
	score_->Init();

	// �G
	enemy_ = new EnemyBase(bike_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });

	//�w���R�v�^�[�@
	helicopter_ = std::make_unique<Helicopter>();
	helicopter_->Init();

	// �X�e�[�W
	stage_ = std::make_unique<Stage>(bike_, enemy_,helicopter_->GetBomb(), this);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>(bike_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&bike_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	// �G�t�F�N�g������
	InitEffect();

	//�G���J�E���g���Z�b�g
	enCounter = 0;

	//�G���������ꂽ��
	isCreateEnemy_ = false;

	hitStopDuration = 6000.0f;
	hitStopTimer = 0.0f;
	isHitStop = false;
}

void GameScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if (stage_->GetLoopStageSize() >= 35)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	float deltaTime = hitStopDuration;
	skyDome_->Update();
	if (!isHitStop)
	{
		bike_->Update();
	}
	else
	{
		// �q�b�g�X�g�b�v���̏ꍇ�A�^�C�}�[���X�V
		hitStopTimer -= deltaTime;
		if (hitStopTimer <= 0.f) {
			isHitStop = false;
		}

		// �q�b�g�G�t�F�N�g
		float scale = 50.0f;
		FireBlessEffect();
		effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
		SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	}
	enemy_->SetBikeTrans(bike_->GetTransform());
	helicopter_->SetBikeTrans(bike_->GetTransform());

	//�G
	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Update();
		if (enemys_[i]->GetIsAddScore())
		{
			isHitStop = true;
			//�X�R�A���Z
			score_->AddScore();
		}
	}

	//�G�o�C�N
	size_t sizeEb = enemyBikes_.size();
	for (int t = 0; t < sizeEb; t++)
	{
		enemyBikes_[t]->Update();
	}
	
	
	if (isHitStop == true)
	{

	}

	//�Փ˔���
	Collision();

	//�X�e�[�W���������ꂽ��G��z�u����
	if (stage_->GetIsMakeLoopStage())
	{

		stage_->SetMakeLoopStage(false);
		
		//�Z���^�[��������̉��̈ړ���
		float shiftX_ = {};


		//���̃����_���ȏꏊ�ɐ���(3�p�^�[��)
		int randDir = GetRand(static_cast<int>(EnemyBase::DIR::MAX) - 1);
		EnemyBase::DIR dir = static_cast<EnemyBase::DIR>(randDir);

		Vector2 randPos;
		switch (dir)
		{
		case EnemyBase::DIR::LEFT:
			shiftX_ = -EnemyBase::DIR_LEN;
			break;
		case EnemyBase::DIR::CENTER:
			shiftX_ = 0.0f;
			break;
		case EnemyBase::DIR::RIGHT:
			shiftX_ = EnemyBase::DIR_LEN;
			break;
		}

		//�c�ɓG�𐶐�����
		for (int i = 0; i < EnemyBase::MAX_MAKE_NUM; i++)
		{
			//�c�ɕ��ԓG�ƓG�̋���
			float len = EnemyBase::X_LEN;

			//�G�̐���
			EnemyBase* e = nullptr;
			int eType = GetRand(static_cast<int>(EnemyBase::TYPE::MAX) - 1);
			EnemyBase::TYPE type = static_cast<EnemyBase::TYPE>(eType);

			switch (type)
			{
			case EnemyBase::TYPE::SHORT_DIS:
				e = new ShortDisEnemy(bike_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case EnemyBase::TYPE::LONG_DIS:
				e = new LongDisEnemy(bike_,stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case EnemyBase::TYPE::BOMB:
				e = new MagicEnemy(bike_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			}
			e->Init();

			EnemyBike* eB = nullptr;
			eB = new EnemyBike(e);
			eB->Init();
			isCreateEnemy_ = true;

			
			////�f�o�b�O
			//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

			////���W�̐ݒ�
			//e->setPos(randPos.ToVector2F());

			//�ϒ��z��ɗv�f��ǉ�
			enemys_.push_back(e);
			enemyBikes_.push_back(eB);
		}
	}
	else
	{
		stage_->Update();
		isCreateEnemy_ = false;
	}
		

	helicopter_->Update();
	score_->Update();
}

void GameScene::Draw(void)
{

	// �w�i
	skyDome_->Draw();
	stage_->Draw();

	bike_->Draw();
	helicopter_->Draw();
	score_->Draw();

	//enemy_->Draw();
	//enemyBike_->Draw();

	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		if (!enemys_[i]->IsDestroy())
		{
			enemys_[i]->Draw();
		}
	}

	size_t sizeEb = enemyBikes_.size();
	for (int i = 0; i < sizeEb; i++)
	{
		if (!enemys_[i]->IsDestroy())
		{
			enemyBikes_[i]->Draw();
		}
	}


	// �w���v
	//DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	//DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	//DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	//DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
	//DrawDubg();
}

std::vector<EnemyBase*> GameScene::GetEnemys(void)
{
	return enemys_;
}

std::vector<EnemyBike*> GameScene::GetEnemyBikes(void)
{
	return enemyBikes_;
}

bool GameScene::GetIsCreateEnemy(void)
{
	return isCreateEnemy_;
}

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000,"DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000,"FPS:%f", GetFPS());
	DrawFormatString(0, 140, 0x000000, "IsHitStop:%d", isHitStop);
}

void GameScene::Collision(void)
{
	//�G���m�̓����蔻��(�e��)
	size_t sizeEb = enemyBikes_.size();
	for (int b1 = 0; b1 < sizeEb; b1++)
	{

		for (int b2 = 0; b2 < sizeEb; b2++)
		{

			if (enemyBikes_[b1] == enemyBikes_[b2])
			{
				continue;
			}

			auto b1Pos = enemyBikes_[b1]->GetCapsule()->GetCenter();
			auto b2Pos = enemyBikes_[b2]->GetCapsule()->GetCenter();

			VECTOR diff = VSub(b1Pos, b2Pos);
			float  dis = AsoUtility::SqrMagnitudeF(diff);
			if (dis < EnemyBase::RADIUS * EnemyBase::RADIUS)
			{

				// �͈͂ɓ�������A���݂���e��
				auto flipDirB1 = VNorm(VSub(b1Pos, b2Pos));
				flipDirB1.y = 0.0f;
				flipDirB1 = VNorm(flipDirB1);
				auto flipDirB2 = VNorm(VSub(b2Pos, b1Pos));
				flipDirB2.y = 0.0f;
				flipDirB2 = VNorm(flipDirB2);

				enemys_[b1]->Flip(flipDirB1);
				enemys_[b2]->Flip(flipDirB2);
			}


		}

	}


	//���e�ƃv���C���[�̓����蔻��
	//HP�����葱���Ă��܂��̂œ����������͏������f
	if (helicopter_->GetBomb()->GetIsCol())
	{
		return;
	}

	auto heliCap = helicopter_->GetBomb()->GetCapsule();
	auto bikeCap = bike_->GetCapsule();

	VECTOR diff = VSub(heliCap.lock()->GetCenter(), bikeCap->GetCenter());
	float  dis = AsoUtility::SqrMagnitudeF(diff);
	if (dis < heliCap.lock()->GetRadius() * bikeCap->GetRadius())
	{
		//�v���C���[�Ƀ_���[�W
		bike_->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

		//��������
		helicopter_->GetBomb()->SetIsCol(true);
	}
}

void GameScene::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HitEffect).handleId_;
}

void GameScene::FireBlessEffect(void)
{
	auto pPos = bike_->GetTransform();
	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, pPos.pos.x, pPos.pos.y, pPos.pos.z + 500);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, pPos.rot.x, pPos.rot.y, pPos.rot.z);
}