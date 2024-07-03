#pragma once

class Score
{

public:

	// コンストラクタ
	Score(void);

	// デストラクタ
	~Score(void);

	void Init(void);

	void SetScore(int score);

	const int GetScore(void) const;

private:

	//スコア
	int score_;
};


