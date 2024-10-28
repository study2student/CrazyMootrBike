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
#include "../Object/Common/Pause.h"
#include "GameScene.h"

#pragma region 定数宣言

	//プレイヤーID 1～4
	const int PLAYER_ID_1 = 0;
	const int PLAYER_ID_2 = 1;
	const int PLAYER_ID_3 = 2;
	const int PLAYER_ID_4 = 3;

	//ひとりプレイ用
	const int ONE_PLAYER = 1;

	//フォントサイズ
	const int FONT_SIZE = 16;

	//プレイ人数の最大数
	const int MAX_PLAYER_NUM = 4;

	//プレイヤー同士の初期幅
	const float PLAYER_WIDTH = 200.0f;

	//コインとステージとのローカル座標
	const VECTOR LOCAL_STAGE_POS = { 0.0f,800.0f,0.0f };

	//Finish文字の初期Y座標
	const int FINISH_START_POS_Y = -40;

	//BGMの音量
	const int BGM_VOLUME = 125;

	//スコア描画のY座標
	const int SCORE_POS_Y = 10;

	//スコア描画文字の大きさ
	const double SCORE_SIZE = 3.0;

	//コイン画像描画のX座標
	const int COIN_IMG_POS_X = 250;

	//コイン画像描画のY座標
	const int COIN_IMG_POS_Y = 40;

	//コイン画像の大きさ
	const double COIN_IMG_SIZE = 0.3;

	//HPバー描画時のローカル座標
	const int HP_BAR_LOCAL_X = 500;
	const int HP_BAR_LOCAL_Y = 100;

	//警告画像最大サイズ
	const float WARNING_IMG_MAX_SCALE = 0.40f;

	//警告画像最小サイズ
	const float WARNING_IMG_MIN_SCALE = 0.20f;

	//警告画像大きさ変化量
	const float WARNING_IMG_CHANGE_SCALE = 0.005f;

	//警告画像Y座標
	const float WARNING_POS_Y = 120.0f;

	//スタート時のカウントの初期値
	const float START_COUNT = 3.0f;

	//スタート時の文字の描画位置,大きさ
	const int START_COUNT_POS_X = 50;
	const int START_COUNT_POS_Y = 95;
	const double START_COUNT_SIZE = 15.0;

	//HPの黒枠の太さ
	const float DRAW_HP_FRAME_THINCKNESS = 13.0f;

	//HP描画の1枠の横の大きさ
	const int DRAW_HP_SIZE_X = 600;

	//画面分割の縦、横の線の太さ
	const int DIVISION_LINE_THICKNESS = 8;

	//四人対戦時のコイン画像の描画位置
	const int COIN_FOUR_IMG_POS_X = 140;
	const int COIN_FOUR_IMG_POS_Y = 31;

	//四人対戦時のHP描画の位置
	const int DRAW_FOUR_HP_POS_X = 300;
	const int DRAW_FOUR_HP_POS_Y = 50;

	//四人対戦時のスコア描画
	const int DRAW_FOUR_SCORE_POS_X = 250;
	const int DRAW_FOUR_SCORE_POS_Y = 20;

	//四人対戦時のスコア描画の大きさ
	const double DRAW_SCORE_SIZE = 2.0;

	//一人用のFINISH画像の描画
	const int FINISH_IMG_MOVE_POS_Y = 10;

	//一人用のFINISH画像の描画を止める位置
	const float FINISH_IMG_STOP_POS_Y = 40.0f;

	//一人用のFINISH画像の描画時の大きさ
	const double FINISH_IMG_SIZE = 1.5;

	//四人用のFINISH画像の描画時の大きさ
	const double FINISH_FOUR_IMG_SIZE = 1.0;

	//ゴールしてから次のシーンになるまでの時間
	const float GOAL_TO_NEXT_SCENE = 2.5f;

	// HPバーの幅と高さ
	const int HP_BER = 10;

	// コントローラーの揺れ値
	const int CE_SWING_VALUE = 1000;

	// コントローラーの揺れ時間
	const int CE_SWING_TIME = 700;

	//4人対戦用のゴール文字位置
	//プレイヤー1
	const Vector2 FINISH_FONT_POS_MULTI_P1 = { 450,250 };

	//プレイヤー2
	const Vector2 FINISH_FONT_POS_MULTI_P2 = { 1400,250 };

	//プレイヤー3
	const Vector2 FINISH_FONT_POS_MULTI_P3 = { 450,750 };

	//プレイヤー4
	const Vector2 FINISH_FONT_POS_MULTI_P4 = { 1400,750 };

	//画面分割枠線の半分の太さ
	const int HALF_BORDER_SIZE = 5;

	//死亡黒背景の左上と右下座標と文字の位置
	//プレイヤー1
	const Vector2 DEAD_BACK_BOX_MIN_POS_P1 = { 0, 0 };
	const Vector2 DEAD_BACK_BOX_MAX_POS_P1 = { Application::SCREEN_SIZE_X / 2 - HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y / 2 - HALF_BORDER_SIZE };
	const Vector2 DEAD_FONT_POS_P1 = { 350,200 };

	//プレイヤー2
	const Vector2 DEAD_BACK_BOX_MIN_POS_P2 = { Application::SCREEN_SIZE_X / 2 + HALF_BORDER_SIZE, 0 };
	const Vector2 DEAD_BACK_BOX_MAX_POS_P2 = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2 - HALF_BORDER_SIZE };
	const Vector2 DEAD_FONT_POS_P2 = { 1300,200 };

	//プレイヤー3
	const Vector2 DEAD_BACK_BOX_MIN_POS_P3 = { 0, Application::SCREEN_SIZE_Y / 2 + HALF_BORDER_SIZE };
	const Vector2 DEAD_BACK_BOX_MAX_POS_P3 = { Application::SCREEN_SIZE_X / 2 - HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y};
	const Vector2 DEAD_FONT_POS_P3 = { 350,700 };

	//プレイヤー4
	const Vector2 DEAD_BACK_BOX_MIN_POS_P4 = { Application::SCREEN_SIZE_X / 2 + HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y / 2 + HALF_BORDER_SIZE };
	const Vector2 DEAD_BACK_BOX_MAX_POS_P4 = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };
	const Vector2 DEAD_FONT_POS_P4 = { 1300,700 };

	//死亡文字大きさ
	const double DEAD_FONT_EXRATE = 6.0;

	//死亡文字
	const std::string DEAD_FONT = "DEAD";

	//死亡背景色
	const unsigned int DEAD_BACK_BOX_COLOR = GetColor(0, 0, 0);

	//死亡文字色
	const unsigned int DEAD_FONT_COLOR = GetColor(255, 0, 0);

	//四人用コイン画像の大きさ
	const double COIN_FOUR_IMG_SIZE = 0.15;
