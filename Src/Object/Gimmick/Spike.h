#pragma once
#include <map>
#include "GimmickBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Spike : public GimmickBase
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

	void Update(void) override;
	void Draw(void) override;

	//バイクの情報設定
	void SetTransform(Transform transformTarget);

	//何かに当たったか設定
	void SetIsCol(bool isCol);

	//当たったか取得
	const bool& GetIsCol(void) const;

	//待機状態かどうか
	const bool IsIdle(void);

private:

	//目標(プレイヤー)の位置回転情報
	Transform transformTarget_;

	// 状態管理
	STATE state_;

	//ターゲットに向けた向き
	VECTOR targetDir_;

	//ターゲットに向けた向き保存用
	VECTOR targetDirSave_;

	// 回転
	Quaternion rotX_;

	//あたったか
	bool isCol_;

	//場所表示時間
	float stepPlaceDrawTime_;

	//とげが出現してから消えるまでの時間
	float stepToDeleteTime_;

	//とげが消えてからどのくらい時間経過したか
	float stepSpikeDestroy_;

	// 発生エフェクト
	int makeEffectResId_;
	int makeEffectPlayId_;

	//爆発エフェクト
	int bombEffectResId_;
	int bombEffectPlayId_;

	// エフェクト初期化
	void InitEffect(void);

	// エフェクト
	void PlayMakeEffect(void);
	void PlayBombEffect(void);

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

	// 初期化(外部読み込み)
	void InitLoad(void) override;

	// 初期化(事後処理)
	void InitPost(void) override;
};
