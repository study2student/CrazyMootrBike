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
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Planet.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	player_ = nullptr;
	enemy_ = nullptr;
	bike_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
}

GameScene::~GameScene(void)
{
	delete player_;
	delete enemy_;
	delete bike_;
	delete stage_;
	delete skyDome_;
}

void GameScene::Init(void)
{

	// �v���C���[
	player_ = new Player();
	player_->Init();

	// �G
	enemy_ = new Enemy();
	enemy_->Init();

	// �o�C�N
	bike_ = new Bike();
	bike_->Init();

	// �X�e�[�W
	stage_ = new Stage(player_, bike_, enemy_);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	skyDome_ = new SkyDome(player_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&player_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

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

	player_->Update();

	enemy_->Update();

	bike_->Update();
}

void GameScene::Draw(void)
{

	// �w�i
	skyDome_->Draw();
	stage_->Draw();

	player_->Draw();

	enemy_->Draw();

	bike_->Draw();

	// �w���v
	DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");

}
