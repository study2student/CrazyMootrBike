#include <string>
#include "..//Application.h"
#include "..///Utility/AsoUtility.h"
#include "..///Manager/InputManager.h"
#include "..//Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "Rotor.h"

Rotor::Rotor()
{

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	isAttack_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;

}

Rotor::~Rotor(void)
{
	delete capsule_;
	//delete animationController_;
}

void Rotor::Init(void)
{

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::HELICOPTER_ROTOR));
	float scale = 5.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 1670.0f, 500.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(135.0f);

	// �̗�
	hp_ = MAX_HP;

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

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

	// �̗͂Ƃ��Q�[�W�Ƃ�
	DrawUI();

	// �ۉe�`��
	DrawShadow();

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

void Rotor::InitAnimation(void)
{
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
	transform_.Update();
}

void Rotor::DrawUI(void)
{
}

void Rotor::DrawShadow(void)
{
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
	float rad = AsoUtility::Deg2RadF(SPEED_ROT);

	// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

	//�N�H�[�^�j�I��(��])�̍���
	rotY_ = rotY_.Mult(rotPow);

}

void Rotor::ProcessJump(void)
{
}

void Rotor::ProcessAttack(void)
{

	NormalAttack();
	SpecialAttack();
	LongAttack();
}

void Rotor::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();
}

void Rotor::NormalAttack(void)
{
}

void Rotor::LongAttack(void)
{

}

void Rotor::SpecialAttack(void)
{
}

void Rotor::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rotor::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, AsoUtility::AXIS_Z);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Rotor::Rotate(void)
{
	//stepRotTime_ -= scnMng_.GetDeltaTime();

	//// ��]�̋��ʕ��
	//rotY_ = Quaternion::Slerp(
	//	rotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Rotor::Collision(void)
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

void Rotor::CollisionGravity(void)
{

	//// �d�͕���
	//VECTOR dirGravity = AsoUtility::DIR_D;

	//// �d�͕����̔���
	//VECTOR dirUpGravity = AsoUtility::DIR_U;

	//// �d�͂̋���
	//float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	//float checkPow = 10.0f;
	//gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	//gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	//gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	//for (const auto c : colliders_)
	//{

	//	// �n�ʂƂ̏Փ�
	//	auto hit = MV1CollCheck_Line(
	//		c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

	//	// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
	//	if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
	//	{

	//		// �Փ˒n�_����A������Ɉړ�
	//		movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

	//	
	//

	//		if (isJump_)
	//		{
	//			// ���n���[�V����
	//			animationController_->Play(
	//				(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
	//		}

	//		isJump_ = false;

	//	}

	//}
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
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	//float dot = VDot(dirGravity, jumpPow_);
	//if (dot >= 0.0f)
	//{
	//	// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
	//	jumpPow_ = gravity;
	//}

}

bool Rotor::IsEndLanding(void)
{
	bool ret = true;

	////�A�j���[�V�������I�����Ă��邩
	//if (animationController_->IsEnd())
	//{
	//	return ret;
	//}

	return false;
}



