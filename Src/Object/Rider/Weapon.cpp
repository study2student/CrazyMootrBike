#include <string>
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
#include "Weapon.h"

Weapon::Weapon(void)
{

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

	isAttack_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	capsule_ = nullptr;
}

Weapon::~Weapon(void)
{
}

void Weapon::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::W_SWORD));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	//transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.pos = {bikeTransform_.pos };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 
		AsoUtility::Deg2RadF(180.0f), 
		AsoUtility::Deg2RadF(0.0f), 
		AsoUtility::Deg2RadF(270.0f) });

	transform_.Update();

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(20.0f);

	
}

void Weapon::Update(void)
{
	ProcessMove();
	ProcessAttack();

	// �ړ������ɉ�������]
	Rotate();
	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	//���W
	VECTOR localPos = { -40,110,0 };
	transform_.pos = bikeTransform_.pos;
	/*transform_.pos = VAdd(transform_.pos, localPos);*/
	VECTOR local= bikeTransform_.quaRot.PosAxis(localPos);
	transform_.pos = VAdd(transform_.pos, local);
	
	
	
	transform_.Update();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void Weapon::Draw(void)
{
	MV1DrawModel(transform_.modelId);
	DrawDebug();
}

void Weapon::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void Weapon::ClearCollider(void)
{
	colliders_.clear();
}

void Weapon::SetTransForm(Transform transform)
{
	bikeTransform_ = transform;
}

const Capsule* Weapon::GetCapsule(void) const
{
    return capsule_;
}

void Weapon::DrawUI(void)
{
}

void Weapon::DrawDebug(void)
{
	capsule_->Draw();
}

void Weapon::ProcessMove(void)
{
}

void Weapon::ProcessAttack(void)
{
}

void Weapon::SetGoalRotate(double rotRad)
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

void Weapon::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Weapon::Collision(void)
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

void Weapon::CollisionGravity(void)
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
				//// ���n���[�V����
				//animationController_->Play(
				//	(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			isJump_ = false;

		}

	}
}

void Weapon::CollisionCapsule(void)
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

void Weapon::CalcGravityPow(void)
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
