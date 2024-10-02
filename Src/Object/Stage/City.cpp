#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "City.h"

City::City(std::shared_ptr<Bike> bike, const Transform& transform)
	:
	bike_(bike),
	state_(STATE::NONE)
{
	transform_ = transform;
}

City::~City(void)
{
}

void City::Init(void)
{
	ChangeState(STATE::IDLE);
}

void City::Update(void)
{

	// 更新ステップ
	switch (state_)
	{
	case City::STATE::NONE:
		UpdateNone();
		break;
	case City::STATE::IDLE:
		UpdateIdle();
		break;
	case City::STATE::MAKE:
		UpdateMake();
		break;
	case City::STATE::BACK:
		UpdateBack();
		break;
	}
}

void City::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void City::Destroy(void)
{
	state_ = STATE::BACK;
	MV1DeleteModel(transform_.modelId);
}

VECTOR City::GetPos(void)
{
	return transform_.pos;;
}

City::STATE City::GetState(void)
{
	return state_;
}

bool City::IsDestroy(void)
{
	return state_ == STATE::BACK;
}

void City::ChangeState(STATE state)
{
	//状態変更
	state_ = state;

	// 更新ステップ
	switch (state_)
	{
	case City::STATE::NONE:
		ChangeStateNone();
		break;
	case City::STATE::IDLE:
		ChangeStateIdle();
		break;
	case City::STATE::MAKE:
		ChangeStateMake();
		break;
	case City::STATE::BACK:
		ChangeStateBack();
		break;
	}
}

void City::ChangeStateNone(void)
{
}

void City::ChangeStateIdle(void)
{
}

void City::ChangeStateMake(void)
{
}

void City::ChangeStateBack(void)
{
}

void City::UpdateNone(void)
{
}

void City::UpdateIdle(void)
{
}

void City::UpdateMake(void)
{
}

void City::UpdateBack(void)
{
}
