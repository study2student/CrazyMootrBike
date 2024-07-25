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

	const int GetScore(void) const;

	std::vector<int> GetScoreArray(void);

	void ResetScore(void);

private:

	// �ÓI�C���X�^���X
	static Score* instance_;

	//�X�R�A1�l�p
	int scoreNum_;

	//�X�R�A�z��4�l�p
	std::vector<int> scoreArray_;
};


