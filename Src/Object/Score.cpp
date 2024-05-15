#include "../Object/Rider/EnemyBase.h"
#include "Score.h"

Score::Score(EnemyBase* enemy)
{
	enemyBase_ = enemy;
}

Score::~Score(void)
{
}

void Score::Init(void)
{
	enemyBase_->Init();
	score_ = 0;
}

void Score::Update(void)
{
	enemyBase_->Update();
	if (enemyBase_->GetIsBikeCol())
	{
		score_++;
	}
}

void Score::Draw(void)
{
	DrawFormatString(0, 120, 0x000000, "SCORE:%d", score_);
}
