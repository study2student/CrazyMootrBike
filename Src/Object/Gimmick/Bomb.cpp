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

// 初期大きさ
const VECTOR INIT_SCL = { 0.15f,0.15f,0.15f };

// 初期回転
const VECTOR INIT_ROT = { 0.0f, 0.0f, 0.0f };

// 初期ローカル回転
const VECTOR INIT_LOCAL_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

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
	bombTargetPos_({})
{
	//位置回転大きさ
	initScl_ = INIT_SCL;
	initRotEuler_ = INIT_ROT;
	initLocalRotEuler_ = INIT_LOCAL_ROT;
  	initPos_ = heliTrans_.pos;

	//カプセル
	capsulePosTop_ = CAPSULE_LOCAL_POS_TOP;
	capsulePosDown_ = CAPSULE_LOCAL_POS_DOWN;
	capsuleRadius_ = CAPSULE_RADIUS;
}

Bomb::~Bomb(void)
{
	StopEffekseer3DEffect(bombEffectPlayId_);
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

void Bomb::InitEffectLoad(void)
{
	bombEffectResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void Bomb::PlayBombEffect(void)
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
}

void Bomb::ChangeStateReserve(void)
{
}

void Bomb::ChangeStateBlast(void)
{
	//爆発エフェクト
	PlayBombEffect();
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

void Bomb::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Bomb::InitLoad(void)
{
	//モデル読み込み
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));

	//爆発エフェクト読み込み
	InitEffectLoad();
}

void Bomb::InitPost(void)
{
	//待機状態
	ChangeState(STATE::IDLE);
}
