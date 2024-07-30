#pragma once
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Player;
class Score;

class Bike : public ActorBase
{
public:
	//プレイヤーの初期座標
	static constexpr VECTOR INIT_POS = { 1270.0f , -260.0f ,0.0f };

	// スピード
	static constexpr float SPEED_MOVE = 100.0f;
	static constexpr float SPEED_RUN = 130.0f;

	// プレイヤーの傾き
	static constexpr float SLOPE = 45.0f;

	// 当たり判定用コライダーの上座標
	static constexpr VECTOR COLLIDER_POS_TOP = { 0.0f, 130.0f, 0.0f };

	// 当たり判定用コライダーの上座標
	static constexpr VECTOR COLLIDER_POS_DOWN = { 0.0f, 130.0f, -150.0f };

	//半径
	static constexpr float RADIUS = 80.0f;

	//ブースト使用時の加速速度
	static constexpr float ADD_SPEED_BOOST = 50.0f;

	//次ブースト使えるようになるまでのカウント
	static constexpr float DELEY_BOOST_MAX = 220.0f;

	//ブースト発動させるためのHP消費量
	static constexpr int BOOST_USE_HP = 40;

	//横移動のスピード
	static constexpr float SPEED_MOVE_X = 18.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	// HPの最大値
	static constexpr int MAX_HP = 100;

	//HPの最低値
	static constexpr int MIN_HP = 0;

	// ジャンプ力
	static constexpr float POW_JUMP = 70.0f;

	// ジャンプ受付時間
	static constexpr float TIME_JUMP_IN = 0.5f;

	//横揺れマックス時間
	static constexpr float SWAY_CURRENT_MAX_TIME = 70.0f;

	//バーンアウトエフェクト初期高さ
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -280.0f; //-290.0f

	//バーンアウトエフェクト最大高さ
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	//待機エフェクト初期高
	static constexpr float IDLE_EFFECT_POS_Y = -510.0f;

	//バイクからフロントタイヤ相対座標
	static constexpr VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS = { 0.0f,38.0f,170.0f };

	//バイクからリアタイヤ相対座標
	static constexpr VECTOR BIKE_TO_REAR_TYRE_LOCALPOS = { -3.0f,53.0f,-47.0f };

	// バイクからブーストエフェクトまでの相対座標
	static constexpr VECTOR RELATIVE_P2EB_POS = { 0.0f, 100.0f, -200.0f };

	// バイクからプレイヤーまでの相対座標
	static constexpr VECTOR RELATIVE_P2B_POS = { 0.0f,40.0f,-65.0f };

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		CRASH,
		DEAD,
		END
	};

	// アニメーション種別
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		SIT
	};

	enum class JoypadButton {
		UP = PAD_INPUT_UP,
		DOWN = PAD_INPUT_DOWN,
		LEFT = PAD_INPUT_LEFT,
		RIGHT = PAD_INPUT_RIGHT,
		ACTION =PAD_INPUT_6
	};

	// プレイヤーごとの入力マッピング
	struct PlayerInput {
		int padId;
		JoypadButton up;
		JoypadButton down;
		JoypadButton left;
		JoypadButton right;
		JoypadButton action;
	};

	// コンストラクタ
	Bike(float localpos, int playerID);

	// デストラクタ
	~Bike();

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	// エフェクト制御
	void SyncBoostEffect(Transform player);


	//ダメージ
	void Damage(int damage);

	//現HP取得
	const int& GetHP(void);

	//バイクが場外に出たか取得
	const bool& GetIsOutSide(void);

	//バイクがゴールしたか設定
	void SetIsGoal(bool isGoal);

	//バイクがゴールしたか取得
	const bool& GetIsGoal(void);

	// スコア加算
	void AddScore(int score);
	// スコアゲッター
	const int GetScore() const;

	//プレイヤーIDのゲッター
	const int GetPlayerID(void) const;

	// プレイヤー同士の当たり判定
	void Flip(VECTOR dir);

	// ブーストを使ったかどうか
	const bool IsBoost(void);

private:

	Transform transformPlayer_;

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;


	// 状態管理
	STATE state_;

	//場外にいるかどうか
	bool isOutSide_;

	// プレイヤーIDを保持
	int playerID_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// プレイヤー同士の横幅の調整
	float localPosX_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;
	float currentTime = 0.0f;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプスピード
	float jumpSpeed_;

	// ジャンプ判定
	bool isJump_;

	// ジャンプの入力受付時間
	float stepJump_;

	//ジャンプしてから秒たったか
	float stepJumpSecond_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 丸影
	int imgShadow_;

	// 体力
	int hp_;

	// スコア
	std::shared_ptr<Score>score_;

	// 攻撃が当たったか
	bool isAttack_;

	//プレイヤー同士の当たり判定用
	float flipSpeed_;
	VECTOR flipDir_;

	//アニメーション
	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);
	void ChangeStateCrash(void);
	void ChangeStateDead(void);


	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);
	void UpdateFliped(void);
	void UpdateCrash(void);
	void UpdateDead(void);

	// 描画系
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessBoost(void); //ブースト
	void ProcessDebug(void);//デバッグ用

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

	//エフェクト系
	// ヒットエフェクト
	int effectSonicResId_;
	int effectSonicPlayId_;

	// ブースト時の加算速度
	float speedBoost_;

	// ブースト使用間隔
	float deleyBoost_;

	// ブーストを使用したか
	bool isBoost_;

	//ゴールしたか
	bool isGoal_;

	//ブーストエフェクト位置
	VECTOR boostEffectPos;

	//ブーストエフェクト
	int effectBoostResId_;
	int effectBoostPlayId_;

	// エフェクト初期化
	void InitEffect(void);
	// ソニックブームエフェクトの位置
	void SonicBoomEffect(void);

};

