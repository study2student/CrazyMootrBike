#include "../Application.h"
#include "Rider/EnemyBase.h"
#include "../Manager/SceneManager.h"
#include "Score.h"

Score* Score::instance_ = nullptr;

void Score::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Score();
	}
	instance_->Init();
}

Score& Score::GetInstance(void)
{
	return *instance_;
}

void Score::Init(void)
{
}

void Score::Update(void)
{
}

void Score::Draw(void)
{
	DrawFormatString(840, 140, 0x000000, "SCORE�F%d", scoreNum_);
	//�X�R�A
	DrawExtendFormatString(Application::SCREEN_SIZE_X - 300, 0, 3, 3, 0xff0000, "�X�R�A:%.d", scoreNum_);
}

void Score::AddScore(void)
{
	//�X�R�A���Z
	scoreNum_ += EnemyBase::SCORE_INCREMENT;
}

int Score::GetScore(void)
{
	return scoreNum_;
}

void Score::ResetScore(void)
{
	scoreNum_ = 0;
}
