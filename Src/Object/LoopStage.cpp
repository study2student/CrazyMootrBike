#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "LoopStage.h"

LoopStage::LoopStage(Bike* bike, const Transform& transform)
{
	bike_ = bike;

	transform_ = transform;

	state_ = STATE::NONE;

}

LoopStage::~LoopStage(void)
{
}

void LoopStage::Init(void)
{

	ChangeState(STATE::IDLE);
}

void LoopStage::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case LoopStage::STATE::NONE:
		UpdateNone();
		break;
	case LoopStage::STATE::IDLE:
		UpdateIdle();
		break;
	case LoopStage::STATE::MAKE:
		UpdateMake();
		break;
	case LoopStage::STATE::BACK:
		UpdateBack();
		break;
	}

}

void LoopStage::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void LoopStage::Destroy(void)
{
	MV1DeleteModel(transform_.modelId);
}

VECTOR LoopStage::GetPos(void)
{
	return transform_.pos;
}

void LoopStage::ChangeState(STATE state)
{
	//状態変更
	state_ = state;

	// 更新ステップ
	switch (state_)
	{
	case LoopStage::STATE::NONE:
		ChangeStateNone();
		break;
	case LoopStage::STATE::IDLE:
		ChangeStateIdle();
		break;
	case LoopStage::STATE::MAKE:
		ChangeStateMake();
		break;
	case LoopStage::STATE::BACK:
		ChangeStateBack();
		break;
	}
}

void LoopStage::ChangeStateNone(void)
{
}

void LoopStage::ChangeStateIdle(void)
{
}

void LoopStage::ChangeStateMake(void)
{


}

void LoopStage::ChangeStateBack(void)
{
}

void LoopStage::UpdateNone(void)
{
}

void LoopStage::UpdateIdle(void)
{
}

void LoopStage::UpdateMake(void)
{
}

void LoopStage::UpdateBack(void)
{
}
