#pragma once
#include <map>
#include "../Common/Transform.h"
#include "../ActorBase.h"
#include "GimmickBase.h"
class Player;
class Collider;
class Capsule;
class Helicopter;

class Bomb : public GimmickBase
{

public:

	//ダメージ
	static constexpr int BOMB_DAMAGE = 50;

	// 状態
	enum class STATE
	{
		NONE,
		IDLE,
		RESERVE,
		BLAST
	};

	// コンストラクタ
	Bomb();

	// デストラクタ
	~Bomb(void);

	void Update(void) override;
	void Draw(void) override;

	//ヘリの情報設定用
	void SetHeliTrans(const Transform& heliTrans);

	//爆弾が何かに当たったか設定
	void SetIsCol(bool isCol);

	//爆弾が当たったが取得
	const bool& GetIsCol(void) const;

	//状態取得
	const STATE& GetState(void) const;

private:

	//ヘリ
	Transform heliTrans_;

	//爆弾が爆発してからどのくらい時間経過したか
	float stepBombBlast_;

	//爆弾場所表示時間
	float stepPlaceDrawTime_;

	//爆弾爆発準備時間
	float stepReserveTime_;

	// 状態管理
	STATE state_;

	//爆発エフェクト
	int bombEffectResId_;
	int bombEffectPlayId_;

	//爆発目標
	VECTOR bombTargetPos_;

	//爆弾が何かに当たったか
	bool isCol_;

	//エフェクト読み込み
	void InitEffectLoad(void);

	//爆発エフェクト再生
	void PlayBombEffect(void);

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateIdle(void);
	void ChangeStateReserve(void);
	void ChangeStateBlast(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateIdle(void);
	void UpdateReserve(void);
	void UpdateBlast(void);

	//爆弾の位置
	void DrawBombPlace(void);

	// 移動量の計算
	void CalcGravityPow(void);

	// 初期化(外部読み込み)
	void InitLoad(void) override;

	// 初期化(事後処理)
	void InitPost(void) override;

};

