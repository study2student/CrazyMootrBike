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

	//����̎n�܂�|�W�V����
	onePersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 - 20 };

	//����̂����|�W�V����
	fourPersonFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 230 , Application::SCREEN_SIZE_Y / 2 + 120 };
}

void SelectScene::Update(void)
{
	//�Q�[���V�[����
	auto& ins_ = InputManager::GetInstance();
	if (ins_.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	//�}�E�X����
	MouseProcess();
}

void SelectScene::Draw(void)
{
	//�����\��
	DrawString(onePersonFontBasePos_.x, onePersonFontBasePos_.y, "1�l��", onePersonFontColor_);
	DrawString(fourPersonFontBasePos_.x, fourPersonFontBasePos_.y, "4�l��", fourPersonFontColor_);
}

void SelectScene::MouseProcess(void)
{
	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();


	//1�l�Ń{�^��
	Vector2 startFontLenPos_ = { onePersonFontBasePos_.x + ONE_PERSON_FONT_LENGTH ,onePersonFontBasePos_.y + ONE_PERSON_FONT_HEIGHT };
	if (mousePos_.x >= onePersonFontBasePos_.x && mousePos_.x <= startFontLenPos_.x
		&& mousePos_.y >= onePersonFontBasePos_.y && mousePos_.y <= startFontLenPos_.y)
	{
		//�{�^���ɂӂ�Ă���ꍇ
		onePersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			//���N���b�N�ŃQ�[�����J�n����
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
		&& mousePos_.y >= fourPersonFontBasePos_.y && mousePos_.y <= exitFontLenPos_.y)
	{
		//�{�^���ɂӂ�Ă���ꍇ
		fourPersonFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			//���N���b�N�ŃQ�[�����I������
			Application::GetInstance().SetIsGameFinishKey(true);
		}
	}
	else
	{
		//�{�^���ɂӂꂢ�Ȃ��ꍇ
		fourPersonFontColor_ = GetColor(255, 255, 255);
	}
}
