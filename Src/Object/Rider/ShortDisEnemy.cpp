#include <DxLib.h>
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
#include "../../Object/Rider/Bike.h"
#include "ShortDisEnemy.h"


ShortDisEnemy::ShortDisEnemy(Bike* bike) : EnemyBase(bike)
{
}

void ShortDisEnemy::SetParam(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_SHORT));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { 700.0f, 700.0f, -500.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();


	// �A�j���[�V�����̐ݒ�
	InitAnimation();


	//// �J�v�Z���R���C�_
	//capsule_ = new Capsule(transform_);
	//capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	//capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	//capsule_->SetRadius(20.0f);

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �������
	ChangeState(STATE::PLAY);
}

void ShortDisEnemy::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBase::STATE::PLAY:
		UpdatePlay();
		break;
	case EnemyBase::STATE::FLIPED:
		UpdateFliped();
		break;
	}


	// ���f������X�V
	transform_.Update();

	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void ShortDisEnemy::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	// �W�����v����
	ProcessJump();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = enemyRotY_;
}

void ShortDisEnemy::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//// �ړ��ʂ��[��
	//movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;

	//VECTOR dir = AsoUtility::DIR_F;
	VECTOR dir;// = AsoUtility::DIR_F;

	//// �J���������ɑO�i������
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	rotRad = AsoUtility::Deg2RadD(0.0);
	//	dir = cameraRot.GetForward();
	//}

	//// �J�������������ނ�����
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	rotRad = AsoUtility::Deg2RadD(180.0);
	//	dir = cameraRot.GetBack();
	//}

	//// �J������������E���ֈړ�������
	//if (ins.IsNew(KEY_INPUT_D))
	//{
	//	rotRad = AsoUtility::Deg2RadD(90.0);
	//	dir = cameraRot.GetRight();
	//}

	//// �J�����������獶���ֈړ�������
	//if (ins.IsNew(KEY_INPUT_A))
	//{
	//	rotRad = AsoUtility::Deg2RadD(270.0);
	//	dir = cameraRot.GetLeft();
	//}

	Transform bikeTrans_ = bike_->GetTransform();

	//
	VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	dir = VNorm(len);


	/*if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {*/

	// �ړ�����
	//speed_ = SPEED_MOVE;
	//�Փ˔���(�G�ƃv���C���[)
	VECTOR diff = VSub(bike_->GetCapsule()->GetCenter(), capsule_->GetCenter());
	float  dis = AsoUtility::SqrMagnitudeF(diff);
	if (dis < RADIUS * RADIUS)
	{
		//�͈͂ɓ�����
		speed_ = 0;
		//�A�j���[�V����
		animationController_->Play((int)ANIM_TYPE::SHORT);
	}
	else
	{
		speed_ = SPEED_MOVE;

		if (!isJump_ && IsEndLanding())
		{

			animationController_->Play((int)ANIM_TYPE::FAST_RUN);

		}
	}

	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/
	moveDir_ = dir;
	movePow_ = VScale(dir, speed_);

	// ��]����(�v���C���[�̕�������������)
	VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	double subDeg = atan2(subVec.x, subVec.z);
	SetGoalRotate(subDeg);


	//}
	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/
}
