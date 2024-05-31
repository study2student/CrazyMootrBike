#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;
class Collider;
class Capsule;

class Bomb : public ActorBase
{

public:


	//���e�ꏊ�\������
	static constexpr float PLACE_DRAW_MAX_TIME = 5.0f;

	//���e������������
	static constexpr float RESERVE_MAX_TIME = 2.5f;

	//���e��������
	static constexpr float BOMB_REMAKE_MAX_TIME = 5.0f;

	// �X�s�[�h
	static constexpr float SPEED = 30.0f;

	//�_���[�W
	static constexpr int BOMB_DAMAGE = 20.0f;

	// ���
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		BLAST
	};

	// �R���X�g���N�^
	Bomb();

	// �f�X�g���N�^
	~Bomb(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;

	//�w���̏��ݒ�p
	void SetHeliTrans(const Transform& heliTrans);

	//���e�������ɓ����������ݒ�
	void SetIsCol(bool isCol);

	//���e�������������擾
	bool GetIsCol(void);

private:

	// �v���C���[
	Bike* bike_;

	//�w��
	Transform heliTrans_;

	//���e���������Ă���ǂ̂��炢���Ԍo�߂�����
	float stepBombBlast_;

	//���e�ꏊ�\������
	float stepPlaceDrawTime_;

	//���e������������
	float stepReserveTime_;

	// ��ԊǗ�
	STATE state_;

	//�����G�t�F�N�g
	int bombEffectResId_;

	//�����G�t�F�N�g
	int bombEffectPlayId_;

	//�����ڕW
	VECTOR bombTargetPos_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// �ړ���
	VECTOR movePow_;

	//���e�������ɓ���������
	bool isCol_;

	// �Փ˔���ɗp������R���C�_
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateReserve(void);
	void ChangeStateBlast(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateBlast(void);

	//���e�̈ʒu
	void DrawBombPlace(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	//���e�̐���
	void CreateBomb(void);

};

