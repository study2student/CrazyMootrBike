#pragma once
#include <vector>

class Score
{

public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Score& GetInstance(void);

	void Init(void);

	//スコア加算
	void AddScore(int score);

	//1人用
	void ScoreSet(int scoreSet);

	//4人用
	void ScoreSetArray(int scoreSet);

	const int GetScore(void) const;

	std::vector<int> GetScoreArray(void);

	void ResetScore(void);

private:

	// 静的インスタンス
	static Score* instance_;

	//スコア1人用
	int scoreNum_;

	//スコア配列4人用
	std::vector<int> scoreArray_;
};


