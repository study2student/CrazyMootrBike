#pragma once
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Score;

class Bike : public ActorBase
{
public:

	// ���a
	static constexpr float RADIUS = 80.0f;

	// HP�̍ő�l
	static constexpr int MAX_HP = 100;

	// HP�̍Œ�l
	static constexpr int MIN_HP = 0;

	// �o�[���A�E�g�G�t�F�N�g��������
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -280.0f;

	// �o�[���A�E�g�G�t�F�N�g�ő卂��
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	// �ҋ@�G�t�F�N�g������
	static constexpr float IDLE_EFFECT_POS_Y = -510.0f;

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

	// �p�b�h�̃{�^���U�蕪��
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
	~Bike(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const { return capsule_; }

	// �G�t�F�N�g����
	void SyncBoostEffect(Transform player);

	// �_���[�W
	void Damage(int damage);

	// ��HP�擾
	const int& GetHP(void)const& { return hp_; }

	// �o�C�N����O�ɏo�����擾
	const bool& GetIsOutSide(void)const& { return isOutSide_; }

	// �o�C�N���S�[���������ݒ�
	void SetIsGoal(bool isGoal);

	// �o�C�N���S�[���������擾
	const bool& GetIsGoal(void) const& { return isGoal_; }

	// �X�R�A���Z
	void AddScore(int score);

	// �X�R�A�Q�b�^�[
	const int GetScore(void) const;

	// �v���C���[ID�̃Q�b�^�[
	const int& GetPlayerID(void) const& { return playerID_; }

	// �v���C���[���m�̓����蔻��
	void Flip(VECTOR dir);

	// �u�[�X�g���g�������ǂ���
	const bool& GetIsBoost(void)const& { return isBoost_; }

private:

	Transform transformPlayer_;

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	// ��ԊǗ�
	STATE state_;

	// ��O�ɂ��邩�ǂ���
	bool isOutSide_;

	// �v���C���[ID��ێ�
	int playerID_;

	// �ړ��X�s�[�h
	float moveSpeed_;

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

	//
	float currentTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	//�W�����v���Ă���b��������
	float stepJumpSecond_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	// �Փ˔���p�̃J�v�Z��
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �̗�
	int hp_;

	// �X�R�A
	std::shared_ptr<Score>score_;

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
	void ProcessBoost(void);//�u�[�X�g
	bool IsBoostPush(void);//�u�[�X�g�{�^�������������ǂ���

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

	// �u�[�X�g���̉��Z���x
	float speedBoost_;

	// �u�[�X�g�g�p�Ԋu
	float deleyBoost_;

	// �u�[�X�g���g�p������
	bool isBoost_;

	//�S�[��������
	bool isGoal_;

	//�u�[�X�g�G�t�F�N�g
	int effectBoostResId_;
	int effectBoostPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);

};

