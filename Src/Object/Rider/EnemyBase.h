#pragma once
#include <map>
//#include <vector>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Bike;

class EnemyBase : public ActorBase
{

public:

	// スピード
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//半径
	static constexpr float RADIUS = 200.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// ジャンプ力
	static constexpr float POW_JUMP = 35.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
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
		VICTORY,
		SHORT,
		LONG,
		BOMB

	};

	//敵の種類
	enum class TYPE
	{
		SHORT_DIS,
		LONG_DIS,
		BOMB,
		MAX
	};

	// コンストラクタ
	EnemyBase(Bike* bike);

	// デストラクタ
	virtual ~EnemyBase(void);

	virtual void Init(void) override;
	virtual void SetParam(void);
	virtual void Update(void) override;
	virtual void Draw(void) override;

	virtual void UpdatePlay(void);
	virtual void ProcessMove(void);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;

	//プレイヤー(バイク)の情報設定
	void SetBikeTrans(Transform bikeTrans);

	//スピード設定
	void SetSpeed(float speed);

	//敵同士が当たったか設定
	void SetIsEnemyCol(bool isEnemyCol);

	// 敵に吹っ飛ばされた
	void Flip(VECTOR dir);

protected:

	// アニメーション
	AnimationController* animationController_;

	//バイク情報
	Bike* bike_;

	// 状態管理
	STATE state_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion enemyRotY_;
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

	// 丸影
	int imgShadow_;

	//プレイヤー(バイク)とあたっているかどうか
	bool isBikeCol_;

	bool isEnemyCol_;

	//攻撃しているか
	bool isAtk_;

	//攻撃用
	VECTOR fowardPos_;
	VECTOR backPos_;

	//攻撃状態までの時間
	float toAtkStep_;

	float flipSpeed_;
	VECTOR flipDir_;

	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateFliped(void);

	// 描画系
	void DrawShadow(void);
	void DrawHpBar(void);

	// 操作
	void ProcessJump(void);

	// 回転
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// 着地モーション終了
	bool IsEndLanding(void);

	//攻撃モーション入ってからどのくらいで攻撃判定になるか管理
	bool IsAtkStart(void);


};





