#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../../Src/Object/Score.h"
#include "../../Src/Object/DataSave.h"
#include "../../Src/Object/Rider/Bike.h"
#include "GameOverScene.h"

GameOverScene::GameOverScene(void)
{
	/*score_ = nullptr;*/
}

GameOverScene::~GameOverScene(void)
{
}

void GameOverScene::Init(void)
{
	/*score_ = std::make_shared<Score>();
	score_->Init();*/

	//�w�i�摜
	img_BG = resMng_.Load(ResourceManager::SRC::IMG_SCORE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;
	imgSelectBG_= resMng_.Load(ResourceManager::SRC::IMG_GAMEOVER_SELECT_BG).handleId_;
	//�o�C�N�̓ǂݍ���
	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(200.0f * (i + 1), i));
	}
	for (auto& bike : bikes_) {
		bike->Init();
	}

	//����̃��g���C�|�W�V����
	int addX;
	addX = 580;
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 10 };

	//����̏I���|�W�V����
	addX = 615;
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 150 };

}

void GameOverScene::Update(void)
{
	//score_.Update();
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	bikes_[0]->Update();

	//����
	DecideProcess();

	//�L�[����
	SelectProcess();
}

void GameOverScene::Draw(void)
{
	using ap = Application;

	//�w�i�摜�`��
	DrawExtendGraph(Application::SCREEN_SIZE_X / 4, 0, ap::SCREEN_SIZE_X - ap::SCREEN_SIZE_X / 4, ap::SCREEN_SIZE_Y + 20, img_BG, true);

	if(data_.GetData().playerNum_ == 1)
	{
		//DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("�X�R�A", strlen("�X�R�A")), 300, 3, 3, "�X�R�A", 0xff0000);

		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 90, Application::SCREEN_SIZE_Y / 2 - GetDrawFormatStringWidth("%.d"),
			10, 10, 0xff0000, "%.d", score_.GetScore());
	}
	else
	{
		//DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("�X�R�A", strlen("�X�R�A")), 100, 3, 3, "�X�R�A", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 -  340, Application::SCREEN_SIZE_Y / 2 - 342 /*- GetDrawFormatStringWidth("%.d")*/ + (0 * 200), 10, 10, "P1", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 -  340, Application::SCREEN_SIZE_Y / 2 - 342 /*- GetDrawFormatStringWidth("%.d")*/ + (1 * 200), 10, 10, "P2", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 -  340, Application::SCREEN_SIZE_Y / 2 - 342 /*- GetDrawFormatStringWidth("%.d")*/ + (2 * 200), 10, 10, "P3", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 -  340, Application::SCREEN_SIZE_Y / 2 - 342 /* - GetDrawFormatStringWidth("%.d")*/ + (3 * 200), 10, 10, "P4", 0xff0000);

		int playNum = 4;
		for (int i = 0; i < playNum; i++)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 -  50, Application::SCREEN_SIZE_Y / 2 - 340 /*- GetDrawFormatStringWidth("%.d")*/ + (i * 200),
				10, 10, 0xff0000, "%.d", score_.GetScoreArray()[i]);
		}
	}
	
	//DrawGraph(200, 700, imgPush_, true);

	//�w�i
	DrawRotaGraphFastF(1600, 600, 3.0f, 0.0f, imgSelectBG_, true);

	//�����\��
	float fontScl = 3.0f;
	DrawExtendFormatString(reTryFontBasePos_.x, reTryFontBasePos_.y, fontScl, fontScl, reTryFontColor_, "���g���C");
	DrawExtendFormatString(endFontBasePos_.x, endFontBasePos_.y, fontScl, fontScl, endFontColor_, "�I���");
}

void GameOverScene::DecideProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();


	//���g���C�{�^��
	//�J�[�\�����������Ă���
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
		//�{�^���ɂӂ�Ă���ꍇ
		reTryFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
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
		nowCursor_ = (int)PAUSE_STATE::END;
		isCursorHit_ = true;
	}
	else
	{
		isCursorHit_ = false;
	}

	if (pState_ == PAUSE_STATE::END)
	{
		//�{�^���ɂӂ�Ă���ꍇ
		endFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) || ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
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
}

void GameOverScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_UP) && nowCursor_ == SELECT_MAX_NUM - 1
		|| ins_.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::L_STICK_UP) && nowCursor_ == SELECT_MAX_NUM - 1)
	{
		// �I�����̉����Đ�
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
		// �I�����̉����Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}

	//���J�[�\�������Ԃ�ω�
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
