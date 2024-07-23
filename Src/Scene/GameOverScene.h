#pragma once
#include <map>
#include "SceneBase.h"
class Score;
class Bike;

class GameOverScene : public SceneBase
{
public:

	enum class PAUSE_STATE
	{
		RETRY,	//���g���C
		END		//�I���
	};

	//���g���C�{�^���̉��̒���
	static constexpr int RETRY_FONT_LENGTH = 200;

	//���g���C�{�^���̍���
	static constexpr int RETRY_FONT_HEIGHT = 48;

	//�I���{�^���̉��̒���
	static constexpr int END_FONT_LENGTH = 150;

	//�I���{�^���̍���
	static constexpr int END_FONT_HEIGHT = 48;

	//�I������
	static constexpr int SELECT_MAX_NUM = 2;

	// �R���X�g���N�^
	GameOverScene(void);

	// �f�X�g���N�^
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;
	//std::shared_ptr<Score> score_;

	//�X�R�A�`��̔w�i
	int img_BG;
	int imgPush_;

	//�Z���N�g�w�i
	int imgSelectBG_;

	//���
	PAUSE_STATE pState_;

	//����̃��g���C�|�W�V����
	Vector2 reTryFontBasePos_;

	//����̏I���|�W�V����
	Vector2 endFontBasePos_;

	//���g���C�����F
	int reTryFontColor_;

	//�I��镶���F
	int endFontColor_;

	//���J�[�\���ʒu
	int nowCursor_;

	//�J�[�\���������ɂ����Ă邩�ǂ���
	bool isCursorHit_;

	//�}�E�X����
	void DecideProcess(void);

	//�L�[����
	void SelectProcess(void);

	//��ԑJ��
	void ChangePState(PAUSE_STATE pState);

	//���J�[�\���ԍ������Ԃ�ω�������
	void CursorToPState(int cursor);
};

