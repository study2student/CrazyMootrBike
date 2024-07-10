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

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		// �J�����̐ݒ�(�ʒu�ƒ����_�ɂ�鐧��)
		SetCameraPositionAndTargetAndUpVec(
			pos_,
			targetPos_,
			cameraUp_
		);
		break;
	}

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
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

	// �J�����̏����ݒ�
	SetDefault();

	// �J�������[�h�̕ύX
	mode_ = mode;

	// �ύX���̏���������
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

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;

	// �����_
	targetPos_ = AsoUtility::VECTOR_ZERO;

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

	angles_.x = AsoUtility::Deg2RadF(30.0f);
	angles_.y = 0.0f;
	angles_.z = 0.0f;

	rot_ = Quaternion();

}

void Camera::SyncFollow(void)
{

	//// ������̈ʒu
	//VECTOR pos = followTransform_->pos;

	//// �d�͂̕�������ɏ]��
	//// ���ʂ���ݒ肳�ꂽY�����A��]������
	//rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	//// ���ʂ���ݒ肳�ꂽX�����A��]������
	//rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	//VECTOR localPos;

	//// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	//localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	//targetPos_ = VAdd(pos, localPos);

	//localPos = rot_.PosAxis(LOCAL_F2C_POS);
	//pos_ = VAdd(pos, localPos);

	//// �J�����̏����
	//cameraUp_ = AsoUtility::DIR_U;





	// ������̈ʒu
	VECTOR pos = followTransform_->pos;

	// �d�͂̕�������ɏ]��
	// ���ʂ���ݒ肳�ꂽY�����A��]������
	rotOutX_ = Quaternion::AngleAxis(angles_.y, AsoUtility::AXIS_Y);

	// ���ʂ���ݒ肳�ꂽX�����A��]������
	rot_ = rotOutX_.Mult(Quaternion::AngleAxis(angles_.x, AsoUtility::AXIS_X));

	VECTOR localPos;

	// �����_(�ʏ�d�͂ł����Ƃ����Y�l��Ǐ]�ΏۂƓ����ɂ���)
	localPos = rotOutX_.PosAxis(LOCAL_F2T_POS);
	targetPos_ = VAdd(pos, localPos);

	// �J�����ʒu
	//�u�[�X�g�g�p���ɃJ�����𓮂���
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

	// �J�����̏����
	cameraUp_ = AsoUtility::DIR_U;

}

void Camera::ProcessRot(void)
{
	////�|�[�Y���͉�]�����Ȃ�
	//if (isPause_)
	//{
	//	return;
	//}


	auto& ins = InputManager::GetInstance();

	float movePow = 5.0f;

	//�{�^���������ꂽ��
	bool isHitKey = false;

	if (!isCameraReset_)
	{
		// �J������]
		if (ins.IsNew(KEY_INPUT_RIGHT))
		{
			// �E��]
			angles_.y += AsoUtility::Deg2RadF(1.0f);
			isHitKey = true;
		}
		if (ins.IsNew(KEY_INPUT_LEFT))
		{
			// ����]
			angles_.y += AsoUtility::Deg2RadF(-1.0f);
			isHitKey = true;
		}

		// ���]
		if (ins.IsNew(KEY_INPUT_UP))
		{
			angles_.x += AsoUtility::Deg2RadF(1.0f);
			if (angles_.x > LIMIT_X_UP_RAD)
			{
				angles_.x = LIMIT_X_UP_RAD;
			}
		}

		// ����]
		if (ins.IsNew(KEY_INPUT_DOWN))
		{
			angles_.x += AsoUtility::Deg2RadF(-1.0f);
			if (angles_.x < -LIMIT_X_DW_RAD)
			{
				angles_.x = -LIMIT_X_DW_RAD;
			}
		}
	}

	//��]���������ĂȂ����J���������X�ɑO�Ɍ�������
	if (!isHitKey || isCameraReset_)
	{
		Quaternion goalQuaRot_ = Quaternion::Euler({ 0.0f,0.0f,0.0f });

		// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
		float angleDiff = Quaternion::Angle(rot_, goalQuaRot_);

		// �������l
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
	//�ŏ��̌Œ�J����
	//�v���C���[���W
	static constexpr VECTOR pPos_ = { -80.0f, -10.0f, -380.0f };

	pos_ = pPos_;
	angles_ = { AsoUtility::Deg2RadF(0.0f),  AsoUtility::Deg2RadF(-10.0f), 0.0f };

	//�J�����̐ݒ�(�ʒu�Ɗp�x�ɂ�鐧��)
	SetCameraPositionAndAngle(
		pos_,
		angles_.x,
		angles_.y,
		angles_.z
	);

}

void Camera::SetBeforeDrawFollow(void)
{

	// �J��������
	ProcessRot();

	// �Ǐ]�ΏۂƂ̑��Έʒu�𓯊�
	SyncFollow();

}

void Camera::SetBeforeDrawSelfShot(void)
{
}
