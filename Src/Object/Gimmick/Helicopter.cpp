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

#pragma region �萔�錾

// �ʏ�X�s�[�h
const float SPEED_MOVE = 120.0f;

// ��]�����܂ł̎���
const float TIME_ROT = 1.0f;

//�U����Ԃɂ��邽�߂̃v���C���[����w���܂ł̑��΍��W
const VECTOR ATTACK_LINE_LOCAL_POS = { 0.0f,0.0f,5000.0f };

//�U����Ԃ̍ő�͈͂���邽�߂̃v���C���[����w���܂ł̑��΍��W
const VECTOR ATTACK_LINE_MAX_LOCAL_POS = { 0.0f,0.0f,10000.0f };

// HP�̍ő�l
const int MAX_HP = 100;

//�w���̑傫��
const float SCL = 5.0f;

// �������W
const VECTOR INIT_POS = { 1670.0f, 500.0f, 0.0f };

// �������[�J��Y��]
const float INIT_LOCAL_ROT_Y = 180.0f;

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 190.0f, -60.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 150.0f, -60.0f };

//�J�v�Z�����a
const float CAPSULE_RADIUS = 135.0f;

//��]�������l
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
	movePow_({}),
	movedPos_({}),
	rotY_(Quaternion()),
	goalQuaRot_(Quaternion()),
	stepRotTime_(0.0f),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	hp_(-1),
	isAttack_(false)
{
}

Helicopter::~Helicopter(void)
{
	delete rotor_;
}

void Helicopter::Init(void)
{
	//�H
	rotor_ = new Rotor();
	rotor_->Init();

	//���e
	//bomb_ = std::make_shared<Bomb>();
	bomb_ = new Bomb();
	bomb_->Init();

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HELICOPTER));
	transform_.scl = { SCL, SCL, SCL };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y), 0.0f });

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

	// �̗�
	hp_ = MAX_HP;

	//�X�s�[�h
	speed_ = SPEED_MOVE;

	// �������
	ChangeState(STATE::MOVE);

}

void Helicopter::Update(void)
{
	// �X�V�X�e�b�v
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

	// ���f������X�V
	transform_.Update();
}

void Helicopter::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	//�H
	rotor_->Draw();

	//���e
	bomb_->Draw();
}

void Helicopter::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Helicopter::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Helicopter::GetCapsule(void) const
{
	return capsule_;
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
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
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

	// �ړ�����
	ProcessMove();

	// �f�o�b�O�p
	ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = rotY_;

}

void Helicopter::UpdateAttack(void)
{

	// �ړ�����
	ProcessMove();

	// �U������
	ProcessAttack();

	// �f�o�b�O�p
	ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
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
	//�H���
	rotor_->Update();
	rotor_->SetTransform(transform_);

	auto& ins = InputManager::GetInstance();

	// �ړ��ʂ��[��
	movePow_ = MyUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;

	VECTOR dir = MyUtility::VECTOR_ZERO;

	//�o�C�N�v���C���[�ɍ��킹��(�X�e�[�W���ɂ���Ƃ��̂�)
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

	//�O�ɐi��
	VECTOR movePowF_ = VScale(transform_.GetForward(), speed_);

	if (!MyUtility::EqualsVZero(dir))
	{
		// �ړ�����
		speed_ = SPEED_MOVE;
	}

	//�O�֐i�ރx�N�g���Ɖ��ɋȂ���x�N�g������������
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, SPEED_MOVE), movePowF_);

	//�o�C�N�Ƃ̋����ɂ�鏈��
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

	//���e�̈ʒu�X�V
	bomb_->SetHeliTrans(transform_);

	//�^�[�Q�b�g����O�ɂ����瓮�����Ȃ�(���e���ݒu��Ԃ̂�)
	if (isTargetOutside_ && bomb_->GetState() == Bomb::STATE::IDLE)
	{
		return;
	}

	//���e
	bomb_->Update();
}

void Helicopter::LongAttack(void)
{
}

void Helicopter::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
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

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > ROTATE_ANGLE_DIFF_MIN)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Helicopter::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	rotY_ = Quaternion::Slerp(
		rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Helicopter::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �ړ�
	transform_.pos = movedPos_;
}

void Helicopter::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto& c : colliders_)
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

void Helicopter::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Helicopter::BikeDisFunc(void)
{
	//�o�C�N�ƃw���̋������͂���
	VECTOR atkLinePos = VAdd(targetTrans_.pos, ATTACK_LINE_LOCAL_POS);
	VECTOR atkLineMaxPos = VAdd(targetTrans_.pos, ATTACK_LINE_MAX_LOCAL_POS);

	switch (state_)
	{
	case Helicopter::STATE::NONE:
		break;
	case Helicopter::STATE::MOVE:

		//������x�������󂢂Ă�����U����ԂɈڍs
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
			//�U����ԂɈڍs���邽�߂ɃX�s�[�h���グ��
			speed_ += SceneManager::GetInstance().GetDeltaTime();
		}
		break;
	case Helicopter::STATE::ATTACK:

		//�o�C�N�Ƃ̋��������ꂷ������X�s�[�h��������
		if (transform_.pos.z >= atkLineMaxPos.z)
		{
			//�X�s�[�h��������
			speed_ -= SceneManager::GetInstance().GetDeltaTime() * 2.0f;
		}
		else if (transform_.pos.z <= atkLinePos.z)
		{
			//�����肷���Ȃ��悤�ɂ��邽�߂̏���
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

