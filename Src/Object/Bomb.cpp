#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Utility/AsoUtility.h"
#include "Bomb.h"

Bomb::Bomb()
{
	state_ = STATE::NONE;
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
	MV1DrawModel(transform_.modelId);
	DrawBombPlace();
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
}

void Bomb::UpdateNone(void)
{
}

void Bomb::UpdateIdle(void)
{
}

void Bomb::UpdateReserve(void)
{
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
