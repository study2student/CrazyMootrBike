#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../Stage/Planet.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "GimmickBase.h"

GimmickBase::GimmickBase()
{
	//移動系
	movePow_ = {};
	movedPos_ = {};
}

GimmickBase::~GimmickBase()
{
}

void GimmickBase::Init(void)
{
	// 初期化(外部読み込み)
	InitLoad();

	// 初期化(モデル設定)
	InitTransform();

	// 初期化(衝突設定)
	InitCollider();

	// 初期化(事後処理)
	InitPost();
}

void GimmickBase::Update(void)
{
}

void GimmickBase::Draw(void)
{
}

void GimmickBase::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void GimmickBase::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& GimmickBase::GetCapsule(void) const
{
	return *capsule_;
}

void GimmickBase::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void GimmickBase::CollisionGravity(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = MyUtility::DIR_U;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	auto gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	auto gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto& c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c.lock()->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		if (hit.HitFlag > 0)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));


		}

	}
}

void GimmickBase::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto& c : colliders_)
	{

		auto hits = MV1CollCheck_Capsule(
			c.lock()->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;

			}

		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);

	}
}

void GimmickBase::InitLoad(void)
{
}

void GimmickBase::InitTransform(void)
{
	transform_.scl = initScl_;
	transform_.quaRot = Quaternion::Euler(initRotEuler_);
	transform_.quaRotLocal = Quaternion::Euler(initLocalRotEuler_);
	transform_.pos = initPos_;
	transform_.Update();
}

void GimmickBase::InitCollider(void)
{
	//衝突用のカプセル
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop(capsulePosTop_);
	capsule_->SetLocalPosDown(capsulePosDown_);
	capsule_->SetRadius(capsuleRadius_);
}

void GimmickBase::InitPost(void)
{
}
