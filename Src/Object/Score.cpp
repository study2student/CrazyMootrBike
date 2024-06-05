#include "../Application.h"
#include "Rider/EnemyBase.h"
#include "Score.h"

Score::Score(void)
{
	score_ = 0;
}

Score::~Score(void)
{
}

void Score::Init(void)
{
}

void Score::Update(void)
{
}

void Score::Draw(void)
{
	DrawFormatString(840, 140, 0x000000, "SCORE：%d", score_);
	//スコア
	DrawExtendFormatString(Application::SCREEN_SIZE_X - 300, 0, 3, 3, 0xff0000, "スコア:%.d", score_);
}

void Score::AddScore(void)
{
	//スコア加算
	score_ += EnemyBase::SCORE_INCREMENT;
}

int Score::GetScore(void)
{
	return score_;
}
