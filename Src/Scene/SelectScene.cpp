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
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 - ONE_PERSON_FONT_LENGTH - ONE_PERSON_FONT_LENGTH / 2, Application::SCREEN_SIZE_Y / 2 - ONE_PERSON_FONT_HEIGHT / 2};
	
	//����̂����|�W�V����
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + FOUR_PERSON_FONT_LENGTH / 2 , Application::SCREEN_SIZE_Y / 2 - FOUR_PERSON_FONT_HEIGHT / 2 };

	aloneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALONE).handleId_;
	everyoneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_EVERYONE).handleId_;

	isCursorHit_ = false;
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
	// �摜�\��:�ЂƂ��
	DrawGraph(onePersonFontBasePos_.x, onePersonFontBasePos_.y, aloneImg_, true);
	// �摜�\��:�݂�Ȃ�
	DrawGraph(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, everyoneImg_, true);

	//�����\��
	DrawString(onePersonFontBasePos_.x, onePersonFontBasePos_.y, "1�l��", onePersonFontColor_);
	DrawString(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, "4�l��", fourPersonFontColor_);
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
