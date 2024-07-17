#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/EnemyBike.h"
#include "../Object/Rider/Rider.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Rider/ShortDisEnemy.h"
#include "../Object/Rider/LongDisEnemy.h"
#include "../Object/Rider/MagicEnemy.h"
#include "../Object/Planet.h"
#include "../Object/Bomb.h"
#include "../Object/Score.h"
#include "../Object/Helicopter.h"
#include "../Object/TyreThrow.h"
#include "../Object/DataSave.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	enemy_ = nullptr;
	stage_ = nullptr;
	enemyBike_ = nullptr;
	helicopter_ = nullptr;
	throwTyre_ = nullptr;
	//score_ = nullptr;

	nowCursor_ = 0;
	stepGoalAfter_ = 0.0f;
}

GameScene::~GameScene(void)
{
	delete enemy_;
	delete throwTyre_;
}

void GameScene::Init(void)
{
	//プレイヤー人数
	playNumber_ = data_.GetData().playerNum_;
	if (playNumber_ == 1)
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
	}
	else
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	}

	// プレイヤー
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(200.0f * (i + 1), i));
	}

	for (auto& bike : bikes_) {
		bike->Init();
	}

	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	////スコア
	//score_ = std::make_shared<Score>();
	//score_->Init();

	// 敵
	for (auto& bike : bikes_) {
		enemy_ = new EnemyBase(bikes_,this, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	//ヘリコプター
	helicopter_ = std::make_shared<Helicopter>();
	helicopter_->Init();

	//投げタイヤ
	throwTyre_ = new TyreThrow();
	throwTyre_->Init();

	// ステージ
	stage_ = std::make_shared<Stage>(bikes_, enemy_, helicopter_->GetBomb(), throwTyre_, this);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	for (auto& bike : bikes_) {
		auto sky = std::make_unique<SkyDome>(bike->GetTransform());
		sky->Init();
		skyDomes_.emplace_back(std::move(sky));
	}

	if (playNumber_ == 1)
	{
		cameras_.push_back(std::make_shared<Camera>());
	}
	else
	{
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
	}


	for (int i = 0; i < cameras_.size(); i++) {
		cameras_[i]->Init();
		cameras_[i]->ChangeMode(Camera::MODE::FOLLOW);
		cameras_[i]->SetFollow(&bikes_[i]->GetTransform()); // 各プレイヤーのバイクを追従
	}

	//for (auto& bike : bikes_) {
	//	SceneManager::GetInstance().GetCamera()->SetFollow(&bike->GetTransform());
	//}
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	// エフェクト初期化
	InitEffect();

	//エンカウントリセット
	enCounter = 0;

	//敵が生成されたか
	isCreateEnemy_ = false;

	hitStopDuration = 6000.0f;
	hitStopTimer = 0.0f;
	isHitStop = false;

	//ポーズメニュー
	//色
	reStartFontColor_ = GetColor(255, 255, 255);
	reTryFontColor_ = GetColor(255, 255, 255);
	endFontColor_ = GetColor(255, 255, 255);

	//左上の再開ポジション
	int addX = -55;
	reStartFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 - 130 };

	//左上のリトライポジション
	addX = -95;
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 10 };

	//左上の終わるポジション
	addX = -70;
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 150 };

	//FINISH文字の初期位置
	Vector2 finishStartPos = { Application::SCREEN_SIZE_X / 2 - 220 ,-40 };
	finishFontPos_ = finishStartPos;

	isPause_ = false;
	isCursorHit_ = false;
	stepPauseKeyHit_ = 0.0f;

	onePersonIsGoal_ = false;

	//画像
	imgWarning_= resMng_.Load(ResourceManager::SRC::WARNING).handleId_;
	warningImgScale_ = WARNING_IMG_MIN_SCALE;

	imgPause_ = resMng_.Load(ResourceManager::SRC::PAUSE).handleId_;


	//スコアリセット
	score_.ResetScore();

	// ゲームスタート時のカウント
	startCount_ = 3.0f;
	isStart_ = false;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	//ポーズメニュー
	if (ins.IsTrgDown(KEY_INPUT_C))
	{
		//ポーズ状態
		isPause_ = true;
	}
	//ポーズ中はカメラの回転をさせないために情報を渡しておく
	SceneManager::GetInstance().GetCamera()->SetIsPause(isPause_);

	//ポーズ中は他処理中断
	if (isPause_)
	{
		Pause();
		return;
	}

	//最初の人がゴールしたら
	if (stage_->GetIsGoal())
	{
		//ゲームオーバーシーンで描画するため保存しておく
		if (playNumber_ == 1)
		{
			//爆弾は出させない
			helicopter_->ChangeState(Helicopter::STATE::MOVE);
		}
		else
		{
			bool allBikeGoal = true;
			for (auto& bike : bikes_)
			{
				// もし一つでもゴールしていないバイクがあればフラグを更新
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // ゴールしていないバイクが見つかった時点でループを抜ける
				}
			}

			// 全員がゴールしたらヘリ停止
			if (allBikeGoal)
			{
				//爆弾は出させない
				helicopter_->ChangeState(Helicopter::STATE::MOVE);
			}
		}

		stepGoalAfter_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepGoalAfter_ >= GOAL_TO_NEXT_SCENE)
		{
			//ゲームオーバーシーンで描画するため保存しておく
			if (playNumber_ == 1)
			{
				score_.ScoreSet(bikes_[0]->GetScore());
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
			else
			{
				bool allBikeGoal = true;
				for (auto& bike : bikes_)
				{
					score_.ScoreSetArray(bike->GetScore());

					// もし一つでもゴールしていないバイクがあればフラグを更新
					if (!bike->GetIsGoal())
					{
						allBikeGoal = false;
						break;  // ゴールしていないバイクが見つかった時点でループを抜ける
					}
				}

				// 全員がゴールした場合にシーンを切り替える
				if (allBikeGoal)
				{
					SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				}
			}
			
		}
	}
	//1人プレイ時ゴールしたかどうかセット
	onePersonIsGoal_ = stage_->GetIsGoal();

	// シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// スタート時のカウントダウンを減らす
	if (startCount_ > 0.0f)
	{
		startCount_ -= 1 / 60.0f;
	}


	if (startCount_ <= 0.0f)
	{
		float deltaTime = hitStopDuration;

		// BGMを再生
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_BGM).handleId_, DX_PLAYTYPE_LOOP, false);

		for (auto& skyDome : skyDomes_)
		{
			skyDome->Update();
		}

		if (!isHitStop)
		{
			if (playNumber_ == 1)
			{
				bikes_[0]->Update();
			}
			else
			{
				for (auto& bike : bikes_) {
					bike->Update();
				}
			}

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
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
		}
		for (auto& bike : bikes_)
		{
			enemy_->SetBikeTrans(bike->GetTransform());
		}

		//ヘリ
		helicopter_->SetBikeTrans(bikes_[0]->GetTransform());
		helicopter_->SetBikeIsOutside(bikes_[0]->GetIsOutSide());

		//投げモノ
		throwTyre_->Update();
		throwTyre_->SetTransform(bikes_[0]->GetTransform());

		//throwTyre_->SetTransform(bikes_[3]->GetTransform());

		//敵
		size_t sizeE = enemys_.size();
		for (int i = 0; i < sizeE; i++)
		{
			enemys_[i]->Update();
			if (enemys_[i]->GetIsAddScore())
			{
				isHitStop = true;
				//スコア加算
				//score_.AddScore();
			}
		}

		//衝突判定
		Collision();

		// バイク同士の衝突判定
		BikeCollision();

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
				for (auto& bike : bikes_) {
					switch (type)
					{
					case EnemyBase::TYPE::SHORT_DIS:
						e = new ShortDisEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					case EnemyBase::TYPE::LONG_DIS:
						e = new LongDisEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					case EnemyBase::TYPE::BOMB:
						e = new MagicEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					}
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
			}
		}
		else
		{
			stage_->Update();
			isCreateEnemy_ = false;
		}


		helicopter_->Update();
		//throwTyre_->Update();

	}
}

