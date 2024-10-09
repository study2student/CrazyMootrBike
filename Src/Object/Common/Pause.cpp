#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Object/Common/Capsule.h"
#include "../../Object/DataSave.h"
#include "../../Object/Score.h"
#include "../../Application.h"
#include "../../Scene/GameScene.h"
#include "Pause.h"

#pragma region 定数宣言

//パッド最大数
const int PAD_MAX = 4;

//再開文字色
const int RESTART_FONT_COLOR = GetColor(255, 255, 255);

//リトライ文字色
const int RETRY_FONT_COLOR = GetColor(255, 255, 255);

//終わる文字色
const int END_FONT_COLOR = GetColor(255, 255, 255);

//左上の再開ポジション
const Vector2 RESTART_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 55 , Application::SCREEN_SIZE_Y / 2 - 130 };

//左上のリトライポジション
const Vector2 RETRY_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 95 , Application::SCREEN_SIZE_Y / 2 + 10 };

//左上の終わるポジション
const Vector2 END_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 70 , Application::SCREEN_SIZE_Y / 2 + 150 };

//再開ボタンの横の長さ
const int RESTART_FONT_LENGTH = 100;

//再開ボタンの高さ
const int RESTART_FONT_HEIGHT = 48;

//リトライボタンの横の長さ
const int RETRY_FONT_LENGTH = 200;

//リトライボタンの高さ
const int RETRY_FONT_HEIGHT = 48;

//終わるボタンの横の長さ
const int END_FONT_LENGTH = 150;

//終わるボタンの高さ
const int END_FONT_HEIGHT = 48;

//選択肢数
const int SELECT_MAX_NUM = 3;

//ポーズキー入力からもう一度押せるようになるまでの時間
const float PAUSE_KEY_HIT_MAX_TIME = 1.25f;

#pragma endregion


Pause::Pause(void)
	:
	nowCursor_(0),
	imgPause_(0),
	reStartFontBasePos_({}),
	reTryFontBasePos_({}),
	endFontBasePos_({}),
	reStartFontColor_(0),
	reTryFontColor_(0),
	endFontColor_(0),
	isPause_(false),
	isCursorHit_(false),
	stepPauseKeyHit_(0.0f)
{
}

Pause::~Pause(void)
{
}

void Pause::Init(void)
{
	//ポーズメニュー
	//色
	reStartFontColor_ = RESTART_FONT_COLOR;
	reTryFontColor_ = RETRY_FONT_COLOR;
	endFontColor_ = END_FONT_COLOR;

	//左上の再開ポジション
	reStartFontBasePos_ = RESTART_FONT_BASE_POS;

	//左上のリトライポジション
	reTryFontBasePos_ = RETRY_FONT_BASE_POS;

	//左上の終わるポジション
	endFontBasePos_ = END_FONT_BASE_POS;

	//初期状態
	state_ = STATE::RESTART;
}

void Pause::Update(void)
{
}

void Pause::Draw(void)
{
	//ポーズ文字描画
	PauseDraw();
}

const bool& Pause::GetIsPause(void) const
{
	return isPause_;
}

void Pause::PausePrevious(void)
{
	InputManager& ins = InputManager::GetInstance();

	//ポーズメニューを開く
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

		if (ins.IsTrgDown(KEY_INPUT_C) || ins.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START))
		{
			//ポーズ状態
			isPause_ = true;
		}
	}
}

void Pause::PauseMidst(void)
{
	//マウス操作
	DecideProcess();

	//キー操作
	SelectProcess();
}

void Pause::SetImgHandle(const int& handle)
{
	imgPause_ = handle;
}

void Pause::DecideProcess(void)
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
			nowCursor_ = (int)STATE::RESTART;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;

		}

		if (state_ == STATE::RESTART)
		{
			//ボタンにふれている場合
			reStartFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// 決定時の音を再生
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

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
			nowCursor_ = (int)STATE::RETRY;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;
		}

		if (state_ == STATE::RETRY)
		{
			//ボタンにふれている場合
			reTryFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
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
			nowCursor_ = (int)STATE::END;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;

		}

		if (state_ == STATE::END)
		{
			//ボタンにふれている場合
			endFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
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

void Pause::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//PC
	//カーソル番号による上下操作
	if (ins_.IsTrgDown(KEY_INPUT_UP) && !(nowCursor_ == 0))
	{
		// 選択時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
	{
		// 選択時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}


	//PAD
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
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_UP) && !(nowCursor_ == 0))
		{
			// 選択時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_--;
			if (nowCursor_ <= 0)
			{
				nowCursor_ = 0;
			}
		}
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
		{
			// 選択時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_++;
			if (nowCursor_ >= SELECT_MAX_NUM - 1)
			{
				nowCursor_ = SELECT_MAX_NUM - 1;
			}
		}
	}

	//現カーソルから状態を変化
	CursorToState(nowCursor_);
}

void Pause::ChangeState(STATE state)
{
	state_ = state;
}

void Pause::CursorToState(int cursor)
{
	switch (cursor)
	{
	case (int)STATE::RESTART:
		ChangeState(STATE::RESTART);
		break;
	case (int)STATE::RETRY:
		ChangeState(STATE::RETRY);
		break;
	case (int)STATE::END:
		ChangeState(STATE::END);
		break;
	}
}

void Pause::PauseDraw(void)
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
