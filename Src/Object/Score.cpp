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
}

void Score::AddScore(void)
{
	//�X�R�A���Z
	score_ += EnemyBase::SCORE_INCREMENT;
}
