#pragma once
#include <map>
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Rotor;
class Bomb;
class GameScene;

class Helicopter : public ActorBase
{
public:

	// ���
	enum class STATE
	{
		NONE,
		MOVE,
		ATTACK,
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

	// �R���X�g���N�^
	Helicopter(GameScene* gameScene);

	// �f�X�g���N�^
	~Helicopter(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	//�o�C�N���̕ۑ�(�o�C�N�̈ʒu��]���A��O�ɂ��邩�ǂ���)
	void SetBikeTrans(const Transform& bikeTrans);
	void SetBikeIsOutside(const bool& isOutside);

	//���e�擾
	Bomb* GetBomb(void);

	// ��ԑJ��
	void ChangeState(STATE state);

private:

	//�H
	Rotor* rotor_;

	//���e
	Bomb* bomb_;

	//�o�C�N���
	Transform targetTrans_;

	//�o�C�N���X�e�[�W�O�ɂ��邩�ǂ���
	bool isTargetOutside_;

	//�Q�[���V�[��
	GameScene* gameScene_;

	// ��ԊǗ�
	STATE state_;

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

	// ��]
	Quaternion rotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

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

	// ��ԑJ��
	void ChangeStateNone(void);
	void ChangeStateMove(void);
	void ChangeStateAttack(void);
	void ChangeStateDead(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateMove(void);
	void UpdateAttack(void);
	void UpdateDead(void);

	// �`��n
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessAttack(void);//�U��
	void ProcessDebug(void);//�f�o�b�O�p

	// �U�����
	void NormalAttack(void);//�ʏ�U��
	void LongAttack(void);//�������U��

	// ��]
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	//�o�C�N�Ƃ̋����ɂ�鏈��
	void BikeDisFunc(void);
};





