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
#include "Bomb.h"
#include "../../Scene/GameScene.h"
#include "Helicopter.h"

#pragma region 定数宣言

// 通常スピード
const float SPEED_MOVE = 120.0f;

// 回転完了までの時間
const float TIME_ROT = 1.0f;

//攻撃状態にするためのプレイヤーからヘリまでの相対座標
const VECTOR ATTACK_LINE_LOCAL_POS = { 0.0f,0.0f,5000.0f };

//攻撃状態の最大範囲を作るためのプレイヤーからヘリまでの相対座標
const VECTOR ATTACK_LINE_MAX_LOCAL_POS = { 0.0f,0.0f,10000.0f };

// HPの最大値
const int MAX_HP = 100;

//ヘリの大きさ
const float SCL = 5.0f;

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

//回転しきい値
const float ROTATE_ANGLE_DIFF_MIN = 0.1f;

#pragma endregion


Helicopter::Helicopter(GameScene* gameScene)
	:
	gameScene_(gameScene),
	rotor_(nullptr),
	bomb_(nullptr),
	targetTrans_(Transform()),
	isTargetOutside_(false),
	state_(STATE::NONE),
	attackState_(),
	speed_(0.0f),
	moveDir_({}),	
	hp_(-1),
	isAttack_(false)
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

Helicopter::~Helicopter(void)
{
	delete rotor_;
}

void Helicopter::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case Helicopter::STATE::NONE:
		UpdateNone();
		break;
	case Helicopter::STATE::MOVE:
		UpdateMove();
		break;
	case Helicopter::STATE::ATTACK:
		UpdateAttack();
		break;
	case Helicopter::STATE::DEAD:
		UpdateDead();
		break;
	}

	// モデル制御更新
	transform_.Update();
}

void Helicopter::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);

	//羽
	rotor_->Draw();

	//爆弾
	bomb_->Draw();
}

void Helicopter::SetBikeIsOutside(const bool& isOutside)
{
	isTargetOutside_ = isOutside;
}

void Helicopter::SetBikeTrans(const Transform& bikeTrans)
{
	targetTrans_ = bikeTrans;
}

Bomb* Helicopter::GetBomb(void)
{
	return bomb_;
}

void Helicopter::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case Helicopter::STATE::NONE:
		ChangeStateNone();
		break;
	case Helicopter::STATE::MOVE:
		ChangeStateMove();
		break;
	case Helicopter::STATE::ATTACK:
		ChangeStateAttack();
		break;
	case Helicopter::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void Helicopter::ChangeStateNone(void)
{
}

void Helicopter::ChangeStateMove(void)
{
}

void Helicopter::ChangeStateAttack(void)
{
}

void Helicopter::ChangeStateDead(void)
{
}

void Helicopter::UpdateNone(void)
{
}

void Helicopter::UpdateMove(void)
{

	// 移動処理
	ProcessMove();

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

}

void Helicopter::UpdateAttack(void)
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

}

void Helicopter::UpdateDead(void)
{
}


void Helicopter::DrawDebug(void)
{
	DrawFormatString(0, 100, 0xffffff, "copterPos : %f, %f, %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void Helicopter::ProcessMove(void)
{
	//羽情報
	rotor_->Update();
	rotor_->SetTransform(transform_);

	auto& ins = InputManager::GetInstance();

	// 移動量をゼロ
	movePow_ = MyUtility::VECTOR_ZERO;

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// 回転したい角度
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;

	VECTOR dir = MyUtility::VECTOR_ZERO;

	//バイクプレイヤーに合わせる(ステージ内にいるときのみ)
	if (!isTargetOutside_)
	{
		int playNum = gameScene_->GetPlayNum();
		if (playNum == 1)
		{
			transform_.pos.x = targetTrans_.pos.x;
		}
		else
		{
			float  rate = 0.015f;
			transform_.pos.x = MyUtility::Lerp(transform_.pos.x, targetTrans_.pos.x, rate);
		}
		
	}

	//前に進む
	VECTOR movePowF_ = VScale(transform_.GetForward(), speed_);

	if (!MyUtility::EqualsVZero(dir))
	{
		// 移動処理
		speed_ = SPEED_MOVE;
	}

	//前へ進むベクトルと横に曲がるベクトルを合成する
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, SPEED_MOVE), movePowF_);

	//バイクとの距離による処理
	BikeDisFunc();
}

void Helicopter::ProcessAttack(void)
{
	NormalAttack();
	LongAttack();
}

void Helicopter::ProcessDebug(void)
{
}

void Helicopter::NormalAttack(void)
{

	//爆弾の位置更新
	bomb_->SetHeliTrans(transform_);

	//ターゲットが場外にいたら動かさない(爆弾未設置状態のみ)
	if (isTargetOutside_ && bomb_->GetState() == Bomb::STATE::IDLE)
	{
		return;
	}

	//爆弾
	bomb_->Update();
}

void Helicopter::LongAttack(void)
{
}

void Helicopter::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Helicopter::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(-1.0f * (float)cameraRot.y + rotRad, MyUtility::AXIS_Z);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Helicopter::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	rotY_ = Quaternion::Slerp(
		rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Helicopter::CalcGravityPow(void)
{
	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Helicopter::BikeDisFunc(void)
{
	//バイクとヘリの距離をはかる
	VECTOR atkLinePos = VAdd(targetTrans_.pos, ATTACK_LINE_LOCAL_POS);
	VECTOR atkLineMaxPos = VAdd(targetTrans_.pos, ATTACK_LINE_MAX_LOCAL_POS);

	switch (state_)
	{
	case Helicopter::STATE::NONE:
		break;
	case Helicopter::STATE::MOVE:

		//ある程度距離が空いていたら攻撃状態に移行
		if (transform_.pos.z >= atkLinePos.z)
		{
			if (!(state_ == STATE::ATTACK))
			{
				speed_ = SPEED_MOVE;
				ChangeState(STATE::ATTACK);
			}

		}
		else
		{
			//攻撃状態に移行するためにスピードを上げる
			speed_ += SceneManager::GetInstance().GetDeltaTime();
		}
		break;
	case Helicopter::STATE::ATTACK:

		//バイクとの距離が離れすぎたらスピードを下げる
		if (transform_.pos.z >= atkLineMaxPos.z)
		{
			//スピードを下げる
			speed_ -= SceneManager::GetInstance().GetDeltaTime() * 2.0f;
		}
		else if (transform_.pos.z <= atkLinePos.z)
		{
			//下がりすぎないようにするための処理
			speed_ += SceneManager::GetInstance().GetDeltaTime() + (SPEED_MOVE - speed_);
		}
		break;
	case Helicopter::STATE::DEAD:
		break;
	case Helicopter::STATE::END:
		break;
	default:
		break;
	}
}

void Helicopter::InitLoad(void)
{
	//羽
	rotor_ = new Rotor();
	rotor_->Init();

	//爆弾
	bomb_ = new Bomb();
	bomb_->Init();

	//モデルの読み込み
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::HELICOPTER));
}

void Helicopter::InitPost(void)
{
	// 体力
	hp_ = MAX_HP;

	//スピード
	speed_ = SPEED_MOVE;

	// 初期状態
	ChangeState(STATE::MOVE);
}

