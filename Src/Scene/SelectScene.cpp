#include <DxLib.h>
#include "SelectScene.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Application.h"
#include "../Object/DataSave.h"
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

	//����̎n�܂�|�W�V����
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 - ONE_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };
	
	//����̂����|�W�V����
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + FOUR_PERSON_FONT_LENGTH, Application::SCREEN_SIZE_Y / 2 };

	aloneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALONE).handleId_;
	everyoneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_EVERYONE).handleId_;

	isCursorHit_ = false;

	selectAloneImgScale_ = 1.5f;
	selectFourImgScale_ = 1.5f;
}

void SelectScene::Update(void)
{
	//�}�E�X����
	MouseProcess();

	//�L�[����
	KeyProcess();
}

void SelectScene::Draw(void)
{
	//�ЂƂ�v���C�I����
	if (state_ == STATE::ONE_PERSON)
	{
		//�g��k��
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

		// �摜�\��:�ЂƂ��
		DrawRotaGraphFastF(onePersonFontBasePos_.x, onePersonFontBasePos_.y,selectAloneImgScale_,0.0, aloneImg_, true);
	}
	else
	{
		selectAloneImgScale_ = SELECT_IMG_MIN_SCALE;
		// �摜�\��:�ЂƂ��
		DrawRotaGraphFastF(onePersonFontBasePos_.x, onePersonFontBasePos_.y, selectAloneImgScale_, 0.0, aloneImg_, true);
	}

	//�l�l�v���C�I����
	if (state_ == STATE::FOUR_PERSON)
	{
		//�g��k��
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

		// �摜�\��:�݂�Ȃ�
		DrawRotaGraphFastF(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, selectFourImgScale_, 0.0, everyoneImg_, true);
	}
	else
	{
		selectFourImgScale_ = SELECT_FOUR_IMG_MIN_SCALE;
		// �摜�\��:�݂�Ȃ�
		DrawRotaGraphFastF(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, selectFourImgScale_, 0.0, everyoneImg_, true);
	}

}


void SelectScene::MouseProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//1�l�Ń{�^��
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
		//�{�^���ɂӂ�Ă���ꍇ
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//�f�[�^�ۑ�
			int playNum = 1;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			//���N���b�N�܂��̓X�y�[�X�L�[�ŃQ�[�����J�n����
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		}
	}
	else
	{
		//�{�^���ɂӂꂢ�Ȃ��ꍇ
		onePersonFontColor_ = GetColor(255, 255, 255);
	}



	//4�l�Ń{�^��
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
		//�{�^���ɂӂ�Ă���ꍇ
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//�f�[�^�ۑ�
			int playNum = 4;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			//���N���b�N�܂��̓X�y�[�X�L�[�ŃQ�[�����J�n����
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);

		}
	}
	else
	{
		//�{�^���ɂӂꂢ�Ȃ��ꍇ
		fourPersonFontColor_ = GetColor(255, 255, 255);
	}
}

void SelectScene::KeyProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_LEFT))
	{
		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_RIGHT))
	{
		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}
	
	//���J�[�\�������Ԃ�ω�
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
