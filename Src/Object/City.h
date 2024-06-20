#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"
#include <vector>
class Capsule;
class Bike;

class City : public ActorBase
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
	City(std::shared_ptr<Bike> bike, const Transform& transform);

	// �f�X�g���N�^
	~City(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

	VECTOR GetPos(void);
	STATE GetState(void);
	bool IsDestroy(void);
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


