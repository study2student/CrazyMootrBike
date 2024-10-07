#include "../Application.h"
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

void Score::AddScore(int score)
{
	//スコア加算
	scoreNum_ += score;
}

void Score::ScoreSet(int scoreSet)
{
	scoreNum_ = scoreSet;
}

void Score::ScoreSetArray(int scoreSet)
{
	scoreArray_.emplace_back(scoreSet);
}

const int& Score::GetScore(void) const
{
	return scoreNum_;
}

const std::vector<int>& Score::GetScoreArray(void) const
{
	return scoreArray_;
}

void Score::ResetScore(void)
{
	scoreNum_ = 0;

	//要素を削除する
	scoreArray_.erase(scoreArray_.begin(), scoreArray_.end());
	
}
