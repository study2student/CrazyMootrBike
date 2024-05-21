#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;


class Bomb : public ActorBase
{

public:


	//���e�ꏊ�\������
	static constexpr float PLACE_DRAW_MAX_TIME = 5.0f;

	//���e������������
	static constexpr float RESERVE_MAX_TIME = 2.5f;

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

	//�w���̏��ݒ�p
	void SetHeliTrans(const Transform& heliTrans);

private:

	// �v���C���[
	Bike* bike_;

	//�w��
	Transform heliTrans_;

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

};

