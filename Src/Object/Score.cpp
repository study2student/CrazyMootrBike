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
	DrawFormatString(840, 140, 0x000000, "SCORE�F%d", score_);
	//�X�R�A
	DrawExtendFormatString(Application::SCREEN_SIZE_X - 300, 0, 3, 3, 0xff0000, "�X�R�A:%.d", score_);
}

void Score::AddScore(void)
{
	//�X�R�A���Z
	score_ += EnemyBase::SCORE_INCREMENT;
}

int Score::GetScore(void)
{
	return score_;
}