void GameScene::Draw(void)
{
	if (playNumber_ == 1)
	{
		SetDrawScreen(mainScreen_);

		// 画面を初期化
		ClearDrawScreen();

		cameras_[0]->SetBeforeDraw(); // 各プレイヤーの視点を設定

		// 背景
		skyDomes_[0]->Draw();

		stage_->Draw();

		//bike_->Draw();
		helicopter_->Draw();

		throwTyre_->Draw();

		//敵描画
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

		// 各バイクを描画
		bikes_[0]->Draw();

		int score = bikes_[0]->GetScore();

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, mainScreen_, false);
		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, score);

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
		}
	}
	else
	{
		for (int i = 0; i < cameras_.size(); i++)
		{
			SetDrawScreen(mainScreen_);

			// 画面を初期化
			ClearDrawScreen();

			cameras_[i]->SetBeforeDraw(); // 各プレイヤーの視点を設定

			// 背景
			skyDomes_[i]->Draw();

			stage_->Draw();

			//bike_->Draw();
			helicopter_->Draw();
			throwTyre_->Draw();

			//敵描画
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

			// 各バイクを描画
			for (auto& bike : bikes_) {
				bike->Draw();
			}

			for (int p = 0; p < bikes_.size(); p++) {
				//int score = bikes_[p]->GetScore();

				SetDrawScreen(DX_SCREEN_BACK);

				int sx = Application::SCREEN_SIZE_X;
				int sy = Application::SCREEN_SIZE_Y;

				switch (i)
				{
				case 0:
					DrawGraph(0, 0, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, bikes_[0]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 1:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawExtendFormatString(sx - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 2, bikes_[1]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 2:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 3, bikes_[2]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 3:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 4, bikes_[3]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				}
			}

			// スタート時のカウントを減らす
			if (startCount_ >= 0.0f)
			{
				DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
			}
		}
	}
	// ヘルプ
	//DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	//DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	//DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	//DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
	DrawDubg();


	////ゴールしたか判定
	//if (stage_->GetIsGoal())
	//{
	//	GoalAfterDraw();
	//}
	//else
	//{
	//	//警告
	//	WarningDraw();
	//}

	//ポーズ中
	if (isPause_)
	{
		PauseDraw();
	}

	//ゴールしたら文字出現
	if (playNumber_ == 1)
	{
		if (stage_->GetIsGoal())
		{
			GoalAfterDraw();
		}
	}

	if(!stage_->GetIsGoal())
	{
		//警告
		WarningDraw();
	}
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

int GameScene::GetPlayNum(void)
{
	return playNumber_;
}

bool GameScene::OnePersonIsGoal(void)
{
	return onePersonIsGoal_;
}

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000, "DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000, "FPS:%f", GetFPS());
	//DrawFormatString(840, 420, 0x000000,"カウンタ = %d", helicopter_.use_count());
	//DrawFormatString(840, 420, 0x000000,"bombPosY = %f", helicopter_->GetBomb().lock()->GetTransform().pos.y);
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

			auto b1Pos = enemyBikes_[b1]->GetCapsule().lock()->GetCenter();
			auto b2Pos = enemyBikes_[b2]->GetCapsule().lock()->GetCenter();

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


	//爆弾とプレイヤーの当たり判定、投げモノとプレイヤーの判定
	//HPが減り続けてしまうので当たった時は処理中断
	/*if (helicopter_->GetBomb()->GetIsCol() || throwTyre_->GetIsCol())
	if (helicopter_->GetBomb()->GetIsCol() || throwTyre_->GetIsCol())	{
		return;
	}*/

	//auto bombCap = helicopter_->GetBomb()->GetCapsule();
	//auto bikeCap = bikes_[3]->GetCapsule();

	//VECTOR diff = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
	//float  dis = AsoUtility::SqrMagnitudeF(diff);
	//if (dis < bombCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
	//{
	//	//プレイヤーにダメージ
	//	bikes_[3]->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

	//	//当たった
	//	helicopter_->GetBomb()->SetIsCol(true);
	//}

	for (const auto& bike : bikes_)
	{
		auto bikeCap = bike->GetCapsule();
		if(!helicopter_->GetBomb()->GetIsCol())
		{
			//爆弾
			auto bombCap = helicopter_->GetBomb()->GetCapsule();

			VECTOR diffB = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disB = AsoUtility::SqrMagnitudeF(diffB);
			if (disB < bombCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//ゴールしてない場合
					if (!stage_->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

					}
				}
				else
				{
					//ゴールしてないプレイヤーにだけダメージ
					if (!bike->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);
					}
				}

				//当たった
				helicopter_->GetBomb()->SetIsCol(true);
			}
		}

		if (!throwTyre_->GetIsCol())
		{

			//投げモノ
			auto throwCap = throwTyre_->GetCapsule();

			VECTOR diffT = VSub(throwCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disT = AsoUtility::SqrMagnitudeF(diffT);
			if (disT < throwCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//ゴールしてない場合
					if (!stage_->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(throwTyre_->THROW_DAMAGE);

					}
				}
				else
				{
					//ゴールしてないプレイヤーにだけダメージ
					if (!bike->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(throwTyre_->THROW_DAMAGE);
					}
				}

				//当たった
				throwTyre_->SetIsCol(true);
			}
		}

		//投げモノ
		auto throwCap = throwTyre_->GetCapsule();

		VECTOR diffT = VSub(throwCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
		float  disT = AsoUtility::SqrMagnitudeF(diffT);
		if (disT < throwCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
		{
			//プレイヤーにダメージ
			bike->Damage(throwTyre_->THROW_DAMAGE);

			//当たった
			throwTyre_->SetIsCol(true);
		}

		//ゲームオーバー処理
		if (bike->GetHP() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		}
	}
}

void GameScene::BikeCollision(void)
{
	//敵同士の当たり判定(弾く)
	size_t sizeBb = bikes_.size();
	for (int b1 = 0; b1 < sizeBb; b1++)
	{

		for (int b2 = 0; b2 < sizeBb; b2++)
		{

			if (bikes_[b1] == bikes_[b2])
			{
				continue;
			}

			auto b1Pos = bikes_[b1]->GetCapsule().lock()->GetCenter();
			auto b2Pos = bikes_[b2]->GetCapsule().lock()->GetCenter();

			VECTOR diff = VSub(b1Pos, b2Pos);
			float  dis = AsoUtility::SqrMagnitudeF(diff);
			if (dis < Bike::RADIUS * Bike::RADIUS)
			{

				// 範囲に入ったら、お互いを弾く
				auto flipDirB1 = VNorm(VSub(b1Pos, b2Pos));
				flipDirB1.y = 0.0f;
				flipDirB1 = VNorm(flipDirB1);
				auto flipDirB2 = VNorm(VSub(b2Pos, b1Pos));
				flipDirB2.y = 0.0f;
				flipDirB2 = VNorm(flipDirB2);

				bikes_[b1]->Flip(flipDirB1);
				bikes_[b2]->Flip(flipDirB2);
			}


		}

	}
}

void GameScene::InitEffect(void)
{
	// ヒットエフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;
}

void GameScene::HitEffect(void)
{
	for (auto& bike : bikes_) {
		const auto pPos = bike->GetTransform();
		SetPosPlayingEffekseer3DEffect(effectHitPlayId_, pPos.pos.x, pPos.pos.y, pPos.pos.z + 500);
		SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, pPos.rot.x, pPos.rot.y, pPos.rot.z);
	}
}

