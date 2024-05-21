#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;


class Bomb : public ActorBase
{

public:


	//爆弾場所表示時間
	static constexpr float PLACE_DRAW_MAX_TIME = 5.0f;

	//爆弾爆発準備時間
	static constexpr float RESERVE_MAX_TIME = 2.5f;

	// 状態
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		BLAST
	};

	// コンストラクタ
	Bomb();

	// デストラクタ
	~Bomb(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//ヘリの情報設定用
	void SetHeliTrans(const Transform& heliTrans);

private:

	// プレイヤー
	Bike* bike_;

	//ヘリ
	Transform heliTrans_;

	//爆弾場所表示時間
	float stepPlaceDrawTime_;

	//爆弾爆発準備時間
	float stepReserveTime_;

	// 状態管理
	STATE state_;

	//爆発エフェクト
	int bombEffectResId_;

	//爆発エフェクト
	int bombEffectPlayId_;
	

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

	//爆弾の位置
	void DrawBombPlace(void);

};

