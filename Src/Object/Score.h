#pragma once

class Score
{

public:

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static Score& GetInstance(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	
	//スコア加算
	void AddScore(void);

	int GetScore(void);

	void ResetScore(void);

private:

	// 静的インスタンス
	static Score* instance_;

	//スコア
	int scoreNum_;
};


