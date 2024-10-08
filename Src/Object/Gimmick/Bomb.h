#pragma once
#include <map>
#include "../Common/Transform.h"
#include "../ActorBase.h"
#include "GimmickBase.h"
class Player;
class Collider;
class Capsule;
class Helicopter;

class Bomb : public GimmickBase
{

public:

	//�_���[�W
	static constexpr int BOMB_DAMAGE = 50;

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

	void Update(void) override;
	void Draw(void) override;

	//�w���̏��ݒ�p
	void SetHeliTrans(const Transform& heliTrans);

	//���e�������ɓ����������ݒ�
	void SetIsCol(bool isCol);

	//���e�������������擾
	const bool& GetIsCol(void) const;

	//��Ԏ擾
	const STATE& GetState(void) const;

private:

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
	int bombEffectPlayId_;

	//�����ڕW
	VECTOR bombTargetPos_;

	//���e�������ɓ���������
	bool isCol_;

	//�G�t�F�N�g�ǂݍ���
	void InitEffectLoad(void);

	//�����G�t�F�N�g�Đ�
	void PlayBombEffect(void);

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

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ������(�O���ǂݍ���)
	void InitLoad(void) override;

	// ������(���㏈��)
	void InitPost(void) override;

};

