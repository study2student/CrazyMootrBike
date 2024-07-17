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

	enum class STATE
	{
		ONE_PERSON,		//1�l��
		FOUR_PERSON		//4�l��
	};

	//1�l�Ń{�^���̉��̒���
	static constexpr int ONE_PERSON_FONT_LENGTH = 400;

	//1�l�Ń{�^���̍���
	static constexpr int ONE_PERSON_FONT_HEIGHT = 392;

	//4�l�Ń{�^���̉��̒���
	static constexpr int FOUR_PERSON_FONT_LENGTH = 400;

	//4�l�Ń{�^���̍���
	static constexpr int FOUR_PERSON_FONT_HEIGHT = 360;

	//�ЂƂ�v���C�I���摜�ő�T�C�Y
	static constexpr float SELECT_IMG_MAX_SCALE = 1.50f;

	//�ЂƂ�v���C�I���摜�ŏ��T�C�Y
	static constexpr float SELECT_IMG_MIN_SCALE = 1.30f;

	//�l�l�v���C�I���摜�ő�T�C�Y
	static constexpr float SELECT_FOUR_IMG_MAX_SCALE = 1.50f;

	//�l�l�v���C�I���摜�ŏ��T�C�Y
	static constexpr float SELECT_FOUR_IMG_MIN_SCALE = 1.30f;

	//�I���摜�傫���ω���
	static constexpr float SELECT_IMG_CHANGE_SCALE = 0.006f;

	//�I������
	static constexpr int SELECT_MAX_NUM = 2;

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

	// �w�i�摜
	int background_;

	// �ЂƂ�őI���摜
	int aloneImg_;

	// �݂�ȂőI���摜
	int everyoneImg_;

	// �ЂƂ�v���C�I���摜�̑傫��
	float selectAloneImgScale_;
	// �l�l�v���C�I���摜�̑傫��
	float selectFourImgScale_;
	//�ő�T�C�Y���ǂ���
	bool isMaxSelectScale_;

	//1�l�ŕ����F
	int onePersonFontColor_;

	//4�l�ŕ����F
	int fourPersonFontColor_;

	//�J�[�\���������ɂ����Ă邩�ǂ���
	bool isCursorHit_;

	//���J�[�\���ʒu
	int nowCursor_;

	//���
	STATE state_;

	//�}�E�X����
	void MouseProcess(void);

	//�L�[����
	void KeyProcess(void);

	//��ԑJ��
	void ChangeState(STATE state);

	//���J�[�\���ԍ������Ԃ�ω�������
	void CursorToState(int cursor);

};


