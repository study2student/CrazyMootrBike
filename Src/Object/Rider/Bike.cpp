#include <string>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../..//Utility/AsoUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "../Score.h"
#include "Player.h"
#include "../Bomb.h"
#include "FrontTyre.h"
#include "RearTyre.h"
#include "Bike.h"

Bike::Bike(float localpos, int playerID) : localPosX_(localpos), playerID_(playerID)
{
	//localPosX_ = localpos;

	animationController_ = nullptr;

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;
	stepJumpSecond_ = 0.0f;
	jumpSpeed_ = 1.0f;

	isAttack_ = false;
	isOutSide_ = false;

	// ブースト時の加算速度
	speedBoost_ = 0.0f;

	// ブースト使用間隔
	deleyBoost_ = 0;

	// 衝突チェック
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;

}

Bike::~Bike(void)
{

	StopEffekseer3DEffect(effectBoostPlayId_);
}

void Bike::Init(void)
{

	//タイヤ
	frontTyre_ = std::make_shared<FrontTyre>();
	frontTyre_->Init();
	frontTyre_->SetTransform(transform_);
	rearTyre_ = std::make_shared<RearTyre>();
	rearTyre_->Init();
	rearTyre_->SetTransform(transform_);

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BIKE));
	float scale = 1.3f;
	transform_.scl = { scale, scale, scale };
	VECTOR initPos = { 1270.0f,-260.0f,0.0f };
	transform_.pos = { 1270.0f + localPosX_, -260.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// モデルの基本設定
	transformPlayer_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	float pScale = 1.3f;
	transformPlayer_.scl = { pScale, pScale, pScale };
	//transformPlayer_.pos = { transform_.pos.x, transform_.pos.y , transform_.pos.z };
	transformPlayer_.pos = VAdd(transform_.pos, RELATIVE_P2B_POS);
	//transformPlayer_.quaRot = Quaternion();
	transformPlayer_.quaRot = transform_.quaRot;
	transformPlayer_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();
	transformPlayer_.Update();


	// アニメーションの設定
	InitAnimation();

	// エフェクト初期化
	InitEffect();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 130.0f, -30.0f });
	capsule_->SetLocalPosDown({ 0.0f, 130.0f, -120.0f });
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
	}

	frontTyre_->Update();
	frontTyre_->SetTransform(transform_);
	rearTyre_->Update();
	rearTyre_->SetTransform(transform_);

	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false);

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

	//タイヤ
	/*frontTyre_->Draw();
	rearTyre_->Draw();*/

	// 体力とかゲージとか
	DrawUI();

	// 丸影描画
	DrawShadow();

	//player_->Draw();

	//// デバッグ描画
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

void Bike::Jump(void)
{
	///*Quaternion q = Quaternion::AngleAxis(AsoUtility::Deg2RadD(-120.0), AsoUtility::AXIS_X);
	//Quaternion e= Quaternion::Normalize(q);*/

	//// 角度（ラジアン）を設定
	//float angle = DX_PI_F / 4.0f; // 45度

	//

	//// XZ平面内での斜め上のベクトルを作成
	//VECTOR jumpVec;
	//jumpVec.x = std::cos(angle);
	//jumpVec.y = std::sin(angle);
	//jumpVec.z = 0.0f;

	//// ベクトルの正規化
	//jumpVec = VNorm(jumpVec);
	//


	////jumpVec = Quaternion::PosAxis(q,transform_.pos);
	//transform_.pos = VAdd(transform_.pos, VScale(jumpVec,400.0f));
	//transform_.Update();




	// ジャンプ
	if ((isJump_ || IsEndLanding()))
	{

		if (!isJump_)
		{
			jumpSpeed_ = 1.0f;
		}

		isJump_ = true;

		// ジャンプの入力受付時間をヘラス
		stepJump_ += scnMng_.GetDeltaTime();
		jumpSpeed_ *= 1.98f;
		if (stepJump_ < TIME_JUMP_IN)
		{
			//jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP);
			//jumpPow_ = VScale(VAdd(AsoUtility::DIR_U, SceneManager::GetInstance().GetCamera()->GetForward()),POW_JUMP);
			jumpPow_ = VScale(VAdd(AsoUtility::DIR_U, transform_.GetForward()), jumpSpeed_ * POW_JUMP);
		}


	}

}

