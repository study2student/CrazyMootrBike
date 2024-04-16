#pragma once
#include <vector>
#include "../Common/Transform.h"

class Turret
{
public:
	// 衝突判定：球体半径
	static constexpr float COLLISION_RADIUS = 200.0f;

	// タレットの大きさ
	static constexpr float SCALE = 8.0f;

	// 状態
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY,
		END
	};
	// コンストラクタ
	Turret(const Transform& transformParent,
		VECTOR localPos, VECTOR localRot);
	// デストラクタ
	~Turret(void);
	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 砲台回転動作制限角(deg)
	static constexpr float ANGLE_Y_MIN_STAND = -30.0f * DX_PI_F / 180.0f;
	static constexpr float ANGLE_Y_MAX_STAND = 30.0f * DX_PI_F / 180.0f;
	// 砲台の回転量(deg)
	static constexpr float ROT_POW_STAND = 0.2f;
	// 砲身回転動作制限角(deg)
	static constexpr float ANGLE_X_MIN_GUN = -10.0f * DX_PI_F / 180.0f;
	static constexpr float ANGLE_X_MAX_GUN = 20.0f * DX_PI_F / 180.0f;

	// 砲身の回転量(deg)
	static constexpr float ROT_POW_GUN = 0.2f;

	const Transform& GetTransform(void) const;

	STATE& GetState(void);

	bool IsAlive(void) const;
private:
	// モデル制御の基本情報
	const Transform& transformParent_;
	// モデル制御の基本情報(砲台)
	Transform transformStand_;
	// モデル制御の基本情報(砲身)
	Transform transformBarrel_;
	// 状態
	STATE state_;

	// 親(戦艦)からの相対座標
	VECTOR localPos_;
	// 親(戦艦)からの相対角度
	VECTOR localRot_;

	// 砲台の回転量(deg)
	float rotPowStand_;
	// 砲身の回転量(deg)
	float rotPowBarrel_;
	// 砲台の回転動作蓄積用
	VECTOR localRotAddStand_;
	VECTOR localRotAddBarrel_;

	float deleyShot_;

	// 状態遷移
	void ChangeState(STATE state);

	// 弾の更新
	void UpdateShot(void);

	void UpdateAttack(void);
	void UpdateDestroy(void);

	// 弾の描画
	void DrawShot(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	// 親(戦艦)との回転と位置の同期
	void SyncParent(Transform& transform, VECTOR addAxis);

	bool isAlive_;
};


