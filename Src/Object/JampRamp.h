#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"

class JampRamp : public ActorBase
{
public:
	// �R���X�g���N�^
	JampRamp();

	// �f�X�g���N�^
	~JampRamp(void);

	void Init(void);
	void Update(void);
	void Draw(void);
private:

	// �V���O���g���Q��
	ResourceManager& resMng_;
};

