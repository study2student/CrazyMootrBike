#pragma once
#include <memory>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Score;

class Bike : public ActorBase
{
public:

	// 半径
	static constexpr float RADIUS = 80.0f;

	// HPの最大値
	static constexpr int MAX_HP = 100;

	// HPの最低値
	static constexpr int MIN_HP = 0;

	// バーンアウトエフェクト初期高さ
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -280.0f;

	// バーンアウトエフェクト最大高さ
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	// 待機エフェクト初期高
	static constexpr float IDLE_EFFECT_POS_Y = -510.0f;

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

	// パッドのボタン振り分け
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
	~Bike(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const { return capsule_; }

	// エフェクト制御
	void SyncBoostEffect(Transform player);

	// ダメージ
	void Damage(int damage);

	// 現HP取得
	const int& GetHP(void)const& { return hp_; }

	// バイクが場外に出たか取得
	const bool& GetIsOutSide(void)const& { return isOutSide_; }

	// バイクがゴールしたか設定
	void SetIsGoal(bool isGoal);

	// バイクがゴールしたか取得
	const bool& GetIsGoal(void) const& { return isGoal_; }

	// スコア加算
	void AddScore(int score);

	// スコアゲッター
	const int GetScore(void) const;

	// プレイヤーIDのゲッター
	const int& GetPlayerID(void) const& { return playerID_; }

	// プレイヤー同士の当たり判定
	void Flip(VECTOR dir);

	// ブーストを使ったかどうか
	const bool& GetIsBoost(void)const& { return isBoost_; }

private:

	Transform transformPlayer_;

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;

	// 状態管理
	STATE state_;

	// 場外にいるかどうか
	bool isOutSide_;

	// プレイヤーIDを保持
	int playerID_;

	// 移動スピード
	float moveSpeed_;

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

	//
	float currentTime_;

	// ジャンプ量
	VECTOR jumpPow_;

	// ジャンプの入力受付時間
	float stepJump_;

	//ジャンプしてから秒たったか
	float stepJumpSecond_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	// 衝突判定用のカプセル
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 体力
	int hp_;

	// スコア
	std::shared_ptr<Score>score_;

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
	void ProcessBoost(void);//ブースト
	bool IsBoostPush(void);//ブーストボタンを押したかどうか

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

	// ブースト時の加算速度
	float speedBoost_;

	// ブースト使用間隔
	float deleyBoost_;

	// ブーストを使用したか
	bool isBoost_;

	//ゴールしたか
	bool isGoal_;

	//ブーストエフェクト
	int effectBoostResId_;
	int effectBoostPlayId_;

	// エフェクト初期化
	void InitEffect(void);

};

