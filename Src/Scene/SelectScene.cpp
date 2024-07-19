#include <DxLib.h>
#include "SelectScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Application.h"
#include "../Object/DataSave.h"

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
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 - ONE_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };
	
	//左上のおわるポジション
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + FOUR_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };

	// 背景画像
	background_ = resMng_.Load(ResourceManager::SRC::IMG_BACKGROUND).handleId_;

	aloneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALONE).handleId_;
	everyoneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_EVERYONE).handleId_;

	// 操作画像
	leftStickImg_ = resMng_.Load(ResourceManager::SRC::IMG_LEFT_STICK).handleId_;

	isCursorHit_ = false;

	selectAloneImgScale_ = 1.5f;
	selectFourImgScale_ = 1.5f;
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

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, background_, true);

	//操作説明画像描画
	//DrawGraph(0, 0, leftStickImg_, true);

	//ひとりプレイ選択時
	if (state_ == STATE::ONE_PERSON)
	{
		//拡大縮小
		if (selectAloneImgScale_ > SELECT_IMG_MAX_SCALE)
		{
			isMaxSelectScale_ = true;
		}
		else if (selectAloneImgScale_ < SELECT_IMG_MIN_SCALE)
		{
			isMaxSelectScale_ = false;
		}

		if (isMaxSelectScale_)
		{
			selectAloneImgScale_ -= SELECT_IMG_CHANGE_SCALE;
		}
		else
		{
			selectAloneImgScale_ += SELECT_IMG_CHANGE_SCALE;
		}

		// 画像表示:ひとりで
		DrawRotaGraphFastF(onePersonFontBasePos_.x, onePersonFontBasePos_.y,selectAloneImgScale_,0.0, aloneImg_, true);
	}
	else
	{
		selectAloneImgScale_ = SELECT_IMG_MIN_SCALE;
		// 画像表示:ひとりで
		DrawRotaGraphFastF(onePersonFontBasePos_.x, onePersonFontBasePos_.y, selectAloneImgScale_, 0.0, aloneImg_, true);
	}

	//四人プレイ選択時
	if (state_ == STATE::FOUR_PERSON)
	{
		//拡大縮小
		if (selectFourImgScale_ > SELECT_FOUR_IMG_MAX_SCALE)
		{
			isMaxSelectScale_ = true;
		}
		else if (selectFourImgScale_ < SELECT_FOUR_IMG_MIN_SCALE)
		{
			isMaxSelectScale_ = false;
		}

		if (isMaxSelectScale_)
		{
			selectFourImgScale_ -= SELECT_IMG_CHANGE_SCALE;
		}
		else
		{
			selectFourImgScale_ += SELECT_IMG_CHANGE_SCALE;
		}

		// 画像表示:みんなで
		DrawRotaGraphFastF(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, selectFourImgScale_, 0.0, everyoneImg_, true);
	}
	else
	{
		selectFourImgScale_ = SELECT_FOUR_IMG_MIN_SCALE;
		// 画像表示:みんなで
		DrawRotaGraphFastF(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, selectFourImgScale_, 0.0, everyoneImg_, true);
	}

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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
		{
			//データ保存
			int playNum = 1;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			// 決定時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_NORMAL, false);

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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)|| static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
		{
			//データ保存
			int playNum = 4;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			// 決定時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_NORMAL, false);


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
	if (ins_.IsTrgDown(KEY_INPUT_LEFT)|| static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT))
	{
		// 決定時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, false);
		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT))
	{
		// 決定時の音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, false);
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
