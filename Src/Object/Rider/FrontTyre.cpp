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
#include "FrontTyre.h"

FrontTyre::FrontTyre()
{
	state_ = STATE::NONE;


	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();

}

FrontTyre::~FrontTyre(void)
{
}

void FrontTyre::Init(void)
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

void FrontTyre::Update(void)
{
	switch (state_)
	{
	case FrontTyre::STATE::NONE:
		UpdateNone();
		break;
	case FrontTyre::STATE::ROT:
		UpdateRot();
		break;
	}

	// モデル制御更新
	transform_.Update();
}

void FrontTyre::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// デバッグ描画
	DrawDebug();
}

void FrontTyre::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void FrontTyre::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case FrontTyre::STATE::NONE:
		ChangeStateNone();
		break;
	case FrontTyre::STATE::ROT:
		ChangeStateRot();
		break;
	}
}

void FrontTyre::ChangeStateNone(void)
{
}

void FrontTyre::ChangeStateRot(void)
{
}

void FrontTyre::UpdateNone(void)
{
	//位置合わせ
	SyncParent();
}

void FrontTyre::UpdateRot(void)
{
	// デバッグ用
	ProcessDebug();

	// 移動方向に応じた回転
	Rotate();

	//位置合わせ
	SyncParent();
}

void FrontTyre::DrawDebug(void)
{
}

void FrontTyre::ProcessDebug(void)
{
}

void FrontTyre::Rotate(void)
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

void FrontTyre::SyncParent(void)
{
	//親バイクの位置に合わせる
	//座標も少し調整
	transform_.pos = { transformParent_.pos.x,0.0f,transformParent_.pos.z };
	transform_.pos = VAdd(transform_.pos, LOCAL_POS);
	transform_.quaRot = transformParent_.quaRot;
	transform_.quaRot.Mult(transform_.quaRot, transformParent_.quaRot);
	//// モデル制御更新
	//transform_.Update();
}
