#include <DxLib.h>
#include "SelectScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"

SelectScene::SelectScene(void)
{
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
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 230 , Application::SCREEN_SIZE_Y / 2 + 120 };
}

void SelectScene::Update(void)
{
	//ゲームシーンへ
	auto& ins_ = InputManager::GetInstance();
	if (ins_.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//マウス操作
	MouseProcess();
}

void SelectScene::Draw(void)
{
	//文字表示
	DrawString(onePersonFontBasePos_.x, onePersonFontBasePos_.y, "1人で", onePersonFontColor_);
	DrawString(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, "4人で", fourPersonFontColor_);
}

void SelectScene::MouseProcess(void)
{
	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();


	//1人でボタン
	Vector2 startFontLenPos_ = { onePersonFontBasePos_.x + ONE_PERSON_FONT_LENGTH ,onePersonFontBasePos_.y + ONE_PERSON_FONT_HEIGHT };
	if (mousePos_.x >= onePersonFontBasePos_.x && mousePos_.x <= startFontLenPos_.x
		&& mousePos_.y >= onePersonFontBasePos_.y && mousePos_.y <= startFontLenPos_.y)
	{
		//ボタンにふれている場合
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			//左クリックでゲームを開始する
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
		&& mousePos_.y >= fourPersonFontBasePos_.y && mousePos_.y <= exitFontLenPos_.y)
	{
		//ボタンにふれている場合
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			//左クリックでゲームを終了する
			Application::GetInstance().SetIsGameFinishKey(true);
		}
	}
	else
	{
		//ボタンにふれいない場合
		fourPersonFontColor_ = GetColor(255, 255, 255);
	}
}
