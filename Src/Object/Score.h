#pragma once

class Score
{

public:

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Score& GetInstance(void);

	void Init(void);

	//�X�R�A���Z
	void AddScore(void);

	void SetScore(int score);

	const int GetScore(void) const;

	void ResetScore(void);

private:

	// �ÓI�C���X�^���X
	static Score* instance_;

	//�X�R�A
	int scoreNum_;
};


