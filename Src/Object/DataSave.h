#pragma once

class DataSave
{

public:


	//�f�[�^
	struct Data
	{
		//�v���C���[��
		int playerNum_;
	};

	enum class DataType
	{
		PLAYER_NUM	//�v���C���[�l���ݒ�
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static DataSave& GetInstance(void);

	void Init(void);

	//�f�[�^��ސݒ�(�Ȃ�̃f�[�^��ݒ肷�邩,�ݒ�l)
	void SetData(DataType dataType,int num);

	//�f�[�^�擾
	const Data& GetData(void) const;

	//�f�[�^���Z�b�g
	void ResetData(void);

private:

	// �ÓI�C���X�^���X
	static DataSave* instance_;

	//�f�[�^
	Data data_;

	
};



