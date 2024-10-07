#pragma once
#include "../Common/Transform.h"
#include "../ActorBase.h"
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
		std::shared_ptr<Bike> bike, const Transform& transform);

	// �f�X�g���N�^
	~LoopStage(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Destroy(void);

	const VECTOR& GetPos(void) const;
	const STATE& GetState(void) const;
	const bool& IsDestroy(void);

private:

	// �v���C���[
	std::shared_ptr<Bike> bike_;

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

