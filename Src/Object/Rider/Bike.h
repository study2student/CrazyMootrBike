#pragma once
#include <map>
#include <DxLib.h>
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Player;
class FrontTyre;
class RearTyre;
class Score;

class Bike : public ActorBase
{
public:
	// �X�s�[�h
	static constexpr float SPEED_MOVE = 100.0f;
	static constexpr float SPEED_RUN = 130.0f;

	//���a
	static constexpr float RADIUS = 50.0f;

	//���ړ��̃X�s�[�h
	static constexpr float SPEED_MOVE_X = 10.0f;

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

	//�o�[���A�E�g�G�t�F�N�g��������
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -290.0f;

	//�o�[���A�E�g�G�t�F�N�g�ő卂��
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	//�o�C�N����t�����g�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS = { 10.0f,68.0f,45.0f };

	//�o�C�N���烊�A�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_REAR_TYRE_LOCALPOS = { -5.0f,58.0f,-157.0f };

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		DEAD,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	// �U�����
	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL,
		SPECIAL,
		LONG,
	};

	//�{�^�����Ƃ̃A�N�V����
	enum class JoypadButton {
		UP = PAD_INPUT_UP,
		DOWN = PAD_INPUT_DOWN,
		LEFT = PAD_INPUT_LEFT,
		RIGHT = PAD_INPUT_RIGHT,
		ACTION = PAD_INPUT_1
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

	// �X�R�A���Z
	void AddScore(int score);

	// �X�R�A�Q�b�^�[
	const int GetScore() const;

	//�W�����v��p�̃W�����v
	void Jump(void);

	//�_���[�W
	void Damage(int damage);

	// �v���C���[���m�̓����蔻��
	void Flip(VECTOR dir);

	const int GetPlayerID(void) const;
private:

	Transform transformPlayer_;

	// �A�j���[�V����
	std::unique_ptr<AnimationController> animationController_;

	//�^�C��
	std::shared_ptr<FrontTyre> frontTyre_;
	std::shared_ptr<RearTyre> rearTyre_;

	// ��ԊǗ�
	STATE state_;

	// �v���C���[ID��ێ�
	int playerID_;

	// �U����ԊǗ�
	ATTACK_TYPE attackState_;

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

	// �̗�
	int hp_;

	// �U��������������
	bool isAttack_;

	//�v���C���[���m�̓����蔻��p
	float flipSpeed_;
	VECTOR flipDir_;

	// �X�R�A
	std::shared_ptr<Score>score_;

	//�A�j���[�V����
	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateFliped(void);

	// �`��n
	void DrawUI(void);
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessJump(void);//�W�����v
	void ProcessAttack(void);//�U��
	void ProcessDebug(void);//�f�o�b�O�p

	// �U�����
	void NormalAttack(void);//�ʏ�U��
	void LongAttack(void);//�������U��
	void SpecialAttack(void);//�K�E�Z

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

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	//�G�t�F�N�g�n
	// �q�b�g�G�t�F�N�g
	int effectSonicResId_;
	int effectSonicPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);
	// Hit�G�t�F�N�g�̈ʒu
	void SonicBoomEffect(void);
};

