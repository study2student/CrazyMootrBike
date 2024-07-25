#include <string>
#include<EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Stage/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "../../Scene/GameScene.h"
#include "CoinBase.h"

CoinBase::CoinBase(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos)
{
	bikes_ = bikes;
	gameScene_ = gameScene;

	state_ = STATE::NONE;

	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	makePos_ = loopStagePos;
	localPos_ = localPos;

	enemyRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = MyUtility::VECTOR_ZERO;

	// 衝突チェック
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	isBikeCol_ = false;

	isAddScore_ = false;

	capsule_ = nullptr;

	isCollGround_ = false;

	stepMade_ = 0.0f;
}

CoinBase::~CoinBase(void)
{
}

void CoinBase::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COPPER_COIN));
	transform_.scl = MyUtility::VECTOR_ONE;
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, makePos_.y, makePos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ COLLIDER_POS_TOP });
	capsule_->SetLocalPosDown({ COLLIDER_POS_DOWN });
	capsule_->SetRadius(20.0f);

	// 初期状態
	ChangeState(STATE::PLAY);

	////敵キャラ個別のパラメータ設定
	SetParam();

	// エフェクト初期化
	InitEffect();
}

void CoinBase::SetParam(void)
{

}

void CoinBase::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case CoinBase::STATE::NONE:
		UpdateNone();
		break;
	case CoinBase::STATE::PLAY:
		UpdatePlay();
		break;
	case CoinBase::STATE::FLIPED:
		UpdateFliped();
		break;
	case CoinBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// モデル制御更新
	transform_.Update();
}

void CoinBase::Draw(void)
{
	//死亡状態は描画しない
	if (state_ == STATE::DEAD)
	{
		return;
	}

	// モデルの描画
	MV1DrawModel(transform_.modelId);

	DrawLine3D(fowardPos_, backPos_, 0x0000ff);

}

void CoinBase::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void CoinBase::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> CoinBase::GetCapsule(void) const
{
	return capsule_;
}

void CoinBase::Flip(VECTOR dir)
{
	flipDir_ = dir;
	flipSpeed_ = 10.0f;
	ChangeState(STATE::FLIPED);
}

bool CoinBase::GetIsBikeCol(void)
{
	return isBikeCol_;
}

bool CoinBase::GetIsAddScore(void)
{
	return isAddScore_;
}

CoinBase::STATE CoinBase::GetState(void)
{
	return state_;
}

bool CoinBase::IsDestroy(void)
{
	return state_ == STATE::DEAD;
}

void CoinBase::Destroy(void)
{
	ChangeState(STATE::DEAD);
}

void CoinBase::AddScoreToPlayer(int playerId, int score)
{
	if (playerId >= 0 && playerId < bikes_.size())
	{
		bikes_[playerId]->AddScore(score);
	}
}

void CoinBase::InitEffect(void)
{
	// ヒットエフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;

}

void CoinBase::HitEffect()
{
	effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

	float scale = 50.0f;

	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + EFF_POS_Z);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	
}

void CoinBase::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case CoinBase::STATE::NONE:
		ChangeStateNone();
		break;
	case CoinBase::STATE::PLAY:
		ChangeStatePlay();
		break;
	case CoinBase::STATE::FLIPED:
		ChangeStateFliped();
		break;
	case CoinBase::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void CoinBase::ChangeStateNone(void)
{
}

void CoinBase::ChangeStatePlay(void)
{
}

void CoinBase::ChangeStateFliped(void)
{
}

void CoinBase::ChangeStateDead(void)
{
}

void CoinBase::UpdateNone(void)
{
}

void CoinBase::UpdateFliped(void)
{

	// 移動処理
	ProcessMove();

	// 吹っ飛ばされる
	flipSpeed_ -= 0.16f;
	if (flipSpeed_ < 0.0f)
	{
		flipSpeed_ = 0.0f;
		ChangeState(STATE::PLAY);
	}
	movePow_ = VAdd(movePow_, VScale(flipDir_, flipSpeed_));

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = enemyRotY_;

}

void CoinBase::UpdateDead(void)
{
	isAddScore_ = false;
}

void CoinBase::UpdatePlay(void)
{
	// 移動処理
	ProcessMove();

	//回転
	RotY();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = enemyRotY_;
}

void CoinBase::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//衝突判定(敵とプレイヤー)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = MyUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//スコア加算
			isAddScore_ = true;
			//衝突判定
			isBikeCol_ = true;

			// ヒットエフェクト
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

			//コイン収集時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);

			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			isBikeCol_ = false;
			isAddScore_ = false;
			
		}
	}

	//削除処理
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}
}

void CoinBase::SetGoalRotate(float rotRad)
{
	//VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1f)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void CoinBase::RotY(void)
{
	//地面と衝突したら回転
	if (isCollGround_)
	{
		//回転
		// デグリーからラジアン(変換)
		float rad = MyUtility::Deg2RadF(SPEED_ROT);

		// ラジアンからクォータニオン(指定軸を指定角分回転させる)
		Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_Y);

		//クォータニオン(回転)の合成
		enemyRotY_ = enemyRotY_.Mult(rotPow);
	}
}

void CoinBase::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	//y座標での死亡判定
	if (transform_.pos.y <= DEAD_POS_Y)
	{
		ChangeState(STATE::DEAD);
	}

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void CoinBase::CollisionGravity(void)
{

	// ジャンプ量を加算
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = MyUtility::DIR_U;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// ジャンプリセット
			jumpPow_ = MyUtility::VECTOR_ZERO;

			isCollGround_ = true;

		}

	}
}

void CoinBase::CollisionCapsule(void)
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

void CoinBase::CalcGravityPow(void)
{
	//1回当たったら中断
	if (isCollGround_)
	{
		transform_.pos.y = COLL_AFTER_POS_Y;
		return;
	}

	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, MyUtility::VECTOR_ZERO);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow_ = gravity;
	}
}