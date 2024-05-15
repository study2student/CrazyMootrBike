#pragma once
class EnemyBase;

class Score
{
public:
	// �R���X�g���N�^
	Score(EnemyBase* enemy);

	// �f�X�g���N�^
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);
private:
	EnemyBase* enemyBase_;

	int score_;
};

