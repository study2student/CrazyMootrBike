#include <DxLib.h>
#include "../../Manager/SceneManager.h"
#include "../Common/Transform.h"
#include "Planet.h"

#pragma region �萔�錾

// �d�͂�������͈�
const float DEFAULT_GRAVITY_RADIUS = 5000.0f;

// �Q�[���I�[�o�[�͈�
const float DEFAULT_DEAD_LENGTH = 1000.0f;

#pragma endregion


Planet::Planet(const Stage::NAME& name, const TYPE& type, const Transform& transform)
	:
	name_(name),
	type_(type),
	gravityPow_(0.0f),
	gravityRadius_(0.0f),
	deadLength_(0.0f)
{
	transform_ = transform;
}

Planet::~Planet(void)
{
}

void Planet::Init(void)
{
	gravityPow_ = DEFAULT_GRAVITY_POW;
	gravityRadius_ = DEFAULT_GRAVITY_RADIUS;
	deadLength_ = DEFAULT_DEAD_LENGTH;
}

void Planet::Update(void)
{
}

void Planet::Draw(void)
{
    MV1DrawModel(transform_.modelId);
}

void Planet::SetPosition(const VECTOR& pos)
{
    transform_.pos = pos;
    transform_.Update();
}

void Planet::SetRotation(const Quaternion& rot)
{
	transform_.quaRot = rot;
	transform_.Update();
}

float Planet::GetGravityPow(void) const
{
	return gravityPow_;
}

void Planet::SetGravityPow(float pow)
{
	gravityPow_ = pow;
}

float Planet::GetGravityRadius(void) const
{
	return gravityRadius_;
}

void Planet::SetGravityRadius(float radius)
{
	gravityRadius_ = radius;
}

const Planet::TYPE& Planet::GetType(void) const
{
	return type_;
}

bool Planet::InRangeGravity(const VECTOR& pos) const
{
	return false;
}

bool Planet::InRangeDead(const VECTOR& pos) const
{
	return false;
}

void Planet::SetDeadLength(float len)
{
	deadLength_ = len;
}

const Stage::NAME& Planet::GetName(void) const
{
	return name_;
}
