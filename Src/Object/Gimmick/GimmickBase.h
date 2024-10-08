#pragma once
#include <map>
#include <vector>
#include "../Common/Transform.h"
#include "../ActorBase.h"
class Capsule;
class Collider;

class GimmickBase : public ActorBase
{

public:

	GimmickBase();

	virtual ~GimmickBase();

	void Init(void) final;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule& GetCapsule(void) const;

protected:

	//�g�����X�t�H�[��
	VECTOR initScl_;
	VECTOR initRotEuler_;
	VECTOR initLocalRotEuler_;
	VECTOR initPos_;

	//�J�v�Z���̏㉺���W
	VECTOR capsulePosTop_;
	VECTOR capsulePosDown_;
	//�J�v�Z�����a
	float capsuleRadius_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::weak_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion rotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// ������(�O���ǂݍ���)
	virtual void InitLoad(void) = 0;

	// ������(���f���ݒ�)
	virtual void InitTransform(void);

	// ������(�Փːݒ�)
	virtual void InitCollider(void);

	// ������(���㏈��)
	virtual void InitPost(void) = 0;

};

