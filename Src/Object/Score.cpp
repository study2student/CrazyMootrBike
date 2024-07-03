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

void Score::SetScore(int score)
{
	score_ += score;
}

const int Score::GetScore(void) const
{
	return score_;
}
