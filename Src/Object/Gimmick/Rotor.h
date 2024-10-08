#pragma once
#include <map>
#include <DxLib.h>
#include "GimmickBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Rotor : public GimmickBase
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

	void Update(void) override;
	void Draw(void) override;

	//親ヘリコプターの情報設定
	void SetTransform(Transform& transformParent);

private:

	Transform transformParent_;

	// 状態管理
	STATE state_;

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

	// 移動量の計算
	void CalcGravityPow(void);

	// 初期化(外部読み込み)
	void InitLoad(void) override;

	// 初期化(事後処理)
	void InitPost(void) override;

};







