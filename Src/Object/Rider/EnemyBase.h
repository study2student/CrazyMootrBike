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
class Score;

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

	//1ループステージあたりの敵の生成数
	static constexpr int MAX_MAKE_NUM = 3;

	//敵の生成時のセンター方向からの横幅
	static constexpr float DIR_LEN = 500.0f;

	//縦の敵間距離
	static constexpr float X_LEN = 400.0f;

	//敵生成時の横の調整用
	static constexpr float ADJUST_POS_X = 14300.0f;

	//倒した時のスコア増分
	static constexpr int SCORE_INCREMENT = 100;

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

	//敵発生場所
	enum class DIR
	{
		LEFT,
		CENTER,
		RIGHT,
		MAX

	};

	// コンストラクタ
	EnemyBase(std::shared_ptr<Bike> bike, VECTOR loopStagePos, VECTOR localPos);

	// デストラクタ
	virtual ~EnemyBase(void);

	virtual void Init(void) override;
	virtual void SetParam(void);
	virtual void Update(void) override;
	virtual void Draw(void) override;

	virtual void UpdatePlay(void);
	virtual void ProcessMove(void);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
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

	// プレイヤーと当たったか
	bool GetIsBikeCol(void);

	//スコアを加算してよいか取得
	bool GetIsAddScore(void);

	//状態取得
	STATE GetState(void);

	//死亡状態か
	bool IsDestroy(void);

	//死亡状態へ
	void Destroy(void);

protected:

	// アニメーション
	AnimationController* animationController_;

	//バイク情報
	std::shared_ptr<Bike> bike_;

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

	//敵の発生座標座標
	VECTOR makePos_;

	//先頭の敵から次の敵までの相対座標
	VECTOR localPos_;

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
	std::vector<std::shared_ptr<Collider>> colliders_;
	Capsule* capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 丸影
	int imgShadow_;

	//プレイヤー(バイク)とあたっているかどうか
	bool isBikeCol_;

	//他の敵と当たっているか
	bool isEnemyCol_;

	//攻撃しているか
	bool isAtk_;

	//攻撃用
	VECTOR fowardPos_;
	VECTOR backPos_;

	//攻撃状態までの時間
	float toAtkStep_;

	//スコアを加算してもよいか
	bool isAddScore_;

	float flipSpeed_;
	VECTOR flipDir_;

	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);
	void ChangeStateDead(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateFliped(void);
	void UpdateDead(void);

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





