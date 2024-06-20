#pragma once
#include <map>
#include <memory>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;

class StageCurve : public ActorBase
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
	StageCurve(
		std::shared_ptr<Bike> bike, const Transform& transform);

	//�f�X�g���N�^
	~StageCurve(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Destroy(void);

	VECTOR GetPos(void);
	STATE GetState(void);
	bool IsDestroy(void);

private:

	//�o�C�N
	std::shared_ptr<Bike> bike_;

	// ���
	STATE state_;

	//��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateMake(void);
	void ChangeStateBack(void);

	//��ԍX�V
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateMake(void);
	void UpdateBack(void);

};

