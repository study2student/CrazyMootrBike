#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../Stage/Planet.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "GimmickBase.h"

GimmickBase::GimmickBase()
{
	//�ړ��n
	movePow_ = {};
	movedPos_ = {};
}

GimmickBase::~GimmickBase()
{
}

void GimmickBase::Init(void)
{
	// ������(�O���ǂݍ���)
	InitLoad();

	// ������(���f���ݒ�)
	InitTransform();

	// ������(�Փːݒ�)
	InitCollider();

	// ������(���㏈��)
	InitPost();
}

void GimmickBase::Update(void)
{
}

void GimmickBase::Draw(void)
{
}

void GimmickBase::AddCollider(std::weak_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void GimmickBase::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule& GimmickBase::GetCapsule(void) const
{
	return *capsule_;
}

void GimmickBase::Collision(void)
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

void GimmickBase::CollisionGravity(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = MyUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	auto gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	auto gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto& c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c.lock()->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		if (hit.HitFlag > 0)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));


		}

	}
}

void GimmickBase::CollisionCapsule(void)
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
			c.lock()->modelId_, -1,
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

void GimmickBase::InitLoad(void)
{
}

void GimmickBase::InitTransform(void)
{
	transform_.scl = initScl_;
	transform_.quaRot = Quaternion::Euler(initRotEuler_);
	transform_.quaRotLocal = Quaternion::Euler(initLocalRotEuler_);
	transform_.pos = initPos_;
	transform_.Update();
}

void GimmickBase::InitCollider(void)
{
	//�Փ˗p�̃J�v�Z��
	capsule_ = std::make_unique<Capsule>(transform_);
	capsule_->SetLocalPosTop(capsulePosTop_);
	capsule_->SetLocalPosDown(capsulePosDown_);
	capsule_->SetRadius(capsuleRadius_);
}

void GimmickBase::InitPost(void)
{
}
