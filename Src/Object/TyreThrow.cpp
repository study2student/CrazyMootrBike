#include <string>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/MyUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"
#include "Common/Capsule.h"
#include "Common/Collider.h"
#include "Planet.h"
#include "Rider/Bike.h"
#include "TyreThrow.h"

TyreThrow::TyreThrow()
{
	state_ = STATE::IDLE;


	speed_ = 0.0f;
	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

TyreThrow::~TyreThrow(void)
{
}

void TyreThrow::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE_BALL));
	float scale = 120.0f;
	transform_.scl = { scale , scale, scale };
	transform_.pos = { 1500.0f,1000.0f, 3000.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// �J�v�Z���R���C�_
	//capsule_ = new Capsule(transform_);
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 30.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 10.0f, 0.0f });
	capsule_->SetRadius(130.0f);

	//�G�t�F�N�g
	InitEffect();

	// �������
	ChangeState(STATE::DESTROY);
}

void TyreThrow::Update(void)
{
	switch (state_)
	{
	case TyreThrow::STATE::IDLE:
		UpdateIdle();
		break;
	case TyreThrow::STATE::THROW_MOVE:
		UpdateThrowMove();
		break;
	case TyreThrow::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void TyreThrow::Draw(void)
{
	// �f�o�b�O�`��
	DrawDebug();

	//�����Ă��鎞�ȊO�͕`�悵�Ȃ�
	if (!(state_ == STATE::THROW_MOVE))
	{
		return;
	}

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void TyreThrow::SetTransform(Transform transformTarget)
{
	transformTarget_ = transformTarget;
}

void TyreThrow::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void TyreThrow::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> TyreThrow::GetCapsule(void) const
{
	return capsule_;
}

void TyreThrow::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

bool TyreThrow::GetIsCol(void)
{
	return isCol_;
}

bool TyreThrow::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void TyreThrow::InitEffect(void)
{
	effectMakeResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::THROW_MAKE_EFFECT).handleId_;

	bombEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void TyreThrow::MakeEffect(void)
{
	effectMakePlayId_ = PlayEffekseer3DEffect(effectMakeResId_);
	float scale = 130.0f;
	SetScalePlayingEffekseer3DEffect(effectMakePlayId_, scale, scale, scale);
	VECTOR localPos = { 0.0f,300.0f,0.0f };
	SetPosPlayingEffekseer3DEffect(effectMakePlayId_, transform_.pos.x, transform_.pos.y + localPos.y, transform_.pos.z);
	SetRotationPlayingEffekseer3DEffect(effectMakePlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void TyreThrow::BombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//�����ɓ����������͑O�Ŕ���
	VECTOR localPos = {};
	if (isCol_)
	{
		localPos = { 0.0f,0.0f,2200.0f };
	}
	else
	{
		localPos = { 0.0f,0.0f,0.0f };
	}
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + localPos.z);
	float scl = 10.0f;
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, scl, scl, scl);
}

void TyreThrow::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case TyreThrow::STATE::IDLE:
		ChangeStateIdle();
		break;
	case TyreThrow::STATE::THROW_MOVE:
		ChangeStateThrow();
		break;
	case TyreThrow::STATE::DESTROY:
		ChangeStateDestroy();
		break;
	}
}

void TyreThrow::ChangeStateIdle(void)
{
	isCol_ = false;
}


