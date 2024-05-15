#pragma once
class EnemyBase;

class Score
{
public:
	// コンストラクタ
	Score(EnemyBase* enemy);

	// デストラクタ
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	EnemyBase* enemyBase_;

	int score_;
};

