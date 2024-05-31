#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "JampRamp.h"
#include "Common/Capsule.h"

JampRamp::JampRamp() : resMng_(ResourceManager::GetInstance())
{
}

JampRamp::~JampRamp(void)
{
}

void JampRamp::Init(void)
{

	transform_.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::JUMP_RAMP));
	float scale = 1.0f;
	transform_.scl = { scale*3,scale,scale*1.5f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = 
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });
	transform_.pos = { 1590.0f, -260.0f, 4000.0f };

	transform_.Update();

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	/*capsule_->SetLocalPosTop({ -200.0f, 100.0f, 0.0f });
	capsule_->SetLocalPosDown({ 380.0f, 100.0f, 0.0f });*/
	capsule_->SetLocalPosTop({ 100.0f, 100.0f, 0.0f });
	capsule_->SetLocalPosDown({100.0f, 100.0f, 0.0f });
	capsule_->SetRadius(1450.0f);
}

void JampRamp::Update(void)
{	
	CollisionCapsule();

	transform_.Update();
}

void JampRamp::Draw(void)
{

	MV1DrawModel(transform_.modelId);
	DrawDebug();
}

void JampRamp::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void JampRamp::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule* JampRamp::GetCapsule(void) const
{
	return capsule_;
}

void JampRamp::CollisionCapsule(void)
{
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	//// �J�v�Z���Ƃ̏Փ˔���
	//for (const auto c : colliders_)
	//{

	//	auto hits = MV1CollCheck_Capsule(
	//		c->modelId_, -1,
	//		cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

	//	for (int i = 0; i < hits.HitNum; i++)
	//	{

	//		auto hit = hits.Dim[i];

	//		for (int tryCnt = 0; tryCnt < 10; tryCnt++)
	//		{

	//			int pHit = HitCheck_Capsule_Triangle(
	//				cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
	//				hit.Position[0], hit.Position[1], hit.Position[2]);

	//			if (pHit)
	//			{
	//				movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
	//				// �J�v�Z�����ړ�������
	//				trans.pos = movedPos_;
	//				trans.Update();
	//				continue;
	//			}

	//			break;

	//		}

	//	}

	//	// ���o�����n�ʃ|���S�����̌�n��
	//	MV1CollResultPolyDimTerminate(hits);

	//}

}

void JampRamp::DrawDebug(void)
{
	capsule_->Draw();
}
