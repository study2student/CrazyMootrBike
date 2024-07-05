#include <string>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
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
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

TyreThrow::~TyreThrow(void)
{
}

void TyreThrow::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TYRE));
	float scale = 2.05f;
	transform_.scl = { scale , scale, scale };
	transform_.pos = { 1500.0f,100.0f, 3000.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();


	// �J�v�Z���R���C�_
	//capsule_ = new Capsule(transform_);
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 30.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 10.0f, 0.0f });
	capsule_->SetRadius(80.0f);

	// �������
	ChangeState(STATE::IDLE);
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
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
	DrawDebug();
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
}


void TyreThrow::ChangeStateThrow(void)
{
	//�����_���Ō��������߂�
	//���̃����_���ȏꏊ�ɐ���(3�p�^�[��)
	int randDir = GetRand(static_cast<int>(TyreThrow::DIR::MAX) - 1);
	TyreThrow::DIR dir = static_cast<TyreThrow::DIR>(randDir);
	VECTOR targetPos;

	Vector2 randPos;
	switch (dir)
	{
	case TyreThrow::DIR::SIDE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_SIDE);
		break;
	case TyreThrow::DIR::SLIGHTLY_OBLIPUE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE);
		break;
	case TyreThrow::DIR::LARGE_OBLIPUE:
		targetPos = VAdd(transformTarget_.pos, THROW_LOCAL_POS_TO_LARGE_OBLIPUE);
		break;
	}

	targetDir_ = VNorm(VSub(targetPos, transform_.pos));
}


void TyreThrow::ChangeStateDestroy(void)
{
}

void TyreThrow::UpdateIdle(void)
{
	//�v���C���[�̏�����őҋ@
	transform_.pos = transformTarget_.pos;

	//5�b��ɓ�����
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

	// �^�C�����ړ�������
	VECTOR movePow;
	movePow = VScale(targetDir_, SPEED_MOVE);

	// �ړ�����
	transform_.pos = VAdd(transform_.pos, movePow);

	// �d�͂ɂ��ړ���
	CalcGravityPow();
	// �Փ˔���
	Collision();
	transform_.Update();

	//3�b��ɔ���
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
	float rad = AsoUtility::Deg2RadF(SPEED_ROT);

	// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_X);

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
