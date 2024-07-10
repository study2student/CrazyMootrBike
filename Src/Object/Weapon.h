#pragma once
#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;

class Weapon : public ActorBase
{
public:
	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// コンストラクタ
	Weapon(void);

	// デストラクタ
	~Weapon(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	void SetTransForm(Transform transform);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;

private:
	Transform bikeTransform_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプの入力受付時間
	float stepJump_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 体力
	int hp_;

	// 攻撃が当たったか
	bool isAttack_;

	//// 状態遷移
	//void ChangeState(STATE state);
	//void ChangeStateNone(void);
	//void ChangeStatePlay(void);

	// 更新ステップ
	//void UpdateNone(void);
	//void UpdatePlay(void);

	// 描画系
	void DrawUI(void);
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessAttack(void);//攻撃

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);
};

