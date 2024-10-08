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

#pragma region �萔�錾

// ��]�����܂ł̎���
const float TIME_ROT = 1.0f;

//��]���x
const float SPEED_ROT = 20.0f;

// HP�̍ő�l
const int MAX_HP = 100;

// �������W
const VECTOR INIT_POS = { 1670.0f, 500.0f, 0.0f };

// �����傫��
const VECTOR INIT_SCL = { 5.0f,5.0f,5.0f };

// ������]
const VECTOR INIT_ROT = { 0.0f, 0.0f, 0.0f };

// �������[�J����]
const VECTOR INIT_LOCAL_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 190.0f, -60.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 150.0f, -60.0f };

//�J�v�Z�����a
const float CAPSULE_RADIUS = 135.0f;

//���΍��W
const VECTOR LOCAL_POS = { 0.0f,250.0f,0.0f };

//��]�������l
const float ROTATE_ANGLE_DIFF_MIN = 0.1f;

#pragma endregion


Rotor::Rotor()
	:
	transformParent_(Transform()),
	state_(STATE::NONE),
	hp_(-1)
{
	//�ʒu��]�傫��
	initScl_ = INIT_SCL;
	initRotEuler_ = INIT_ROT;
	initLocalRotEuler_ = INIT_LOCAL_ROT;
	initPos_ = INIT_POS;

	//�J�v�Z��
	capsulePosTop_ = CAPSULE_LOCAL_POS_TOP;
	capsulePosDown_ = CAPSULE_LOCAL_POS_DOWN;
	capsuleRadius_ = CAPSULE_RADIUS;

	//��]
	rotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;
}

Rotor::~Rotor(void)
{
}

void Rotor::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Rotor::STATE::NONE:
		UpdateNone();
		break;
	case Rotor::STATE::PLAY:
		UpdatePlay();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void Rotor::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
	DrawDebug();
}

void Rotor::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void Rotor::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
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

	// �ړ�����
	ProcessMove();

	// �f�o�b�O�p
	ProcessDebug();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = rotY_;
	transform_.Update();
}

void Rotor::DrawDebug(void)
{
}

void Rotor::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//�e�w���̈ʒu�ɍ��킹��
	transform_.pos = transformParent_.pos;
	transform_.pos = VAdd(transform_.pos, LOCAL_POS);

	//�H�̉�]
	// �f�O���[���烉�W�A��(�ϊ�)
	float rad = MyUtility::Deg2RadF(SPEED_ROT);

	// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_Y);

	//�N�H�[�^�j�I��(��])�̍���
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

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
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

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rotor::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Rotor::InitLoad(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::HELICOPTER_ROTOR));
}

void Rotor::InitPost(void)
{
	// �̗�
	hp_ = MAX_HP;

	// �������
	ChangeState(STATE::PLAY);
}




