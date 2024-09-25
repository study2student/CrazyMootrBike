#include <string>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../..//Utility/MyUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Stage/Planet.h"
#include "../Score.h"
#include "Bike.h"

#pragma region 変数宣言
	// プレイヤーのモデルの大きさ
	const float PLAYER_SCALE = 1.3f;

	// 初期位置
	const VECTOR INIT_POS = { 1270.0f , -260.0f ,0.0f };
	const VECTOR INIT_LOCAL_POS = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// 当たり判定用コライダーの上座標
	const VECTOR COLLIDER_POS_TOP = { 0.0f, 130.0f, 0.0f };
	// 当たり判定用コライダーの下座標
	const VECTOR COLLIDER_POS_DOWN = { 0.0f, 130.0f, -150.0f };

	// スピード
	const float SPEED_MOVE = 100.0f;

	// プレイヤーの傾き
	const float SLOPE = 45.0f;

	// ブースト使用時の加速速度
	const float ADD_SPEED_BOOST = 50.0f;
	// 次ブースト使えるようになるまでのカウント
	const float DELEY_BOOST_MAX = 220.0f;
	// ブースト発動させるためのHP消費量
	const int BOOST_USE_HP = 40;

	// 横移動のスピード
	const float SPEED_MOVE_X = 18.0f;

	// 回転完了までの時間
	const float TIME_ROT = 1.0f;

	// 横揺れマックス時間
	const float SWAY_CURRENT_MAX_TIME = 70.0f;

#pragma endregion

Bike::Bike(float localpos, int playerID) :
	
	animationController_(nullptr),
	state_(STATE::NONE),
	isOutSide_(false),
	playerID_(playerID),
	moveSpeed_(0.0f),
	moveDir_({}),
	movePow_({}),
	movedPos_({}),
	localPosX_(localpos),
	playerRotY_({}),
	goalQuaRot_({}),
	stepRotTime_(0.0f),
	currentTime_(0.0f),
	jumpPow_({}),
	stepJump_(0.0f),
	stepJumpSecond_(0.0f),
	colliders_(0),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	hp_(0),
	score_(0),
	flipSpeed_(0.0f),
	flipDir_({}),
	speedBoost_(0.0f),
	deleyBoost_(0.0f),
	isBoost_(false),
	isGoal_(false),
	effectBoostResId_(-1),
	effectBoostPlayId_(-1)
{
}

Bike::~Bike(void)
{
	StopEffekseer3DEffect(effectBoostPlayId_);
}

void Bike::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BIKE));
	transform_.scl = { PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE };
	transform_.pos = { INIT_POS.x + localPosX_, INIT_POS.y, INIT_POS.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(INIT_LOCAL_POS);

	// モデルの基本設定
	transformPlayer_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transformPlayer_.scl = { PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE };
	transformPlayer_.pos = VAdd(transform_.pos, RELATIVE_P2B_POS);
	//transformPlayer_.quaRot = Quaternion();
	transformPlayer_.quaRot = transform_.quaRot;
	transformPlayer_.quaRotLocal =
		Quaternion::Euler(INIT_LOCAL_POS);
	transform_.Update();
	transformPlayer_.Update();

	// アニメーションの設定
	InitAnimation();

	// エフェクト初期化
	InitEffect();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ COLLIDER_POS_TOP });
	capsule_->SetLocalPosDown({ COLLIDER_POS_DOWN });
	capsule_->SetRadius(RADIUS);

	// 体力
	hp_ = MAX_HP;

	// スコア
	score_ = std::make_shared<Score>();
	score_->Init();

	// 初期状態
	ChangeState(STATE::PLAY);

}

void Bike::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case Bike::STATE::NONE:
		UpdateNone();
		break;
	case Bike::STATE::PLAY:
		UpdatePlay();
		break;
	case Bike::STATE::FLIPED:
		UpdateFliped();
		break;
	case STATE::CRASH:
		UpdateCrash();
		break;
	case Bike::STATE::DEAD:
		UpdateDead();
		break;
	}

	// モデル制御更新
	transform_.Update();
	transformPlayer_.Update();
	// アニメーション再生
	animationController_->Update();
}

void Bike::Draw(void)
{
	// モデルの描画
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(transformPlayer_.modelId);

	// デバッグ描画
	//DrawDebug();
}

void Bike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Bike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Bike::GetCapsule(void) const
{
	return capsule_;
}

void Bike::Damage(int damage)
{
	hp_ -= damage;
	ChangeState(STATE::CRASH);

	//HP下限値
	if (hp_ <= MIN_HP)
	{
		hp_ = MIN_HP;
 		ChangeState(STATE::DEAD);
	}
}

