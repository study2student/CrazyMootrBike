#include <DxLib.h>
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
}

void Score::SetScore(int score)
{
	score_ += score;
}

const int Score::GetScore(void) const
{
	return score_;
}
