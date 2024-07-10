#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../../Src/Object/Score.h"
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

	//�o�C�N�̓ǂݍ���
	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(200.0f * (i + 1), i));
	}
	for (auto& bike : bikes_) {
		bike->Init();
	}

}

void GameOverScene::Update(void)
{
	//score_.Update();
	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	bikes_[0]->Update();
}

void GameOverScene::Draw(void)
{
	DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("�X�R�A", strlen("�X�R�A")), 300, 3, 3, "�X�R�A", 0xff0000);
	DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 50, Application::SCREEN_SIZE_Y / 2 - GetDrawFormatStringWidth("%.d"),
		10, 10, 0xff0000, "%.d", score_.GetScore());

}
