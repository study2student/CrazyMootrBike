#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../Utility/MyUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Stage/Planet.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Coin/CoinBase.h"
#include "../Object/Coin/GoldCoin.h"
#include "../Object/Coin/SilverCoin.h"
#include "../Object/Coin/CopperCoin.h"
#include "../Object/Gimmick/Bomb.h"
#include "../Object/Gimmick/Helicopter.h"
#include "../Object/Gimmick/Spike.h"
#include "../Object/DataSave.h"
#include "../Object/Score.h"
#include "../Application.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	coin_ = nullptr;
	stage_ = nullptr;
	helicopter_ = nullptr;
	spike_ = nullptr;
	//score_ = nullptr;

	nowCursor_ = 0;
	stepGoalAfter_ = 0.0f;
}

GameScene::~GameScene(void)
{
	delete coin_;
	delete spike_;
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
	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(PLAYER_WIDTH * (i + 1), i));
	}

	for (auto& bike : bikes_) {
		bike->Init();
	}

	// コイン
	for (auto& bike : bikes_) {
		coin_ = new CoinBase(bikes_,this, { 0.0f,800.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	//ヘリコプター
	helicopter_ = std::make_shared<Helicopter>(this);
	helicopter_->Init();

	//投げトゲ
	spike_ = new Spike();
	spike_->Init();

	// ステージ
	stage_ = std::make_shared<Stage>(bikes_, coin_, helicopter_->GetBomb(), spike_, this);
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

	// エフェクト初期化
	InitEffect();

	//エンカウントリセット
	enCounter = 0;

	//コインが生成されたか
	isCreateCoin_ = false;

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
	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) -1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}

		if (ins.IsTrgDown(KEY_INPUT_C) || ins.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START))
		{
			//ポーズ状態
			isPause_ = true;
		}
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
		FirstPersonGoal();
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
			ResourceManager::SRC::SND_GAME_BGM).handleId_, DX_PLAYTYPE_LOOP, false);

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
		}

		//ヘリ
		//先頭(座標)の要素番号取得
		size_t posZMaxIndex = GetBikeMaxPosZIndex();
		helicopter_->SetBikeTrans(bikes_[posZMaxIndex]->GetTransform());
		helicopter_->SetBikeIsOutside(bikes_[0]->GetIsOutSide());

		//投げモノ
		spike_->Update();
		spike_->SetTransform(bikes_[posZMaxIndex]->GetTransform());


		//コイン
		size_t sizeE = coins_.size();
		for (int i = 0; i < sizeE; i++)
		{
			coins_[i]->Update();
			if (coins_[i]->GetIsAddScore())
			{
				isHitStop = true;
				// ヒットエフェクト
				HitEffect();
			}
		}

		//衝突判定
		Collision();

		// バイク同士の衝突判定
		BikeCollision();

		//ステージが生成されたらコインを配置する
		if (stage_->GetIsMakeLoopStage())
		{
			CoinPlace();
		}
		else
		{
			stage_->Update();
			isCreateCoin_ = false;
		}

		helicopter_->Update();

	}
}