const int& Bike::GetHP(void)
{
	return hp_;
}

const bool& Bike::GetIsOutSide(void)
{
	return isOutSide_;
}

void Bike::SetIsGoal(bool isGoal)
{
	isGoal_ = isGoal;
}

const bool& Bike::GetIsGoal(void)
{
	return isGoal_;
}

void Bike::AddScore(int score)
{
	score_->AddScore(score);
}

const int Bike::GetScore() const
{
	return score_->GetScore();
}

const int Bike::GetPlayerID(void) const
{
	return playerID_;
}

void Bike::Flip(VECTOR dir)
{
	if (state_ == STATE::DEAD || state_ == STATE::CRASH)
	{
		return;
	}

	flipDir_ = dir;
	flipSpeed_ = 5.0f;
	ChangeState(STATE::FLIPED);
}

const bool Bike::IsBoost(void)
{
	return isBoost_;
}

void Bike::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transformPlayer_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::SIT, path + "Sit.mv1", 1.0f);
	animationController_->Play((int)ANIM_TYPE::SIT, true, 33.0f, 36.0f);
}

void Bike::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case STATE::NONE:
		ChangeStateNone();
		break;
	case STATE::PLAY:
		ChangeStatePlay();
		break;
	case STATE::FLIPED:
		ChangeStateFliped();
		break;
	case STATE::CRASH:
		ChangeStateCrash();
		break;
	case Bike::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void Bike::ChangeStateNone(void)
{
}

void Bike::ChangeStatePlay(void)
{
	currentTime_ = 0.0f;
}

void Bike::ChangeStateFliped(void)
{
}

void Bike::ChangeStateCrash(void)
{
	moveSpeed_ = 10.0f;
}

void Bike::ChangeStateDead(void)
{
}

void Bike::UpdateNone(void)
{
}

void Bike::UpdatePlay(void)
{

	// 移動処理
	ProcessMove();

	//ブースト
	ProcessBoost();

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = playerRotY_;
	transformPlayer_.quaRot = playerRotY_;

	//アニメーションループ
	animationController_->SetEndLoop(33.0f, 36.0f, 1.0f);
}

void Bike::UpdateFliped(void)
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

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = playerRotY_;
}

void Bike::UpdateCrash(void)
{

	if (currentTime_ >= SWAY_CURRENT_MAX_TIME)
	{
		transform_.quaRotLocal =Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
		transformPlayer_.quaRotLocal =Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
		ChangeState(STATE::PLAY);
	}
	else
	{
		// Y軸起点に揺れを追加
		float swayAngle = MyUtility::Deg2RadF(45.0f);  // 揺れの角度の範囲
		float swaySpeed = 0.1f;  // 揺れの速度
		float sway = swayAngle * sinf(swaySpeed * currentTime_);

		// Y軸周りの揺れを設定
		transform_.quaRotLocal = Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f) + sway, 0.0f });
		transformPlayer_.quaRotLocal = Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f) + sway, 0.0f });

		currentTime_ += 2.0f;
	}

	// 他の更新処理
	transform_.Update();
}

void Bike::UpdateDead(void)
{
}

void Bike::DrawDebug(void)
{
	capsule_->Draw();
	//DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	DrawFormatString(0, 40, 0xffffff,
		"バイクの回転：%f,%f,%f",
		MyUtility::Rad2DegF(transform_.rot.x),
		MyUtility::Rad2DegF(transform_.quaRot.ToEuler().y),
		MyUtility::Deg2RadF(transform_.quaRotLocal.ToEuler().z));

	DrawFormatString(0, 80, 0xffffff, "bikePos : %f, %f, %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Bike::ProcessMove(void)
{
	if (state_ == STATE::DEAD)
	{
		return;
	}

	auto& ins = InputManager::GetInstance();

	// 移動量をゼロ
	movePow_ = MyUtility::VECTOR_ZERO;


	std::array<PlayerInput, 4> playerInputs = { {
	{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
	{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
	{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
	{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
} };

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// 回転したい角度
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;

	VECTOR dir = MyUtility::VECTOR_ZERO;

	//前に進む
	VECTOR movePowF_ = VScale(cameraRot.GetForward(), SPEED_MOVE + speedBoost_);

	// プレイヤーごとの入力処理
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	//右
	if (padState & static_cast<int>(input.right)) {
		rotRadZ = MyUtility::Deg2RadF(-SLOPE);
		dir = cameraRot.GetRight();
	}

	//左
	if (padState & static_cast<int>(input.left)) {
		rotRadZ = MyUtility::Deg2RadF(SLOPE);
		dir = cameraRot.GetLeft();
	}

	//もし上ってしまったら戻す
	//右側
	if (transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX)
	{
		dir = MyUtility::DIR_L;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//場外にでている
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
		{
			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);

			// カメラ方向から右側へ移動したい
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
				dir = cameraRot.GetRight();
			}

			// カメラ方向から左側へ移動したい
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//左側
	if (transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX)
	{
		dir = MyUtility::DIR_R;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//場外にでている
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX))
		{

			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);

			// カメラ方向から右側へ移動したい
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadF(-SLOPE);
				dir = cameraRot.GetRight();
			}

			// カメラ方向から左側へ移動したい
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//場外にでていない
	if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX) && !(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
	{
		isOutSide_ = false;
	}

	if (!MyUtility::EqualsVZero(dir) /*&& (isJump_)*/) {

		// 移動処理
		moveSpeed_ = SPEED_MOVE;


		if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) &  PAD_INPUT_LEFT || PAD_INPUT_RIGHT))
		{
			moveSpeed_ = SPEED_MOVE_X;
		}

		// 回転処理
		SetGoalRotate(rotRad);
		SetGoalRotateZ(rotRadZ);

	}
	else
	{
		//傾きっぱになるので角度リセットしておく
		rotRad = MyUtility::Deg2RadF(0.0f);
		dir = cameraRot.GetForward();

		// 回転処理
		SetGoalRotateZ(rotRadZ);
	}

	//前へ進むベクトルと横に曲がるベクトルを合成する
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, moveSpeed_), movePowF_);

}

