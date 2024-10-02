#include <string>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Stage/Planet.h"
#include "../Rider/Bike.h"
#include "Spike.h"

#pragma region 定数宣言

//回転速度
const float SPEED_ROT = 20.0f;

//速度
const float SPEED_MOVE = 60.0f;

//場所表示時間
const float PLACE_DRAW_MAX_TIME = 3.0f;

//消えるまでの時間
const float TO_DELETE_MAX_TIME = 4.5f;

//とげ復活時間
const float SPIKE_REMAKE_MAX_TIME = 6.0f;

//右から投げる位置(少し横に)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_SIDE = { -900.0f,100.0f,13500.0f };

//右から投げる位置(少し斜めに)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { -750.0f,100.0f,11000.0f };

//右から投げる位置(大きく斜めに)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { -600.0f,100.0f,7000.0f };

//左から投げる位置(少し横に)
const VECTOR LEFT_THROW_LOCAL_POS_TO_SIDE = { 600.0f,100.0f,7000.0f };

//左から投げる位置(少し斜めに)
const VECTOR LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { 770.0f,100.0f,12800.0f };

//左から投げる位置(大きく斜めに)
const VECTOR LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { 900.0f,100.0f,13500.0f };

//右の場合の出現するX座標
const float MAKE_RIGHT_POS_X = 2499.0f;

//左の場合の出現するX座標
const float MAKE_LEFT_POS_X = 909.0f;

//ローカル待機座標
const VECTOR SPIKE_IDLE_ROCAL_POS = { 0.0f,100.0f,15000.0f };

//とげの大きさ
const float SCL = 120.0f;

//初期座標
const VECTOR INIT_POS = { 1500.0f, 1000.0f, 3000.0f };

//カプセルローカル座標上
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 30.0f, 0.0f };

//カプセルローカル座標下
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 10.0f, 0.0f };

//カプセル半径
const float CAPSULE_RADIUS = 130.0f;

//発生エフェクト大きさ
const float MAKE_EFFECT_SCL = 130.0f;

//発生エフェクトローカル座標
const VECTOR MKAE_EFFECT_LOCAL_POS = { 0.0f,300.0f,0.0f };

//爆発エフェクト大きさ
const float BOMB_EFFECT_SCL = 30.0f;

#pragma endregion


Spike::Spike()
	:
	transformTarget_(Transform()),
	state_(STATE::IDLE),
	targetDir_({}),
	targetDirSave_({}),
	movePow_({}),
	movedPos_({}),
	rotX_(Quaternion()),
	isCol_(false),
	stepPlaceDrawTime_(0.0f),
	stepToDeleteTime_(0.0f),
	stepSpikeDestroy_(0.0f),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	makeEffectResId_(-1),
	makeEffectPlayId_(-1),
	bombEffectResId_(-1),
	bombEffectPlayId_(-1)
{
}

Spike::~Spike(void)
{
}

void Spike::Init(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE_BALL));
	transform_.scl = { SCL , SCL, SCL };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

	//エフェクト
	InitEffect();

	// 初期状態
	ChangeState(STATE::DESTROY);
}

void Spike::Update(void)
{
	switch (state_)
	{
	case Spike::STATE::IDLE:
		UpdateIdle();
		break;
	case Spike::STATE::THROW_MOVE:
		UpdateThrowMove();
		break;
	case Spike::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	// モデル制御更新
	transform_.Update();
}

void Spike::Draw(void)
{

	//動いている時以外は描画しない
	if (!(state_ == STATE::THROW_MOVE))
	{
		return;
	}

	// モデルの描画
	MV1DrawModel(transform_.modelId);
}

void Spike::SetTransform(Transform transformTarget)
{
	transformTarget_ = transformTarget;
}

void Spike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Spike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Spike::GetCapsule(void) const
{
	return capsule_;
}

void Spike::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

const bool& Spike::GetIsCol(void) const
{
	return isCol_;
}

const bool Spike::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void Spike::InitEffect(void)
{
	makeEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::THROW_MAKE_EFFECT).handleId_;

	bombEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void Spike::MakeEffect(void)
{
	makeEffectPlayId_ = PlayEffekseer3DEffect(makeEffectResId_);
	SetScalePlayingEffekseer3DEffect(makeEffectPlayId_, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL);
	VECTOR localPos = MKAE_EFFECT_LOCAL_POS;
	SetPosPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.pos.x, transform_.pos.y + localPos.y, transform_.pos.z);
	SetRotationPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void Spike::BombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//何かに当たった時は爆発
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, BOMB_EFFECT_SCL, BOMB_EFFECT_SCL, BOMB_EFFECT_SCL);
}

void Spike::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case Spike::STATE::IDLE:
		ChangeStateIdle();
		break;
	case Spike::STATE::THROW_MOVE:
		ChangeStateThrow();
		break;
	case Spike::STATE::DESTROY:
		ChangeStateDestroy();
		break;
	}
}

void Spike::ChangeStateIdle(void)
{
	isCol_ = false;
}


void Spike::ChangeStateThrow(void)
{

	transform_.pos = VAdd(transformTarget_.pos, SPIKE_IDLE_ROCAL_POS);

	//どこから投げるかランダムで決める
	int randDir = GetRand(static_cast<int>(Spike::DIR::MAX) - 1);
	Spike::DIR dir = static_cast<Spike::DIR>(randDir);

	switch (dir)
	{
	case Spike::DIR::LEFT:
		transform_.pos.x = MAKE_LEFT_POS_X;
		break;
	case Spike::DIR::RIGHT:
		transform_.pos.x = MAKE_RIGHT_POS_X;
		break;
	case Spike::DIR::MAX:
		break;
	}

	//発生エフェクト
	MakeEffect();

	//ランダムな3パターンの動作
	int randAngle = GetRand(static_cast<int>(Spike::ANGLE::MAX) - 1);
	Spike::ANGLE angle = static_cast<Spike::ANGLE>(randAngle);
	VECTOR targetPos;
	VECTOR throwLocalPos;

	switch (angle)
	{
	case Spike::ANGLE::SIDE:
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
	case Spike::ANGLE::SLIGHTLY_OBLIPUE:
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
	case Spike::ANGLE::LARGE_OBLIPUE:
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


void Spike::ChangeStateDestroy(void)
{
	//爆発エフェクト
	BombEffect();
}

void Spike::UpdateIdle(void)
{
	//プレイヤーの少し先で待機
	transform_.pos = VAdd(transformTarget_.pos, SPIKE_IDLE_ROCAL_POS);

	//3秒後に投げる
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//爆弾爆発準備状態に移行
		ChangeState(STATE::THROW_MOVE);
		stepPlaceDrawTime_ = 0.0f;
	}
}

void Spike::UpdateThrowMove(void)
{

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

void Spike::UpdateDestroy(void)
{
	//8秒後に復活
	stepSpikeDestroy_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepSpikeDestroy_ >= SPIKE_REMAKE_MAX_TIME)
	{
		//爆弾爆発発射前状態に移行
		ChangeState(STATE::IDLE);
		stepSpikeDestroy_ = 0.0f;
	}
}

void Spike::Rotate(void)
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

void Spike::Collision(void)
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

void Spike::CollisionGravity(void)
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

void Spike::CollisionCapsule(void)
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
