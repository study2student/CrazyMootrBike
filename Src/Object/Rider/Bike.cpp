#include <string>
#include "../../Application.h"
#include "../..//Utility/AsoUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "Player.h"
#include "Weapon.h"
#include "Bike.h"

Bike::Bike(void)
{
	player_ = nullptr;

	weapon_ = nullptr;

	animationController_ = nullptr;

	state_ = STATE::NONE;

	attackState_ = ATTACK_TYPE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	playerRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	isAttack_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;

}

Bike::~Bike(void)
{
	delete player_;
	delete weapon_;
	delete capsule_;
	delete animationController_;
}

void Bike::Init(void)
{
	// �v���C���[
	player_ = new Player();
	player_->Init();
	
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
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// ���f���̊�{�ݒ�
	transformPlayer_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	float pScale = 1.0f;
	transformPlayer_.scl = { pScale, pScale, pScale };
	transformPlayer_.pos = { transform_.pos.x, transform_.pos.y + 20.0f, transform_.pos.z };
	//transformPlayer_.quaRot = Quaternion();
	transformPlayer_.quaRot = transform_.quaRot;
	transformPlayer_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();
	transformPlayer_.Update();

	weapon_->SetTransForm(transform_);

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = new Capsule(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 190.0f, -60.0f });
	capsule_->SetLocalPosDown({ 0.0f, 150.0f, -60.0f });
	capsule_->SetRadius(135.0f);

	// �̗�
	hp_ = 100;

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �������
	ChangeState(STATE::PLAY);

}

void Bike::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Bike::STATE::NONE:
		UpdateNone();
		break;
	case Bike::STATE::PLAY:
		UpdatePlay();
		break;
	}

	player_->Update();

	weapon_->Update();

	weapon_->SetTransForm(transform_);
	// ���f������X�V
	transform_.Update();
	transformPlayer_.Update();
	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void Bike::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
	MV1DrawModel(transformPlayer_.modelId);

	// ����
	weapon_->Draw();

	// �̗͂Ƃ��Q�[�W�Ƃ�
	DrawUI();

	// �ۉe�`��
	DrawShadow();

	//player_->Draw();

	// �f�o�b�O�`��
	DrawDebug();
}

void Bike::AddCollider(Collider* collider)
{
	colliders_.push_back(collider);
}

void Bike::ClearCollider(void)
{
	colliders_.clear();
}

const Capsule* Bike::GetCapsule(void) const
{
	return capsule_;
}

void Bike::InitAnimation(void)
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

void Bike::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case Bike::STATE::NONE:
		ChangeStateNone();
		break;
	case Bike::STATE::PLAY:
		ChangeStatePlay();
		break;
	}
}

void Bike::ChangeStateNone(void)
{
}

void Bike::ChangeStatePlay(void)
{
}

void Bike::UpdateNone(void)
{
}

void Bike::UpdatePlay(void)
{

	// �ړ�����
	ProcessMove();

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
	transformPlayer_.quaRot = playerRotY_;
}

void Bike::DrawUI(void)
{
	int sc_x = Application::SCREEN_SIZE_X;
	int sc_y = Application::SCREEN_SIZE_Y;

	// HP�Q�[�W
	//DrawBox(sc_x - 500, sc_y - 100,
	//	sc_x - 10, sc_y - 10,
	//	0x00aeef, true);

	// �`��pHP
	int drawHp_ = hp_;
	drawHp_ *5;

	// HP�Q�[�W
	DrawBox(sc_x - 500, sc_y - 100,
		sc_x - 500 + drawHp_, sc_y - 10,
		0x00aeef, true);

	// HP�̍��g
	DrawBoxAA(sc_x - 500, sc_y - 100,
		sc_x - 10, sc_y - 10,
		0x000000, false, 13.0f);

	// HP
	DrawFormatString(0, 20, 0x000000, "HP : %d",hp_);
}

void Bike::DrawShadow(void)
{
}

void Bike::DrawDebug(void)
{
	capsule_->Draw();
	DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);
	
	// �U��������������
	if (isAttack_ == true)
	{
		DrawString(0, 0, "Attack", 0x000000);
	}
}

void Bike::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// �ړ��ʂ��[��
	movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;
	

	VECTOR dir = AsoUtility::VECTOR_ZERO;

	if (ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		rotRad = AsoUtility::Deg2RadD(0.0);
		dir = cameraRot.GetForward();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_UP))
	{
		rotRad = AsoUtility::Deg2RadD(0.0);
		dir = cameraRot.GetForward();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN))
	{
		rotRad = AsoUtility::Deg2RadD(180.0);
		dir = cameraRot.GetBack();
	}

	// �J���������ɑO�i������
	if (ins.IsNew(KEY_INPUT_W))
	{
		rotRad = AsoUtility::Deg2RadD(0.0);
		dir = cameraRot.GetForward();
	}

	// �J�������������ނ�����
	if (ins.IsNew(KEY_INPUT_S))
	{
		rotRad = AsoUtility::Deg2RadD(180.0);
		dir = cameraRot.GetBack();
	}

	// �J������������E���ֈړ�������
	if (ins.IsNew(KEY_INPUT_D))
	{
		//rotRad = AsoUtility::Deg2RadD(90.0);
		dir = cameraRot.GetRight();
	}

	// �J�����������獶���ֈړ�������
	if (ins.IsNew(KEY_INPUT_A))
	{
		//rotRad = AsoUtility::Deg2RadD(270.0);
		transform_.rot.z += 90.0f;
		dir = cameraRot.GetLeft();
	}

	if (!AsoUtility::EqualsVZero(dir) /*&& (isJump_)*/) {


		// �ړ�����
		speed_ = SPEED_MOVE;

		if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D))
		{
			speed_ = SPEED_MOVE_X;
		}
		if (ins.IsNew(KEY_INPUT_RSHIFT))
		{
			speed_ = SPEED_RUN;
			if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D))
			{
				speed_ = SPEED_MOVE_X;
			}
		}
		moveDir_ = dir;
		movePow_ = VScale(dir, speed_);


		// ��]����
		SetGoalRotate(rotRad);

		if (!isJump_ && IsEndLanding())
		{
			//�A�j���[�V����
			if (ins.IsNew(KEY_INPUT_RSHIFT))
			{
				animationController_->Play((int)ANIM_TYPE::FAST_RUN);
			}
			else
			{
				animationController_->Play((int)ANIM_TYPE::RUN);
			}
		}

	}
	else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}

}

void Bike::ProcessJump(void)
{
}

void Bike::ProcessAttack(void)
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

void Bike::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();

	if (ins.IsNew(KEY_INPUT_C))
	{
		hp_ -= 1;
	}
}

void Bike::SetGoalRotate(double rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((double)cameraRot.y + rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Bike::Rotate(void)
{
	stepRotTime_ -= scnMng_.GetDeltaTime();

	// ��]�̋��ʕ��
	playerRotY_ = Quaternion::Slerp(
		playerRotY_, goalQuaRot_, (TIME_ROT - stepRotTime_) / TIME_ROT);
}

void Bike::Collision(void)
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
	transformPlayer_.pos = movedPos_;
}

void Bike::CollisionGravity(void)
{
	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

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

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// ���n���[�V����
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			isJump_ = false;

		}

	}
}

void Bike::CollisionCapsule(void)
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

void Bike::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

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

bool Bike::IsEndLanding(void)
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

