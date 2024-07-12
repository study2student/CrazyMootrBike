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

void Score::AddScore(void)
{
	//ÉXÉRÉAâ¡éZ
	scoreNum_ += EnemyBase::SCORE_INCREMENT;
}

void Score::SetScore(int score)
{
	scoreNum_ += score;
}

const int Score::GetScore(void) const
{
	return scoreNum_;
}

void Score::ResetScore(void)
{
	scoreNum_ = 0;
}