void TyreThrow::ChangeStateThrow(void)
{

	transform_.pos = VAdd(transformTarget_.pos, TYRE_IDLE_ROCAL_POS);

	//�ǂ����瓊���邩�����_���Ō��߂�
	int randDir = GetRand(static_cast<int>(TyreThrow::DIR::MAX) - 1);
	TyreThrow::DIR dir = static_cast<TyreThrow::DIR>(randDir);

	switch (dir)
	{
	case TyreThrow::DIR::LEFT:
		transform_.pos.x = MAKE_LEFT_POS_X;
		break;
	case TyreThrow::DIR::RIGHT:
		transform_.pos.x = MAKE_RIGHT_POS_X;
		break;
	case TyreThrow::DIR::MAX:
		break;
	}

	//�����G�t�F�N�g
	MakeEffect();

	//�����_����3�p�^�[���̓���
	int randAngle = GetRand(static_cast<int>(TyreThrow::ANGLE::MAX) - 1);
	TyreThrow::ANGLE angle = static_cast<TyreThrow::ANGLE>(randAngle);
	VECTOR targetPos;
	VECTOR throwLocalPos;

	switch (angle)
	{
	case TyreThrow::ANGLE::SIDE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_SIDE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_SIDE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	case TyreThrow::ANGLE::SLIGHTLY_OBLIPUE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	case TyreThrow::ANGLE::LARGE_OBLIPUE:
		if (dir == DIR::LEFT)
		{
			throwLocalPos = LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE;
		}
		if (dir == DIR::RIGHT)
		{
			throwLocalPos = RIGHT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE;
		}

		targetPos = VAdd(transformTarget_.pos, throwLocalPos);
		break;
	}

	targetDir_ = VNorm(VSub(targetPos, transform_.pos));
	//�ۑ�
	targetDirSave_ = targetDir_;
}


void TyreThrow::ChangeStateDestroy(void)
{
	//�����G�t�F�N�g
	BombEffect();
}

void TyreThrow::UpdateIdle(void)
{
	//�v���C���[�̏�����őҋ@
	transform_.pos = VAdd(transformTarget_.pos, TYRE_IDLE_ROCAL_POS);

	//3�b��ɓ�����
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//���e����������ԂɈڍs
		ChangeState(STATE::THROW_MOVE);
		stepPlaceDrawTime_ = 0.0f;
	}
}

void TyreThrow::UpdateThrowMove(void)
{
	//// �f�o�b�O�p
	//ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	//�ǂɓ����������甽��
	if (transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX)
	{
		targetDir_ = VAdd(targetDirSave_, Stage::LEFT_NORMAL_VEC);
	}
	if (transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX)
	{
		targetDir_ = VAdd(targetDirSave_, Stage::RIGHT_NORMAL_VEC);

	}

	// �ړ�������
	VECTOR movePow;
	movePow = VScale(targetDir_, SPEED_MOVE);

	// �ړ�����
	transform_.pos = VAdd(transform_.pos, movePow);

	// �d�͂ɂ��ړ���
	CalcGravityPow();
	// �Փ˔���
	Collision();
	transform_.Update();

	//4.5�b��ɔ���
	stepToDeleteTime_ += SceneManager::GetInstance().GetDeltaTime();

	//�����ɓ������Ă���������
	if (stepToDeleteTime_ >= TO_DELETE_MAX_TIME || isCol_)
	{
		//�폜��ԂɈڍs
		ChangeState(STATE::DESTROY);
		stepToDeleteTime_ = 0.0f;
	}
}

void TyreThrow::UpdateDestroy(void)
{
	//8�b��ɕ���
	stepTyreDestroy_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepTyreDestroy_ >= TYRE_REMAKE_MAX_TIME)
	{
		//���e�������ˑO��ԂɈڍs
		ChangeState(STATE::IDLE);
		stepTyreDestroy_ = 0.0f;
	}
}

void TyreThrow::DrawDebug(void)
{
}

void TyreThrow::ProcessDebug(void)
{
}

void TyreThrow::Rotate(void)
{
	auto& ins = InputManager::GetInstance();

	//���A�^�C���̉�]
	// �f�O���[���烉�W�A��(�ϊ�)
	float rad = MyUtility::Deg2RadF(SPEED_ROT);

	// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_X);

	//�N�H�[�^�j�I��(��])�̍���
	rotX_ = rotX_.Mult(rotPow);

	// ��]������
	transform_.quaRot = rotX_;
	transform_.Update();
}

void TyreThrow::Collision(void)
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

void TyreThrow::CollisionGravity(void)
{
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

		if (hit.HitFlag > 0)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));


		}

	}
}

void TyreThrow::CollisionCapsule(void)
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

void TyreThrow::CalcGravityPow(void)
{
}
