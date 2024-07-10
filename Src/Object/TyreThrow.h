#pragma once
#include <map>
#include <DxLib.h>
#include "ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class TyreThrow : public ActorBase
{
public:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//回転速度
	static constexpr float SPEED_ROT = 20.0f;

	//速度
	static constexpr float SPEED_MOVE = 60.0f;

	//場所表示時間
	static constexpr float PLACE_DRAW_MAX_TIME = 3.0f;

	//消えるまでの時間
	static constexpr float TO_DELETE_MAX_TIME = 4.5f;

	//タイヤ復活時間
	static constexpr float TYRE_REMAKE_MAX_TIME = 8.0f;

	//タイヤの投げる位置(少し横に)
	static constexpr VECTOR THROW_LOCAL_POS_TO_SIDE = { -900.0f,100.0f,13500.0f };

	//タイヤの投げる位置(少し斜めに)
	static constexpr VECTOR THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { -750.0f,100.0f,11000.0f };

	//タイヤの投げる位置(大きく斜めに)
	static constexpr VECTOR THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { -600.0f,100.0f,7000.0f };

	//タイヤが出現するX座標
	static constexpr float TYRE_MAKE_POS_X = 2499.0f;

	//タイヤローカル待機座標
	static constexpr VECTOR TYRE_IDLE_ROCAL_POS = { 500.0f,100.0f,15000.0f };

	//ダメージ
	static constexpr int THROW_DAMAGE = 20;


	// 状態
	enum class STATE
	{
		IDLE,
		THROW_MOVE,
		DESTROY
	};

	//投げる向き
	enum class DIR
	{
		SIDE,				//横へ
		SLIGHTLY_OBLIPUE,	//少し斜めに
		LARGE_OBLIPUE,		//大きく斜めに
		MAX
	};


	// コンストラクタ
	TyreThrow();

	// デストラクタ
	~TyreThrow(void);

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

	// スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

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
	float stepTyreDestroy_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// 発生エフェクト
	int effectMakeResId_;
	int effectMakePlayId_;

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

	// 描画系
	void DrawDebug(void);

	// 操作
	void ProcessDebug(void);//デバッグ用

	// 回転
	void Rotate(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

};
