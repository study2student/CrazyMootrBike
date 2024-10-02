#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Rotor : public ActorBase
{

public:

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	// コンストラクタ
	Rotor();

	// デストラクタ
	~Rotor(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;

	//親ヘリコプターの情報設定
	void SetTransform(Transform& transformParent);

private:

	Transform transformParent_;

	// 状態管理
	STATE state_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion rotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 体力
	int hp_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	// 描画系
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessDebug(void);//デバッグ用

	// 回転
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);

	// 衝突判定
	void Collision(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

};







