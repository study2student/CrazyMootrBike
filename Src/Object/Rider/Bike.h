#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Player;
class Weapon;

class Bike : public ActorBase
{
public:
	// スピード
	static constexpr float SPEED_MOVE = 100.0f;
	static constexpr float SPEED_RUN = 130.0f;

	//横移動のスピード
	static constexpr float SPEED_MOVE_X = 10.0f;

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

	// 攻撃種別
	enum class ATTACK_TYPE
	{
		NONE,
		NORMAL,
		SPECIAL,
		LONG,
	};

	// コンストラクタ
	Bike(float localpos);

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

	void SetSpeed(float speed, float rotRad, float posY);

	//ジャンプ台用のジャンプ
	void Jump(void);

	//ダメージ
	void Damage(int damage);

private:

	Transform transformPlayer_;

	// アニメーション
	std::unique_ptr<AnimationController> animationController_;


	Weapon* weapon_;

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
	
	// プレイヤー同士の横幅の調整
	float localPosX_;

	// 回転
	Quaternion playerRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

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

	// 攻撃が当たったか
	bool isAttack_;

	//アニメーション
	void InitAnimation(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdatePlay(void);

	// 描画系
	void DrawUI(void);
	void DrawShadow(void);
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessJump(void);//ジャンプ
	void ProcessAttack(void);//攻撃
	void ProcessDebug(void);//デバッグ用

	// 攻撃種別
	void NormalAttack(void);//通常攻撃
	void LongAttack(void);//遠距離攻撃
	void SpecialAttack(void);//必殺技

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

	//エフェクト系
	// ヒットエフェクト
	int effectSonicResId_;
	int effectSonicPlayId_;

	// エフェクト初期化
	void InitEffect(void);
	// Hitエフェクトの位置
	void SonicBoomEffect(void);
};