void GameScene::MouseProcess(void)
{

	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//再開ボタン
	//カーソルが当たっている
	Vector2 reStartFontLenPos_ = { reStartFontBasePos_.x + RESTART_FONT_LENGTH ,reStartFontBasePos_.y + RESTART_FONT_HEIGHT };
	if (mousePos_.x >= reStartFontBasePos_.x && mousePos_.x <= reStartFontLenPos_.x
		&& mousePos_.y >= reStartFontBasePos_.y && mousePos_.y <= reStartFontLenPos_.y)
	{
		nowCursor_ = (int)PAUSE_STATE::RESTART;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (pState_ == PAUSE_STATE::RESTART)
	{
		//ボタンにふれている場合
		reStartFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//ポーズ解除
			stepPauseKeyHit_ = 0.0f;
			isPause_ = false;
		}
	}
	else
	{
		//ボタンにふれいない場合
		reStartFontColor_ = GetColor(255, 255, 255);
	}



	//リトライボタン
	//カーソルが当たっている
	Vector2 reTryFontLenPos_ = { reTryFontBasePos_.x + RETRY_FONT_LENGTH ,reTryFontBasePos_.y + RETRY_FONT_HEIGHT };
	if (mousePos_.x >= reTryFontBasePos_.x && mousePos_.x <= reTryFontLenPos_.x
		&& mousePos_.y >= reTryFontBasePos_.y && mousePos_.y <= reTryFontLenPos_.y)
	{
		nowCursor_ = (int)PAUSE_STATE::RETRY;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (pState_ == PAUSE_STATE::RETRY)
	{
		//ボタンにふれている場合
		reTryFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでリトライ
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}
	else
	{
		//ボタンにふれいない場合
		reTryFontColor_ = GetColor(255, 255, 255);
	}



	//終わるボタン
	//カーソルが当たっている
	Vector2 endFontLenPos_ = { endFontBasePos_.x + END_FONT_LENGTH ,endFontBasePos_.y + END_FONT_HEIGHT };
	if (mousePos_.x >= endFontBasePos_.x && mousePos_.x <= endFontLenPos_.x
		&& mousePos_.y >= endFontBasePos_.y && mousePos_.y <= endFontLenPos_.y)
	{
		nowCursor_ = (int)PAUSE_STATE::END;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (pState_ == PAUSE_STATE::END)
	{
		//ボタンにふれている場合
		endFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでタイトルへ
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	else
	{
		//ボタンにふれいない場合
		endFontColor_ = GetColor(255, 255, 255);
	}


	//ポーズキーがもう一度押せるようになるまで
	stepPauseKeyHit_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepPauseKeyHit_ >= PAUSE_KEY_HIT_MAX_TIME)
	{
		//もう一度ポーズキー押下でポーズメニュー解除
		if (ins_.IsTrgDown(KEY_INPUT_C) && isPause_)
		{
			//ポーズ解除
			stepPauseKeyHit_ = 0.0f;
			isPause_ = false;
		}
	}


}

void GameScene::KeyProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//カーソル番号による上下操作
	if (ins_.IsTrgDown(KEY_INPUT_UP))
	{
		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN))
	{
		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}

	//現カーソルから状態を変化
	CursorToPState(nowCursor_);
}