#pragma endregion


GameScene::GameScene(void)
	:
	mainScreen_(0),
	onePersonIsGoal_(false),
	startCount_(0.0f),
	isStart_(false),
	imgWarning_(0),
	warningImgScale_(0.0f),
	isMaxWarningScale_(false),
	imgPause_(0),
	imgFinish_(0),
	imgCoin_(0),
	cameras_({}),
	stage_(nullptr),
	skyDomes_(0),
	spike_(nullptr),
	bikes_({}),
	helicopter_(nullptr),
	coin_(nullptr),
	coins_({}),
	enCounter(0),
	isCreateCoin_(false),
	pause_(nullptr),
	hitStopDuration(0.0f),
	hitStopTimer(0.0f),
	isHitStop(false),
	playNumber_(0),
	isPause_(false),
	finishImgMovePos_({}),
	stepGoalAfter_(0.0f)
{
}

GameScene::~GameScene(void)
{
	delete coin_;
}

void GameScene::Init(void)
{
	// DrawString で描画する文字列の大きさを設定
	SetFontSize(FONT_SIZE);
	// DrawString で描画するフォントを変更
	ChangeFont("Nikkyou Sans");

	//プレイヤー人数
	playNumber_ = data_.GetData().playerNum_;
	if (playNumber_ == ONE_PLAYER)
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
	}
	else
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	}

	// プレイヤー
	for (int i = 0; i < MAX_PLAYER_NUM; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(PLAYER_WIDTH * (i + 1), i));
	}

	//プレイヤー初期化
	for (auto& bike : bikes_) {
		bike->Init();
	}

	// コイン
	for (auto& bike : bikes_) {
		coin_ = new CoinBase(bikes_,this, LOCAL_STAGE_POS, MyUtility::VECTOR_ZERO);
	}

	//ヘリコプター
	helicopter_ = std::make_shared<Helicopter>(this);
	helicopter_->Init();

	//投げトゲ
	spike_ = std::make_shared<Spike>();
	spike_->Init();

	// ステージ
	stage_ = std::make_shared<Stage>(bikes_, coin_, helicopter_->GetBomb(), spike_,this);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	for (auto& bike : bikes_) {
		auto sky = std::make_unique<SkyDome>(bike->GetTransform());
		sky->Init();
		skyDomes_.emplace_back(std::move(sky));
	}

	if (playNumber_ == ONE_PLAYER)
	{
		cameras_.push_back(std::make_shared<Camera>());
	}
	else
	{
		// 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); 
		cameras_.push_back(std::make_shared<Camera>());
		cameras_.push_back(std::make_shared<Camera>());
		cameras_.push_back(std::make_shared<Camera>());
	}

	// 各プレイヤーのバイクを追従
	for (int i = 0; i < cameras_.size(); i++) {
		cameras_[i]->Init();
		cameras_[i]->ChangeMode(Camera::MODE::FOLLOW);
		cameras_[i]->SetFollow(&bikes_[i]->GetTransform()); 
	}

	//ポーズ
	pause_ = std::make_unique<Pause>();
	pause_->Init();

	//エンカウントリセット
	enCounter = 0;

	//コインが生成されたか
	isCreateCoin_ = false;

	//FINISH文字の初期位置
	Vector2 finishStartPos = { Application::SCREEN_SIZE_X / 2, FINISH_START_POS_Y };
	finishImgMovePos_ = finishStartPos;

	isPause_ = false;

	onePersonIsGoal_ = false;

	//画像
	imgWarning_= resMng_.Load(ResourceManager::SRC::WARNING).handleId_;
	warningImgScale_ = WARNING_IMG_MIN_SCALE;

	//ポーズ背景
	//読み込みと設定
	imgPause_ = resMng_.Load(ResourceManager::SRC::PAUSE).handleId_;
	pause_->SetImgHandle(imgPause_);

	//FINISH文字とコイン画像
	imgFinish_= resMng_.Load(ResourceManager::SRC::IMG_FINISH).handleId_;
	imgCoin_= resMng_.Load(ResourceManager::SRC::IMG_COIN).handleId_;

	//スコアリセット
	score_.ResetScore();

	// ゲームスタート時のカウント
	startCount_ = START_COUNT;
	isStart_ = false;

	//エンジン音
	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false);
}

