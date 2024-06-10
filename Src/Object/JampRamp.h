#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"
#include <vector>
class Capsule;

class JampRamp : public ActorBase
{
public:
	// �R���X�g���N�^
	JampRamp();

	// �f�X�g���N�^
	~JampRamp(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;
private:

	// �ړ���̍��W
	VECTOR movedPos_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	void CollisionCapsule(void);

	void DrawDebug(void);
};

