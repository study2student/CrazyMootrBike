#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Stage;
class SkyDome;
class Rider;
class Bike;
class EnemyBase;
class ShortDisEnemy;
class LongDisEnemy;
class MagicEnemy;
class EnemyBike;
class Helicopter;
class Score;

class GameScene : public SceneBase
{

public:

	enum class PAUSE_STATE
	{
		RESTART,//�ĊJ
		RETRY,	//���g���C
		END		//�I���
	};

	//�ĊJ�{�^���̉��̒���
	static constexpr int RESTART_FONT_LENGTH = 40;

	//�ĊJ�{�^���̍���
	static constexpr int RESTART_FONT_HEIGHT = 20;

	//���g���C�{�^���̉��̒���
	static constexpr int RETRY_FONT_LENGTH = 80;

	//���g���C�{�^���̍���
	static constexpr int RETRY_FONT_HEIGHT = 20;

	//�I���{�^���̉��̒���
	static constexpr int END_FONT_LENGTH = 60;

	//�I���{�^���̍���
	static constexpr int END_FONT_HEIGHT = 20;

	//�I������
	static constexpr int SELECT_MAX_NUM = 3;

	//�|�[�Y�L�[���͂��������x������悤�ɂȂ�܂ł̎���
	static constexpr float PAUSE_KEY_HIT_MAX_TIME = 0.25f;

	//�G���J�E���g�l
	static constexpr int ENCOUNT = 350;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�G���
	std::vector<EnemyBase*> GetEnemys(void);
	std::vector<EnemyBike*> GetEnemyBikes(void);

	//�G���ǉ����ꂽ�^�C�~���O���ǂ���
	bool GetIsCreateEnemy(void);
 
private:

	// �X�e�[�W
	std::shared_ptr<Stage> stage_;

	////�X�R�A
	//std::shared_ptr<Score> score_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;


	// �v���C���[
	std::shared_ptr<Bike> bike_;

	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;

	//�w���R�v�^�[
	std::shared_ptr<Helicopter> helicopter_;

	////	�G
	//Enemy* enemy_;

	//�G
	EnemyBase* enemy_;
	EnemyBike* enemyBike_;

	//�����̓G
	std::vector<EnemyBase*>enemys_;	
	std::vector<EnemyBike*>enemyBikes_;

	//�G�̔����p�x
	int enCounter;

	//�G���ǉ����ꂽ�^�C�~���O���ǂ���
	bool isCreateEnemy_;

	void DrawDubg(void);
	
	//�Փ˔���
	void Collision(void);

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
	PAUSE_STATE pState_;

	//�}�E�X����
	void MouseProcess(void);

	//�L�[����
	void KeyProcess(void);

	//��ԑJ��
	void ChangePState(PAUSE_STATE pState);

	//���J�[�\���ԍ������Ԃ�ω�������
	void CursorToPState(int cursor);

	//�|�[�Y
	void Pause(void);
};
