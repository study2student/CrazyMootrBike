#pragma once
#include <map>
#include <DxLib.h>
#include <memory>
#include "ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Goal : public ActorBase
{
public:

	//�S�[����{�ʒu
	static constexpr VECTOR GOAL_BASE_POS = { 1650.0f,-800.0f,0.0f };

	//�S�[���𐶐����邽�߂̍ő�X�e�[�W��
	//static constexpr int STAGE_NUM_MAX_FOR_GOAL = 68;
	static constexpr int STAGE_NUM_MAX_FOR_GOAL = 28;

	// �R���X�g���N�^
	Goal(void);

	// �f�X�g���N�^
	~Goal(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	//�ʒu�ݒ�
	void SetPosZ(float z);

private:

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

	//�`��n
	void DrawDebug(void);

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);
};






