#pragma once
#include <map>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Spike : public ActorBase
{
public:

	//ダメージ
	static constexpr int SPIKE_DAMAGE = 25;


	// 状態
	enum class STATE
	{
		IDLE,
		THROW_MOVE,
		DESTROY
	};

	//投げる角度
	enum class ANGLE
	{
		SIDE,				//横へ
		SLIGHTLY_OBLIPUE,	//少し斜めに
		LARGE_OBLIPUE,		//大きく斜めに
		MAX
	};

	//どこから投げるか
	enum class DIR
	{
		LEFT,
		RIGHT,
		MAX
	};


	// コンストラクタ
	Spike();

	// デストラクタ
	~Spike(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//バイクの情報設定
	void SetTransform(Transform transformTarget);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	//何かに当たったか設定
	void SetIsCol(bool isCol);

	//当たったか取得
	bool GetIsCol(void);

	//待機状態かどうか
	bool IsIdle(void);

private:

	Transform transformTarget_;

	// 状態管理
	STATE state_;

	//ターゲットに向けた向き
	VECTOR targetDir_;

	//ターゲットに向けた向き保存用
	VECTOR targetDirSave_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion rotX_;

	//あたったか
	bool isCol_;

	//場所表示時間
	float stepPlaceDrawTime_;

	//タイヤが出現してから消えるまでの時間
	float stepToDeleteTime_;

	//タイヤが消えてからどのくらい時間経過したか
	float stepSpikeDestroy_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 発生エフェクト
	int makeEffectResId_;
	int makeEffectPlayId_;

	//爆発エフェクト
	int bombEffectResId_;
	int bombEffectPlayId_;

	// エフェクト初期化
	void InitEffect(void);

	// エフェクト
	void MakeEffect(void);
	void BombEffect(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateThrow(void);
	void ChangeStateDestroy(void);

	// 更新ステップ
	void UpdateIdle(void);
	void UpdateThrowMove(void);
	void UpdateDestroy(void);

	// 回転
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);
};
