#pragma once
#include <map>
#include <memory>
#include <vector>
#include "GimmickBase.h"
class AnimationController;
class Collider;
class Capsule;
class Rotor;
class Bomb;
class GameScene;

class Helicopter : public GimmickBase
{

public:

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
	Helicopter(GameScene* gameScene);

	// デストラクタ
	~Helicopter(void);

	void Update(void) override;
	void Draw(void) override;

	//バイク情報の保存(バイクの位置回転情報、場外にいるかどうか)
	void SetBikeTrans(const Transform& bikeTrans);
	void SetBikeIsOutside(const bool& isOutside);

	//爆弾取得
	Bomb* GetBomb(void);

	// 状態遷移
	void ChangeState(STATE state);

private:

	//羽
	Rotor* rotor_;

	//爆弾
	Bomb* bomb_;

	//バイク情報
	Transform targetTrans_;

	//バイクがステージ外にいるかどうか
	bool isTargetOutside_;

	//ゲームシーン
	GameScene* gameScene_;

	// 状態管理
	STATE state_;

	// 攻撃状態管理
	ATTACK_TYPE attackState_;

	// 移動スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 体力
	int hp_;

	// 攻撃が当たったか
	bool isAttack_;

	// 状態遷移
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
	void DrawDebug(void);

	// 操作
	void ProcessMove(void);//移動
	void ProcessAttack(void);//攻撃
	void ProcessDebug(void);//デバッグ用

	// 攻撃種別
	void NormalAttack(void);//通常攻撃
	void LongAttack(void);//遠距離攻撃

	// 回転
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);
	void Rotate(void);

	// 移動量の計算
	void CalcGravityPow(void);

	//バイクとの距離による処理
	void BikeDisFunc(void);

	// 初期化(外部読み込み)
	void InitLoad(void) override;

	// 初期化(事後処理)
	void InitPost(void) override;
};





