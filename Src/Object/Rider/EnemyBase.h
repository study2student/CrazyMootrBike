#pragma once
#include <map>
//#include <vector>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Bike;

class EnemyBase : public ActorBase
{

public:

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//���a
	static constexpr float RADIUS = 200.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �W�����v��
	static constexpr float POW_JUMP = 35.0f;

	// �W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
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
		VICTORY,
		SHORT,
		LONG,
		BOMB

	};

	//�G�̎��
	enum class TYPE
	{
		SHORT_DIS,
		LONG_DIS,
		BOMB,
		MAX
	};

	// �R���X�g���N�^
	EnemyBase(Bike* bike);

	// �f�X�g���N�^
	virtual ~EnemyBase(void);

	virtual void Init(void) override;
	virtual void SetParam(void);
	virtual void Update(void) override;
	virtual void Draw(void) override;

	virtual void UpdatePlay(void);
	virtual void ProcessMove(void);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;

	//�v���C���[(�o�C�N)�̏��ݒ�
	void SetBikeTrans(Transform bikeTrans);

	//�X�s�[�h�ݒ�
	void SetSpeed(float speed);

	//�G���m�������������ݒ�
	void SetIsEnemyCol(bool isEnemyCol);

	// �G�ɐ�����΂��ꂽ
	void Flip(VECTOR dir);

protected:

	// �A�j���[�V����
	AnimationController* animationController_;

	//�o�C�N���
	Bike* bike_;

	// ��ԊǗ�
	STATE state_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion enemyRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	// �Փ˔���ɗp������R���C�_
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �ۉe
	int imgShadow_;

	//�v���C���[(�o�C�N)�Ƃ������Ă��邩�ǂ���
	bool isBikeCol_;

	bool isEnemyCol_;

	//�U�����Ă��邩
	bool isAtk_;

	//�U���p
	VECTOR fowardPos_;
	VECTOR backPos_;

	//�U����Ԃ܂ł̎���
	float toAtkStep_;

	float flipSpeed_;
	VECTOR flipDir_;

	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateFliped(void);

	// �`��n
	void DrawShadow(void);
	void DrawHpBar(void);

	// ����
	void ProcessJump(void);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	//�U�����[�V���������Ă���ǂ̂��炢�ōU������ɂȂ邩�Ǘ�
	bool IsAtkStart(void);


};





