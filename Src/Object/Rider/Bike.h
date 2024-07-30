#pragma once
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Player;
class Score;

class Bike : public ActorBase
{
public:
	//�v���C���[�̏������W
	static constexpr VECTOR INIT_POS = { 1270.0f , -260.0f ,0.0f };

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 100.0f;
	static constexpr float SPEED_RUN = 130.0f;

	// �v���C���[�̌X��
	static constexpr float SLOPE = 45.0f;

	// �����蔻��p�R���C�_�[�̏���W
	static constexpr VECTOR COLLIDER_POS_TOP = { 0.0f, 130.0f, 0.0f };

	// �����蔻��p�R���C�_�[�̏���W
	static constexpr VECTOR COLLIDER_POS_DOWN = { 0.0f, 130.0f, -150.0f };

	//���a
	static constexpr float RADIUS = 80.0f;

	//�u�[�X�g�g�p���̉������x
	static constexpr float ADD_SPEED_BOOST = 50.0f;

	//���u�[�X�g�g����悤�ɂȂ�܂ł̃J�E���g
	static constexpr float DELEY_BOOST_MAX = 220.0f;

	//�u�[�X�g���������邽�߂�HP�����
	static constexpr int BOOST_USE_HP = 40;

	//���ړ��̃X�s�[�h
	static constexpr float SPEED_MOVE_X = 18.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// HP�̍ő�l
	static constexpr int MAX_HP = 100;

	//HP�̍Œ�l
	static constexpr int MIN_HP = 0;

	// �W�����v��
	static constexpr float POW_JUMP = 70.0f;

	// �W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//���h��}�b�N�X����
	static constexpr float SWAY_CURRENT_MAX_TIME = 70.0f;

	//�o�[���A�E�g�G�t�F�N�g��������
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -280.0f; //-290.0f

	//�o�[���A�E�g�G�t�F�N�g�ő卂��
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	//�ҋ@�G�t�F�N�g������
	static constexpr float IDLE_EFFECT_POS_Y = -510.0f;

	//�o�C�N����t�����g�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS = { 0.0f,38.0f,170.0f };

	//�o�C�N���烊�A�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_REAR_TYRE_LOCALPOS = { -3.0f,53.0f,-47.0f };

	// �o�C�N����u�[�X�g�G�t�F�N�g�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_P2EB_POS = { 0.0f, 100.0f, -200.0f };

	// �o�C�N����v���C���[�܂ł̑��΍��W
	static constexpr VECTOR RELATIVE_P2B_POS = { 0.0f,40.0f,-65.0f };

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		CRASH,
		DEAD,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		SIT
	};

	enum class JoypadButton {
		UP = PAD_INPUT_UP,
		DOWN = PAD_INPUT_DOWN,
		LEFT = PAD_INPUT_LEFT,
		RIGHT = PAD_INPUT_RIGHT,
		ACTION =PAD_INPUT_6
	};

	// �v���C���[���Ƃ̓��̓}�b�s���O
	struct PlayerInput {
		int padId;
		JoypadButton up;
		JoypadButton down;
		JoypadButton left;
		JoypadButton right;
		JoypadButton action;
	};

	// �R���X�g���N�^
	Bike(float localpos, int playerID);

	// �f�X�g���N�^
	~Bike();

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	// �G�t�F�N�g����
	void SyncBoostEffect(Transform player);


	//�_���[�W
	void Damage(int damage);

	//��HP�擾
	const int& GetHP(void);

	//�o�C�N����O�ɏo�����擾
	const bool& GetIsOutSide(void);

	//�o�C�N���S�[���������ݒ�
	void SetIsGoal(bool isGoal);

	//�o�C�N���S�[���������擾
	const bool& GetIsGoal(void);

	// �X�R�A���Z
	void AddScore(int score);
	// �X�R�A�Q�b�^�[
	const int GetScore() const;

	//�v���C���[ID�̃Q�b�^�[
	const int GetPlayerID(void) const;

	// �v���C���[���m�̓����蔻��
	void Flip(VECTOR dir);

	// �u�[�X�g���g�������ǂ���
	const bool IsBoost(void);

private:

	Transform transformPlayer_;

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;


	// ��ԊǗ�
	STATE state_;

	//��O�ɂ��邩�ǂ���
	bool isOutSide_;

	// �v���C���[ID��ێ�
	int playerID_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// �v���C���[���m�̉����̒���
	float localPosX_;

	// ��]
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float currentTime = 0.0f;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v�X�s�[�h
	float jumpSpeed_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	//�W�����v���Ă���b��������
	float stepJumpSecond_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �ۉe
	int imgShadow_;

	// �̗�
	int hp_;

	// �X�R�A
	std::shared_ptr<Score>score_;

	// �U��������������
	bool isAttack_;

	//�v���C���[���m�̓����蔻��p
	float flipSpeed_;
	VECTOR flipDir_;

	//�A�j���[�V����
	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);
	void ChangeStateCrash(void);
	void ChangeStateDead(void);


	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateFliped(void);
	void UpdateCrash(void);
	void UpdateDead(void);

	// �`��n
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessBoost(void); //�u�[�X�g
	void ProcessDebug(void);//�f�o�b�O�p

	// ��]
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	//�G�t�F�N�g�n
	// �q�b�g�G�t�F�N�g
	int effectSonicResId_;
	int effectSonicPlayId_;

	// �u�[�X�g���̉��Z���x
	float speedBoost_;

	// �u�[�X�g�g�p�Ԋu
	float deleyBoost_;

	// �u�[�X�g���g�p������
	bool isBoost_;

	//�S�[��������
	bool isGoal_;

	//�u�[�X�g�G�t�F�N�g�ʒu
	VECTOR boostEffectPos;

	//�u�[�X�g�G�t�F�N�g
	int effectBoostResId_;
	int effectBoostPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);
	// �\�j�b�N�u�[���G�t�F�N�g�̈ʒu
	void SonicBoomEffect(void);

};

