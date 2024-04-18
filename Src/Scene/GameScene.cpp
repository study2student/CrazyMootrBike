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

	// プレイヤー
	rider_ = new Rider();
	rider_->Init();

	// 敵
	enemy_ = new Enemy();
	enemy_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = new Bike();
	bike_->Init();

	// ステージ
	stage_ = new Stage(rider_, enemy_);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	skyDome_ = new SkyDome(rider_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&bike_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

}

void GameScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	skyDome_->Update();

	stage_->Update();

	bike_->Update();

	enemy_->Update();

}

void GameScene::Draw(void)
{

	// 背景
	skyDome_->Draw();
	stage_->Draw();

	bike_->Draw();

	enemy_->Draw();

	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");

}
