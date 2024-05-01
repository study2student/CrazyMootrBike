#include <DxLib.h>
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
#include "../../Object/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "ShortDisEnemy.h"


ShortDisEnemy::ShortDisEnemy(Bike* bike) : EnemyBase(bike)
{
}

void ShortDisEnemy::SetParam(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHORT));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 700.0f, -300.0f, -500.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();


	// アニメーションの設定
	InitAnimation();


	//// カプセルコライダ
	//capsule_ = new Capsule(transform_);
	//capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	//capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	//capsule_->SetRadius(20.0f);

	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// 初期状態
	ChangeState(STATE::PLAY);
}

void ShortDisEnemy::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBase::STATE::PLAY:
		UpdatePlay();
		break;
	}


	// モデル制御更新
	transform_.Update();

	// アニメーション再生
	animationController_->Update();
}

void ShortDisEnemy::UpdatePlay(void)
{
	// 移動処理
	ProcessMove();

	// ジャンプ処理
	ProcessJump();

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = enemyRotY_;
}

void ShortDisEnemy::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//// 移動量をゼロ
	//movePow_ = AsoUtility::VECTOR_ZERO;

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// 回転したい角度
	double rotRad = 0;

	//VECTOR dir = AsoUtility::DIR_F;
	VECTOR dir;// = AsoUtility::DIR_F;

	//// カメラ方向に前進したい
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	rotRad = AsoUtility::Deg2RadD(0.0);
	//	dir = cameraRot.GetForward();
	//}

	//// カメラ方向から後退したい
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	rotRad = AsoUtility::Deg2RadD(180.0);
	//	dir = cameraRot.GetBack();
	//}

	//// カメラ方向から右側へ移動したい
	//if (ins.IsNew(KEY_INPUT_D))
	//{
	//	rotRad = AsoUtility::Deg2RadD(90.0);
	//	dir = cameraRot.GetRight();
	//}

	//// カメラ方向から左側へ移動したい
	//if (ins.IsNew(KEY_INPUT_A))
	//{
	//	rotRad = AsoUtility::Deg2RadD(270.0);
	//	dir = cameraRot.GetLeft();
	//}

	Transform bikeTrans_ = bike_->GetTransform();

	//
	VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	dir = VNorm(len);


	/*if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {*/

	// 移動処理
	//speed_ = SPEED_MOVE;
	//衝突判定(敵とプレイヤー)
	VECTOR diff = VSub(bike_->GetCapsule()->GetCenter(), capsule_->GetCenter());
	float  dis = AsoUtility::SqrMagnitudeF(diff);
	if (dis < RADIUS * RADIUS)
	{
		//範囲に入った
		speed_ = 0;
		//アニメーション
		animationController_->Play((int)ANIM_TYPE::SHORT);
	}
	else
	{
		speed_ = SPEED_MOVE;

		if (!isJump_ && IsEndLanding())
		{

			animationController_->Play((int)ANIM_TYPE::FAST_RUN);

		}
	}

	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/
	moveDir_ = dir;
	movePow_ = VScale(dir, speed_);

	// 回転処理(プレイヤーの方向を向かせる)
	VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	double subDeg = atan2(subVec.x, subVec.z);
	SetGoalRotate(subDeg);


	//}
	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/
}