void GameScene::Draw(void)
{
	if (playNumber_ == 1)
	{

		DrawObject(0);

		// 各バイクを描画
		bikes_[0]->Draw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, mainScreen_, false);
		//スコア描画
		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, bikes_[0]->GetScore());
		//HP描画
		using ap = Application;
		int sc_x = ap::SCREEN_SIZE_X - 500;
		int sc_y = ap::SCREEN_SIZE_Y - 100;

		// HPバーの幅
		int HP_BAR_WIDTH = ap::SCREEN_SIZE_X - HP_BER - sc_x;
		// HPバーの高さ
		int HP_BAR_HEIGHT = ap::SCREEN_SIZE_Y - HP_BER;
		// HPバーを描画
		DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HPバーの背景
		DrawBox(sc_x, sc_y, sc_x + (bikes_[0]->GetHP() * HP_BAR_WIDTH) / Bike::MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HPバー

		// HPの黒枠
		DrawBoxAA((float)sc_x, (float)sc_y,
			(float)ap::SCREEN_SIZE_X - HP_BER, (float)HP_BAR_HEIGHT,
			0x000000, false, 13.0f);

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 50 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2 -95, 15, 15, 0xffffff, "%.f", startCount_);
		}


	}
	else
	{
		int sx = Application::SCREEN_SIZE_X;
		int sy = Application::SCREEN_SIZE_Y;

		for (int i = 0; i < cameras_.size(); i++)
		{

			DrawObject(i);

			// 各バイクを描画
			for (auto& bike : bikes_) {
				bike->Draw();
				
			}

			// Effekseerにより再生中のエフェクトを更新する。
			UpdateEffekseer3D();

			// Effekseerにより再生中のエフェクトを描画する。
			DrawEffekseer3D();

			for (int p = 0; p < bikes_.size(); p++) {
				//int score = bikes_[p]->GetScore();

				SetDrawScreen(DX_SCREEN_BACK);

				int width = 600;

				//FINISH文字色
				int finishFontColor_ = GetColor(255, 165, 0);

				//死亡文字色
				int deadFontColor_ = GetColor(255, 0, 0);

				//枠線の半分の太さ
				int halfBorderSize = 5;

				switch (i)
				{
				case 0:
					DrawGraph(0, 0, mainScreen_, false);
					DrawUI(sx / 2 - width, 0, 0);

					//エフェクトの再生
					if (bikes_[0]->IsBoost())
					{
						bikes_[0]->SyncBoostEffect(bikes_[0]->GetTransform());
					}

					//ゴール文字
					if (bikes_[0]->GetIsGoal())
					{
						//GoalAfterDraw();
						//FINISH文字描画
						Vector2 finishFontPos = { 300,200 };
						DrawExtendFormatString(finishFontPos.x, finishFontPos.y, 6, 6, finishFontColor_, "FINISH");
					}

					//死亡文字
					if (bikes_[0]->GetHP() <= 0)
					{
						//黒い背景
						DrawBox(0, 0, sx / 2 - halfBorderSize, sy / 2 - halfBorderSize, 0x000000, true);

						//死亡文字描画
						Vector2 deadFontPos = { 350,200 };
						DrawExtendFormatString(deadFontPos.x, deadFontPos.y, 6, 6, deadFontColor_, "DEAD");
					}
					break;
				case 1:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawUI(sx - width, 0, 1);

					//エフェクトの再生
					if (bikes_[1]->IsBoost())
					{
						bikes_[1]->SyncBoostEffect(bikes_[1]->GetTransform());
					}

					//ゴール文字
					if (bikes_[1]->GetIsGoal())
					{
						//GoalAfterDraw();
						//FINISH文字描画
						Vector2 finishFontPos = { 1250,200 };
						DrawExtendFormatString(finishFontPos.x, finishFontPos.y, 6, 6, finishFontColor_, "FINISH");
					}

					//死亡文字
					if (bikes_[1]->GetHP() <= 0)
					{
						//黒い背景
						DrawBox(sx / 2 + halfBorderSize, 0, sx, sy / 2 - halfBorderSize, 0x000000, true);

						//死亡文字描画
						Vector2 deadFontPos = { 1300,200 };
						DrawExtendFormatString(deadFontPos.x, deadFontPos.y, 6, 6, deadFontColor_, "DEAD");
					}
					break;
				case 2:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawUI(sx / 2 - width, sy / 2, 2);

					//エフェクトの再生
					if (bikes_[2]->IsBoost())
					{
						bikes_[2]->SyncBoostEffect(bikes_[2]->GetTransform());
					}

					//ゴール文字
					if (bikes_[2]->GetIsGoal())
					{
						//GoalAfterDraw();
						//FINISH文字描画
						Vector2 finishFontPos = { 300,700 };
						DrawExtendFormatString(finishFontPos.x, finishFontPos.y, 6, 6, finishFontColor_, "FINISH");
					}

					//死亡文字
					if (bikes_[2]->GetHP() <= 0)
					{
						//黒い背景
						DrawBox(0, sy / 2 + halfBorderSize, sx / 2 - halfBorderSize, sy, 0x000000, true);

						//死亡文字描画
						Vector2 deadFontPos = { 350,700 };
						DrawExtendFormatString(deadFontPos.x, deadFontPos.y, 6, 6, deadFontColor_, "DEAD");
					}
					break;
				case 3:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawUI(sx - width, sy / 2, 3);

					//エフェクトの再生
					if (bikes_[3]->IsBoost())
					{
						bikes_[3]->SyncBoostEffect(bikes_[3]->GetTransform());
					}

					//ゴール文字
					if (bikes_[3]->GetIsGoal())
					{
						//GoalAfterDraw();
						//FINISH文字描画
						Vector2 finishFontPos = { 1250,700 };
						DrawExtendFormatString(finishFontPos.x, finishFontPos.y, 6, 6, finishFontColor_, "FINISH");
					}

					//死亡文字
					if (bikes_[3]->GetHP() <= 0)
					{
						//黒い背景
						DrawBox(sx / 2 + halfBorderSize, sy / 2 + halfBorderSize, sx, sy , 0x000000, true);

						//死亡文字描画
						Vector2 deadFontPos = { 1300,700 };
						DrawExtendFormatString(deadFontPos.x, deadFontPos.y, 6, 6, deadFontColor_, "DEAD");
					}
					break;
				}
			}
		}

		// 横の線
		DrawLine(0, sy / 2, sx, sy / 2, 0xffff00, 8);
		// 縦の線
		DrawLine(sx / 2, 0, sx / 2, sy, 0xffff00, 8);

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{

			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 50 - GetDrawFormatStringWidth("%.f", startCount_), Application::SCREEN_SIZE_Y / 2 -95, 15, 15, 0xffffff, "%.f", startCount_);

			//DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 400, Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);

		}
		
	}
	// ヘルプ
	//DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	//DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	//DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	//DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
	//DrawDubg();

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
		else
		{
			//警告
			WarningDraw();
		}
	}
	else
	{
		bool allBikeGoal = true;
		for (auto& bike : bikes_)
		{
			// プレイヤーのHPが0でないバイクだけをゴールのチェック対象とする
			if (bike->GetHP() > 0)
			{
				// もし一つでもゴールしていないバイクがあればフラグを更新
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // ゴールしていないバイクが見つかった時点でループを抜ける
				}
			}
		}

		// 全員がゴールしたら警告停止
		if (!allBikeGoal)
		{
			//警告
			WarningDraw();
		}
		
	}

	
}

