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
#include "ShortDisEnemy.h"
#include <EffekseerForDXLib.h>


ShortDisEnemy::ShortDisEnemy(const std::vector<std::shared_ptr<Bike>>& bikes, VECTOR loopStagePos, VECTOR localPos) : EnemyBase(bikes, loopStagePos, localPos)
{
	makePos_ = loopStagePos;
	localPos_ = localPos;
}

void ShortDisEnemy::SetParam(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOLD_MEDAL));
	float SCL = 200.0f;
	transform_.scl = { SCL,SCL,SCL };
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, 700.0f, makePos_.z + localPos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();


	//// �A�j���[�V�����̐ݒ�
	//InitAnimation();


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
	case EnemyBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// ���f������X�V
	transform_.Update();

	//// �A�j���[�V�����Đ�
	//animationController_->Update();
}

void ShortDisEnemy::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	// �W�����v����
	ProcessJump();

	//��]
	RotY();

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

	//VECTOR dir;

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//�v���C���[�֌����������擾
	/*VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	dir = VNorm(len);*/


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

			// �R�C�����W���̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);

			// �q�b�g�G�t�F�N�g
			float scale = 50.0f;
			HitEffect(bikes_[3]->GetTransform().pos, bikes_[3]->GetTransform().rot);
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
			
			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}

			//�A�j���[�V����
			animationController_->Play((int)ANIM_TYPE::SHORT);
		}
		else
		{
			speed_ = SPEED_MOVE;
			isAddScore_ = false;
			if (!isJump_ && IsEndLanding())
			{

				animationController_->Play((int)ANIM_TYPE::FAST_RUN);

			}
		}
	}
	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/

	////�����Ă�����Ɉړ�
	//moveDir_ = dir;
	//movePow_ = VScale(dir, speed_);

	// ��]����(�v���C���[�̕�������������)
	//VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	//double subDeg = atan2(subVec.x, subVec.z);
	//SetGoalRotate(subDeg);


	//}
	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/

	//�폜����
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}


}
