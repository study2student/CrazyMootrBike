#include <string>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "Rider/Bike.h"
#include "TyreThrow.h"

TyreThrow::TyreThrow()
{
	state_ = STATE::IDLE;


	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

TyreThrow::~TyreThrow(void)
{
}

void TyreThrow::Init(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TYRE));
	float scale = 2.05f;
	transform_.scl = { scale , scale, scale };
	transform_.pos = { 1500.0f,100.0f, 3000.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();


	// カプセルコライダ
	//capsule_ = new Capsule(transform_);
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 30.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 10.0f, 0.0f });
	capsule_->SetRadius(80.0f);

	// 初期状態
	ChangeState(STATE::IDLE);
}

void TyreThrow::Update(void)
{
	switch (state_)
	{
	case TyreThrow::STATE::IDLE:
		UpdateIdle();
		break;
	case TyreThrow::STATE::THROW_MOVE:
		UpdateThrowMove();
		break;
	case TyreThrow::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	// モデル制御更新
	transform_.Update();
}

void TyreThrow::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();
}

void TyreThrow::SetTransform(Transform transformTarget)
{
	transformTarget_ = transformTarget;
}

void TyreThrow::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void TyreThrow::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> TyreThrow::GetCapsule(void) const
{
	return capsule_;
}

void TyreThrow::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

bool TyreThrow::GetIsCol(void)
{
	return isCol_;
}

void TyreThrow::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case TyreThrow::STATE::IDLE:
		ChangeStateIdle();
		break;
	case TyreThrow::STATE::THROW_MOVE:
		ChangeStateThrow();
		break;
	case TyreThrow::STATE::DESTROY:
		ChangeStateDestroy();
		break;
	}
}

void TyreThrow::ChangeStateIdle(void)
{
}


void TyreThrow::ChangeStateThrow(void)
{
	//ランダムで向きを決める
	//道のランダムな場所に生成(3パターン)
	int randDir = GetRand(static_cast<int>(TyreThrow::DIR::MAX) - 1);
	TyreThrow::DIR dir = static_cast<TyreThrow::DIR>(randDir);
	VECTOR targetPos;

	Vector2 randPos;
	switch (dir)
	{
	case TyreThrow::DIR::SIDE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_SIDE);
		break;
	case TyreThrow::DIR::SLIGHTLY_OBLIPUE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE);
		break;
	case TyreThrow::DIR::LARGE_OBLIPUE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_LARGE_OBLIPUE);
		break;
	}

	targetDir_ = VNorm(VSub(targetPos, transform_.pos));
}


void TyreThrow::ChangeStateDestroy(void)
{
}

void TyreThrow::UpdateIdle(void)
{
	//プレイヤーの少し先で待機
	transform_.pos = transformTarget_.pos;

	//5秒後に投げる
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//爆弾爆発準備状態に移行
		ChangeState(STATE::THROW_MOVE);
		stepPlaceDrawTime_ = 0.0f;
	}
}

void TyreThrow::UpdateThrowMove(void)
{
	//// デバッグ用
	//ProcessDebug();

	// 移動方向に応じた回転
	Rotate();

	// タイヤを移動させる
	VECTOR movePow;
	movePow = VScale(targetDir_, SPEED_MOVE);

	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow);

	// 重力による移動量
	CalcGravityPow();
	// 衝突判定
	Collision();
	transform_.Update();

	//3秒後に爆発
	stepToDeleteTime_ += SceneManager::GetInstance().GetDeltaTime();

	//何かに当たっても爆発する
	if (stepToDeleteTime_ >= TO_DELETE_MAX_TIME || isCol_)
	{
		//削除状態に移行
		ChangeState(STATE::DESTROY);
		stepToDeleteTime_ = 0.0f;
	}
}

void TyreThrow::UpdateDestroy(void)
{
}

void TyreThrow::DrawDebug(void)
{
}

void TyreThrow::ProcessDebug(void)
{
}

void TyreThrow::Rotate(void)
{
	auto& ins = InputManager::GetInstance();

	//リアタイヤの回転
	// デグリーからラジアン(変換)
	float rad = AsoUtility::Deg2RadF(SPEED_ROT);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_X);

	//クォータニオン(回転)の合成
	rotX_ = rotX_.Mult(rotPow);

	// 回転させる
	transform_.quaRot = rotX_;
	transform_.Update();
}

void TyreThrow::Collision(void)
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

void TyreThrow::CollisionGravity(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		if (hit.HitFlag > 0)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));


		}

	}
}

void TyreThrow::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto c : colliders_)
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

void TyreThrow::CalcGravityPow(void)
{

}
