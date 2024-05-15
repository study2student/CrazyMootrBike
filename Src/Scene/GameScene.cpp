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
#include "../Object/Rider/MagicEnemy.h"
#include "../Object/Planet.h"
#include "../Object/Score.h"
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

	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	// 敵
	enemy_ = new EnemyBase(bike_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });

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

	//敵が生成されたか
	isCreateEnemy_ = false;

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

	bike_->Update();
	enemy_->SetBikeTrans(bike_->GetTransform());
	//enemy_->Update();
	//enemyBike_->Update();

	//敵
	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Update();
	}

	//敵バイク
	size_t sizeEb = enemyBikes_.size();
	for (int t = 0; t < sizeEb; t++)
	{
		enemyBikes_[t]->Update();
	}


	//敵同士の当たり判定(弾く)
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

				// 範囲に入ったら、お互いを弾く
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


	//ステージが生成されたら敵を配置する
	if (stage_->GetIsMakeLoopStage())
	{

		stage_->SetMakeLoopStage(false);
		
		//センター方向からの横の移動幅
		float shiftX_ = {};


		//道のランダムな場所に生成(3パターン)
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

		//縦に敵を生成する
		for (int i = 0; i < EnemyBase::MAX_MAKE_NUM; i++)
		{
			//縦に並ぶ敵と敵の距離
			float len = EnemyBase::X_LEN;

			//敵の生成
			EnemyBase* e = nullptr;
			int eType = GetRand(static_cast<int>(EnemyBase::TYPE::MAX) - 1);
			EnemyBase::TYPE type = static_cast<EnemyBase::TYPE>(eType);

			switch (type)
			{
			case EnemyBase::TYPE::SHORT_DIS:
				e = new ShortDisEnemy(bike_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case EnemyBase::TYPE::LONG_DIS:
				e = new LongDisEnemy(bike_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
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

			score_ = new Score(e);
			score_->Init();

			////デバッグ
			//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

			////座標の設定
			//e->setPos(randPos.ToVector2F());

			//可変長配列に要素を追加
			enemys_.push_back(e);
			enemyBikes_.push_back(eB);
		}
	}
	else
	{
		stage_->Update();
		isCreateEnemy_ = false;
	}
		
	score_->Update();
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
		if (enemys_[i]->GetIsBikeCol() == false)
		{
			enemys_[i]->Draw();
			score_->Draw();
		}
	}

	size_t sizeEb = enemyBikes_.size();
	for (int i = 0; i < sizeEb; i++)
	{
		if (enemys_[i]->GetIsBikeCol() == false)
		{
			enemyBikes_[i]->Draw();
		}
	}


	// ヘルプ
	DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
	DrawDubg();
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
}
