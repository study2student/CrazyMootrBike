#pragma once

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
	
	//�X�R�A���Z
	void AddScore(void);

private:

	//�X�R�A
	int score_;



};


