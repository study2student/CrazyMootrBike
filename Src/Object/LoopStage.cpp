#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "Common/Transform.h"
#include "../Object/Rider/Player.h"
#include "LoopStage.h"

LoopStage::LoopStage(Player* player, const Transform& transform)
{
	player_ = player;

	transform_ = transform;

	state_ = STATE::NONE;

}

LoopStage::~LoopStage(void)
{
}

void LoopStage::Init(void)
{
	// モデルの基本情報
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::DEMO_STAGE)
	);
	transform_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE);

	//最初の情報
	transform_.pos = { -5000.0f, -5600.0f, 6500.0f };
	transform_.scl = { 1.0f,1.0f,1.0f };
	transform_.quaRot = Quaternion();
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	
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

	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));

	/*if (loopTrans.pos.z + 5000 <= player_->GetTransform().pos.z)
	{
		loopTrans.pos.z += 6500;

	}*/

	float scale = 1.0f;
	transform_.scl = { scale,scale,scale };
	transform_.quaRot = Quaternion();

	// 当たり判定(コライダ)作成
	transform_.MakeCollider(Collider::TYPE::STAGE);
	transform_.Update();

	if (transform_.pos.z + 5000 <= player_->GetTransform().pos.z)
	{
		transform_.pos.z += 6500;
		ChangeState(STATE::IDLE);
	}

	
}

void LoopStage::ChangeStateBack(void)
{
}

void LoopStage::UpdateNone(void)
{
}

void LoopStage::UpdateIdle(void)
{
	if (transform_.pos.z + 5000 <= player_->GetTransform().pos.z)
	{
		ChangeState(STATE::MAKE);
	}
}

void LoopStage::UpdateMake(void)
{
}

void LoopStage::UpdateBack(void)
{
}
