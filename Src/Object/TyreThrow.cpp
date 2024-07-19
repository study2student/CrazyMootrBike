#include <string>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/MyUtility.h"
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
	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

TyreThrow::~TyreThrow(void)
{
}

void TyreThrow::Init(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE_BALL));
	float scale = 120.0f;
	transform_.scl = { scale , scale, scale };
	transform_.pos = { 1500.0f,1000.0f, 3000.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// カプセルコライダ
	//capsule_ = new Capsule(transform_);
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 30.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 10.0f, 0.0f });
	capsule_->SetRadius(130.0f);

	//エフェクト
	InitEffect();

	// 初期状態
	ChangeState(STATE::DESTROY);
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
	// デバッグ描画
	DrawDebug();

	//動いている時以外は描画しない
	if (!(state_ == STATE::THROW_MOVE))
	{
		return;
	}

	// モデルの描画
	MV1DrawModel(transform_.modelId);
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

bool TyreThrow::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void TyreThrow::InitEffect(void)
{
	effectMakeResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::THROW_MAKE_EFFECT).handleId_;

	bombEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void TyreThrow::MakeEffect(void)
{
	effectMakePlayId_ = PlayEffekseer3DEffect(effectMakeResId_);
	float scale = 130.0f;
	SetScalePlayingEffekseer3DEffect(effectMakePlayId_, scale, scale, scale);
	VECTOR localPos = { 0.0f,300.0f,0.0f };
	SetPosPlayingEffekseer3DEffect(effectMakePlayId_, transform_.pos.x, transform_.pos.y + localPos.y, transform_.pos.z);
	SetRotationPlayingEffekseer3DEffect(effectMakePlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void TyreThrow::BombEffect(void)
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
	isCol_ = false;
}


void TyreThrow::ChangeStateThrow(void)
{

	transform_.pos = VAdd(transformTarget_.pos, TYRE_IDLE_ROCAL_POS);

	//どこから投げるかランダムで決める
	int randDir = GetRand(static_cast<int>(TyreThrow::DIR::MAX) - 1);
	TyreThrow::DIR dir = static_cast<TyreThrow::DIR>(randDir);

	switch (dir)
	{
	case TyreThrow::DIR::LEFT:
		transform_.pos.x = MAKE_LEFT_POS_X;
		break;
	case TyreThrow::DIR::RIGHT:
		transform_.pos.x = MAKE_RIGHT_POS_X;
		break;
	case TyreThrow::DIR::MAX:
		break;
	}

	//発生エフェクト
	MakeEffect();

	//ランダムな3パターンの動作
	int randAngle = GetRand(static_cast<int>(TyreThrow::ANGLE::MAX) - 1);
	TyreThrow::ANGLE angle = static_cast<TyreThrow::ANGLE>(randAngle);
	VECTOR targetPos;
	VECTOR throwLocalPos;

	switch (angle)
	{
	case TyreThrow::ANGLE::SIDE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_SIDE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_SIDE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	case TyreThrow::ANGLE::SLIGHTLY_OBLIPUE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	case TyreThrow::ANGLE::LARGE_OBLIPUE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	}

	targetDir_ = VNorm(VSub(targetPos, transform_.pos));
	//保存
	targetDirSave_ = targetDir_;
}


void TyreThrow::ChangeStateDestroy(void)
{
	//爆発エフェクト
	BombEffect();
}

void TyreThrow::UpdateIdle(void)
{
	//プレイヤーの少し先で待機
	transform_.pos = VAdd(transformTarget_.pos, TYRE_IDLE_ROCAL_POS);

	//3秒後に投げる
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

	//壁に当たったたら反射
	if (transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX)
	{
		targetDir_ = VAdd(targetDirSave_, Stage::LEFT_NORMAL_VEC);
	}
	if (transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX)
	{
		targetDir_ = VAdd(targetDirSave_, Stage::RIGHT_NORMAL_VEC);

	}

	// 移動させる
	VECTOR movePow;
	movePow = VScale(targetDir_, SPEED_MOVE);

	// 移動処理
	transform_.pos = VAdd(transform_.pos, movePow);

	// 重力による移動量
	CalcGravityPow();
	// 衝突判定
	Collision();
	transform_.Update();

	//4.5秒後に爆発
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
	//8秒後に復活
	stepTyreDestroy_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepTyreDestroy_ >= TYRE_REMAKE_MAX_TIME)
	{
		//爆弾爆発発射前状態に移行
		ChangeState(STATE::IDLE);
		stepTyreDestroy_ = 0.0f;
	}
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
	float rad = MyUtility::Deg2RadF(SPEED_ROT);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_X);

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
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = MyUtility::DIR_U;

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
