#include <string>
#include<EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Stage/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "../../Scene/GameScene.h"
#include "CoinBase.h"

CoinBase::CoinBase(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos)
{
	bikes_ = bikes;
	gameScene_ = gameScene;

	state_ = STATE::NONE;

	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	makePos_ = loopStagePos;
	localPos_ = localPos;

	enemyRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = MyUtility::VECTOR_ZERO;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	isBikeCol_ = false;

	isAddScore_ = false;

	capsule_ = nullptr;

	isCollGround_ = false;

	stepMade_ = 0.0f;
}

CoinBase::~CoinBase(void)
{
}

void CoinBase::Init(void)
{

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COPPER_COIN));
	transform_.scl = MyUtility::VECTOR_ONE;
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, makePos_.y, makePos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ COLLIDER_POS_TOP });
	capsule_->SetLocalPosDown({ COLLIDER_POS_DOWN });
	capsule_->SetRadius(20.0f);

	// �������
	ChangeState(STATE::PLAY);

	////�G�L�����ʂ̃p�����[�^�ݒ�
	SetParam();

	// �G�t�F�N�g������
	InitEffect();
}

void CoinBase::SetParam(void)
{

}

void CoinBase::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case CoinBase::STATE::NONE:
		UpdateNone();
		break;
	case CoinBase::STATE::PLAY:
		UpdatePlay();
		break;
	case CoinBase::STATE::FLIPED:
		UpdateFliped();
		break;
	case CoinBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// ���f������X�V
	transform_.Update();
}

void CoinBase::Draw(void)
{
	//���S��Ԃ͕`�悵�Ȃ�
	if (state_ == STATE::DEAD)
	{
		return;
	}

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	DrawLine3D(fowardPos_, backPos_, 0x0000ff);

}

void CoinBase::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void CoinBase::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> CoinBase::GetCapsule(void) const
{
	return capsule_;
}

void CoinBase::Flip(VECTOR dir)
{
	flipDir_ = dir;
	flipSpeed_ = 10.0f;
	ChangeState(STATE::FLIPED);
}

bool CoinBase::GetIsBikeCol(void)
{
	return isBikeCol_;
}

bool CoinBase::GetIsAddScore(void)
{
	return isAddScore_;
}

CoinBase::STATE CoinBase::GetState(void)
{
	return state_;
}

bool CoinBase::IsDestroy(void)
{
	return state_ == STATE::DEAD;
}

void CoinBase::Destroy(void)
{
	ChangeState(STATE::DEAD);
}

void CoinBase::AddScoreToPlayer(int playerId, int score)
{
	if (playerId >= 0 && playerId < bikes_.size())
	{
		bikes_[playerId]->AddScore(score);
	}
}

void CoinBase::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;

}

void CoinBase::HitEffect()
{
	effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

	float scale = 50.0f;

	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + EFF_POS_Z);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	
}

void CoinBase::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case CoinBase::STATE::NONE:
		ChangeStateNone();
		break;
	case CoinBase::STATE::PLAY:
		ChangeStatePlay();
		break;
	case CoinBase::STATE::FLIPED:
		ChangeStateFliped();
		break;
	case CoinBase::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void CoinBase::ChangeStateNone(void)
{
}

void CoinBase::ChangeStatePlay(void)
{
}

void CoinBase::ChangeStateFliped(void)
{
}

void CoinBase::ChangeStateDead(void)
{
}

void CoinBase::UpdateNone(void)
{
}

void CoinBase::UpdateFliped(void)
{

	// �ړ�����
	ProcessMove();

	// ������΂����
	flipSpeed_ -= 0.16f;
	if (flipSpeed_ < 0.0f)
	{
		flipSpeed_ = 0.0f;
		ChangeState(STATE::PLAY);
	}
	movePow_ = VAdd(movePow_, VScale(flipDir_, flipSpeed_));

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = enemyRotY_;

}

void CoinBase::UpdateDead(void)
{
	isAddScore_ = false;
}

void CoinBase::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	//��]
	RotY();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = enemyRotY_;
}

void CoinBase::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//�Փ˔���(�G�ƃv���C���[)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = MyUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//�X�R�A���Z
			isAddScore_ = true;
			//�Փ˔���
			isBikeCol_ = true;

			// �q�b�g�G�t�F�N�g
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

			//�R�C�����W���̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);

			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			isBikeCol_ = false;
			isAddScore_ = false;
			
		}
	}

	//�폜����
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}
}

void CoinBase::SetGoalRotate(float rotRad)
{
	//VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, MyUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1f)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void CoinBase::RotY(void)
{
	//�n�ʂƏՓ˂������]
	if (isCollGround_)
	{
		//��]
		// �f�O���[���烉�W�A��(�ϊ�)
		float rad = MyUtility::Deg2RadF(SPEED_ROT);

		// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
		Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_Y);

		//�N�H�[�^�j�I��(��])�̍���
		enemyRotY_ = enemyRotY_.Mult(rotPow);
	}
}

void CoinBase::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	//y���W�ł̎��S����
	if (transform_.pos.y <= DEAD_POS_Y)
	{
		ChangeState(STATE::DEAD);
	}

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void CoinBase::CollisionGravity(void)
{

	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = MyUtility::DIR_U;

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
			jumpPow_ = MyUtility::VECTOR_ZERO;

			isCollGround_ = true;

		}

	}
}

void CoinBase::CollisionCapsule(void)
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

void CoinBase::CalcGravityPow(void)
{
	//1�񓖂������璆�f
	if (isCollGround_)
	{
		transform_.pos.y = COLL_AFTER_POS_Y;
		return;
	}

	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, MyUtility::VECTOR_ZERO);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}