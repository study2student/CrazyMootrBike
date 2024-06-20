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


	// 状態
	enum class STATE
	{
		NONE,
		IDLE,
		MAKE,
		BACK
	};

	// コンストラクタ
	StageCurve(
		std::shared_ptr<Bike> bike, const Transform& transform);

	//デストラクタ
	~StageCurve(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Destroy(void);

	VECTOR GetPos(void);
	STATE GetState(void);
	bool IsDestroy(void);

private:

	//バイク
	std::shared_ptr<Bike> bike_;

	// 状態
	STATE state_;

	//状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateMake(void);
	void ChangeStateBack(void);

	//状態更新
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateMake(void);
	void UpdateBack(void);

};

