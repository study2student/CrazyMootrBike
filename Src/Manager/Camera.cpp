#include <math.h>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Transform.h"
#include "../Manager/SceneManager.h"
#include "Camera.h"

Camera::Camera(void)
{
	angles_ = VECTOR();
	cameraUp_ = VECTOR();
	mode_ = MODE::NONE;
	pos_ = AsoUtility::VECTOR_ZERO;
	targetPos_ = AsoUtility::VECTOR_ZERO;
	followTransform_ = nullptr;
	stepRotTime_ = 0.0f;
	stepMoveFront_ = 0.0f;
	isPause_ = false;
	isBoost_ = false;
	isCameraReset_ = false;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	ChangeMode(MODE::FIXED_POINT);

}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		// カメラの設定(位置と注視点による制御)
		SetCameraPositionAndTargetAndUpVec(
			pos_,
			targetPos_,
			cameraUp_
		);
		break;
	}

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();

}

void Camera::Draw(void)
{
}

void Camera::SetFollow(const Transform* follow)
{
	followTransform_ = follow;
}

void Camera::SetIsPause(bool isPause)
{
	isPause_ = isPause;
}

void Camera::SetIsCameraReset(bool isCameraReset)
{
	isCameraReset_ = isCameraReset;
}

void Camera::SetIsBoost(bool isBoost)
{
	isBoost_ = isBoost;
	if (isBoost)
	{
		boostLocalPos_ = BOOST_LOCAL_F2C_MAX_POS;

	}
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

VECTOR Camera::GetAngles(void) const
{
	return angles_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

Quaternion Camera::GetQuaRot(void) const
{
	return rot_;
}

Quaternion Camera::GetQuaRotOutX(void) const
{
	return rotOutX_;
}

VECTOR Camera::GetForward(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	//// 同期先の位置
	//VECTOR pos = followTransform_->pos;

	//// 重力の方向制御に従う
	//// 正面から設定されたY軸分、回転させる
	//rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	//// 正面から設定されたX軸分、回転させる
	//rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	//VECTOR localPos;

	//// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	//localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	//targetPos_ = VAdd(pos, localPos);

	//localPos = rot_.PosAxis(LOCAL_F2C_POS);
	//pos_ = VAdd(pos, localPos);

	//// カメラの上方向
	//cameraUp_ = AsoUtility::DIR_U;





	// 同期先の位置
	VECTOR pos = followTransform_->pos;

	// 重力の方向制御に従う
	// 正面から設定されたY軸分、回転させる
	rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// 正面から設定されたX軸分、回転させる
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// 注視点(通常重力でいうところのY値を追従対象と同じにする)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// カメラ位置
	//ブースト使用時にカメラを動かす
	if (isBoost_)
	{


		velocity_ += ADD_CAMERA_SPEED;
		if (velocity_ >= CAMERA_SPEED_MAX)
		{
			velocity_ = CAMERA_SPEED_MAX;
		}

		boostLocalPos_.z += velocity_;
		if (boostLocalPos_.z >= LOCAL_F2C_POS.z)
		{
			boostLocalPos_.z = LOCAL_F2C_POS.z;
		}

		localPos = rot_.PosAxis(boostLocalPos_);
	}
	else
	{
		localPos = rot_.PosAxis(LOCAL_F2C_POS);
		velocity_ = 0.0f;

	}
	//localPos = rot_.PosAxis(LOCAL_F2C_POS);
	pos_ = VAdd(pos, localPos);

	// カメラの上方向
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::ProcessRot(void)
{
	////ポーズ中は回転させない
	//if (isPause_)
	//{
	//	return;
	//}


	auto& ins = InputManager::GetInstance();

	float movePow = 5.0f;

	//ボタンが押されたか
	bool isHitKey = false;

	if (!isCameraReset_)
	{
		// カメラ回転
		if (ins.IsNew(KEY_INPUT_RIGHT))
		{
			// 右回転
			angles_.y += AsoUtility::Deg2RadF(1.0f);
			isHitKey = true;
		}
		if (ins.IsNew(KEY_INPUT_LEFT))
		{
			// 左回転
			angles_.y += AsoUtility::Deg2RadF(-1.0f);
			isHitKey = true;
		}

		// 上回転
		if (ins.IsNew(KEY_INPUT_UP))
		{
			angles_.x += AsoUtility::Deg2RadF(1.0f);
			if (angles_.x > LIMIT_X_UP_RAD)
			{
				angles_.x = LIMIT_X_UP_RAD;
			}
		}

		// 下回転
		if (ins.IsNew(KEY_INPUT_DOWN))
		{
			angles_.x += AsoUtility::Deg2RadF(-1.0f);
			if (angles_.x < -LIMIT_X_DW_RAD)
			{
				angles_.x = -LIMIT_X_DW_RAD;
			}
		}
	}

	//回転処理をしてない時カメラを徐々に前に向かせる
	if (!isHitKey || isCameraReset_)
	{
		Quaternion goalQuaRot_ = Quaternion::Euler({ 0.0f,0.0f,0.0f });

		// 現在設定されている回転との角度差を取る
		float angleDiff = Quaternion::Angle(rot_, goalQuaRot_);

		// しきい値
		if (angleDiff > 0.1)
		{
			stepRotTime_ = TIME_ROT;
		}

		stepRotTime_ -= SceneManager::GetInstance().GetDeltaTime();
		rot_ = Quaternion::Slerp(rot_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

		VECTOR r = rot_.ToEuler();
		angles_ = r;

	}


}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//最初の固定カメラ
	//プレイヤー座標
	static constexpr VECTOR pPos_ = { -80.0f, -10.0f, -380.0f };

	pos_ = pPos_;
	angles_ = { AsoUtility::Deg2RadF(0.0f),  AsoUtility::Deg2RadF(-10.0f), 0.0f };

	//カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

}

void Camera::SetBeforeDrawFollow(void)
{

	// カメラ操作
	ProcessRot();

	// 追従対象との相対位置を同期
	SyncFollow();

}

void Camera::SetBeforeDrawSelfShot(void)
{
}
