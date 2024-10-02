#include <string>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Stage/Planet.h"
#include "Rotor.h"

#pragma region 定数宣言

// 回転完了までの時間
const float TIME_ROT = 1.0f;

//回転速度
const float SPEED_ROT = 20.0f;

// HPの最大値
const int MAX_HP = 100;

//羽の大きさ
const float SCL = 5.0f;

//初期座標
const VECTOR INIT_POS = { 1670.0f, 500.0f, 0.0f };

// 初期ローカルY回転
const float INIT_LOCAL_ROT_Y = 180.0f;

//カプセルローカル座標上
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 190.0f, -60.0f };

//カプセルローカル座標下
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 150.0f, -60.0f };

//カプセル半径
const float CAPSULE_RADIUS = 135.0f;

//相対座標
const VECTOR LOCAL_POS = { 0.0f,250.0f,0.0f };

//回転しきい値
const float ROTATE_ANGLE_DIFF_MIN = 0.1f;

#pragma endregion


Rotor::Rotor()
	:
	transformParent_(Transform()),
	state_(STATE::NONE),
	movePow_({}),
	movedPos_({}),
	rotY_(Quaternion()),
	goalQuaRot_(Quaternion()),
	stepRotTime_(0.0f),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	hp_(-1)
{
}

Rotor::~Rotor(void)
{
	delete capsule_;
}

void Rotor::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HELICOPTER_ROTOR));
	transform_.scl = { SCL, SCL, SCL };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y), 0.0f });

	// カプセルコライダ
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

	// 体力
	hp_ = MAX_HP;

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

	// デバッグ用
	ProcessDebug();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = rotY_;
	transform_.Update();
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
	float rad = MyUtility::Deg2RadF(SPEED_ROT);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_Y);

	//クォータニオン(回転)の合成
	rotY_ = rotY_.Mult(rotPow);

}

void Rotor::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();
}

void Rotor::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
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

void Rotor::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, MyUtility::AXIS_Z);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}


void Rotor::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 移動
	transform_.pos = movedPos_;
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
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

}




