#pragma once
#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;

class Weapon : public ActorBase
{
public:
	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �R���X�g���N�^
	Weapon(void);

	// �f�X�g���N�^
	~Weapon(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	void SetTransForm(Transform transform);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;

private:
	Transform bikeTransform_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion playerRotY_;
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

	// �̗�
	int hp_;

	// �U��������������
	bool isAttack_;

	//// ��ԑJ��
	//void ChangeState(STATE state);
	//void ChangeStateNone(void);
	//void ChangeStatePlay(void);

	// �X�V�X�e�b�v
	//void UpdateNone(void);
	//void UpdatePlay(void);

	// �`��n
	void DrawUI(void);
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessAttack(void);//�U��

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);
};

