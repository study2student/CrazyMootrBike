#pragma once
#include <vector>
#include "../Common/Transform.h"
#include "../ActorBase.h"
class Collider;
class Capsule;
class Player;
class Bike;

class Rider : public ActorBase
{
public:
	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �^���b�g�̑傫��
	static constexpr float SCALE = 8.0f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		DESTROY,
		END
	};
	// �R���X�g���N�^
	Rider(void);
	// �f�X�g���N�^
	~Rider(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	STATE& GetState(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;
private:
	Player* player_;
	Bike* bike_;

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

	// ���
	STATE state_;

	// �Փ˔���ɗp������R���C�_
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// �v���C���[�̍X�V
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateDestroy(void);

	// �v���C���[�̕`��
	void DrawPlay(void);
	void DrawDestroy(void);
	void DrawDebug(void);

	// �e(���)�Ƃ̉�]�ƈʒu�̓���
	void SyncParent(Transform& transform, VECTOR addAxis);
	// ����
	void ProcessMove(void);

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