void GameScene::ChangePState(PAUSE_STATE pState)
{
	pState_ = pState;
}

void GameScene::CursorToPState(int cursor)
{
	switch (cursor)
	{
	case (int)PAUSE_STATE::RESTART:
		ChangePState(PAUSE_STATE::RESTART);
		break;
	case (int)PAUSE_STATE::RETRY:
		ChangePState(PAUSE_STATE::RETRY);
		break;
	case (int)PAUSE_STATE::END:
		ChangePState(PAUSE_STATE::END);
		break;
	}
}

void GameScene::Pause(void)
{

	//マウス操作
	MouseProcess();

	//キー操作
	KeyProcess();


}

void GameScene::WarningDraw(void)
{
	//投げモノが待機状態のときに描画
	if (throwTyre_->IsIdle())
	{
		//ポーズの時は止める
		if(!isPause_)
		{
			//拡大縮小
			if (warningImgScale_ > WARNING_IMG_MAX_SCALE)
			{
				isMaxWarningScale_ = true;
			}
			else if (warningImgScale_ < WARNING_IMG_MIN_SCALE)
			{
				isMaxWarningScale_ = false;
			}

			if (isMaxWarningScale_)
			{
				warningImgScale_ -= WARNING_IMG_CHANGE_SCALE;
			}
			else
			{
				warningImgScale_ += WARNING_IMG_CHANGE_SCALE;
			}
		}

		DrawRotaGraphFastF(Application::SCREEN_SIZE_X / 2, 100, warningImgScale_, 0.0, imgWarning_, true);
	}
	else
	{
		warningImgScale_ = WARNING_IMG_MIN_SCALE;
	}
}

