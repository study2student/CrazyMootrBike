#include <string>
#include "..//Application.h"
#include "..///Utility/AsoUtility.h"
#include "..///Manager/InputManager.h"
#include "..//Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "Rotor.h"

Rotor::Rotor()
{

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	isAttack_ = false;

	// 衝突チェック
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;

}

Rotor::~Rotor(void)
{
	delete capsule_;
	//delete animationController_;
}

void Rotor::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HELICOPTER_ROTOR));
	float scale = 5.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 1670.0f, 500.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// アニメーションの設定
	InitAnimation();

	// カプセルコライダ
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(135.0f);

	// 体力
	hp_ = MAX_HP;

	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// 初期状態
	ChangeState(STATE::PLAY);

}

void Rotor::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case Rotor::STATE::NONE:
		UpdateNone();
		break;
	case Rotor::STATE::PLAY:
		UpdatePlay();
		break;
	}


	// モデル制御更新
	transform_.Update();
}

void Rotor::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// 体力とかゲージとか
	DrawUI();

	// 丸影描画
	DrawShadow();

	// デバッグ描画
	DrawDebug();
}

void Rotor::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void Rotor::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule* Rotor::GetCapsule(void) const
{
	return capsule_;
}

void Rotor::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void Rotor::InitAnimation(void)
{
}

void Rotor::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case Rotor::STATE::NONE:
		ChangeStateNone();
		break;
	case Rotor::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void Rotor::ChangeStateNone(void)
{
}

void Rotor::ChangeStatePlay(void)
{
}

void Rotor::UpdateNone(void)
{
}

void Rotor::UpdatePlay(void)
{

	// 移動処理
	ProcessMove();

	// 攻撃処理
	ProcessAttack();

	// デバッグ用
	ProcessDebug();

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = rotY_;
	transform_.Update();
}

void Rotor::DrawUI(void)
{
}

void Rotor::DrawShadow(void)
{
}

void Rotor::DrawDebug(void)
{
}

void Rotor::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//親ヘリの位置に合わせる
	transform_.pos = transformParent_.pos;
	transform_.pos = VAdd(transform_.pos, LOCAL_POS);

	//羽の回転
	// デグリーからラジアン(変換)
	float rad = AsoUtility::Deg2RadF(SPEED_ROT);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

	//クォータニオン(回転)の合成
	rotY_ = rotY_.Mult(rotPow);

}

void Rotor::ProcessJump(void)
{
}

void Rotor::ProcessAttack(void)
{

	NormalAttack();
	SpecialAttack();
	LongAttack();
}

void Rotor::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();
}

void Rotor::NormalAttack(void)
{
}

void Rotor::LongAttack(void)
{

}

void Rotor::SpecialAttack(void)
{
}

void Rotor::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, AsoUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rotor::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, AsoUtility::AXIS_Z);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rotor::Rotate(void)
{
	//stepRotTime_ -= scnMng_.GetDeltaTime();

	//// 回転の球面補間
	//rotY_ = Quaternion::Slerp(
	//	rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Rotor::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void Rotor::CollisionGravity(void)
{

	//// 重力方向
	//VECTOR dirGravity = AsoUtility::DIR_D;

	//// 重力方向の反対
	//VECTOR dirUpGravity = AsoUtility::DIR_U;

	//// 重力の強さ
	//float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	//float checkPow = 10.0f;
	//gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	//gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	//gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	//for (const auto c : colliders_)
	//{

	//	// 地面との衝突
	//	auto hit = MV1CollCheck_Line(
	//		c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

	//	// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
	//	if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
	//	{

	//		// 衝突地点から、少し上に移動
	//		movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

	//	
	//

	//		if (isJump_)
	//		{
	//			// 着地モーション
	//			animationController_->Play(
	//				(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
	//		}

	//		isJump_ = false;

	//	}

	//}
}

void Rotor::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto c : colliders_)
	{

		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;

			}

		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);

	}

}

void Rotor::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	//float dot = VDot(dirGravity, jumpPow_);
	//if (dot >= 0.0f)
	//{
	//	// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
	//	jumpPow_ = gravity;
	//}

}

bool Rotor::IsEndLanding(void)
{
	bool ret = true;

	////アニメーションが終了しているか
	//if (animationController_->IsEnd())
	//{
	//	return ret;
	//}

	return false;
}