void GameScene::Update(void)
{

	//ポーズを開く
	pause_->PausePrevious();

	//ポーズ状態かどうか取得
	isPause_ = pause_->GetIsPause();
	
	//ポーズ中はカメラの回転をさせないために情報を渡しておく
	SceneManager::GetInstance().GetCamera()->SetIsPause(isPause_);

	//ポーズ中は他処理中断
	if (isPause_)
	{
		//ポーズ中処理
		pause_->PauseMidst();
		return;
	}

	//最初の人がゴールしたら
	if (stage_->GetIsGoal())
	{
		GoalProcess();
	}
	
	//1人プレイ時ゴールしたかどうかセット
	onePersonIsGoal_ = stage_->GetIsGoal();

	// スタート時のカウントダウンを減らす
	if (startCount_ > 0.0f)
	{
		startCount_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	if (startCount_ <= 0.0f)
	{
		float deltaTime = hitStopDuration;

		//BGMを再生
		ChangeVolumeSoundMem(BGM_VOLUME,PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_GAME_BGM).handleId_, DX_PLAYTYPE_LOOP, false));

		for (auto& skyDome : skyDomes_)
		{
			skyDome->Update();
		}

		if (!isHitStop)
		{
			if (playNumber_ == ONE_PLAYER)
			{
				bikes_[PLAYER_ID_1]->Update();
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
		helicopter_->SetBikeIsOutside(bikes_[PLAYER_ID_1]->GetIsOutSide());

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
	if (playNumber_ == ONE_PLAYER)
	{
		// カメラやスクリーン表示
		DrawObject(PLAYER_ID_1);

		// 各バイクを描画
		bikes_[PLAYER_ID_1]->Draw();

		// Effekseerにより再生中のエフェクトを更新する。
		UpdateEffekseer3D();

		// Effekseerにより再生中のエフェクトを描画する。
		DrawEffekseer3D();

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, mainScreen_, false);
		using ap = Application;
		//スコア描画
		DrawExtendFormatString(ap::SCREEN_SIZE_X / 2, SCORE_POS_Y, SCORE_SIZE, SCORE_SIZE, 0xff0000, "Player     :%d", bikes_[0]->GetScore());

		//コイン描画
		DrawRotaGraph(ap::SCREEN_SIZE_X / 2 + COIN_IMG_POS_X, COIN_IMG_POS_Y, COIN_IMG_SIZE, 0.0, imgCoin_, true);

		//HP描画
		int sc_x = ap::SCREEN_SIZE_X - HP_BAR_LOCAL_X;
		int sc_y = ap::SCREEN_SIZE_Y - HP_BAR_LOCAL_Y;

		// HPバーの幅
		int HP_BAR_WIDTH = ap::SCREEN_SIZE_X - HP_BER - sc_x;
		// HPバーの高さ
		int HP_BAR_HEIGHT = ap::SCREEN_SIZE_Y - HP_BER;
		// HPバーを描画
		DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HPバーの背景
		DrawBox(sc_x, sc_y, sc_x + (bikes_[PLAYER_ID_1]->GetHP() * HP_BAR_WIDTH) / Bike::MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HPバー

		// HPの黒枠
		DrawBoxAA((float)sc_x, (float)sc_y,
			(float)ap::SCREEN_SIZE_X - HP_BER, (float)HP_BAR_HEIGHT,
			0x000000, false, DRAW_HP_FRAME_THINCKNESS);

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(ap::SCREEN_SIZE_X / 2 - START_COUNT_POS_X - GetDrawFormatStringWidth("%.f"), ap::SCREEN_SIZE_Y / 2 - START_COUNT_POS_Y, START_COUNT_SIZE, START_COUNT_SIZE, 0xffffff, "%.f", startCount_);
		}

	}
	else
	{
		//スクリーンサイズ
		int sx = Application::SCREEN_SIZE_X;
		int sy = Application::SCREEN_SIZE_Y;

		for (int i = 0; i < cameras_.size(); i++)
		{

			// カメラやスクリーンを描画
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

				SetDrawScreen(DX_SCREEN_BACK);

				switch (i)
				{
				case PLAYER_ID_1:
					DrawGraph(0, 0, mainScreen_, false);
					DrawUI(sx / 2 - DRAW_HP_SIZE_X, 0, 0);
					CoinImgDraw(sx / 2 - COIN_FOUR_IMG_POS_X, COIN_FOUR_IMG_POS_Y);

					//エフェクトの再生
					if (bikes_[PLAYER_ID_1]->GetIsBoost())
					{
						bikes_[PLAYER_ID_1]->SyncBoostEffect(bikes_[PLAYER_ID_1]->GetTransform());
					}

					//ゴール文字
					if (bikes_[PLAYER_ID_1]->GetIsGoal())
					{
						//FINISH文字描画
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P1);
					}

					//死亡文字
					if (bikes_[PLAYER_ID_1]->GetHP() <= 0)
					{
						//死亡文字描画
						DeadAfterDraw(DEAD_FONT_POS_P1, DEAD_BACK_BOX_MIN_POS_P1, DEAD_BACK_BOX_MAX_POS_P1);
					}
					break;
				case PLAYER_ID_2:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawUI(sx - DRAW_HP_SIZE_X, 0, 1);
					CoinImgDraw(sx - COIN_FOUR_IMG_POS_X, COIN_FOUR_IMG_POS_Y);

					//エフェクトの再生
					if (bikes_[PLAYER_ID_2]->GetIsBoost())
					{
						bikes_[PLAYER_ID_2]->SyncBoostEffect(bikes_[PLAYER_ID_2]->GetTransform());
					}

					//ゴール文字
					if (bikes_[PLAYER_ID_2]->GetIsGoal())
					{
						//FINISH文字描画
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P2);
					}

					//死亡文字
					if (bikes_[PLAYER_ID_2]->GetHP() <= 0)
					{
						//死亡文字描画
						DeadAfterDraw(DEAD_FONT_POS_P2, DEAD_BACK_BOX_MIN_POS_P2, DEAD_BACK_BOX_MAX_POS_P2);
					}
					break;
				case PLAYER_ID_3:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawUI(sx / 2 - DRAW_HP_SIZE_X, sy / 2, 2);
					CoinImgDraw(sx / 2 - COIN_FOUR_IMG_POS_X, sy / 2 + COIN_FOUR_IMG_POS_Y);

					//エフェクトの再生
					if (bikes_[PLAYER_ID_3]->GetIsBoost())
					{
						bikes_[PLAYER_ID_3]->SyncBoostEffect(bikes_[PLAYER_ID_3]->GetTransform());
					}

					//ゴール文字
					if (bikes_[PLAYER_ID_3]->GetIsGoal())
					{
						//FINISH文字描画
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P3);
					}

					//死亡文字
					if (bikes_[PLAYER_ID_3]->GetHP() <= 0)
					{
						//死亡文字描画
						DeadAfterDraw(DEAD_FONT_POS_P3, DEAD_BACK_BOX_MIN_POS_P3, DEAD_BACK_BOX_MAX_POS_P3);
					}
					break;
				case PLAYER_ID_4:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawUI(sx - DRAW_HP_SIZE_X, sy / 2, 3);
					CoinImgDraw(sx - COIN_FOUR_IMG_POS_X, sy / 2 + COIN_FOUR_IMG_POS_Y);

					//エフェクトの再生
					if (bikes_[PLAYER_ID_4]->GetIsBoost())
					{
						bikes_[PLAYER_ID_4]->SyncBoostEffect(bikes_[PLAYER_ID_4]->GetTransform());
					}

					//ゴール文字
					if (bikes_[PLAYER_ID_4]->GetIsGoal())
					{
						//FINISH文字描画
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P4);
					}

					//死亡文字
					if (bikes_[PLAYER_ID_4]->GetHP() <= 0)
					{
						//死亡文字描画
						DeadAfterDraw(DEAD_FONT_POS_P4, DEAD_BACK_BOX_MIN_POS_P4, DEAD_BACK_BOX_MAX_POS_P4);
					}
					break;
				}
			}
		}

		// 分割横の線
		DrawLine(0, sy / 2, sx, sy / 2, 0xffff00, DIVISION_LINE_THICKNESS);
		// 分割縦の線
		DrawLine(sx / 2, 0, sx / 2, sy, 0xffff00, DIVISION_LINE_THICKNESS);

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - START_COUNT_POS_Y - GetDrawFormatStringWidth("%.f", startCount_), Application::SCREEN_SIZE_Y / 2 - START_COUNT_POS_Y, START_COUNT_SIZE, START_COUNT_SIZE, 0xffffff, "%.f", startCount_);
		}
	}

	//ゴールしたら文字出現
	if (playNumber_ == ONE_PLAYER)
	{
		if (stage_->GetIsGoal())
		{
			Vector2 noUse = {};
			GoalAfterDraw(playNumber_, noUse);
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

	//ポーズ状態かどうか取得
	bool isPause = pause_->GetIsPause();

	//ポーズ中
	if (isPause)
	{
		pause_->Draw();
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

void GameScene::DrawUI(int x, int y, int playerID)
{
	using ap = Application;
	int sc_x = x - DRAW_FOUR_HP_POS_X;
	int sc_y = y + DRAW_FOUR_HP_POS_Y;

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

	// スコア描画
	DrawExtendFormatString(x + DRAW_FOUR_SCORE_POS_X, y + DRAW_FOUR_SCORE_POS_Y, DRAW_SCORE_SIZE, DRAW_SCORE_SIZE, 0xff0000, "Player %d     :%d", playerID + 1, bikes_[playerID]->GetScore());
}

void GameScene::GoalProcess(void)
{
	//ゲームオーバーシーンで描画するため保存しておく
	if (playNumber_ == ONE_PLAYER)
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
		if (playNumber_ == ONE_PLAYER)
		{
			score_.ScoreSet(bikes_[PLAYER_ID_1]->GetScore());
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
				//金コイン配置
				c = new GoldCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::SILVER:
				//銀コイン配置
				c = new SilverCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::COPPER:
				//銅コイン配置
				c = new CopperCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			}
		}
		c->Init();

		isCreateCoin_ = true;

		//可変長配列に要素を追加
		coins_.emplace_back(c);
	}
}

std::vector<CoinBase*> GameScene::GetCoins(void)
{
	return coins_;
}

bool GameScene::GetIsCreateCoin(void)
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

	//爆弾
	for (const auto& bike : bikes_)
	{
		auto bikeCap = bike->GetCapsule();
		if(!(helicopter_->GetBomb()->GetState()== Bomb::STATE::IDLE))
		{
			if (!helicopter_->GetBomb()->GetIsCol())
			{
				Capsule bombCap = helicopter_->GetBomb()->GetCapsule();
				
				VECTOR diffB = VSub(bombCap.GetCenter(), bikeCap.lock()->GetCenter());
				float  disB = MyUtility::SqrMagnitudeF(diffB);
				if (disB < bombCap.GetRadius() * bikeCap.lock()->GetRadius())
				{
					if (playNumber_ == ONE_PLAYER)
					{
						//ゴールしてない場合
						if (!stage_->GetIsGoal())
						{
							//プレイヤーにダメージ
							bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

							//コントローラー振動
							StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

						}
					}
					else
					{
						//ゴールしてないプレイヤーにだけダメージ
						if (!bike->GetIsGoal())
						{
							//プレイヤーにダメージ
							bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);


							//ぶつかったプレイヤーのコントローラーを振動
							// 衝突したバイクのインデックスを取得
							auto it = std::find(bikes_.begin(), bikes_.end(), bike);
							int bikeIndex = std::distance(bikes_.begin(), it);
							int pad = DX_INPUT_PAD1 + bikeIndex;
							StartJoypadVibration(pad, CE_SWING_VALUE, CE_SWING_TIME, -1);
						}
					}

					//当たった
					helicopter_->GetBomb()->SetIsCol(true);

					// 効果音再生
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}
		}

		//投げモノ
		if (!spike_->GetIsCol())
		{

			Capsule throwCap = spike_->GetCapsule();

			VECTOR diffT = VSub(throwCap.GetCenter(), bikeCap.lock()->GetCenter());
			float  disT = MyUtility::SqrMagnitudeF(diffT);
			if (disT < throwCap.GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == ONE_PLAYER)
				{
					//ゴールしてない場合
					if (!stage_->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(spike_->SPIKE_DAMAGE);

						//コントローラー振動
						StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

					}
				}
				else
				{
					//ゴールしてないプレイヤーにだけダメージ
					if (!bike->GetIsGoal())
					{
						//プレイヤーにダメージ
						bike->Damage(spike_->SPIKE_DAMAGE);

						//ぶつかったプレイヤーのコントローラーを振動
						// 衝突したバイクのインデックスを取得
						auto it = std::find(bikes_.begin(), bikes_.end(), bike);
						int bikeIndex = std::distance(bikes_.begin(), it);
						int pad = DX_INPUT_PAD1 + bikeIndex;
						StartJoypadVibration(pad, CE_SWING_VALUE, CE_SWING_TIME, -1);
					}
				}

				//当たった
				spike_->SetIsCol(true);

				// 効果音再生
				PlaySoundMem(ResourceManager::GetInstance().Load(
					ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, true);
			}
		}

		//死亡処理
		//ゲームオーバーシーンで描画するため保存しておく
		if (playNumber_ == ONE_PLAYER)
		{
			//ゲームオーバー処理
			if (bikes_[PLAYER_ID_1]->GetHP() <= 0)
			{
				//スコアを保持
				score_.ScoreSet(bikes_[PLAYER_ID_1]->GetScore());
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

void GameScene::GoalAfterDraw(int playNum, Vector2 drawPos)
{
	//1人用
	if (playNum == ONE_PLAYER)
	{
		//座標
		if (!isPause_)
		{
			finishImgMovePos_.y += FINISH_IMG_MOVE_POS_Y;
		}

		float stopPosY = Application::SCREEN_SIZE_Y / 2 - FINISH_IMG_STOP_POS_Y;
		if (finishImgMovePos_.y >= stopPosY)
		{
			finishImgMovePos_.y = stopPosY;
		}

		//FINISH文字描画
		DrawRotaGraph(finishImgMovePos_.x, finishImgMovePos_.y, FINISH_IMG_SIZE, 0.0, imgFinish_, true);
	}
	//4人用
	else
	{
		DrawRotaGraph(drawPos.x, drawPos.y, FINISH_FOUR_IMG_SIZE, 0.0, imgFinish_, true);
	}

}

void GameScene::DeadAfterDraw(Vector2 drawPos, Vector2 boxMinPos, Vector2 boxMaxPos)
{

	//黒い背景
	DrawBox(boxMinPos.x, boxMinPos.y, boxMaxPos.x, boxMaxPos.y, DEAD_BACK_BOX_COLOR, true);

	//死亡文字
	DrawExtendFormatString(drawPos.x, drawPos.y, DEAD_FONT_EXRATE, DEAD_FONT_EXRATE, DEAD_FONT_COLOR, DEAD_FONT.c_str());

}

void GameScene::CoinImgDraw(int x, int y)
{
	DrawRotaGraph(x, y, COIN_FOUR_IMG_SIZE, 0.0, imgCoin_, true);
}
