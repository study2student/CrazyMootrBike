#pragma once
#include <vector>

class Score
{

public:

	// �R���X�g���N�^
	Score(void);

	// �f�X�g���N�^
	~Score(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	void SetScore(int score);

	const int GetScore(void) const;

private:

	//�X�R�A
	int score_;
};


