#pragma once

class Score
{

public:

	// �R���X�g���N�^
	Score(void);

	// �f�X�g���N�^
	~Score(void);

	void Init(void);

	void SetScore(int score);

	const int GetScore(void) const;

private:

	//�X�R�A
	int score_;
};


