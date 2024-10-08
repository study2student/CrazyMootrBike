#pragma once
#include <map>
#include <vector>
#include "../Common/Transform.h"
#include "../ActorBase.h"
class Capsule;
class Collider;

class GimmickBase : public ActorBase
{

public:

	GimmickBase();

	virtual ~GimmickBase();

	void Init(void) final;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::weak_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const Capsule& GetCapsule(void) const;

protected:

	//トランスフォーム
	VECTOR initScl_;
	VECTOR initRotEuler_;
	VECTOR initLocalRotEuler_;
	VECTOR initPos_;

	//カプセルの上下座標
	VECTOR capsulePosTop_;
	VECTOR capsulePosDown_;
	//カプセル半径
	float capsuleRadius_;

	// 衝突判定に用いられるコライダ
	std::vector<std::weak_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion rotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 初期化(外部読み込み)
	virtual void InitLoad(void) = 0;

	// 初期化(モデル設定)
	virtual void InitTransform(void);

	// 初期化(衝突設定)
	virtual void InitCollider(void);

	// 初期化(事後処理)
	virtual void InitPost(void) = 0;

};

