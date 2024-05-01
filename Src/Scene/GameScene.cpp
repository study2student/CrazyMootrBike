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
#include "../Object/Rider/EnemyBike.h"
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
	enemyBike_ = nullptr;

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
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = new Bike();
	bike_->Init();

	// 敵
	enemy_ = new EnemyBase(bike_);
	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	// ステージ
	stage_ = new Stage(bike_, enemy_,this);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	skyDome_ = new SkyDome(bike_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&bike_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	//エンカウントリセット
	enCounter = 0;

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
	enemy_->SetBikeTrans(bike_->GetTransform());
	//enemy_->Update();
	//enemyBike_->Update();

	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Update();

	}


	//敵同士の当たり判定
	size_t sizeEb = enemyBikes_.size();
	for (int t = 0; t < sizeEb; t++)
	{
		enemyBikes_[t]->Update();

		////2体以上敵が生成されたら
		//if (sizeE >= 1)
		//{
		//	//衝突判定(敵と敵)
		//	VECTOR diff = VSub(enemyBikes_[t]->GetCapsule()->GetCenter(), enemyBikes_[t-1]->GetCapsule()->GetCenter());
		//	float  dis = AsoUtility::SqrMagnitudeF(diff);
		//	if (dis < EnemyBase::RADIUS * EnemyBase::RADIUS)
		//	{
		//		//範囲に入った
		//		enemys_[t]->SetSpeed(0.0f);
		//	}
		//	else
		//	{
		//		enemys_[t]->SetSpeed(EnemyBase::SPEED_MOVE);
		//	}
		//}
	}


	enCounter++;
	if (enCounter > ENCOUNT)
	{
		//エンカウンターリセット
		enCounter = 0;

		//敵の生成
		EnemyBase* e = nullptr;
		int eType = GetRand(static_cast<int>(EnemyBase::TYPE::MAX) - 1);
		EnemyBase::TYPE type = static_cast<EnemyBase::TYPE>(eType);

		switch (type)
		{
		case EnemyBase::TYPE::SHORT_DIS:
			e = new ShortDisEnemy(bike_);
			break;
		case EnemyBase::TYPE::LONG_DIS:
			e =  new LongDisEnemy(bike_);
			break;
		case EnemyBase::TYPE::BOMB:
			e =  new BombEnemy(bike_);
			break;
		}
		e->Init();

		EnemyBike* eB = nullptr;
		eB = new EnemyBike(e);
		eB->Init();


		////画面端のランダムな場所に生成
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

		////デバッグ
		//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

		////座標の設定
		//e->setPos(randPos.ToVector2F());

		//可変長配列に要素を追加
		enemys_.push_back(e);
		enemyBikes_.push_back(eB);
	}

}

void GameScene::Draw(void)
{

	// 背景
	skyDome_->Draw();
	stage_->Draw();

	bike_->Draw();

	//enemy_->Draw();
	//enemyBike_->Draw();

	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Draw();
	}

	size_t sizeEb = enemyBikes_.size();
	for (int i = 0; i < sizeEb; i++)
	{
		enemyBikes_[i]->Draw();
	}


	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");

}

std::vector<EnemyBase*> GameScene::GetEnemys(void)
{
	return enemys_;
}

std::vector<EnemyBike*> GameScene::GetEnemyBikes(void)
{
	return enemyBikes_;
}
