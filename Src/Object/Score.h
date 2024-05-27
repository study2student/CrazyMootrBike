#pragma once

class Score
{

public:

	// コンストラクタ
	Score(void);

	// デストラクタ
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	
	//スコア加算
	void AddScore(void);

private:

	//スコア
	int score_;



};


