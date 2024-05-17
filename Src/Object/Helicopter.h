#pragma once
#include <map>
#include <DxLib.h>
#include "ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Rotor;

class Helicopter : public ActorBase
{
public:

	// 通常スピード
	static constexpr float SPEED_MOVE = 110.0f;

	// 追いつくためのスピード
	static constexpr float SPEED_FAST = 130.0f;

	//横移動のスピード
	static constexpr float SPEED_MOVE_X = 80.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// HPの最大値
	static constexpr int MAX_HP = 100;

	// 状態
	enum class STATE
	{
		NONE,
		MOVE,
		ATTACK,
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

	// 攻撃種別
	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL,
		SPECIAL,
		LONG,
	};

	// コンストラクタ
	Helicopter(void);

	// デストラクタ
	~Helicopter(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;

	//バイク情報の保存
	void SetBikeTrans(const Transform& bikeTrans);

private:

	//羽
	Rotor* rotor_;

	//バイク情報
	Transform bikeTrans_;

	//// アニメーション
	//AnimationController* animationController_;

	// 状態管理
	STATE state_;

	// 攻撃状態管理
	ATTACK_TYPE attackState_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

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

	// 丸影
	int imgShadow_;

	// 体力
	int hp_;

	// 攻撃が当たったか
	bool isAttack_;

	//アニメーション
	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateMove(void);
	void ChangeStateAttack(void);
	void ChangeStateDead(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateMove(void);
	void UpdateAttack(void);
	void UpdateDead(void);

	// 描画系
	void DrawUI(void);
	void DrawShadow(void);
	void DrawBombPlace(void);
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessJump(void);//ジャンプ
	void ProcessAttack(void);//攻撃
	void ProcessDebug(void);//デバッグ用

	// 攻撃種別
	void NormalAttack(void);//通常攻撃
	void LongAttack(void);//遠距離攻撃

	// 回転
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// 着地モーション終了
	bool IsEndLanding(void);
};





