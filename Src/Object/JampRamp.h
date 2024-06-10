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
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;
private:

	// 移動後の座標
	VECTOR movedPos_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	void CollisionCapsule(void);

	void DrawDebug(void);
};

