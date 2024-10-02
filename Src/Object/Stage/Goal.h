#pragma once
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;

class Goal : public ActorBase
{

public:

	//ゴールを生成するための最大ステージ数
	static constexpr int STAGE_NUM_MAX_FOR_GOAL = 75;


	// コンストラクタ
	Goal(void);

	// デストラクタ
	~Goal(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	//位置設定
	void SetPosZ(float z);

private:

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	//描画系
	void DrawDebug(void);

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);
};






