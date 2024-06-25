#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "Bike.h"
#include "RearTyre.h"

RearTyre::RearTyre()
{
	state_ = STATE::NONE;


	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

RearTyre::~RearTyre(void)
{
}

void RearTyre::Init(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TYRE));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 1670.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// 初期状態
	ChangeState(STATE::ROT);
}

void RearTyre::Update(void)
{
	switch (state_)
	{
	case RearTyre::STATE::NONE:
		UpdateNone();
		break;
	case RearTyre::STATE::ROT:
		UpdateRot();
		break;
	}

	// モデル制御更新
	transform_.Update();
}

void RearTyre::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();
}

void RearTyre::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void RearTyre::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case RearTyre::STATE::NONE:
		ChangeStateNone();
		break;
	case RearTyre::STATE::ROT:
		ChangeStateRot();
		break;
	}
}

void RearTyre::ChangeStateNone(void)
{
}

void RearTyre::ChangeStateRot(void)
{
}

void RearTyre::UpdateNone(void)
{
	//位置合わせ
	SyncParent();
}

void RearTyre::UpdateRot(void)
{
	// デバッグ用
	ProcessDebug();

	// 移動方向に応じた回転
	Rotate();

	//位置合わせ
	SyncParent();
}

void RearTyre::DrawDebug(void)
{
}

void RearTyre::ProcessDebug(void)
{
}

void RearTyre::Rotate(void)
{
	auto& ins = InputManager::GetInstance();

	//羽の回転
	// デグリーからラジアン(変換)
	float rad = AsoUtility::Deg2RadF(SPEED_ROT);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_X);

	//クォータニオン(回転)の合成
	rotX_ = rotX_.Mult(rotPow);

	// 回転させる
	transform_.quaRot = rotX_;
	transform_.Update();
}

void RearTyre::SyncParent(void)
{
	//親バイクの位置に合わせる
	//座標も少し調整
	//transform_.pos = transformParent_.pos;
	transform_.pos = { transformParent_.pos.x, 0.0f, transformParent_.pos.z };
	transform_.pos = VAdd(transform_.pos, LOCAL_POS);
	transform_.quaRot = transformParent_.quaRot;
	transform_.quaRot.Mult(transform_.quaRot, transformParent_.quaRot);
}
