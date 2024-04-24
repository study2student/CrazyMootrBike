#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/Rider.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Rider/ShortDisEnemy.h"
#include "../Object/Rider/LongDisEnemy.h"
#include "../Object/Rider/BombEnemy.h"
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	rider_ = nullptr;
	//player_ = nullptr;
	bike_ = nullptr;
	enemy_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;

}

GameScene::~GameScene(void)
{
	//delete player_;
	delete bike_;
	delete enemy_;
	delete rider_;
	delete stage_;
	delete skyDome_;
}

void GameScene::Init(void)
{

	// �v���C���[
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = new Bike();
	bike_->Init();

	// �G
	enemy_ = new EnemyBase(bike_);
	enemy_->Init();

	// �X�e�[�W
	stage_ = new Stage(bike_, enemy_,this);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = new SkyDome(bike_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&bike_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	//�G���J�E���g���Z�b�g
	enCounter = 0;

}

void GameScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	skyDome_->Update();

	stage_->Update();

	bike_->Update();
	enemy_->SetBikeTrans(bike_->GetTransform());
	enemy_->Update();


	size_t size = enemys_.size();
	for (int i = 0; i < size; i++)
	{
		enemys_[i]->Update();
	}

	enCounter++;
	if (enCounter > ENCOUNT)
	{
		//�G���J�E���^�[���Z�b�g
		enCounter = 0;

		//�G�̐���
		EnemyBase* e = nullptr;
		int eType = GetRand(static_cast<int>(EnemyBase::TYPE::MAX) - 1);
		EnemyBase::TYPE type = static_cast<EnemyBase::TYPE>(eType);

		switch (type)
		{
		case EnemyBase::TYPE::SHORT_DIS:
			e = new ShortDisEnemy();
			break;
		case EnemyBase::TYPE::LONG_DIS:
			e =  new LongDisEnemy();
			break;
		case EnemyBase::TYPE::BOMB:
			e =  new BombEnemy();
			break;
		}
		e->Init();

		////��ʒ[�̃����_���ȏꏊ�ɐ���
		//int randDir = GetRand(static_cast<int>(AsoUtility::DIR::MAX) - 1);
		//AsoUtility::DIR dir = static_cast<AsoUtility::DIR>(randDir);

		//Vector2 randPos;
		//switch (dir)
		//{
		//case AsoUtility::DIR::UP:
		//	randPos.x = cameraPos_.x + GetRand(Application::SCREEN_SIZE_X);
		//	randPos.y = cameraPos_.y;
		//	break;
		//case AsoUtility::DIR::RIGHT:
		//	randPos.x = cameraPos_.x + Application::SCREEN_SIZE_X;
		//	randPos.y = cameraPos_.y + GetRand(Application::SCREEN_SIZE_Y);
		//	break;
		//case AsoUtility::DIR::DOWN:
		//	randPos.x = cameraPos_.x + GetRand(Application::SCREEN_SIZE_X);
		//	randPos.y = cameraPos_.y + Application::SCREEN_SIZE_Y;
		//	break;
		//case AsoUtility::DIR::LEFT:
		//	randPos.x = cameraPos_.x;
		//	randPos.y = cameraPos_.y + GetRand(Application::SCREEN_SIZE_Y);
		//	break;
		//}

		////�f�o�b�O
		//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

		////���W�̐ݒ�
		//e->setPos(randPos.ToVector2F());

		//�ϒ��z��ɗv�f��ǉ�
		enemys_.push_back(e);
	}

}

void GameScene::Draw(void)
{

	// �w�i
	skyDome_->Draw();
	stage_->Draw();

	bike_->Draw();

	enemy_->Draw();

	size_t size = enemys_.size();
	for (int i = 0; i < size; i++)
	{
		enemys_[i]->Draw();
	}


	// �w���v
	DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");

}

std::vector<EnemyBase*> GameScene::GetEnemys(void)
{
	return enemys_;
}
