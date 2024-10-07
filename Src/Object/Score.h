#pragma once
#include <vector>

class Score
{

public:

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static Score& GetInstance(void);

	void Init(void);

	//�X�R�A���Z
	void AddScore(int score);

	//1�l�p
	void ScoreSet(int scoreSet);

	//4�l�p
	void ScoreSetArray(int scoreSet);

	//�X�R�A�擾(��l�p)
	const int& GetScore(void) const;

	//�X�R�A�擾(�l�l�p)
	const std::vector<int>& GetScoreArray(void) const;

	void ResetScore(void);

private:

	// �ÓI�C���X�^���X
	static Score* instance_;

	//�X�R�A1�l�p
	int scoreNum_;

	//�X�R�A�z��4�l�p
	std::vector<int> scoreArray_;
};


