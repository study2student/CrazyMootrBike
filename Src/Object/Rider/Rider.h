#pragma once
#include <vector>
#include "../Common/Transform.h"
#include "../ActorBase.h"
class Collider;
class Capsule;
class Player;
class Bike;

class Rider : public ActorBase
{
public:
	// スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// タレットの大きさ
	static constexpr float SCALE = 8.0f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		DESTROY,
		END
	};
	// コンストラクタ
	Rider(void);
	// デストラクタ
	~Rider(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	STATE& GetState(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;
private:
	Player* player_;
	Bike* bike_;

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

	// 状態
	STATE state_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// プレイヤーの更新
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateDestroy(void);

	// プレイヤーの描画
	void DrawPlay(void);
	void DrawDestroy(void);
	void DrawDebug(void);

	// 親(戦艦)との回転と位置の同期
	void SyncParent(Transform& transform, VECTOR addAxis);
	// 操作
	void ProcessMove(void);

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


