#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "StageCurve.h"

StageCurve::StageCurve(std::shared_ptr<Bike> bike, const Transform& transform)
{
	bike_ = bike;

	transform_ = transform;

	state_ = STATE::NONE;
}

StageCurve::~StageCurve(void)
{
}

void StageCurve::Init(void)
{
	ChangeState(STATE::IDLE);
}

void StageCurve::Update(void)
{
	//èÛë‘çXêV
	switch (state_)
	{
	case StageCurve::STATE::NONE:
		UpdateNone();
		break;
	case StageCurve::STATE::IDLE:
		UpdateIdle();
		break;
	case StageCurve::STATE::MAKE:
		UpdateMake();
		break;
	case StageCurve::STATE::BACK:
		UpdateBack();
		break;
	}
}

void StageCurve::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void StageCurve::Destroy(void)
{
	state_ = STATE::BACK;
	MV1DeleteModel(transform_.modelId);
}

VECTOR StageCurve::GetPos(void)
{
	return transform_.pos;
}

StageCurve::STATE StageCurve::GetState(void)
{
	return state_;
}

bool StageCurve::IsDestroy(void)
{
	return state_ == STATE::BACK;
}

void StageCurve::ChangeState(STATE state)
{
	//èÛë‘
	state_ = state;

	// èÛë‘ëJà⁄
	switch (state_)
	{
	case StageCurve::STATE::NONE:
		ChangeStateNone();
		break;
	case StageCurve::STATE::IDLE:
		ChangeStateIdle();
		break;
	case StageCurve::STATE::MAKE:
		ChangeStateMake();
		break;
	case StageCurve::STATE::BACK:
		ChangeStateBack();
		break;
	}
}

void StageCurve::ChangeStateNone(void)
{
}

void StageCurve::ChangeStateIdle(void)
{
}

void StageCurve::ChangeStateMake(void)
{
}

void StageCurve::ChangeStateBack(void)
{
}

void StageCurve::UpdateNone(void)
{
}

void StageCurve::UpdateIdle(void)
{
}

void StageCurve::UpdateMake(void)
{
}

void StageCurve::UpdateBack(void)
{
}
