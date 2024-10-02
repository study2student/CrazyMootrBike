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

//�H�̑傫��
const float SCL = 5.0f;

//�������W
const VECTOR INIT_POS = { 1670.0f, 500.0f, 0.0f };

// �������[�J��Y��]
const float INIT_LOCAL_ROT_Y = 180.0f;

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

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HELICOPTER_ROTOR));
	transform_.scl = { SCL, SCL, SCL };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y), 0.0f });

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

	// �̗�
	hp_ = MAX_HP;

	// �������
	ChangeState(STATE::PLAY);

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


void Rotor::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �ړ�
	transform_.pos = movedPos_;
}

void Rotor::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
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
					// �J�v�Z�����ړ�������
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;

			}

		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);

	}

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




