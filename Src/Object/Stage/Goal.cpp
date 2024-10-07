#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "Planet.h"
#include "Goal.h"

#pragma region �萔�錾

//�S�[����{�ʒu
const VECTOR GOAL_BASE_POS = { 1650.0f,-800.0f,200000000.0f };

//�S�[���̑傫��
const float SCL = 7.0f;

//�S�[���̑傫�������p�l
const float SCL_ADJUST_NUM = 1.1f;

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 190.0f, 0.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 150.0f, 0.0f };

//�J�v�Z�����a
const float CAPSULE_RADIUS = 135.0f;

#pragma endregion


Goal::Goal(void)
	:
	movePow_({}),
	movedPos_({}),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({})
{
}

Goal::~Goal(void)
{
}

void Goal::Init(void)
{

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOAL));
	transform_.scl = { SCL / SCL_ADJUST_NUM, SCL, SCL };
	//�ŏ��͌����Ȃ��ꏊ��
	transform_.pos = { GOAL_BASE_POS.x, GOAL_BASE_POS.y ,GOAL_BASE_POS.z};
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();


	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);
}

void Goal::Update(void)
{

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	//�Փ˔���
	Collision();

	// ���f������X�V
	transform_.Update();
}

void Goal::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void Goal::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Goal::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Goal::GetCapsule(void) const
{
	return capsule_;
}

void Goal::SetPosZ(float z)
{
	transform_.pos.z = z;
}

void Goal::DrawDebug(void)
{
	capsule_->Draw();
}

void Goal::Collision(void)
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

void Goal::CollisionGravity(void)
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
	for (const auto& c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		if (hit.HitFlag > 0)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

		}

	}
}

void Goal::CollisionCapsule(void)
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

void Goal::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
}
