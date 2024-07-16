#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
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

	//バイクの読み込み
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
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	bikes_[0]->Update();
}

void GameOverScene::Draw(void)
{
	if(data_.GetData().playerNum_ == 1)
	{
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("スコア", strlen("スコア")), 300, 3, 3, "スコア", 0xff0000);

		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 90, Application::SCREEN_SIZE_Y / 2 - GetDrawFormatStringWidth("%.d"),
			10, 10, 0xff0000, "%.d", score_.GetScore());
	}
	else
	{
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawStringWidth("スコア", strlen("スコア")), 100, 3, 3, "スコア", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 350, Application::SCREEN_SIZE_Y / 2 - 300 - GetDrawFormatStringWidth("%.d") + (0 * 200), 10, 10, "P1", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 350, Application::SCREEN_SIZE_Y / 2 - 300 - GetDrawFormatStringWidth("%.d") + (1 * 200), 10, 10, "P2", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 350, Application::SCREEN_SIZE_Y / 2 - 300 - GetDrawFormatStringWidth("%.d") + (2 * 200), 10, 10, "P3", 0xff0000);
		DrawExtendString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 350, Application::SCREEN_SIZE_Y / 2 - 300 - GetDrawFormatStringWidth("%.d") + (3 * 200), 10, 10, "P4", 0xff0000);

		int playNum = 4;
		for (int i = 0; i < playNum; i++)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.d") - 50, Application::SCREEN_SIZE_Y / 2 - 300 - GetDrawFormatStringWidth("%.d") + (i * 200),
				10, 10, 0xff0000, "%.d", score_.GetScoreArray()[i]);
		}
	}
	

}
