#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../Stage/Planet.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "Bomb.h"

#pragma region 定数宣言

//爆弾場所表示時間
const float PLACE_DRAW_MAX_TIME = 5.0f;

//爆弾爆発準備時間
const float RESERVE_MAX_TIME = 2.5f;

//爆弾復活時間
const float BOMB_REMAKE_MAX_TIME = 0.8f;

//カプセルローカル座標上
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 30.0f, 0.0f };

//カプセルローカル座標下
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 10.0f, 0.0f };

//カプセル半径
const float CAPSULE_RADIUS = 100.0f;

//エフェクト大きさ
const float EFFECT_SCL = 30.0f;

// スピード
const float SPEED = 30.0f;

//大きさ
const float SCL = 0.15f;

//初期相対回転Y
const float INIT_LOCAL_ROT_Y = 180.0f;

#pragma endregion


Bomb::Bomb()
	:
	heliTrans_(Transform()),
	state_(STATE::NONE),
	stepPlaceDrawTime_(0.0f),
	stepReserveTime_(0.0f),
	stepBombBlast_(0.0f),
	isCol_(false),
	bombEffectResId_(-1),
	bombEffectPlayId_(-1),
	bombTargetPos_({}),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	movedPos_({}),
	movePow_({})
{
}

Bomb::~Bomb(void)
{
	StopEffekseer3DEffect(bombEffectPlayId_);
}

void Bomb::Init(void)
{
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));
	transform_.scl = { SCL, SCL, SCL };
	//ヘリに乗せとく
	transform_.pos = heliTrans_.pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y), 0.0f });
	transform_.Update();

	//爆発エフェクト
	InitEffect();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

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
		break;
	case Bomb::STATE::IDLE:
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

const bool& Bomb::GetIsCol(void) const
{
	return isCol_;
}

const Bomb::STATE& Bomb::GetState(void) const
{
	return state_;
}

void Bomb::InitEffect(void)
{
	bombEffectResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;

}

void Bomb::BombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//何かに当たった時は爆発
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, EFFECT_SCL, EFFECT_SCL, EFFECT_SCL);
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

	////爆発場所エフェクト
	//bombPlaceEffectPlayId_ = PlayEffekseer3DEffect(bombPlaceEffectResId_);
	//SyncBombPlaceEffect();
}

void Bomb::ChangeStateReserve(void)
{
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


	//1.5秒後に復活
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
	VECTOR localPos1 = { 0.0f,-600.0f,-390.0f };
	VECTOR localPos2 = { 0.0f,-590.0f,-390.0f };
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

void Bomb::CollisionCapsule(void)
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

void Bomb::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);


}
