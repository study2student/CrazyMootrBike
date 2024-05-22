#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Common/Vector2.h"
class SceneManager;
class SkyDome;
class Vector2;

class SelectScene : public SceneBase
{

public:


	//1�l�Ń{�^���̉��̒���
	static constexpr int ONE_PERSON_FONT_LENGTH = 160;

	//1�l�Ń{�^���̍���
	static constexpr int ONE_PERSON_FONT_HEIGHT = 50;

	//4�l�Ń{�^���̉��̒���
	static constexpr int FOUR_PERSON_FONT_LENGTH = 160;

	//4�l�Ń{�^���̍���
	static constexpr int FOUR_PERSON_FONT_HEIGHT = 50;

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �X�J�C�h�[���p
	Transform spaceDomeTran_;

	// �X�J�C�h�[��(�w�i)
	SkyDome* skyDome_;

	//�����1�l�Ń|�W�V����
	Vector2 onePersonFontBasePos_;

	//�����4�l�Ń|�W�V����
	Vector2 fourPersonFontBasePos_;

	//1�l�ŕ����F
	int onePersonFontColor_;

	//4�l�ŕ����F
	int fourPersonFontColor_;

	//�}�E�X����
	void MouseProcess(void);


};