bool Bike::IsBoostPush(void)
{
	auto& ins = InputManager::GetInstance();

	std::array<PlayerInput, 4> playerInputs = { {
	{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
	{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
	{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
	{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
} };

	// プレイヤーごとの入力処理
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	return ins.IsTrgDown(KEY_INPUT_E) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP || padState & static_cast<int>(input.action) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP;
}


void Bike::ProcessBoost(void)
{

	//Eキーもしくはpadのアクションボタンを押すと、ブースト発動
	if (IsBoostPush())
	{
		// ブースト判定をtrueに
		isBoost_ = true;

		//HPを消費して発動(ブーストで死なないように40以上の場合のみ)
		hp_ -= BOOST_USE_HP;

		//音を再生
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_BOOST).handleId_, DX_PLAYTYPE_BACK, true);

		SceneManager::GetInstance().GetCamera()->SetIsBoost(true);
		deleyBoost_ = DELEY_BOOST_MAX;
		speedBoost_ = ADD_SPEED_BOOST;
		effectBoostPlayId_ = PlayEffekseer3DEffect(effectBoostResId_);

		// 大きさ
		float boostSCALE = 30.0f;
		SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, boostSCALE, boostSCALE, boostSCALE);
		SyncBoostEffect(transform_);
	}
	else
	{
		isBoost_ = false;

		deleyBoost_--;
		if (deleyBoost_ <= 0)
		{
			deleyBoost_ = 0.0f;
		}
		speedBoost_ -= 0.340f;
		if (speedBoost_ <= 0)
		{
			speedBoost_ = 0.0f;
			SceneManager::GetInstance().GetCamera()->SetIsBoost(false);
		}

		SyncBoostEffect(transform_);
	}
}

void Bike::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Bike::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(-1.0f * (float)cameraRot.z + rotRad, MyUtility::AXIS_Z);

	// 現在設定されている回転との角度差を取る
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Bike::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// 回転の球面補間
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Bike::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	VECTOR movedPPos = VAdd(VAdd(transform_.pos, RELATIVE_P2B_POS), movePow_);
	//movedPos_ = VAdd(transformPlayer_.pos, movmoePow_);

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
	transformPlayer_.pos = movedPPos;

}

void Bike::CollisionGravity(void)
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
			stepJump_ = 0.0f;

		}

	}
}

void Bike::CollisionCapsule(void)
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

void Bike::CalcGravityPow(void)
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

void Bike::InitEffect(void)
{
	//ブーストエフェクト
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST_EFFECT).handleId_;
}

void Bike::SyncBoostEffect(Transform player)
{
	VECTOR pos = player.pos;

	pos = Quaternion::PosAxis(transform_.quaRot, RELATIVE_P2EB_POS);
	pos = VAdd(player.pos, pos);
	SetPosPlayingEffekseer3DEffect(effectBoostPlayId_, pos.x, pos.y, pos.z);

	//角度の同期
	//VECTOR angles = transform_.quaRot.ToEuler();
	VECTOR angles = player.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(effectBoostPlayId_, angles.x, angles.y, angles.z);

	// 大きさ
	float boostSCALE = 30.0f;
	SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, boostSCALE, boostSCALE, boostSCALE);
}
