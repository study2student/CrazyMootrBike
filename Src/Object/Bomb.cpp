#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Object/Planet.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Bomb.h"

Bomb::Bomb()
{
	state_ = STATE::NONE;

	stepPlaceDrawTime_ = 0.0f;
	stepReserveTime_ = 0.0f;

	stepBombBlast_ = 0.0f;
	isCol_ = false;
}

Bomb::~Bomb(void)
{
	StopEffekseer3DEffect(bombEffectPlayId_);
}

void Bomb::Init(void)
{
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));
	float scale = 0.15f;
	transform_.scl = { scale, scale, scale };
	//ヘリに乗せとく
	transform_.pos = heliTrans_.pos;
	//transform_.pos = { 1670.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//爆発エフェクト
	InitEffect();

	// カプセルコライダ
	//capsule_ = new Capsule(transform_);
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 30.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 10.0f, 0.0f });
	capsule_->SetRadius(90.0f);

	ChangeState(STATE::IDLE);
}

void Bomb::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case Bomb::STATE::NONE:
		UpdateNone();
		break;
	case Bomb::STATE::IDLE:
		UpdateIdle();
		break;
	case Bomb::STATE::RESERVE:
		UpdateReserve();
		break;
	case Bomb::STATE::BLAST:
		UpdateBlast();
		break;
	}
}

void Bomb::Draw(void)
{

	// 更新ステップ
	switch (state_)
	{
	case Bomb::STATE::NONE:
		//MV1DrawModel(transform_.modelId);
		break;
	case Bomb::STATE::IDLE:
		//MV1DrawModel(transform_.modelId);
		DrawBombPlace();
		break;
	case Bomb::STATE::RESERVE:
		MV1DrawModel(transform_.modelId);
		break;
	case Bomb::STATE::BLAST:
		break;
	}

}

void Bomb::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Bomb::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Bomb::GetCapsule(void) const
{
	return capsule_;
}

void Bomb::SetHeliTrans(const Transform& heliTrans)
{
	heliTrans_ = heliTrans;
}

void Bomb::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

bool Bomb::GetIsCol(void)
{
	return isCol_;
}

const Bomb::STATE& Bomb::GetState(void)
{
	return state_;
}

void Bomb::InitEffect(void)
{
	bombEffectResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;

	bombPlaceEffectResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_PLACE_EFFECT).handleId_;
}

void Bomb::BombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//何かに当たった時は前で爆発
	VECTOR localPos = {};
	if (isCol_)
	{
		localPos = { 0.0f,0.0f,2200.0f };
	}
	else
	{
		localPos = { 0.0f,0.0f,0.0f };
	}
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + localPos.z);
	float scl = 10.0f;
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, scl, scl, scl);
}

<<<<<<< HEAD
void Bomb::SyncBombPlaceEffect(void)
{
	float scale = 10.0f;
	SetScalePlayingEffekseer3DEffect(bombPlaceEffectPlayId_, scale, scale, scale);
	SetPosPlayingEffekseer3DEffect(bombPlaceEffectPlayId_, bombTargetPos_.x, 0.0f, bombTargetPos_.z);
=======
void Bomb::BombPlaceEffect(void)
{
	float scale = 100.0f;
	SetScalePlayingEffekseer3DEffect(bombPlaceEffectPlayId_, scale, scale, scale);
	SetPosPlayingEffekseer3DEffect(bombPlaceEffectPlayId_, bombTargetPos_.x, bombTargetPos_.y, bombTargetPos_.z);
>>>>>>> origin/multiPlay
	SetRotationPlayingEffekseer3DEffect(bombPlaceEffectPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void Bomb::ChangeState(STATE state)
{
	//状態変更
	state_ = state;

	// 更新ステップ
	switch (state_)
	{
	case Bomb::STATE::NONE:
		ChangeStateNone();
		break;
	case Bomb::STATE::IDLE:
		ChangeStateIdle();
		break;
	case Bomb::STATE::RESERVE:
		ChangeStateReserve();
		break;
	case Bomb::STATE::BLAST:
		ChangeStateBlast();
		break;
	}
}

void Bomb::ChangeStateNone(void)
{
}

void Bomb::ChangeStateIdle(void)
{
	isCol_ = false;

<<<<<<< HEAD
	////爆発場所エフェクト
	//bombPlaceEffectPlayId_ = PlayEffekseer3DEffect(bombPlaceEffectResId_);
	//SyncBombPlaceEffect();
=======
	//爆発場所エフェクト
	BombPlaceEffect();
>>>>>>> origin/multiPlay
}

void Bomb::ChangeStateReserve(void)
{
	StopEffekseer3DEffect(bombPlaceEffectPlayId_);
}

void Bomb::ChangeStateBlast(void)
{
	//爆発エフェクト
	BombEffect();
}

void Bomb::UpdateNone(void)
{
}

void Bomb::UpdateIdle(void)
{
	//ヘリに乗せとく
	transform_.pos = heliTrans_.pos;

	//5秒後に設置
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//爆弾爆発準備状態に移行
		ChangeState(STATE::RESERVE);
		stepPlaceDrawTime_ = 0.0f;
	}

	////爆発場所エフェクト
	//SyncBombPlaceEffect();
}

void Bomb::UpdateReserve(void)
{
	// 弾を移動させる
	VECTOR movePow;
	VECTOR targetDir = VNorm(VSub(bombTargetPos_, transform_.pos));
	movePow = VScale(targetDir, SPEED);

	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow);

	// 重力による移動量
	CalcGravityPow();
	// 衝突判定
	Collision();
	transform_.Update();

	//3秒後に爆発
	stepReserveTime_ += SceneManager::GetInstance().GetDeltaTime();

	//何かに当たっても爆発する
	if (stepReserveTime_ >= RESERVE_MAX_TIME || isCol_)
	{
		//爆弾爆発状態に移行
		ChangeState(STATE::BLAST);
		stepReserveTime_ = 0.0f;
	}

}

void Bomb::UpdateBlast(void)
{
	// 重力による移動量
	CalcGravityPow();
	// 衝突判定
	Collision();
	transform_.Update();


	//5秒後に復活
	stepBombBlast_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepBombBlast_ >= BOMB_REMAKE_MAX_TIME)
	{
		//爆弾爆発発射前状態に移行
		ChangeState(STATE::IDLE);
		stepBombBlast_ = 0.0f;
	}


}

void Bomb::DrawBombPlace(void)
{
	VECTOR localPos1 = { 100.0f,-600.0f,-390.0f };
	VECTOR localPos2 = { 100.0f,-590.0f,-390.0f };
	VECTOR pos1 = VAdd(heliTrans_.pos, localPos1);
	VECTOR pos2 = VAdd(heliTrans_.pos, localPos2);
	DrawCapsule3D(pos1, pos2, 100.0f, 1, 0xff0000, 0xffffff, 0);

	//爆発目標の設定
	bombTargetPos_ = pos1;
	transform_.pos = bombTargetPos_;
	transform_.Update();
}

void Bomb::Collision(void)
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

void Bomb::CollisionGravity(void)
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

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		if (hit.HitFlag > 0)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));


		}

	}
}

void Bomb::CollisionCapsule(void)
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

void Bomb::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);


}
