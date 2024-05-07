#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;

class Bomb : public ActorBase
{

public:


	// 状態
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		BLAST
	};

	// コンストラクタ
	Bomb(
		Bike* bike, const Transform& transform);

	// デストラクタ
	~Bomb(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// プレイヤー
	Bike* bike_;

	// 状態管理
	STATE state_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateReserve(void);
	void ChangeStateBlast(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateBlast(void);

};