void GameScene::PauseDraw(void)
{
	//ポーズ背景画像
	float imgScl = 3.0f;
	DrawRotaGraphFastF(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, imgScl, 0.0f, imgPause_, true);

	//文字表示
	float fontScl = 3.0f;
	DrawExtendFormatString(reStartFontBasePos_.x, reStartFontBasePos_.y, fontScl, fontScl, reStartFontColor_, "再開");
	DrawExtendFormatString(reTryFontBasePos_.x, reTryFontBasePos_.y, fontScl, fontScl, reTryFontColor_, "リトライ");
	DrawExtendFormatString(endFontBasePos_.x, endFontBasePos_.y, fontScl, fontScl, endFontColor_, "終わる");
}

void GameScene::GoalAfterDraw(void)
{
	//FINISH文字色
	int finishFontColor_ = GetColor(255, 165, 0);

	//座標
	if(!isPause_)
	{
		float addPosY = 10;
		finishFontPos_.y += addPosY;
	}

	int stopPosY = Application::SCREEN_SIZE_Y / 2 - 120;
	if (finishFontPos_.y >= stopPosY)
	{
		finishFontPos_.y = stopPosY;
	}

	//スコア
	DrawExtendFormatString(finishFontPos_.x, finishFontPos_.y, 9, 9, finishFontColor_, "FINISH");

}
