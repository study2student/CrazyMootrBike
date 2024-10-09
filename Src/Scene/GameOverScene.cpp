#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../../Src/Object/Score.h"
#include "../../Src/Object/DataSave.h"
#include "../../Src/Object/Rider/Bike.h"
#include "GameOverScene.h"

#pragma region 定数宣言
	//リトライボタンの横の長さ
	const int RETRY_FONT_LENGTH = 200;
	//リトライボタンの高さ
	const int RETRY_FONT_HEIGHT = 48;
	//終わるボタンの横の長さ
	const int END_FONT_LENGTH = 150;
	//終わるボタンの高さ
	const int END_FONT_HEIGHT = 48;
	//選択肢数
	const int SELECT_MAX_NUM = 2;
	//プレイヤー人数
	const int PLAYER_NUM = 4;
	//プレイヤー初期値
	const float PLAYER_LOCAL_POS = 200.0f;
	//左上のリトライポジションx,y
	const int RETRY_POS_X = 580;
	const int RETRY_POS_Y = 10;
	//左上の終わるポジションx,y
	const int END_POS_X = 615;
	const int END_POS_Y = 150;
	//背景画像描画拡大描画用座標
	const int IMG_BG_POS_Y = 20;
	//スコア表示座標
	const int SCORE_STRING_POS_X = 90;
	//プレイヤーナンバー文字描画座標
	const int DRAW_PLAYER_NUM_POS_X = 340;
	const int DRAW_PLAYER_NUM_POS_Y = 342;
	//スコア表示文字描画
	const int DRAW_SCORE_POS_X = 50;
	const int DRAW_SCORE_POS_Y = 340;
	//文字描画のY座標のずらし幅
	const int SHIFT_WIDTH = 200;
	//文字描画の拡大率
	const int MAG_RATE = 10;
	
#pragma endregion


GameOverScene::GameOverScene(void)
	:
	bikes_({}),
	img_BG(0),
	imgPush_(0),
	imgSelectBG_(0),
	pState_(PAUSE_STATE::END),
	reTryFontBasePos_({}),
	endFontBasePos_({}),
	reTryFontColor_(0),
	endFontColor_(0),
	nowCursor_(0),
	isCursorHit_(false)
{
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	//背景画像
	img_BG = resMng_.Load(ResourceManager::SRC::IMG_SCORE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;
	imgSelectBG_= resMng_.Load(ResourceManager::SRC::IMG_GAMEOVER_SELECT_BG).handleId_;
	//バイクの読み込み
	for (int i = 0; i < PLAYER_NUM; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(PLAYER_LOCAL_POS * (i + 1), i));
	}
	for (auto& bike : bikes_) {
		bike->Init();
	}

	//左上のリトライポジション
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + RETRY_POS_X , Application::SCREEN_SIZE_Y / 2 + RETRY_POS_Y };

	//左上の終わるポジション
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + END_POS_X , Application::SCREEN_SIZE_Y / 2 + END_POS_Y };

}

void GameOverScene::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	bikes_[0]->Update();

	//操作
	DecideProcess();

	//キー操作
	SelectProcess();
}

void GameOverScene::Draw(void)
{
	using ap = Application;

	//背景画像描画
	DrawExtendGraph(Application::SCREEN_SIZE_X / 4, 0, ap::SCREEN_SIZE_X - ap::SCREEN_SIZE_X / 4, ap::SCREEN_SIZE_Y + IMG_BG_POS_Y, img_BG, true);

	if(data_.GetData().playerNum_ == 1)
	{

		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - SCORE_STRING_POS_X, Application::SCREEN_SIZE_Y / 2 - GetDrawFormatStringWidth("%.d"),
			10, 10, 0xff0000, "%.d", score_.GetScore());
	}
	else
	{
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - DRAW_PLAYER_NUM_POS_X, Application::SCREEN_SIZE_Y / 2 - DRAW_PLAYER_NUM_POS_Y + (0 * SHIFT_WIDTH), MAG_RATE, MAG_RATE, "P1", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - DRAW_PLAYER_NUM_POS_X, Application::SCREEN_SIZE_Y / 2 - DRAW_PLAYER_NUM_POS_Y + (1 * SHIFT_WIDTH), MAG_RATE, MAG_RATE, "P2", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - DRAW_PLAYER_NUM_POS_X, Application::SCREEN_SIZE_Y / 2 - DRAW_PLAYER_NUM_POS_Y + (2 * SHIFT_WIDTH), MAG_RATE, MAG_RATE, "P3", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - DRAW_PLAYER_NUM_POS_X, Application::SCREEN_SIZE_Y / 2 - DRAW_PLAYER_NUM_POS_Y + (3 * SHIFT_WIDTH), MAG_RATE, MAG_RATE, "P4", 0xff0000);

		for (int i = 0; i < PLAYER_NUM; i++)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - DRAW_SCORE_POS_X, Application::SCREEN_SIZE_Y / 2 - DRAW_SCORE_POS_Y + (i * SHIFT_WIDTH),
				MAG_RATE, MAG_RATE, 0xff0000, "%.d", score_.GetScoreArray()[i]);
		}
	}

	//背景
	DrawRotaGraphFastF(1600, 600, 3.0f, 0.0f, imgSelectBG_, true);

	//文字表示
	float fontScl = 3.0f;
	DrawExtendFormatString(reTryFontBasePos_.x, reTryFontBasePos_.y, fontScl, fontScl, reTryFontColor_, "リトライ");
	DrawExtendFormatString(endFontBasePos_.x, endFontBasePos_.y, fontScl, fontScl, endFontColor_, "終わる");
}

void GameOverScene::DecideProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			// 決定時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			// 決定時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

			//左クリックまたはスペースキーでタイトルへ
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	else
	{
		//ボタンにふれいない場合
		endFontColor_ = GetColor(255, 255, 255);
	}
}

void GameOverScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//カーソル番号による上下操作
	if (ins_.IsTrgDown(KEY_INPUT_UP) && nowCursor_ == SELECT_MAX_NUM - 1
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_UP) && nowCursor_ == SELECT_MAX_NUM - 1)
	{
		// 選択時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN) && nowCursor_ == 0
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_DOWN) && nowCursor_ == 0)
	{
		// 選択時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}

	//現カーソルから状態を変化
	CursorToPState(nowCursor_);
}

void GameOverScene::ChangePState(PAUSE_STATE pState)
{
	pState_ = pState;
}

void GameOverScene::CursorToPState(int cursor)
{
	switch (cursor)
	{
	case (int)PAUSE_STATE::RETRY:
		ChangePState(PAUSE_STATE::RETRY);
		break;
	case (int)PAUSE_STATE::END:
		ChangePState(PAUSE_STATE::END);
		break;
	}
}
