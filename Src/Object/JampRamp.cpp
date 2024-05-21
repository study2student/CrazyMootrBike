#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "JampRamp.h"

JampRamp::JampRamp() : resMng_(ResourceManager::GetInstance())
{
}

JampRamp::~JampRamp(void)
{
}

void JampRamp::Init(void)
{

	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::JUMP_RAMP));
	float scale = 1.0f;
	transform_.scl = { scale,scale,scale };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });;
	transform_.pos = { 1500.0f, -260.0f, 1000.0f };
	
	transform_.Update();
}

void JampRamp::Update(void)
{
}

void JampRamp::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}
