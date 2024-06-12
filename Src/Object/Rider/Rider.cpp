#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Planet.h"
#include "Player.h"
#include "Bike.h"
#include "Rider.h"

Rider::Rider(void)
{
	bike_ = nullptr;
	player_ = nullptr;

	capsule_ = nullptr;

	state_ = STATE::NONE;

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

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;
}

Rider::~Rider(void)
{
	delete capsule_;
	delete player_;
	delete bike_;
}

void Rider::Init(void)
{
	// �v���C���[
	player_ = new Player();
	player_->Init();

	// �o�C�N
	bike_ = new Bike(2000);
	bike_->Init();

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	capsule_->SetRadius(30.0f);

	// �������
	ChangeState(STATE::PLAY);
}

void Rider::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Rider::STATE::NONE:
		UpdateNone();
		break;
	case Rider::STATE::PLAY:
		UpdatePlay();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void Rider::Draw(void)
{
	DrawPlay();
	DrawDebug();
}

void Rider::Release(void)
{
}

void Rider::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void Rider::ClearCollider(void)
{
	colliders_.clear();
}

Rider::STATE& Rider::GetState(void)
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return state_;
}

const Capsule* Rider::GetCapsule(void) const
{
	return capsule_;
}

void Rider::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case Rider::STATE::NONE:
		ChangeStateNone();
		break;
	case Rider::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void Rider::ChangeStateNone(void)
{
}

void Rider::ChangeStatePlay(void)
{
}

void Rider::UpdateNone(void)
{
}

void Rider::UpdatePlay(void)
{
	player_->Update();
	bike_->Update();

	// �ړ�����
	ProcessMove();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void Rider::UpdateDestroy(void)
{
}

void Rider::DrawPlay(void)
{
	player_->Draw();
	bike_->Draw();
}

void Rider::DrawDestroy(void)
{
}

void Rider::DrawDebug(void)
{
	capsule_->Draw();
}

void Rider::SyncParent(Transform& transform, VECTOR addAxis)
{
}

void Rider::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// �ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	// �J���������ɑO�i������
	if (ins.IsNew(KEY_INPUT_W))
	{
		rotRad = AsoUtility::Deg2RadD(0.0);
		dir = cameraRot.GetForward();
	}

	// �J�������������ނ�����
	if (ins.IsNew(KEY_INPUT_S))
	{
		rotRad = AsoUtility::Deg2RadD(180.0);
		dir = cameraRot.GetBack();
	}

	// �J������������E���ֈړ�������
	if (ins.IsNew(KEY_INPUT_D))
	{
		rotRad = AsoUtility::Deg2RadD(90.0);
		dir = cameraRot.GetRight();
	}

	// �J�����������獶���ֈړ�������
	if (ins.IsNew(KEY_INPUT_A))
	{
		rotRad = AsoUtility::Deg2RadD(270.0);
		dir = cameraRot.GetLeft();
	}

	/*if (!AsoUtility::EqualsVZero(dir) && (isJump_))*/ {

		// �ړ�����
		speed_ = SPEED_MOVE;
		if (ins.IsNew(KEY_INPUT_RSHIFT))
		{
			speed_ = SPEED_RUN;
		}
		moveDir_ = dir;
		movePow_ = VScale(dir, speed_);

		// ��]����
		SetGoalRotate(rotRad);

		//if (!isJump_ && IsEndLanding())
		//{
		//	// �A�j���[�V����
		//	if (ins.IsNew(KEY_INPUT_RSHIFT))
		//	{
		//		animationController_->Play((int)ANIM_TYPE::FAST_RUN);
		//	}
		//	else
		//	{
		//		animationController_->Play((int)ANIM_TYPE::RUN);
		//	}
		//}

	}
	//else
	{
		//if (!isJump_ && IsEndLanding())
		//{
		//	animationController_->Play((int)ANIM_TYPE::IDLE);
		//}
	}
}

void Rider::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rider::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Rider::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void Rider::CollisionGravity(void)
{
	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// ���n���[�V����
				//animationController_->Play(
				//	(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			isJump_ = false;

		}

	}
}

void Rider::CollisionCapsule(void)
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

void Rider::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}

}
