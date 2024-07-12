#include <DxLib.h>
#include "SelectScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"

SelectScene::SelectScene(void)
{
	nowCursor_ = 0;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	onePersonFontColor_ = GetColor(255, 255, 255);
	fourPersonFontColor_ = GetColor(255, 255, 255);

	//左上の始まるポジション
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 - 20 };

	//左上のおわるポジション
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 + 120 };

	isCursorHit_ = false;

	playNumber_ = 1;
}

void SelectScene::Update(void)
{
	//マウス操作
	MouseProcess();

	//キー操作
	KeyProcess();
}

void SelectScene::Draw(void)
{
	//文字表示
	DrawString(onePersonFontBasePos_.x, onePersonFontBasePos_.y, "1人で", onePersonFontColor_);
	DrawString(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, "4人で", fourPersonFontColor_);
}

const int SelectScene::GetPlayNumber(void) const
{
	return playNumber_;
}

void SelectScene::MouseProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//1人でボタン
	Vector2 startFontLenPos_ = { onePersonFontBasePos_.x + ONE_PERSON_FONT_LENGTH ,onePersonFontBasePos_.y + ONE_PERSON_FONT_HEIGHT };
	if (mousePos_.x >= onePersonFontBasePos_.x && mousePos_.x <= startFontLenPos_.x
		&& mousePos_.y >= onePersonFontBasePos_.y && mousePos_.y <= startFontLenPos_.y)
	{
		nowCursor_ = (int)STATE::ONE_PERSON;
		playNumber_ = 1;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (state_ == STATE::ONE_PERSON)
	{
		//ボタンにふれている場合
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでゲームを開始する
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		}
	}
	else
	{
		//ボタンにふれいない場合
		onePersonFontColor_ = GetColor(255, 255, 255);
	}



	//4人でボタン
	Vector2 exitFontLenPos_ = { fourPersonFontBasePos_.x + FOUR_PERSON_FONT_LENGTH ,fourPersonFontBasePos_.y + FOUR_PERSON_FONT_HEIGHT };
	if (mousePos_.x >= fourPersonFontBasePos_.x && mousePos_.x <= exitFontLenPos_.x
		&& mousePos_.y >= fourPersonFontBasePos_.y && mousePos_.y <= exitFontLenPos_.y )
	{
		nowCursor_ = (int)STATE::FOUR_PERSON;
		playNumber_ = 4;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (state_ == STATE::FOUR_PERSON)
	{
		//ボタンにふれている場合
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでゲームを終了する
			//左クリックまたはスペースキーでゲームを開始する
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		}
	}
	else
	{
		//ボタンにふれいない場合
		fourPersonFontColor_ = GetColor(255, 255, 255);
	}
}

void SelectScene::KeyProcess(void)
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
	CursorToState(nowCursor_);

}

void SelectScene::ChangeState(STATE state)
{
	state_ = state;
}

void SelectScene::CursorToState(int cursor)
{
	switch (cursor)
	{
	case (int)STATE::ONE_PERSON:
		ChangeState(STATE::ONE_PERSON);
		break;
	case (int)STATE::FOUR_PERSON:
		ChangeState(STATE::FOUR_PERSON);
		break;
	}
}
