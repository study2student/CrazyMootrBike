#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "Planet.h"
#include "Goal.h"

#pragma region 定数宣言

//ゴール基本位置
const VECTOR GOAL_BASE_POS = { 1650.0f,-800.0f,200000000.0f };

//ゴールの大きさ
const float SCL = 7.0f;

//ゴールの大きさ調整用値
const float SCL_ADJUST_NUM = 1.1f;

//カプセルローカル座標上
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 190.0f, 0.0f };

//カプセルローカル座標下
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 150.0f, 0.0f };

//カプセル半径
const float CAPSULE_RADIUS = 135.0f;

#pragma endregion


Goal::Goal(void)
	:
	movePow_({}),
	movedPos_({}),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({})
{
}

Goal::~Goal(void)
{
}

void Goal::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOAL));
	transform_.scl = { SCL / SCL_ADJUST_NUM, SCL, SCL };
	//最初は見えない場所へ
	transform_.pos = { GOAL_BASE_POS.x, GOAL_BASE_POS.y ,GOAL_BASE_POS.z};
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();


	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);
}

void Goal::Update(void)
{

	// 重力による移動量
	CalcGravityPow();

	//衝突判定
	Collision();

	// モデル制御更新
	transform_.Update();
}

void Goal::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
}

void Goal::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Goal::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Goal::GetCapsule(void) const
{
	return capsule_;
}

void Goal::SetPosZ(float z)
{
	transform_.pos.z = z;
}

void Goal::DrawDebug(void)
{
	capsule_->Draw();
}

void Goal::Collision(void)
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

void Goal::CollisionGravity(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = MyUtility::DIR_U;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto& c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		if (hit.HitFlag > 0)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

		}

	}
}

void Goal::CollisionCapsule(void)
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
			c->modelId_, -1,
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

void Goal::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
}
