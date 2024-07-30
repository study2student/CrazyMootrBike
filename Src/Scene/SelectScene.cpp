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

	stepFlash_ = 0.0f;
	isInvisible_ = false;
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

	// �w�i�摜
	background_ = resMng_.Load(ResourceManager::SRC::IMG_BACKGROUND).handleId_;

	aloneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_ALONE).handleId_;
	everyoneImg_ = resMng_.Load(ResourceManager::SRC::IMG_SELECT_EVERYONE).handleId_;

	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	Operation = resMng_.Load(ResourceManager::SRC::IMG_OPERATION).handleId_;

	//�R�C���摜
	coinImg_ = resMng_.Load(ResourceManager::SRC::IMG_COIN).handleId_;
	coinImgPos_ = { Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 4 - 80 };

	// �o�C�N�摜
	bikeImg_ = resMng_.Load(ResourceManager::SRC::IMG_BIKE).handleId_;
	bikeImgPos_ = { Application::SCREEN_SIZE_X / 4,Application::SCREEN_SIZE_Y % 3 };

	selectSE_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_;
	decideSE_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_;
	
	isCursorHit_ = false;

	selectAloneImgScale_ = 1.0f;
	selectFourImgScale_ = 1.5f;
}

void SelectScene::Update(void)
{
	//����
	DecideProcess();

	//�L�[����
	SelectProcess();

	//�o�C�N�摜�̓���
	BikeImgUpdate();
}

void SelectScene::Draw(void)
{

	DrawExtendGraph(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, background_, true);


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



	//�_�ŕ\��
	float maxStepFlash = 0.80f;
	float minStepFlash = 0.0f;
	if (!isInvisible_)
	{
		stepFlash_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepFlash_ >= maxStepFlash)
		{
			isInvisible_ = true;
		}
	}
	else
	{
		stepFlash_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepFlash_ <= minStepFlash)
		{
			isInvisible_ = false;
		}
	}

	if (!isInvisible_)
	{
		DrawGraph(Application::SCREEN_SIZE_X / 2 - 145, 700, imgPush_, true);
	}

	DrawOpe();
}


void SelectScene::DrawOpe(void)
{
	int scX = Application::SCREEN_SIZE_X;
	int scY = Application::SCREEN_SIZE_Y;

	//��������摜�`��
	DrawRotaGraph(scX / 2, 280, 0.8, 0.0, Operation, true);


	if (bikeImgPos_.x - 1666 / 5 <= coinImgPos_.x)
	{
		// �R�C���摜�̑傫��
		int size = 60;
		for (int i = 0; i < 180; i += size)
		{
			DrawExtendGraph(coinImgPos_.x + i, coinImgPos_.y, coinImgPos_.x + size + i, coinImgPos_.y + size, coinImg_, true);
		}
	}


	// �o�C�N�摜
	DrawExtendGraph(bikeImgPos_.x, bikeImgPos_.y,
			bikeImgPos_.x - 1666 / 4, bikeImgPos_.y + 1111 / 4,
			bikeImg_, true);


}

void SelectScene::DecideProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//1�l�Ń{�^��
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
		//�{�^���ɂӂ�Ă���ꍇ
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			//�f�[�^�ۑ�
			int playNum = 1;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			// ���莞�̉����Đ�
			PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);

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
		/*nowCursor_ = (int)STATE::FOUR_PERSON;
		isCursorHit_ = true;*/
	}
	else
	{
		isCursorHit_ = false;
	}

	if (state_ == STATE::FOUR_PERSON)
	{
		//�{�^���ɂӂ�Ă���ꍇ
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		{
			//�f�[�^�ۑ�
			int playNum = 4;
			data_.SetData(DataSave::DataType::PLAYER_NUM, playNum);

			// ���莞�̉����Đ�
			PlaySoundMem(decideSE_, DX_PLAYTYPE_BACK, true);


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

void SelectScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_LEFT) && nowCursor_ == SELECT_MAX_NUM - 1
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_LEFT) && nowCursor_== SELECT_MAX_NUM - 1)
	{
		// �I�����̉����Đ�
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
		// �I�����̉����Đ�
		PlaySoundMem(selectSE_, DX_PLAYTYPE_BACK, true);
		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}
	
	//���J�[�\�������Ԃ�ω�
	CursorToState(nowCursor_);

}

void SelectScene::BikeImgUpdate(void)
{
	if (bikeImgPos_.x > Application::SCREEN_SIZE_X)
	{
		// �����l�ɖ߂�
		bikeImgPos_.x = Application::SCREEN_SIZE_X / 4;
	}
	bikeImgPos_.x += 10;
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
