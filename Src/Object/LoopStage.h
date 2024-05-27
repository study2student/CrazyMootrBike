#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;

class LoopStage : public ActorBase
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
	LoopStage(
		Bike* bike, const Transform& transform);

	// デストラクタ
	~LoopStage(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Destroy(void);

	VECTOR GetPos(void);
	STATE GetState(void);
	bool IsDestroy(void);

private:

	// プレイヤー
	Bike* bike_;

	// 状態管理
	STATE state_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateMake(void);
	void ChangeStateBack(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateMake(void);
	void UpdateBack(void);

};

