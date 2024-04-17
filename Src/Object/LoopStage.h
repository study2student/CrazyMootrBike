#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;

class LoopStage : public ActorBase
{

public:


	// ���
	enum class STATE
	{
		NONE,
		IDLE,
		MAKE,
		BACK
	};

	// �R���X�g���N�^
	LoopStage(
		Player* player, const Transform& transform);

	// �f�X�g���N�^
	~LoopStage(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �v���C���[
	Player* player_;

	// ��ԊǗ�
	STATE state_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateMake(void);
	void ChangeStateBack(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateMake(void);
	void UpdateBack(void);

};

