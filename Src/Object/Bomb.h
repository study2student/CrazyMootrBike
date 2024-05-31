#pragma once
#include <map>
#include "Common/Transform.h"
#include "ActorBase.h"
class Player;
class Bike;
class Collider;
class Capsule;

class Bomb : public ActorBase
{

public:


	//爆弾場所表示時間
	static constexpr float PLACE_DRAW_MAX_TIME = 5.0f;

	//爆弾爆発準備時間
	static constexpr float RESERVE_MAX_TIME = 2.5f;

	//爆弾復活時間
	static constexpr float BOMB_REMAKE_MAX_TIME = 5.0f;

	// スピード
	static constexpr float SPEED = 30.0f;

	//ダメージ
	static constexpr int BOMB_DAMAGE = 20.0f;

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

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;

	//ヘリの情報設定用
	void SetHeliTrans(const Transform& heliTrans);

	//爆弾が何かに当たったか設定
	void SetIsCol(bool isCol);

	//爆弾が当たったが取得
	bool GetIsCol(void);

private:

	// プレイヤー
	Bike* bike_;

	//ヘリ
	Transform heliTrans_;

	//爆弾が爆発してからどのくらい時間経過したか
	float stepBombBlast_;

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

	//爆発目標
	VECTOR bombTargetPos_;

	// 移動後の座標
	VECTOR movedPos_;

	// 移動量
	VECTOR movePow_;

	//爆弾が何かに当たったか
	bool isCol_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	

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

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	//爆弾の生成
	void CreateBomb(void);

};

