#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;
class Player;

class Bike : public ActorBase
{
public:
	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
	};
	// �R���X�g���N�^
	Bike(void);

	// �f�X�g���N�^
	~Bike(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;
private:
	Transform transformPlayer_;

	Player* player_;

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

	// �ۉe
	int imgShadow_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

	// �`��n
	void DrawShadow(void);
	void DrawDebug(void);

	// ����
	void ProcessMove(void);
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

};

