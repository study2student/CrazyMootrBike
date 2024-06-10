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

	// プレイヤー
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = std::make_shared<Bike>();
	bike_->Init();

	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	//スコア
	score_ = std::make_shared<Score>();
	score_->Init();

	// 敵
	enemy_ = new EnemyBase(bike_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });

	//ヘリコプター　
	helicopter_ = std::make_unique<Helicopter>();
	helicopter_->Init();

	// ステージ
	stage_ = std::make_unique<Stage>(bike_, enemy_,helicopter_->GetBomb(), this);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	skyDome_ = std::make_unique<SkyDome>(bike_->GetTransform());
	skyDome_->Init();

	SceneManager::GetInstance().GetCamera()->SetFollow(&bike_->GetTransform());
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	// エフェクト初期化
	InitEffect();

	//エンカウントリセット
	enCounter = 0;

	//敵が生成されたか
	isCreateEnemy_ = false;

	hitStopDuration = 6000.0f;
	hitStopTimer = 0.0f;
	isHitStop = false;
}

void GameScene::Update(void)
{

	// シーン遷移
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
		// ヒットストップ中の場合、タイマーを更新
		hitStopTimer -= deltaTime;
		if (hitStopTimer <= 0.f) {
			isHitStop = false;
		}

		// ヒットエフェクト
		float scale = 50.0f;
		FireBlessEffect();
		effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
		SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	}
	enemy_->SetBikeTrans(bike_->GetTransform());
	helicopter_->SetBikeTrans(bike_->GetTransform());

	//敵
	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Update();
		if (enemys_[i]->GetIsAddScore())
		{
			isHitStop = true;
			//スコア加算
			score_->AddScore();
		}
	}

	//敵バイク
	size_t sizeEb = enemyBikes_.size();
	for (int t = 0; t < sizeEb; t++)
	{
		enemyBikes_[t]->Update();
	}
	
	
	if (isHitStop == true)
	{

	}

	//衝突判定
	Collision();

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
		

	helicopter_->Update();
	score_->Update();
}

void GameScene::Draw(void)
{

	// 背景
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


	// ヘルプ
	//DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	//DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	//DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	//DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
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
	//敵同士の当たり判定(弾く)
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


	//爆弾とプレイヤーの当たり判定
	//HPが減り続けてしまうので当たった時は処理中断
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
		//プレイヤーにダメージ
		bike_->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

		//当たった
		helicopter_->GetBomb()->SetIsCol(true);
	}
}

void GameScene::InitEffect(void)
{
	// ヒットエフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HitEffect).handleId_;
}

void GameScene::FireBlessEffect(void)
{
	auto pPos = bike_->GetTransform();
	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, pPos.pos.x, pPos.pos.y, pPos.pos.z + 500);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, pPos.rot.x, pPos.rot.y, pPos.rot.z);
}