void GameScene::DrawObject(int playerID)
{
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	cameras_[playerID]->SetBeforeDraw(); // 各プレイヤーの視点を設定

	// 背景
	skyDomes_[playerID]->Draw();

	stage_->Draw();

	helicopter_->Draw();
	spike_->Draw();

	//コイン描画
	size_t sizeC = coins_.size();
	for (int i = 0; i < sizeC; i++)
	{
		if (!coins_[i]->IsDestroy())
		{
			coins_[i]->Draw();
		}
	}
}

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000, "DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000, "FPS:%f", GetFPS());
	DrawFormatString(0, 140, 0x000000, "IsHitStop:%d", isHitStop);
}

void GameScene::DrawUI(int x, int y, int playerID)
{

	using ap = Application;
	int sc_x = x - 300;
	int sc_y = y + 50;

	// HPバーの幅
	int HP_BAR_WIDTH = x - HP_BER - sc_x;
	// HPバーの高さ
	int HP_BAR_HEIGHT = y + HP_BER;
	// HPバーを描画
	DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HPバーの背景
	DrawBox(sc_x, sc_y, sc_x + (bikes_[playerID]->GetHP() * HP_BAR_WIDTH) / Bike::MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HPバー

	// HPの黒枠g
	DrawBox(sc_x, sc_y,
		x - HP_BER, HP_BAR_HEIGHT,
		0x000000, false);

	//DrawBoxAA(sc_x, sc_y,
	//	x - 10, HP_BAR_HEIGHT,
	//	0x000000, false, 13.0f);

	// スコア描画
	DrawExtendFormatString(x, y + 20, 2, 2, 0xff0000, "Player %d Score:%d", playerID + 1, bikes_[playerID]->GetScore());
}

void GameScene::FirstPersonGoal(void)
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
			// プレイヤーのHPが0でないバイクだけをゴールのチェック対象とする
			if (bike->GetHP() > 0)
			{
				// もし一つでもゴールしていないバイクがあればフラグを更新
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // ゴールしていないバイクが見つかった時点でループを抜ける
				}
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

				// プレイヤーのHPが0でないバイクだけをゴールのチェック対象とする
				if (bike->GetHP() > 0)
				{
					// もし一つでもゴールしていないバイクがあればフラグを更新
					if (!bike->GetIsGoal())
					{
						allBikeGoal = false;
						break;  // ゴールしていないバイクが見つかった時点でループを抜ける
					}
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

void GameScene::CoinPlace(void)
{
	stage_->SetMakeLoopStage(false);

	//センター方向からの横の移動幅
	float shiftX_ = {};

	//道のランダムな場所に生成(3パターン)
	int randDir = GetRand(static_cast<int>(CoinBase::DIR::MAX) - 1);
	CoinBase::DIR dir = static_cast<CoinBase::DIR>(randDir);

	Vector2 randPos;
	switch (dir)
	{
	case CoinBase::DIR::LEFT:
		shiftX_ = -CoinBase::DIR_LEN;
		break;
	case CoinBase::DIR::CENTER:
		shiftX_ = 0.0f;
		break;
	case CoinBase::DIR::RIGHT:
		shiftX_ = CoinBase::DIR_LEN;
		break;
	}

	//縦にコインを生成する
	for (int i = 0; i < CoinBase::MAX_MAKE_NUM; i++)
	{
		//縦に並ぶコインとコインの距離
		float len = CoinBase::X_LEN;

		//コインの生成
		CoinBase* c = nullptr;
		int eType = GetRand(static_cast<int>(CoinBase::TYPE::MAX) - 1);
		CoinBase::TYPE type = static_cast<CoinBase::TYPE>(eType);
		for (auto& bike : bikes_) {
			switch (type)
			{
			case CoinBase::TYPE::GOLD:
				c = new GoldCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::SILVER:
				c = new SilverCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::COPPER:
				c = new CopperCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			}
		}
		c->Init();

		isCreateCoin_ = true;

		//可変長配列に要素を追加
		coins_.push_back(c);
	}
}

std::vector<CoinBase*> GameScene::GetEnemys(void)
{
	return coins_;
}


bool GameScene::GetIsCreateEnemy(void)
{
	return isCreateCoin_;
}

int GameScene::GetPlayNum(void)
{
	return playNumber_;
}

bool GameScene::OnePersonIsGoal(void)
{
	return onePersonIsGoal_;
}

size_t GameScene::GetBikeMaxPosZIndex(void)
{
	//pos.zが一番大きいBike要素を取得
	auto posZMaxElementIt = std::max_element(bikes_.begin(), bikes_.end(), [](const std::shared_ptr<Bike>& a, const std::shared_ptr<Bike>& b) {
		return a->GetTransform().pos.z < b->GetTransform().pos.z; });  // 比較の基準としてpos.zを使う

	//pos.z が最大の要素のインデックスを取得する
	size_t posZMaxIndex = std::distance(bikes_.begin(), posZMaxElementIt);

	return posZMaxIndex;
}

void GameScene::Collision(void)
{

	//爆弾とプレイヤーの当たり判定、投げモノとプレイヤーの判定
	//HPが減り続けてしまうので当たった時は処理中断

	for (const auto& bike : bikes_)
	{
		auto bikeCap = bike->GetCapsule();
		if(!helicopter_->GetBomb()->GetIsCol())
		{
			//爆弾
			auto bombCap = helicopter_->GetBomb()->GetCapsule();

			VECTOR diffB = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disB = MyUtility::SqrMagnitudeF(diffB);
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

				//コントローラーを振動
				StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

				// 効果音再生
				PlaySoundMem(ResourceManager::GetInstance().Load(
					ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, false);
			}
		}

		if (!spike_->GetIsCol())
		{

			//投げモノ
			auto throwCap = spike_->GetCapsule();

			VECTOR diffT = VSub(throwCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disT = MyUtility::SqrMagnitudeF(diffT);
			if (disT < throwCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//ゴールしてない場合
					if (!stage_->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(spike_->THROW_DAMAGE);

					}
				}
				else
				{
					//ゴールしてないプレイヤーにだけダメージ
					if (!bike->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(spike_->THROW_DAMAGE);
					}
				}

				//当たった
				spike_->SetIsCol(true);

				//コントローラーを振動
				StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

				// 効果音再生
				PlaySoundMem(ResourceManager::GetInstance().Load(
					ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, false);
			}
		}

		
		//死亡処理
		//ゲームオーバーシーンで描画するため保存しておく
		if (playNumber_ == 1)
		{
			//ゲームオーバー処理
			if (bikes_[0]->GetHP() <= 0)
			{
				//スコアを保持
				score_.ScoreSet(bikes_[0]->GetScore());
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
		}
		else
		{
			bool allBikeDead = true;
			for (auto& bike : bikes_)
			{
			
				// もし一つでも死亡していないバイクがあればフラグを更新
				if (!(bike->GetHP() <= 0))
				{
					allBikeDead = false;
					break;  // 死亡していないバイクが見つかった時点でループを抜ける
				}
			}

			// 全員が死亡した場合にシーンを切り替える
			if (allBikeDead)
			{
				for (auto& bike : bikes_)
				{
					//スコアを保持
					score_.ScoreSetArray(bike->GetScore());
				}
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
		}
	}
}

void GameScene::BikeCollision(void)
{
	//バイク同士の当たり判定(弾く)
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
			float  dis = MyUtility::SqrMagnitudeF(diff);
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
	//effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
	//float scale = 50.0f;
	//for (auto& enemy : enemys_) {
	//	const auto ePos = enemy->GetTransform();
	//	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, ePos.pos.x, ePos.pos.y, ePos.pos.z + 500);
	//	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, ePos.rot.x, ePos.rot.y, ePos.rot.z);
	//	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	//}
}

void GameScene::DecideProcess(void)
{

	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();


	
	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}


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
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
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
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
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
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) 
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
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
			if (ins_.IsTrgDown(KEY_INPUT_C) || ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START) && isPause_)
			{
				//ポーズ解除
				stepPauseKeyHit_ = 0.0f;
				isPause_ = false;
			}

		}
	}


}

void GameScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();



	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}

		//カーソル番号による上下操作
		if (ins_.IsTrgDown(KEY_INPUT_UP) || ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_UP))
		{
			nowCursor_--;
			if (nowCursor_ <= 0)
			{
				nowCursor_ = 0;
			}
		}
		if (ins_.IsTrgDown(KEY_INPUT_DOWN) || ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_DOWN))
		{
			nowCursor_++;
			if (nowCursor_ >= SELECT_MAX_NUM - 1)
			{
				nowCursor_ = SELECT_MAX_NUM - 1;
			}
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
	DecideProcess();

	//キー操作
	SelectProcess();


}

void GameScene::WarningDraw(void)
{
	//投げモノが待機状態のときに描画
	if (spike_->IsIdle())
	{
		//警告音
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_WARNING).handleId_, DX_PLAYTYPE_LOOP, false);

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

		DrawRotaGraphFastF((float)Application::SCREEN_SIZE_X / 2, WARNING_POS_Y, warningImgScale_, 0.0, imgWarning_, true);
	}
	else
	{
		warningImgScale_ = WARNING_IMG_MIN_SCALE;
		StopSoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_WARNING).handleId_);
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
		int addPosY = 10;
		finishFontPos_.y += addPosY;
	}

	float stopPosY = Application::SCREEN_SIZE_Y / 2 - WARNING_POS_Y;
	if (finishFontPos_.y >= stopPosY)
	{
		finishFontPos_.y = stopPosY;
	}

	//FINISH文字描画
	DrawExtendFormatString(finishFontPos_.x, finishFontPos_.y, 9, 9, finishFontColor_, "FINISH");

}