void Bike::Damage(int damage)
{
	hp_ -= damage;
}

const int& Bike::GetHP(void)
{
	return hp_;
}

const bool& Bike::GetIsOutSide(void)
{
	return isOutSide_;
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
	flipDir_ = dir;
	flipSpeed_ = 5.0f;
	ChangeState(STATE::FLIPED);
}

void Bike::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transformPlayer_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
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
	case Bike::STATE::NONE:
		ChangeStateNone();
		break;
	case Bike::STATE::PLAY:
		ChangeStatePlay();
		break;
	case Bike::STATE::FLIPED:
		ChangeStateFliped();
		break;
	}
}

void Bike::ChangeStateNone(void)
{
}

void Bike::ChangeStatePlay(void)
{
}

void Bike::ChangeStateFliped(void)
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

	// 回転させる
	transform_.quaRot = playerRotY_;
	transformPlayer_.quaRot = playerRotY_;

	//アニメーションループ
	animationController_->SetEndLoop(33.0f, 36.0f, 1.0f);

	//HP下限値
	if (hp_ <= MIN_HP)
	{
		hp_ = MIN_HP;
		state_ = STATE::DEAD;
	}
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

	// ジャンプ処理
	ProcessJump();

	// 移動方向に応じた回転
	Rotate();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	// 回転させる
	transform_.quaRot = playerRotY_;
}

