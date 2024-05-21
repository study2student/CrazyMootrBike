#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "Bomb.h"

Bomb::Bomb()
{
	state_ = STATE::NONE;

	stepPlaceDrawTime_ = 0.0f;
	stepReserveTime_ = 0.0f;
}

Bomb::~Bomb(void)
{
}

void Bomb::Init(void)
{
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));
	float scale = 0.15f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 1670.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	//爆発エフェクト
	bombEffectResId_ = LoadEffekseerEffect("Data/Effect/Bomb/bombEffect.efkefc",1.0f);

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
		MV1DrawModel(transform_.modelId);
		break;
	case Bomb::STATE::IDLE:
		MV1DrawModel(transform_.modelId);
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
}

void Bomb::ChangeStateReserve(void)
{
}

void Bomb::ChangeStateBlast(void)
{
	//爆発エフェクト
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, 10.0f, 10.0f, 10.0f);
}

void Bomb::UpdateNone(void)
{
}

void Bomb::UpdateIdle(void)
{
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
	//3秒後に爆発
	stepReserveTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepReserveTime_ >= RESERVE_MAX_TIME)
	{
		//爆弾爆発準備状態に移行
		ChangeState(STATE::BLAST);
		stepReserveTime_ = 0.0f;
	}
}

void Bomb::UpdateBlast(void)
{

}

void Bomb::DrawBombPlace(void)
{
	VECTOR localPos1 = { 100.0f,-600.0f,-390.0f };
	VECTOR localPos2 = { 100.0f,-590.0f,-390.0f };
	VECTOR pos1 = VAdd(heliTrans_.pos, localPos1);
	VECTOR pos2 = VAdd(heliTrans_.pos, localPos2);
	DrawCapsule3D(pos1, pos2, 50.0f, 1, 0xff0000, 0xffffff, 0);

	transform_.pos = pos1;
	transform_.Update();
}
