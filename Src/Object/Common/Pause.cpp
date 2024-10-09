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

#pragma region �萔�錾

//�p�b�h�ő吔
const int PAD_MAX = 4;

//�ĊJ�����F
const int RESTART_FONT_COLOR = GetColor(255, 255, 255);

//���g���C�����F
const int RETRY_FONT_COLOR = GetColor(255, 255, 255);

//�I��镶���F
const int END_FONT_COLOR = GetColor(255, 255, 255);

//����̍ĊJ�|�W�V����
const Vector2 RESTART_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 55 , Application::SCREEN_SIZE_Y / 2 - 130 };

//����̃��g���C�|�W�V����
const Vector2 RETRY_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 95 , Application::SCREEN_SIZE_Y / 2 + 10 };

//����̏I���|�W�V����
const Vector2 END_FONT_BASE_POS = { Application::SCREEN_SIZE_X / 2 - 70 , Application::SCREEN_SIZE_Y / 2 + 150 };

//�ĊJ�{�^���̉��̒���
const int RESTART_FONT_LENGTH = 100;

//�ĊJ�{�^���̍���
const int RESTART_FONT_HEIGHT = 48;

//���g���C�{�^���̉��̒���
const int RETRY_FONT_LENGTH = 200;

//���g���C�{�^���̍���
const int RETRY_FONT_HEIGHT = 48;

//�I���{�^���̉��̒���
const int END_FONT_LENGTH = 150;

//�I���{�^���̍���
const int END_FONT_HEIGHT = 48;

//�I������
const int SELECT_MAX_NUM = 3;

//�|�[�Y�L�[���͂��������x������悤�ɂȂ�܂ł̎���
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
	//�|�[�Y���j���[
	//�F
	reStartFontColor_ = RESTART_FONT_COLOR;
	reTryFontColor_ = RETRY_FONT_COLOR;
	endFontColor_ = END_FONT_COLOR;

	//����̍ĊJ�|�W�V����
	reStartFontBasePos_ = RESTART_FONT_BASE_POS;

	//����̃��g���C�|�W�V����
	reTryFontBasePos_ = RETRY_FONT_BASE_POS;

	//����̏I���|�W�V����
	endFontBasePos_ = END_FONT_BASE_POS;

	//�������
	state_ = STATE::RESTART;
}

void Pause::Update(void)
{
}

void Pause::Draw(void)
{
	//�|�[�Y�����`��
	PauseDraw();
}

const bool& Pause::GetIsPause(void) const
{
	return isPause_;
}

void Pause::PausePrevious(void)
{
	InputManager& ins = InputManager::GetInstance();

	//�|�[�Y���j���[���J��
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
			//�|�[�Y���
			isPause_ = true;
		}
	}
}

void Pause::PauseMidst(void)
{
	//�}�E�X����
	DecideProcess();

	//�L�[����
	SelectProcess();
}

void Pause::SetImgHandle(const int& handle)
{
	imgPause_ = handle;
}

void Pause::DecideProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
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


		//�ĊJ�{�^��
		//�J�[�\�����������Ă���
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
			//�{�^���ɂӂ�Ă���ꍇ
			reStartFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//�|�[�Y����
				stepPauseKeyHit_ = 0.0f;
				isPause_ = false;
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			reStartFontColor_ = GetColor(255, 255, 255);
		}



		//���g���C�{�^��
		//�J�[�\�����������Ă���
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
			//�{�^���ɂӂ�Ă���ꍇ
			reTryFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//���N���b�N�܂��̓X�y�[�X�L�[�Ń��g���C
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			reTryFontColor_ = GetColor(255, 255, 255);
		}


		//�I���{�^��
		//�J�[�\�����������Ă���
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
			//�{�^���ɂӂ�Ă���ꍇ
			endFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//���N���b�N�܂��̓X�y�[�X�L�[�Ń^�C�g����
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			endFontColor_ = GetColor(255, 255, 255);
		}


		//�|�[�Y�L�[��������x������悤�ɂȂ�܂�
		stepPauseKeyHit_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepPauseKeyHit_ >= PAUSE_KEY_HIT_MAX_TIME)
		{

			//������x�|�[�Y�L�[�����Ń|�[�Y���j���[����
			if (ins_.IsTrgDown(KEY_INPUT_C) || ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START) && isPause_)
			{
				//�|�[�Y����
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
	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_UP) && !(nowCursor_ == 0))
	{
		// �I�����̉����Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
	{
		// �I�����̉����Đ�
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

		//�J�[�\���ԍ��ɂ��㉺����
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_UP) && !(nowCursor_ == 0))
		{
			// �I�����̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_--;
			if (nowCursor_ <= 0)
			{
				nowCursor_ = 0;
			}
		}
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
		{
			// �I�����̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_++;
			if (nowCursor_ >= SELECT_MAX_NUM - 1)
			{
				nowCursor_ = SELECT_MAX_NUM - 1;
			}
		}
	}

	//���J�[�\�������Ԃ�ω�
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
	//�|�[�Y�w�i�摜
	float imgScl = 3.0f;
	DrawRotaGraphFastF(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, imgScl, 0.0f, imgPause_, true);

	//�����\��
	float fontScl = 3.0f;
	DrawExtendFormatString(reStartFontBasePos_.x, reStartFontBasePos_.y, fontScl, fontScl, reStartFontColor_, "�ĊJ");
	DrawExtendFormatString(reTryFontBasePos_.x, reTryFontBasePos_.y, fontScl, fontScl, reTryFontColor_, "���g���C");
	DrawExtendFormatString(endFontBasePos_.x, endFontBasePos_.y, fontScl, fontScl, endFontColor_, "�I���");
}
