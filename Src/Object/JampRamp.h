#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"

class JampRamp : public ActorBase
{
public:
	// コンストラクタ
	JampRamp();

	// デストラクタ
	~JampRamp(void);

	void Init(void);
	void Update(void);
	void Draw(void);
private:

	// シングルトン参照
	ResourceManager& resMng_;
};

