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

#pragma region 定数宣言

//当たり判定用コライダーの上座標
const VECTOR COLLIDER_POS_TOP = { 0.0f, 110.0f, 0.0f };

//当たり判定用コライダーの下座標
const VECTOR COLLIDER_POS_DOWN = { 0.0f, 30.0f, 0.0f };

//当たり判定用コライダーの半径
const float COLLIDER_RADIUS = 20.0f;

//回転完了までの時間
const float TIME_ROT = 1.0f;

//当たった時のスコア増分
const int SCORE_INCREMENT = 100;

//地面衝突後の座標
const float COLL_AFTER_POS_Y = -150.0f;

//回転スピード
const float SPEED_ROT = 7.0f;

// 削除状態になるY座標
const float DEAD_POS_Y = -500.0f;

// エフェクト大きさ
const float EFF_SCL = 50.0f;

// エフェクト出現位置Z座標
const int EFF_POS_Z = 500;

//回転しきい値
const float ROTATE_ANGLE_DIFF_MIN = 0.1f;

//スコア増分値
const int ADD_SCORE_NUM = 10;

//地面衝突後の上方向の力
const float AFTER_HIT_GROUND_UP_POW = 2.0f;

//衝突回数
const int TRY_MAX = 10;

//内積
const float GRAVITY_THRES_HOLD = 0.9f;

#pragma endregion


CoinBase::CoinBase(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos)
	:
	bikes_(bikes),
	gameScene_(gameScene),
	state_(STATE::NONE),
	moveDir_({}),
	movePow_({}),
	movedPos_({}),
	makePos_(loopStagePos),
	localPos_(localPos),
	rotY_({}),
	goalQuaRot_({}),
	stepRotTime_(0.0f),
	jumpPow_({}),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	isBikeCol_(false),
	isAddScore_(false),
	colliders_({}),
	capsule_(nullptr),
	isCollGround_ (false),
	stepMade_(0.0f),
	effectHitResId_(-1),
	effectHitPlayId_(-1)
{
}

CoinBase::~CoinBase(void)
{
}

void CoinBase::Init(void)
{

	//コイン個別のパラメータ設定
	SetParam();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ COLLIDER_POS_TOP });
	capsule_->SetLocalPosDown({ COLLIDER_POS_DOWN });
	capsule_->SetRadius(COLLIDER_RADIUS);

	// 初期状態
	ChangeState(STATE::PLAY);

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
	if (IsDestroy())
	{
		return;
	}

	// モデルの描画
	MV1DrawModel(transform_.modelId);

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

const bool& CoinBase::GetIsBikeCol(void) const
{
	return isBikeCol_;
}

const bool& CoinBase::GetIsAddScore(void) const
{
	return isAddScore_;
}

const CoinBase::STATE& CoinBase::GetState(void) const
{
	return state_;
}

const bool& CoinBase::IsDestroy(void) const
{
	return state_ == STATE::DEAD;
}

void CoinBase::Destroy(void)
{
	ChangeState(STATE::DEAD);
}

void CoinBase::AddScoreToPlayer(int playerId, int score)
{
	//プレイヤーにスコアを与える
	if (playerId >= 0 && playerId < bikes_.size())
	{
		bikes_[playerId]->AddScore(score);
	}
}

void CoinBase::InitEffect(void)
{
	// ヒットエフェクト読み込み
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;

}

void CoinBase::PlayHitEffect()
{
	//ヒットエフェクト再生
	effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

	//位置回転大きさ設定
	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + EFF_POS_Z);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, EFF_SCL, EFF_SCL, EFF_SCL);
	
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

void CoinBase::ChangeStateDead(void)
{
}

void CoinBase::UpdateNone(void)
{
}

void CoinBase::UpdateDead(void)
{
	//スコアを追加しない
	isAddScore_ = false;
}

void CoinBase::UpdatePlay(void)
{
	// 動き処理
	ProcessMove();
}

void CoinBase::ProcessMove(void)
{

	//回転
	RotY();

	// 重力による移動量
	CalcGravityPow();

	// 地形衝突判定
	CollisionGround();

	// 回転させる
	transform_.quaRot = rotY_;

	//バイク(プレイヤー)との衝突判定
	BikeCollision();

	//時間がある程度たったら削除(取り逃したコイン)
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}
}

void CoinBase::SetGoalRotate(float rotRad)
{
	Quaternion axis = Quaternion::AngleAxis(rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
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
		rotY_ = rotY_.Mult(rotPow);
	}
}

void CoinBase::CollisionGround(void)
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
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * AFTER_HIT_GROUND_UP_POW));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto& c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > GRAVITY_THRES_HOLD)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, AFTER_HIT_GROUND_UP_POW));

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
	for (const auto& c : colliders_)
	{

		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < TRY_MAX; tryCnt++)
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

void CoinBase::BikeCollision(void)
{
	//バイクとの衝突判定

	auto& ins = InputManager::GetInstance();

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	//バイク情報取得
	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//衝突判定(コインとプレイヤー)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = MyUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//範囲に入った
			isBikeCol_ = true;
			isAddScore_ = true;

			// ヒットエフェクト再生
			PlayHitEffect();

			int playNum = gameScene_->GetPlayNum();
			if (playNum == 1)
			{
				if (!gameScene_->OnePersonIsGoal())
				{
					//スコア加算
					AddScoreToPlayer(bike->GetPlayerID(), ADD_SCORE_NUM);

					// コイン収集時の音を再生
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}
			else
			{
				//ゴールしてないプレイヤーにだけ
				if (!bike->GetIsGoal())
				{
					//スコア加算
					AddScoreToPlayer(bike->GetPlayerID(), ADD_SCORE_NUM);

					// コイン収集時の音を再生
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}

			if (isBikeCol_)
			{
				//当たったら削除
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			//スコア追加しない
			isAddScore_ = false;
		}
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