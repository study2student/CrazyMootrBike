#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Player;

class Bike : public ActorBase
{
public:
	// �X�s�[�h
	static constexpr float SPEED_MOVE = 100.0f;
	static constexpr float SPEED_RUN = 130.0f;

	//���ړ��̃X�s�[�h
	static constexpr float SPEED_MOVE_X = 10.0f;

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

	// �A�j���[�V����
	AnimationController* animationController_;

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

	// �̗�
	int hp_;

	// �U��������������
	bool isAttack_;

	//�A�j���[�V����
	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

	// �`��n
	void DrawUI(void);
	void DrawShadow(void);
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessJump(void);//�W�����v
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

	// ���n���[�V�����I��
	bool IsEndLanding(void);
};

