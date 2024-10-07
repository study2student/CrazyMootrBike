#include <DxLib.h>
#include "SelectScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Application.h"
#include "../Object/DataSave.h"

#pragma region 定数宣言
	//1人でボタンの横の長さ
	const int ONE_PERSON_FONT_LENGTH = 400;
	//1人でボタンの高さ
	const int ONE_PERSON_FONT_HEIGHT = 392;
	//4人でボタンの横の長さ
	const int FOUR_PERSON_FONT_LENGTH = 400;
	//4人でボタンの高さ
	const int FOUR_PERSON_FONT_HEIGHT = 360;
	//ひとりプレイ選択画像最大サイズ
	const float SELECT_IMG_MAX_SCALE = 1.2f;
	//ひとりプレイ選択画像最小サイズ
	const float SELECT_IMG_MIN_SCALE = 1.0f;
	//四人プレイ選択画像最大サイズ
	const float SELECT_FOUR_IMG_MAX_SCALE = 1.2f;
	//四人プレイ選択画像最小サイズ
	const float SELECT_FOUR_IMG_MIN_SCALE = 1.0f;
	//選択画像大きさ変化量
	const float SELECT_IMG_CHANGE_SCALE = 0.006f;
	//選択肢数
	const int SELECT_MAX_NUM = 2;
	//コイン画像の配置のY座標
	const int IMG_COIN_POS_Y = Application::SCREEN_SIZE_Y / 4 - 80;
	//バイク画像の座標
	const int IMG_BIKE_POS_X = Application::SCREEN_SIZE_X;
	const int IMG_BIKE_POS_Y = Application::SCREEN_SIZE_Y % 3;
	//四人プレイ選択時の画像の大きさの初期値
	const float SELECT_FOUR_IMG_SCALE = 1.5f;
	//点滅表示ステップ
	const float MAX_STEP_FLASH = 0.8f;
	const float MIN_STEP_FLASH = 0.0f;
	//PUSH画像の座標
	const int IMG_PUSH_POS_X = Application::SCREEN_SIZE_X / 2 - 145;
	const int IMG_PUSH_POS_Y = 700;
	//操作説明画像座標
	const int IMG_OPERATION_POS_Y = 280;
	//操作説明画像の拡大率
	const double IMG_OPERATION_MAGNIFICATION = 0.8;
	//バイク画像のX座標移動
	const int IMG_BIKE_MOVE_BOX_POS_X = 1666;
	//バイク画像のX座標移動
	const int IMG_BIKE_MOVE_POS_RATIO = 5;
	//バイク画像を描画する矩形の右下頂点＋１の座標
	const int IMG_BIKE_BOX_POS_Y = 1111;
	//バイク画像を描画する矩形の右下頂点＋１の座標の拡大率
	const int IMG_BIKE_BOX_POS = 4;
	//コイン画像の大きさ
	const int IMG_COIN_SIZE = 60;
	//プレイヤー人数
	const int PLAYER_NUM = 4;
	//バイク画像の移動量
	const int IMG_BIKE_MOVE_POS_X = 10;
	//画面サイズの四分の一
	const int SCREEN_SIZE_QUARTER = 4;
#pragma endregion


SelectScene::SelectScene(void)
	:
	skyDome_(nullptr),
	onePersonFontBasePos_({}),
	fourPersonFontBasePos_({}),
	background_(0),
	aloneImg_(0),
	everyoneImg_(0),
	Operation(0),
	imgPush_(0),
	coinImg_(0),
	coinImgPos_({}),
	bikeImg_(0),
	bikeImgPos_({}),
	selectAloneImgScale_(0.0f),
	selectFourImgScale_(0.0f),
	isMaxSelectScale_(false),
	onePersonFontColor_(0),
	fourPersonFontColor_(0),
	isCursorHit_(false),
	nowCursor_(0),
	selectSE_(0),
	decideSE_(0),
	state_(STATE::ONE_PERSON),
	stepFlash_(0.0f),
	isInvisible_(false)
{
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{

	//左上の始まるポジション
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 - ONE_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };
	
	//左上のおわるポジション
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + FOUR_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };

	// 背景画像
	background_ = resMng_.Load(ResourceManager::SRC::IMG_BACKGROUND).handleId_;

	aloneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALONE).handleId_;
	everyoneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_EVERYONE).handleId_;

	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	Operation = resMng_.Load(ResourceManager::SRC::IMG_OPERATION).handleId_;

	//コイン画像
	coinImg_ = resMng_.Load(ResourceManager::SRC::IMG_COIN).handleId_;
	coinImgPos_ = { Application::SCREEN_SIZE_X / 2, IMG_COIN_POS_Y };

	// バイク画像
	bikeImg_ = resMng_.Load(ResourceManager::SRC::IMG_BIKE).handleId_;
	bikeImgPos_ = { IMG_BIKE_POS_X, IMG_BIKE_POS_Y };

	selectSE_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_;
	decideSE_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_;
	
	isCursorHit_ = false;

	selectAloneImgScale_ = SELECT_IMG_MIN_SCALE;
	selectFourImgScale_ = SELECT_FOUR_IMG_SCALE;
}