void Bike::DrawUI(void)
{
	using ap = Application;
	int sc_x = ap::SCREEN_SIZE_X - 500;
	int sc_y = ap::SCREEN_SIZE_Y - 100;

	// HPバーの幅
	int HP_BAR_WIDTH = ap::SCREEN_SIZE_X - 10 - sc_x;
	// HPバーの高さ
	int HP_BAR_HEIGHT = ap::SCREEN_SIZE_Y - 10;
	// HPバーを描画
	DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HPバーの背景
	DrawBox(sc_x, sc_y, sc_x + (hp_ * HP_BAR_WIDTH) / MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HPバー

	// HPの黒枠
	DrawBoxAA(sc_x, sc_y,
		ap::SCREEN_SIZE_X - 10, HP_BAR_HEIGHT,
		0x000000, false, 13.0f);

	// HP
	DrawFormatString(0, 20, 0x00ff00, "HP : %d", hp_);
}

void Bike::DrawShadow(void)
{
}

void Bike::DrawDebug(void)
{
	capsule_->Draw();
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	// 攻撃が当たったか
	if (isAttack_ == true)
	{
		DrawString(0, 0, "Attack", 0x000000);
	}

	DrawFormatString(0, 40, 0x000000,
		"バイクの回転：%f,%f,%f",
		AsoUtility::Rad2DegD(transform_.rot.x),
		AsoUtility::Rad2DegD(transform_.quaRot.ToEuler().y),
		AsoUtility::Deg2RadF(transform_.quaRotLocal.ToEuler().z));

	DrawFormatString(0, 80, 0xffffff, "bikePos : %f, %f, %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Bike::ProcessMove(void)
{


	auto& ins = InputManager::GetInstance();

	// 移動量をゼロ
	movePow_ = AsoUtility::VECTOR_ZERO;


	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// 回転したい角度
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;


	std::array<PlayerInput, 4> playerInputs = { {
		{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
		{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
		{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
		{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
	} };

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	//前に進む
	VECTOR movePowF_ = VScale(cameraRot.GetForward(), SPEED_MOVE + speedBoost_);

	// プレイヤーごとの入力処理
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	if (padState & static_cast<int>(input.right)) {
		rotRadZ = AsoUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetRight();
	}

	if (padState & static_cast<int>(input.left)) {
		rotRadZ = AsoUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT))
	{
		rotRadZ = AsoUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetRight();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT))
	{
		rotRadZ = AsoUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN))
	{
		rotRad = AsoUtility::Deg2RadD(180.0f);
		dir = cameraRot.GetBack();
	}

	/*if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		rotRad = AsoUtility::Deg2RadD(0.0f);
		dir = cameraRot.GetForward();
	}

	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		rotRad = AsoUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetForward();
	}

	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		rotRad = AsoUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT))
	{
		rotRadZ = AsoUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetRight();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT))
	{
		rotRadZ = AsoUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN))
	{
		rotRad = AsoUtility::Deg2RadD(180.0f);
		dir = cameraRot.GetBack();
	}*/

	// カメラ方向に前進したい
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	rotRad = AsoUtility::Deg2RadD(0.0f);
	//	dir = cameraRot.GetForward();
	//}


	//もし上ってしまったら戻す
	//右側
	if (transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX)
	{
		dir = AsoUtility::DIR_L;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//場外にでている
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
		{
			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);
			// カメラ方向から後退したい
			if (ins.IsNew(KEY_INPUT_S))
			{
				rotRad = AsoUtility::Deg2RadD(180.0f);
				dir = cameraRot.GetBack();
			}

			// カメラ方向から右側へ移動したい
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = AsoUtility::Deg2RadD(-45.0f);
				dir = cameraRot.GetRight();
			}

			// カメラ方向から左側へ移動したい
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = AsoUtility::Deg2RadD(45.0f);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//左側
	if (transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX)
	{
		dir = AsoUtility::DIR_R;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//場外にでている
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX))
		{

			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);
			// カメラ方向から後退したい
			if (ins.IsNew(KEY_INPUT_S))
			{
				rotRad = AsoUtility::Deg2RadD(180.0f);
				dir = cameraRot.GetBack();
			}

			// カメラ方向から右側へ移動したい
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = AsoUtility::Deg2RadD(-45.0f);
				dir = cameraRot.GetRight();
			}

			// カメラ方向から左側へ移動したい
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = AsoUtility::Deg2RadD(45.0f);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//場外にでていない
	if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX) && !(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
	{
		isOutSide_ = false;
	}


	if (!AsoUtility::EqualsVZero(dir) /*&& (isJump_)*/) {

		// 移動処理
		speed_ = SPEED_MOVE;


		if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT || PAD_INPUT_RIGHT))
		{
			speed_ = SPEED_MOVE_X;
		}
		if (ins.IsNew(KEY_INPUT_RSHIFT))
		{
			speed_ = SPEED_RUN;
			if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D))
			{
				speed_ = SPEED_MOVE_X;
			}
		}

		/*moveDir_ = dir;
		movePow_ = VAdd(VScale(dir, speed_), movePowF_);*/


		// 回転処理
		SetGoalRotate(rotRad);
		SetGoalRotateZ(rotRadZ);

		if (!isJump_ && IsEndLanding())
		{
			////アニメーション
			//if (ins.IsNew(KEY_INPUT_RSHIFT))
			//{
			//	animationController_->Play((int)ANIM_TYPE::FAST_RUN);
			//}
			//else
			//{
			//	animationController_->Play((int)ANIM_TYPE::RUN);
			//}
		}

	}
	else
	{
		if (!isJump_ && IsEndLanding())
		{
		/*	animationController_->Play((int)ANIM_TYPE::IDLE);*/
		}

		//傾きっぱになるので角度リセットしておく
		rotRad = AsoUtility::Deg2RadD(0.0f);
		dir = cameraRot.GetForward();

		// 回転処理
		SetGoalRotateZ(rotRadZ);
	}


	//前へ進むベクトルと横に曲がるベクトルを合成する
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, speed_), movePowF_);

	// ソニックブームエフェクト
	float scale = 10.0f;
	SonicBoomEffect();
	effectSonicPlayId_ = PlayEffekseer3DEffect(effectSonicResId_);
	SetScalePlayingEffekseer3DEffect(effectSonicPlayId_, scale, scale, scale);

}

void Bike::ProcessJump(void)
{
}

