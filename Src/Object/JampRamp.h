#pragma once
#include "Common/Transform.h"
#include "../Object/ActorBase.h"
#include <vector>
class Capsule;

class JampRamp : public ActorBase
{
public:
	// コンストラクタ
	JampRamp();

	// デストラクタ
	~JampRamp(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(Collider* collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule* GetCapsule(void) const;
private:

	// シングルトン参照
	ResourceManager& resMng_;

	// 移動後の座標
	VECTOR movedPos_;

	// 衝突判定に用いられるコライダ
	std::vector<Collider*> colliders_;
	Capsule* capsule_;

	void CollisionCapsule(void);

	void DrawDebug(void);
};

