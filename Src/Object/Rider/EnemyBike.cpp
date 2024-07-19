#include <string>
#include "../../Application.h"
#include "../..//Utility/MyUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "Weapon.h"
#include "EnemyBase.h"
#include "EnemyBike.h"

EnemyBike::EnemyBike(EnemyBase* enemy)
{

	enemy_ = enemy;

	weapon_ = nullptr;

	animationController_ = nullptr;

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = MyUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	isAttack_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;
}

EnemyBike::~EnemyBike(void)
{
	delete weapon_;
	delete animationController_;
}

void EnemyBike::Init(void)
{

	// ����
	weapon_ = new Weapon();
	weapon_->Init();

	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BIKE));
	float scale = 1.3f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// ���f���̊�{�ݒ�
	transform_.Update();

	weapon_->SetTransForm(transform_);

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(135.0f);


	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �������
	ChangeState(STATE::PLAY);
}

void EnemyBike::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case EnemyBike::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBike::STATE::PLAY:
		UpdatePlay();
		//����
		ProcessMove();
		break;
	}

	weapon_->Update();
	weapon_->SetTransForm(transform_);
	// ���f������X�V
	transform_.Update();
	// �A�j���[�V�����Đ�
	animationController_->Update();

	
}

void EnemyBike::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// ����
	weapon_->Draw();

	// �ۉe�`��
	DrawShadow();

	// �f�o�b�O�`��
	DrawDebug();
}

void EnemyBike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EnemyBike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> EnemyBike::GetCapsule(void) const
{
	return capsule_;
}

void EnemyBike::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(transformPlayer_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EnemyBike::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case EnemyBike::STATE::NONE:
		ChangeStateNone();
		break;
	case EnemyBike::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void EnemyBike::ChangeStateNone(void)
{
	
}

void EnemyBike::ChangeStatePlay(void)
{
}

void EnemyBike::UpdateNone(void)
{
}

void EnemyBike::UpdatePlay(void)
{
	//// �ړ�����
	//ProcessMove();

	// �W�����v����
	ProcessJump();

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

	// 

	// ��]������
	transform_.quaRot = playerRotY_;
}

void EnemyBike::DrawUI(void)
{
}

void EnemyBike::DrawShadow(void)
{
}

void EnemyBike::DrawDebug(void)
{
	capsule_->Draw();
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	// �U��������������
	if (isAttack_ == true)
	{
		DrawString(0, 0, "Attack", 0x000000);
	}

	DrawFormatString(10, 10, 0xffffff, "bikePos = %f,%f,%f", transform_.pos.x, transform_.pos.y, transform_.pos.z);
}

void EnemyBike::ProcessMove(void)
{
	transform_.pos = enemy_->GetTransform().pos;
	transform_.quaRot = enemy_->GetTransform().quaRot;
	transform_.Update();


	
}

void EnemyBike::ProcessJump(void)
{
}

void EnemyBike::ProcessAttack(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_Z))
	{
		attackState_ = ATTACK_TYPE::NORMAL;
		animationController_->Play((int)ANIM_TYPE::FALLING);
		isAttack_ = true;
	}
	else
	{
		attackState_ = ATTACK_TYPE::NONE;
		isAttack_ = false;
	}
}

void EnemyBike::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_C))
	{
		hp_ -= 1;
	}

}

void EnemyBike::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void EnemyBike::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);

}

void EnemyBike::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);
	movedPos_ = VAdd(transformPlayer_.pos, movePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void EnemyBike::CollisionGravity(void)
{
	//// �W�����v�ʂ����Z
	//movedPos_ = VAdd(movedPos_, jumpPow_);

	//// �d�͕���
	//VECTOR dirGravity = MyUtility::DIR_D;

	//// �d�͕����̔���
	//VECTOR dirUpGravity = MyUtility::DIR_U;

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

	//		// �W�����v���Z�b�g
	//		jumpPow_ = MyUtility::VECTOR_ZERO;
	//		stepJump_ = 0.0f;

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

void EnemyBike::CollisionCapsule(void)
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

void EnemyBike::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}

bool EnemyBike::IsEndLanding(void)
{
	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}
