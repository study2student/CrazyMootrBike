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
#include "../../Object/Score.h"
#include "LongDisEnemy.h"

LongDisEnemy::LongDisEnemy(const std::vector<std::shared_ptr<Bike>>& bikes, VECTOR loopStagePos, VECTOR localPos) : EnemyBase(bikes, loopStagePos,localPos)
{
	makePos_ = loopStagePos;
	localPos_ = localPos;
}

void LongDisEnemy::SetParam(void)
{

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::ENEMY_LONG));
	transform_.scl = AsoUtility::VECTOR_ONE;
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, 700.0f, makePos_.z + localPos_.z };
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

void LongDisEnemy::Update(void)
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
	case EnemyBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// ���f������X�V
	transform_.Update();

	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void LongDisEnemy::UpdatePlay(void)
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

void LongDisEnemy::ProcessMove(void)
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

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	////�v���C���[�֌����������擾
	//VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	//dir = VNorm(len);


	/*if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {*/

	// �ړ�����
	//speed_ = SPEED_MOVE;
	//�Փ˔���(�G�ƃv���C���[)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = AsoUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//�͈͂ɓ�����
			speed_ = 0;
			isBikeCol_ = true;
			isAddScore_ = true;
			AddScoreToPlayer(bike->GetPlayerID(), 10);
			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			speed_ = SPEED_MOVE;
			//isAddScore_ = false;
		}
	}
	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/

	//�����Ă�����Ɉړ�
	/*moveDir_ = dir;
	movePow_ = VScale(dir, speed_);*/

	// ��]����(�v���C���[�̕�������������)
	//VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	//double subDeg = atan2(subVec.x, subVec.z);
	//SetGoalRotate(subDeg);

	if (!isJump_ && IsEndLanding())
	{

		animationController_->Play((int)ANIM_TYPE::LONG);

	}

	//}
	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/


	VECTOR fowardPos;
	VECTOR backPos;

	//�U��
	if(ins.IsNew(KEY_INPUT_C))
	{
		float Length = 100.0f;
		fowardPos_ = VAdd(transform_.pos, VGet(0, 0, Length));
		backPos_ = transform_.pos;
	}

	
	

}
