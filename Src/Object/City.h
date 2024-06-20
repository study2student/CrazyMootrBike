#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"
#include <vector>
class Capsule;
class Bike;

class City : public ActorBase
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
	City(std::shared_ptr<Bike> bike, const Transform& transform);

	// デストラクタ
	~City(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Destroy(void);

	VECTOR GetPos(void);
	STATE GetState(void);
	bool IsDestroy(void);
private:

	// プレイヤー
	std::shared_ptr<Bike> bike_;

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


