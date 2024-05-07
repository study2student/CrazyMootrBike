#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "Bomb.h"

Bomb::Bomb(Bike* bike, const Transform& transform)
{
	bike_ = bike;

	transform_ = transform;

	state_ = STATE::NONE;
}

Bomb::~Bomb(void)
{
}

void Bomb::Init(void)
{
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
