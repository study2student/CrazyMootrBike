#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../../Src/Object/Score.h"
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
}

void GameOverScene::Update(void)
{
<<<<<<< HEAD
=======
	score_.Update();
>>>>>>> main
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameOverScene::Draw(void)
{
	DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("�X�R�A", strlen("�X�R�A")), 300, 3, 3, "�X�R�A", 0xff0000);
	DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 50, Application::SCREEN_SIZE_Y / 2 - GetDrawFormatStringWidth("%.d"),
		10, 10, 0xff0000, "%.d", score_.GetScore());

}
