#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Camera;
class Stage;
class SkyDome;
class Bike;
class CoinBase;
class GoldCoin;
class SilverCoin;
class CopperCoin;
class Helicopter;
class Score;
class Spike;

class GameScene : public SceneBase
{

public:

	enum class PAUSE_STATE
	{
		RESTART,//�ĊJ
		RETRY,	//���g���C
		END		//�I���
	};

	// �v���C���[���m�̏�����
	static constexpr float PLAYER_WIDTH = 200.0f;

	//�p�b�h�ő吔
	static constexpr int PAD_MAX = 4;

	//�ĊJ�{�^���̉��̒���
	static constexpr int RESTART_FONT_LENGTH = 100;

	//�ĊJ�{�^���̍���
	static constexpr int RESTART_FONT_HEIGHT = 48;

	//���g���C�{�^���̉��̒���
	static constexpr int RETRY_FONT_LENGTH = 200;

	//���g���C�{�^���̍���
	static constexpr int RETRY_FONT_HEIGHT = 48;

	//�I���{�^���̉��̒���
	static constexpr int END_FONT_LENGTH = 150;

	//�I���{�^���̍���
	static constexpr int END_FONT_HEIGHT = 48;

	//�I������
	static constexpr int SELECT_MAX_NUM = 3;

	//�x���摜�ő�T�C�Y
	static constexpr float WARNING_IMG_MAX_SCALE = 0.40f;

	//�x���摜�ŏ��T�C�Y
	static constexpr float WARNING_IMG_MIN_SCALE = 0.20f;

	//�x���摜�傫���ω���
	static constexpr float WARNING_IMG_CHANGE_SCALE = 0.005f;

	//�x���摜Y���W
	static constexpr float WARNING_POS_Y = 120.0f;

	//�|�[�Y�L�[���͂��������x������悤�ɂȂ�܂ł̎���
	static constexpr float PAUSE_KEY_HIT_MAX_TIME = 1.25f;

	//�S�[�����Ă��玟�̃V�[���ɂȂ�܂ł̎���
	static constexpr float GOAL_TO_NEXT_SCENE = 2.5f;

	// HP�o�[�̕��ƍ���
	static constexpr int HP_BER = 10;

	// �R���g���[���[�̗h��l
	static constexpr int CE_SWING_VALUE = 1000;

	// �R���g���[���[�̗h�ꎞ��
	static constexpr int CE_SWING_TIME = 700;

	//�G���J�E���g�l
	static constexpr int ENCOUNT = 350;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�R�C�����
	std::vector<CoinBase*> GetEnemys(void);

	//�R�C�����ǉ����ꂽ�^�C�~���O���ǂ���
	bool GetIsCreateEnemy(void);

	//�v���C���[�l���擾
	int GetPlayNum(void);

	//1�l�v���C���S�[���������ǂ���
	bool OnePersonIsGoal(void);

	//�擪(���W)�̃o�C�N�̗v�f�ԍ����擾
	size_t GetBikeMaxPosZIndex(void);
	

private:
	int mainScreen_;

	//1�l�v���C���S�[���������ǂ���
	bool onePersonIsGoal_;

	// �Q�[���X�^�[�g���̃J�E���g
	float startCount_;
	// �X�^�[�g�t���O
	bool isStart_;

	// �x���摜
	int imgWarning_;
	//�傫��
	float warningImgScale_;
	//�ő�T�C�Y���ǂ���
	bool isMaxWarningScale_;

	// �|�[�Y�w�i�摜
	int imgPause_;

	//FINISH�摜
	int imgFinish_;

	//�R�C���摜
	int imgCoin_;

	// �J����
	std::vector<std::shared_ptr<Camera>> cameras_;

	// �X�e�[�W
	std::shared_ptr<Stage> stage_;

	// �X�J�C�h�[��
	std::vector<std::unique_ptr<SkyDome>> skyDomes_;

	//�g�Q
	std::shared_ptr<Spike> spike_;

	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;

	//�w���R�v�^�[
	std::shared_ptr<Helicopter> helicopter_;

	//�R�C��
	CoinBase* coin_;

	//�����̃R�C��
	std::vector<CoinBase*>coins_;

	//�R�C���̔����p�x
	int enCounter;

	//�R�C�����ǉ����ꂽ�^�C�~���O���ǂ���
	bool isCreateCoin_;

	// �J������X�N���[���̕\���̊֐���
	void DrawObject(int playerID);
	//�f�o�b�O�\��
	void DrawDubg(void);
	// UI�\��
	void DrawUI(int x, int y, int playerID);

	// �S�[���������̏���
	void GoalProcess(void);

	// �R�C���̔z�u
	void CoinPlace(void);

	//�Փ˔���
	void Collision(void);

	// �o�C�N���m�̏Փ˔���
	void BikeCollision(void);

	float hitStopDuration;  // �q�b�g�X�g�b�v�̎������ԁi�b�j
	float hitStopTimer;      // �q�b�g�X�g�b�v�̃^�C�}�[
	bool isHitStop;        // �q�b�g�X�g�b�v�����ǂ����̃t���O

	// �q�b�g�G�t�F�N�g
	int effectHitResId_;
	int effectHitPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);
	// Hit�G�t�F�N�g�̈ʒu
	void HitEffect(void);

	//�v���C���[�l��
	int playNumber_;

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

	//FINISH�����̈ʒu
	Vector2 finishFontPos_;

	//�S�[�����Ă���̌o�ߎ���
	float stepGoalAfter_;

	//���
	PAUSE_STATE pState_;

	//�}�E�X����
	void DecideProcess(void);

	//�L�[����
	void SelectProcess(void);

	//��ԑJ��
	void ChangePState(PAUSE_STATE pState);

	//���J�[�\���ԍ������Ԃ�ω�������
	void CursorToPState(int cursor);

	//�|�[�Y
	void Pause(void);

	//�x��
	void WarningDraw(void);

	//�|�[�Y�����`��
	void PauseDraw(void);

	//�S�[���������Ƃ̏���
	void GoalAfterDraw(void);

	//�R�C���摜�`��
	void CoinImgDraw(int x, int y);
};