void Bike::ProcessAttack(void)
{

	//// 攻撃更新
	//switch (attackState_)
	//{
	//case Bike::ATTACK_TYPE::NONE:
	//	break;
	//case Bike::ATTACK_TYPE::NORMAL:
	//	NormalAttack();
	//	break;
	//case Bike::ATTACK_TYPE::SPECIAL:
	//	SpecialAttack();
	//	break;
	//case Bike::ATTACK_TYPE::LONG:
	//	LongAttack();
	//	break;
	//default:
	//	break;
	//}

	NormalAttack();
	SpecialAttack();
	LongAttack();
}

void Bike::ProcessBoost(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsTrgDown(KEY_INPUT_E) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP)
	{
		//HPを消費して発動(ブーストで死なないように40以上の場合のみ)
		hp_ -= BOOST_USE_HP;

		SceneManager::GetInstance().GetCamera()->SetIsBoost(true);
		deleyBoost_ = DELEY_BOOST_MAX;
		speedBoost_ = 80.0f;
		effectBoostPlayId_ = PlayEffekseer3DEffect(effectBoostResId_);

		// 大きさ
		float boostSCALE = 30.0f;
		SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, boostSCALE, boostSCALE, boostSCALE);

		SyncBoostEffect();
	}
	else
	{
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

		SyncBoostEffect();
	}
}

void Bike::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_C))
	{
		hp_ -= 1;
	}
}

void Bike::NormalAttack(void)
{
	//auto& ins = InputManager::GetInstance();

	//if (ins.IsNew(KEY_INPUT_Z))
	//{
	//	attackState_ = ATTACK_TYPE::NORMAL;
	//	animationController_->Play((int)ANIM_TYPE::FALLING);

	//	isAttack_ = true;
	//}
	//else
	//{
	//	attackState_ = ATTACK_TYPE::NONE;
	//	isAttack_ = false;
	//}
}

void Bike::LongAttack(void)
{

}

void Bike::SpecialAttack(void)
{
	auto& ins = InputManager::GetInstance();

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	float rotRad = 0.0f;
	VECTOR dir = AsoUtility::VECTOR_ZERO;

	/*if (ins.IsNew(KEY_INPUT_Z))
	{
		attackState_ = ATTACK_TYPE::SPECIAL;
		animationController_->Play((int)ANIM_TYPE::FALLING);
		rotRad = AsoUtility::Deg2RadD(180.0f);

		VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
		Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

		goalQuaRot_ = axis;

		isAttack_ = true;
	}
	else
	{
		attackState_ = ATTACK_TYPE::NONE;
		isAttack_ = false;
	}*/
}

void Bike::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

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
	Quaternion axis = Quaternion::AngleAxis(-1.0f * (float)cameraRot.y + rotRad, AsoUtility::AXIS_Z);

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
	VECTOR dirGravity = AsoUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = AsoUtility::DIR_U;

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
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			//if (isJump_)
			//{
			//	// 着地モーション
			//	animationController_->Play(
			//		(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			//}

			isJump_ = false;

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
	VECTOR dirGravity = AsoUtility::DIR_D;

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

bool Bike::IsEndLanding(void)
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

void Bike::InitEffect(void)
{
	// ヒットエフェクト
	effectSonicResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SONICEFFECT).handleId_;

	//ブーストエフェクト
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST_EFFECT).handleId_;
}

void Bike::SonicBoomEffect(void)
{
	SetPosPlayingEffekseer3DEffect(effectSonicPlayId_, transform_.pos.x, transform_.pos.y + 200.0f, transform_.pos.z + 1000);
	SetRotationPlayingEffekseer3DEffect(effectSonicPlayId_, transform_.rot.x, transform_.rot.y + 180.0f, transform_.rot.z);
}

void Bike::SyncBoostEffect(void)
{
	VECTOR pos = transform_.pos;

	pos = Quaternion::PosAxis(transform_.quaRot, RELATIVE_P2EB_POS);
	pos = VAdd(transform_.pos, pos);
	SetPosPlayingEffekseer3DEffect(effectBoostPlayId_, pos.x, pos.y, pos.z);

	//角度の同期
	VECTOR angles = transform_.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(effectBoostPlayId_, angles.x, angles.y, angles.z);
}
