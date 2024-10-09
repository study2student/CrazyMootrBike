#pragma once
#include <string>
#include <DxLib.h>
#include <memory>
#include "Transform.h"
#include "../../Common/Vector2.h"
#include "../../Scene/GameScene.h"
class SceneManager;
class GameScene;
class Vector2;

class Pause
{

public:

	enum class STATE
	{
		RESTART,//�ĊJ
		RETRY,	//���g���C
		END		//�I���
	};

	// �R���X�g���N�^
	Pause(void);

	// �f�X�g���N�^
	~Pause(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//�|�[�Y��Ԃ��ǂ����擾
	const bool& GetIsPause(void) const;

	//�|�[�Y�O
	void PausePrevious(void);

	//�|�[�Y��
	void PauseMidst(void);

	//�|�[�Y�摜�ݒ�
	void SetImgHandle(const int& handle);

private:

	// �|�[�Y�w�i�摜
	int imgPause_;

	//����̍ĊJ�|�W�V����
	Vector2 reStartFontBasePos_;

	//����̃��g���C�|�W�V����
	Vector2 reTryFontBasePos_;

	//����̏I���|�W�V����
	Vector2 endFontBasePos_;

	//�ĊJ�����F
	int reStartFontColor_;

	//���g���C�����F
	int reTryFontColor_;

	//�I��镶���F
	int endFontColor_;

	//���J�[�\���ʒu
	int nowCursor_;

	//�|�[�Y���j���[�����ǂ���
	bool isPause_;

	//�J�[�\���������ɂ����Ă邩�ǂ���
	bool isCursorHit_;

	//�|�[�Y�L�[���͂���̎���
	float stepPauseKeyHit_;

	//���
	STATE state_;

	//�}�E�X����
	void DecideProcess(void);

	//�L�[����
	void SelectProcess(void);

	//��ԑJ��
	void ChangeState(STATE pState);

	//���J�[�\���ԍ������Ԃ�ω�������
	void CursorToState(int cursor);

	//�|�[�Y�����`��
	void PauseDraw(void);
};