void SelectScene::Update(void)
{
	//操作
	DecideProcess();

	//キー操作
	SelectProcess();

	//バイク画像の動き
	BikeImgUpdate();
}

void SelectScene::Draw(void)
{

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, background_, true);


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



	//点滅表示
	if (!isInvisible_)
	{
		stepFlash_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepFlash_ >= MAX_STEP_FLASH)
		{
			isInvisible_ = true;
		}
	}
	else
	{
		stepFlash_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepFlash_ <= MIN_STEP_FLASH)
		{
			isInvisible_ = false;
		}
	}

	if (!isInvisible_)
	{
		DrawGraph(IMG_PUSH_POS_X, IMG_PUSH_POS_Y, imgPush_, true);
	}

	DrawOpe();
}


void SelectScene::DrawOpe(void)
{
	int scX = Application::SCREEN_SIZE_X;
	int scY = Application::SCREEN_SIZE_Y;

	//操作説明画像描画
	DrawRotaGraphFast(scX / 2, IMG_OPERATION_POS_Y, IMG_OPERATION_MAGNIFICATION, 0.0f, Operation, true);


	if (bikeImgPos_.x - IMG_BIKE_MOVE_BOX_POS_X / IMG_BIKE_MOVE_POS_RATIO <= coinImgPos_.x)
	{
		// コイン画像の大きさ
		for (int i = 0; i < 180; i += IMG_COIN_SIZE)
		{
			DrawExtendGraph(coinImgPos_.x + i, coinImgPos_.y, coinImgPos_.x + IMG_COIN_SIZE + i, coinImgPos_.y + IMG_COIN_SIZE, coinImg_, true);
		}
	}


	// バイク画像
	DrawExtendGraph(bikeImgPos_.x, bikeImgPos_.y,
			bikeImgPos_.x - IMG_BIKE_MOVE_BOX_POS_X / IMG_BIKE_BOX_POS, bikeImgPos_.y + IMG_BIKE_BOX_POS_Y / IMG_BIKE_BOX_POS,
			bikeImg_, true);


}

void SelectScene::DecideProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//1人でボタン
	Vector2 startFontLenPos_ = { onePersonFontBasePos_.x + ONE_PERSON_FONT_LENGTH ,onePersonFontBasePos_.y + ONE_PERSON_FONT_HEIGHT };
	if (mousePos_.x >= onePersonFontBasePos_.x && mousePos_.x <= startFontLenPos_.x
		&& mousePos_.y >= onePersonFontBasePos_.y && mousePos_.y <= startFontLenPos_.y)
	{
		/*nowCursor_ = (int)STATE::ONE_PERSON;
		isCursorHit_ = true;*/
	}
	else
	{
		isCursorHit_ = false;
	}

	if (state_ == STATE::ONE_PERSON)
	{
		//ボタンにふれている場合
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			//データ保存
			int playNum = 1;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			// 決定時の音を再生
			PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

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
		/*nowCursor_ = (int)STATE::FOUR_PERSON;
		isCursorHit_ = true;*/
	}
	else
	{
		isCursorHit_ = false;
	}

	if (state_ == STATE::FOUR_PERSON)
	{
		//ボタンにふれている場合
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			//データ保存
			data_.SetData(DataSave::DataType::PLAYER_NUM, PLAYER_NUM);

			// 決定時の音を再生
			PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);


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

void SelectScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//カーソル番号による上下操作
	if (ins_.IsTrgDown(KEY_INPUT_LEFT) && nowCursor_ == SELECT_MAX_NUM - 1
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_LEFT) && nowCursor_== SELECT_MAX_NUM - 1)
	{
		// 選択時の音を再生
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK, true);
		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT) && nowCursor_ == 0 
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_RIGHT) && nowCursor_ == 0)
	{
		// 選択時の音を再生
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK, true);
		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}
	
	//現カーソルから状態を変化
	CursorToState(nowCursor_);

}

void SelectScene::BikeImgUpdate(void)
{
	if (bikeImgPos_.x > Application::SCREEN_SIZE_X)
	{
		// 初期値に戻す
		bikeImgPos_.x = Application::SCREEN_SIZE_X / SCREEN_SIZE_QUARTER;
	}
	bikeImgPos_.x += IMG_BIKE_MOVE_POS_X;
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
