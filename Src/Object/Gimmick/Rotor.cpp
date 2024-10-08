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

// 初期座標
const VECTOR INIT_POS = { 1670.0f, 500.0f, 0.0f };

// 初期大きさ
const VECTOR INIT_SCL = { 5.0f,5.0f,5.0f };

// 初期回転
const VECTOR INIT_ROT = { 0.0f, 0.0f, 0.0f };

// 初期ローカル回転
const VECTOR INIT_LOCAL_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

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
	hp_(-1)
{
	//位置回転大きさ
	initScl_ = INIT_SCL;
	initRotEuler_ = INIT_ROT;
	initLocalRotEuler_ = INIT_LOCAL_ROT;
	initPos_ = INIT_POS;

	//カプセル
	capsulePosTop_ = CAPSULE_LOCAL_POS_TOP;
	capsulePosDown_ = CAPSULE_LOCAL_POS_DOWN;
	capsuleRadius_ = CAPSULE_RADIUS;

	//回転
	rotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;
}

Rotor::~Rotor(void)
{
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

void Rotor::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Rotor::InitLoad(void)
{
	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::HELICOPTER_ROTOR));
}

void Rotor::InitPost(void)
{
	// 体力
	hp_ = MAX_HP;

	// 初期状態
	ChangeState(STATE::PLAY);
}




