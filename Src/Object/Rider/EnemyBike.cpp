#include <string>
#include "../../Application.h"
#include "../..//Utility/MyUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "Weapon.h"
#include "EnemyBase.h"
#include "EnemyBike.h"

EnemyBike::EnemyBike(EnemyBase* enemy)
{

	enemy_ = enemy;

	weapon_ = nullptr;

	animationController_ = nullptr;

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = MyUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	isAttack_ = false;

	// 衝突チェック
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;
}

EnemyBike::~EnemyBike(void)
{
	delete weapon_;
	delete animationController_;
}

void EnemyBike::Init(void)
{

	// 武器
	weapon_ = new Weapon();
	weapon_->Init();

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BIKE));
	float scale = 1.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// モデルの基本設定
	transform_.Update();

	weapon_->SetTransForm(transform_);

	// アニメーションの設定
	InitAnimation();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(135.0f);


	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// 初期状態
	ChangeState(STATE::PLAY);
}

void EnemyBike::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case EnemyBike::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBike::STATE::PLAY:
		UpdatePlay();
		//動作
		ProcessMove();
		break;
	}

	weapon_->Update();
	weapon_->SetTransForm(transform_);
	// モデル制御更新
	transform_.Update();
	// アニメーション再生
	animationController_->Update();

	
}

void EnemyBike::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// 武器
	weapon_->Draw();

	// 丸影描画
	DrawShadow();

	// デバッグ描画
	DrawDebug();
}

void EnemyBike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EnemyBike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> EnemyBike::GetCapsule(void) const
{
	return capsule_;
}

void EnemyBike::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transformPlayer_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EnemyBike::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case EnemyBike::STATE::NONE:
		ChangeStateNone();
		break;
	case EnemyBike::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void EnemyBike::ChangeStateNone(void)
{
	
}

void EnemyBike::ChangeStatePlay(void)
{
}

void EnemyBike::UpdateNone(void)
{
}

void EnemyBike::UpdatePlay(void)
{
	//// 移動処理
	//ProcessMove();

	// ジャンプ処理
	ProcessJump();

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

	// 

	// 回転させる
	transform_.quaRot = playerRotY_;
}

void EnemyBike::DrawUI(void)
{
}

void EnemyBike::DrawShadow(void)
{
}

void EnemyBike::DrawDebug(void)
{
	capsule_->Draw();
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	// 攻撃が当たったか
	if (isAttack_ == true)
	{
		DrawString(0, 0, "Attack", 0x000000);
	}

	DrawFormatString(10, 10, 0xffffff, "bikePos = %f,%f,%f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void EnemyBike::ProcessMove(void)
{
	transform_.pos = enemy_->GetTransform().pos;
	transform_.quaRot = enemy_->GetTransform().quaRot;
	transform_.Update();


	
}

void EnemyBike::ProcessJump(void)
{
}

void EnemyBike::ProcessAttack(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_Z))
	{
		attackState_ = ATTACK_TYPE::NORMAL;
		animationController_->Play((int)ANIM_TYPE::FALLING);
		isAttack_ = true;
	}
	else
	{
		attackState_ = ATTACK_TYPE::NONE;
		isAttack_ = false;
	}
}

void EnemyBike::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_C))
	{
		hp_ -= 1;
	}

}

void EnemyBike::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void EnemyBike::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void EnemyBike::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);
	movedPos_ = VAdd(transformPlayer_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void EnemyBike::CollisionGravity(void)
{
	//// ジャンプ量を加算
	//movedPos_ = VAdd(movedPos_, jumpPow_);

	//// 重力方向
	//VECTOR dirGravity = MyUtility::DIR_D;

	//// 重力方向の反対
	//VECTOR dirUpGravity = MyUtility::DIR_U;

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

	//		// ジャンプリセット
	//		jumpPow_ = MyUtility::VECTOR_ZERO;
	//		stepJump_ = 0.0f;

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

void EnemyBike::CollisionCapsule(void)
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

void EnemyBike::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow_ = gravity;
	}
}

bool EnemyBike::IsEndLanding(void)
{
	bool ret = true;

	// アニメーションがジャンプではない
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	// アニメーションが終了しているか
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}
