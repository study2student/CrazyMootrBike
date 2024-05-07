#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;

class Bomb : public ActorBase
{

public:


	// ���
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		BLAST
	};

	// �R���X�g���N�^
	Bomb(
		Bike* bike, const Transform& transform);

	// �f�X�g���N�^
	~Bomb(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �v���C���[
	Bike* bike_;

	// ��ԊǗ�
	STATE state_;

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

};

