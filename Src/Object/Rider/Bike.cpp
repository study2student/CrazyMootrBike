#include <string>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../..//Utility/MyUtility.h"
#include "../..//Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Stage/Planet.h"
#include "../Score.h"
#include "Bike.h"

Bike::Bike(float localpos, int playerID) : localPosX_(localpos), playerID_(playerID)
{
	//localPosX_ = localpos;

	animationController_ = nullptr;

	state_ = STATE::NONE;

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
	stepJumpSecond_ = 0.0f;
	jumpSpeed_ = 1.0f;

	isAttack_ = false;
	isOutSide_ = false;
	isGoal_ = false;

	// �u�[�X�g���̉��Z���x
	speedBoost_ = 0.0f;

	// �u�[�X�g�g�p�Ԋu
	deleyBoost_ = 0;

	// �u�[�X�g���g�p������
	isBoost_ = false;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	imgShadow_ = -1;

	hp_ = 0;

	capsule_ = nullptr;

}

Bike::~Bike(void)
{

	StopEffekseer3DEffect(effectBoostPlayId_);
}

void Bike::Init(void)
{


	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::BIKE));
	float scale = 1.3f;
	transform_.scl = { scale, scale, scale };
	VECTOR initPos = { 1270.0f,-260.0f,0.0f };
	transform_.pos = { 1270.0f + localPosX_, -260.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	//transform_.Update();

	// ���f���̊�{�ݒ�
	transformPlayer_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	float pScale = 1.3f;
	transformPlayer_.scl = { pScale, pScale, pScale };
	//transformPlayer_.pos = { transform_.pos.x, transform_.pos.y , transform_.pos.z };
	transformPlayer_.pos = VAdd(transform_.pos, RELATIVE_P2B_POS);
	//transformPlayer_.quaRot = Quaternion();
	transformPlayer_.quaRot = transform_.quaRot;
	transformPlayer_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();
	transformPlayer_.Update();


	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �G�t�F�N�g������
	InitEffect();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 130.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 130.0f, -150.0f });
	capsule_->SetRadius(RADIUS);

	// �̗�
	hp_ = MAX_HP;


	// �X�R�A
	score_ = std::make_shared<Score>();
	score_->Init();

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
	case Bike::STATE::FLIPED:
		UpdateFliped();
		break;
	}

	ChangeVolumeSoundMem(10, PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false));


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


	// �̗͂Ƃ��Q�[�W�Ƃ�
	DrawUI();

	// �ۉe�`��
	DrawShadow();


	// �f�o�b�O�`��
	//DrawDebug();
}

void Bike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Bike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Bike::GetCapsule(void) const
{
	return capsule_;
}

void Bike::Damage(int damage)
{
	hp_ -= damage;
}

const int& Bike::GetHP(void)
{
	return hp_;
}

const bool& Bike::GetIsOutSide(void)
{
	return isOutSide_;
}

void Bike::SetIsGoal(bool isGoal)
{
	isGoal_ = isGoal;
}

const bool& Bike::GetIsGoal(void)
{
	return isGoal_;
}

void Bike::AddScore(int score)
{
	score_->AddScore(score);
}

const int Bike::GetScore() const
{
	return score_->GetScore();
}

const int Bike::GetPlayerID(void) const
{
	return playerID_;
}

void Bike::Flip(VECTOR dir)
{
	flipDir_ = dir;
	flipSpeed_ = 5.0f;
	ChangeState(STATE::FLIPED);
}

const bool Bike::IsBoost(void)
{
	return isBoost_;
}

void Bike::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = std::make_unique<AnimationController>(transformPlayer_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::SIT, path + "Sit.mv1", 1.0f);

	animationController_->Play((int)ANIM_TYPE::SIT, true, 33.0f, 36.0f);
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
	case Bike::STATE::FLIPED:
		ChangeStateFliped();
		break;
	}
}

void Bike::ChangeStateNone(void)
{
}

void Bike::ChangeStatePlay(void)
{
}

void Bike::ChangeStateFliped(void)
{
}


void Bike::UpdateNone(void)
{
}



void Bike::UpdatePlay(void)
{

	// �ړ�����
	ProcessMove();

	//�u�[�X�g
	ProcessBoost();

	// �f�o�b�O�p
	ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
	transformPlayer_.quaRot = playerRotY_;

	//�A�j���[�V�������[�v
	animationController_->SetEndLoop(33.0f, 36.0f, 1.0f);

	//HP�����l
	if (hp_ <= MIN_HP)
	{
		hp_ = MIN_HP;
		state_ = STATE::DEAD;
	}
}

void Bike::UpdateFliped(void)
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

	// �ړ������ɉ�������]
	Rotate();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = playerRotY_;
}

void Bike::DrawUI(void)
{
	using ap = Application;
	int sc_x = ap::SCREEN_SIZE_X - 500;
	int sc_y = ap::SCREEN_SIZE_Y - 100;

	// HP�o�[�̕�
	int HP_BAR_WIDTH = ap::SCREEN_SIZE_X - 10 - sc_x;
	// HP�o�[�̍���
	int HP_BAR_HEIGHT = ap::SCREEN_SIZE_Y - 10;
	// HP�o�[��`��
	DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HP�o�[�̔w�i
	DrawBox(sc_x, sc_y, sc_x + (hp_ * HP_BAR_WIDTH) / MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HP�o�[

	// HP�̍��g
	DrawBoxAA(sc_x, sc_y,
		ap::SCREEN_SIZE_X - 10, HP_BAR_HEIGHT,
		0x000000, false, 13.0f);
}

void Bike::DrawShadow(void)
{
}

void Bike::DrawDebug(void)
{
	//capsule_->Draw();
	//DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	// �U��������������
	if (isAttack_ == true)
	{
		DrawString(0, 0, "Attack", 0x000000);
	}

	DrawFormatString(0, 40, 0xffffff,
		"�o�C�N�̉�]�F%f,%f,%f",
		MyUtility::Rad2DegD(transform_.rot.x),
		MyUtility::Rad2DegF(transform_.quaRot.ToEuler().y),
		MyUtility::Deg2RadF(transform_.quaRotLocal.ToEuler().z));

	DrawFormatString(0, 80, 0xffffff, "bikePos : %f, %f, %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Bike::ProcessMove(void)
{


	auto& ins = InputManager::GetInstance();

	// �ړ��ʂ��[��
	movePow_ = MyUtility::VECTOR_ZERO;


	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;

	std::array<PlayerInput, 4> playerInputs = { {
		{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
		{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
		{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
		{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
	} };

	VECTOR dir = MyUtility::VECTOR_ZERO;

	//�O�ɐi��
	VECTOR movePowF_ = VScale(cameraRot.GetForward(), SPEED_MOVE + speedBoost_);

	// �v���C���[���Ƃ̓��͏���
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	if (padState & static_cast<int>(input.right)) {
		rotRadZ = MyUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetRight();
	}

	if (padState & static_cast<int>(input.left)) {
		rotRadZ = MyUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_RIGHT))
	{
		rotRadZ = MyUtility::Deg2RadD(-45.0f);
		dir = cameraRot.GetRight();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT))
	{
		rotRadZ = MyUtility::Deg2RadD(45.0f);
		dir = cameraRot.GetLeft();
	}

	if (static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_DOWN))
	{
		rotRad = MyUtility::Deg2RadD(180.0f);
		dir = cameraRot.GetBack();
	}

	//��������Ă��܂�����߂�
	//�E��
	if (transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX)
	{
		dir = MyUtility::DIR_L;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//��O�ɂłĂ���
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
		{
			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);
			// �J�������������ނ�����
			if (ins.IsNew(KEY_INPUT_S))
			{
				rotRad = MyUtility::Deg2RadD(180.0f);
				dir = cameraRot.GetBack();
			}

			// �J������������E���ֈړ�������
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadD(-45.0f);
				dir = cameraRot.GetRight();
			}

			// �J�����������獶���ֈړ�������
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadD(45.0f);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//����
	if (transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX)
	{
		dir = MyUtility::DIR_R;
		SceneManager::GetInstance().GetCamera()->SetIsCameraReset(true);

		//��O�ɂłĂ���
		isOutSide_ = true;
	}
	else
	{
		if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX))
		{

			SceneManager::GetInstance().GetCamera()->SetIsCameraReset(false);
			// �J�������������ނ�����
			if (ins.IsNew(KEY_INPUT_S))
			{
				rotRad = MyUtility::Deg2RadD(180.0f);
				dir = cameraRot.GetBack();
			}

			// �J������������E���ֈړ�������
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadD(-45.0f);
				dir = cameraRot.GetRight();
			}

			// �J�����������獶���ֈړ�������
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadD(45.0f);
				dir = cameraRot.GetLeft();

			}
		}
	}

	//��O�ɂłĂ��Ȃ�
	if (!(transform_.pos.x >= Stage::STAGE_RIGHT_POS_X_MAX) && !(transform_.pos.x <= Stage::STAGE_LEFT_POS_X_MAX))
	{
		isOutSide_ = false;
	}


	if (!MyUtility::EqualsVZero(dir) /*&& (isJump_)*/) {

		// �ړ�����
		speed_ = SPEED_MOVE;


		if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_LEFT || PAD_INPUT_RIGHT))
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


		// ��]����
		SetGoalRotate(rotRad);
		SetGoalRotateZ(rotRadZ);

	}
	else
	{
		//�X�����ςɂȂ�̂Ŋp�x���Z�b�g���Ă���
		rotRad = MyUtility::Deg2RadD(0.0f);
		dir = cameraRot.GetForward();

		// ��]����
		SetGoalRotateZ(rotRadZ);
	}


	//�O�֐i�ރx�N�g���Ɖ��ɋȂ���x�N�g������������
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, speed_), movePowF_);

	// �\�j�b�N�u�[���G�t�F�N�g
	float scale = 10.0f;
	SonicBoomEffect();
	effectSonicPlayId_ = PlayEffekseer3DEffect(effectSonicResId_);
	SetScalePlayingEffekseer3DEffect(effectSonicPlayId_, scale, scale, scale);

}


void Bike::ProcessBoost(void)
{
	auto& ins = InputManager::GetInstance();

	std::array<PlayerInput, 4> playerInputs = { {
		{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
		{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
		{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
		{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
	} };

	// �v���C���[���Ƃ̓��͏���
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	if (ins.IsTrgDown(KEY_INPUT_E) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP || padState & static_cast<int>(input.action) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP)
	{
		// �u�[�X�g�����true��
		isBoost_ = true;

		//HP������Ĕ���(�u�[�X�g�Ŏ��ȂȂ��悤��40�ȏ�̏ꍇ�̂�)
		hp_ -= BOOST_USE_HP;

		SceneManager::GetInstance().GetCamera()->SetIsBoost(true);
		deleyBoost_ = DELEY_BOOST_MAX;
		speedBoost_ = ADD_SPEED_BOOST;
		effectBoostPlayId_ = PlayEffekseer3DEffect(effectBoostResId_);

		// �傫��
		float boostSCALE = 30.0f;
		SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, boostSCALE, boostSCALE, boostSCALE);
		SyncBoostEffect(transform_);
	}
	else
	{
		isBoost_ = false;

		deleyBoost_--;
		if (deleyBoost_ <= 0)
		{
			deleyBoost_ = 0.0f;
		}
		speedBoost_ -= 0.340f;
		if (speedBoost_ <= 0)
		{
			speedBoost_ = 0.0f;
			SceneManager::GetInstance().GetCamera()->SetIsBoost(false);
		}

		SyncBoostEffect(transform_);
	}
}

void Bike::ProcessDebug(void)
{
	auto& ins = InputManager::GetInstance();

	/*if (ins.IsNew(KEY_INPUT_C))
	{
		hp_ -= 1;
	}*/
}

void Bike::SetGoalRotate(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis((float)cameraRot.y + rotRad, MyUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void Bike::SetGoalRotateZ(float rotRad)
{
	VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(-1.0f * (float)cameraRot.z + rotRad, MyUtility::AXIS_Z);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	float angleDiff = Quaternion::Angle(axis, goalQuaRot_);

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

	VECTOR movedPPos = VAdd(VAdd(transform_.pos, RELATIVE_P2B_POS), movePow_);
	//movedPos_ = VAdd(transformPlayer_.pos, movmoePow_);

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
	transformPlayer_.pos = movedPPos;

}

void Bike::CollisionGravity(void)
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
			stepJump_ = 0.0f;

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

void Bike::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectSonicResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SONICEFFECT).handleId_;

	//�u�[�X�g�G�t�F�N�g
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST_EFFECT).handleId_;
}

void Bike::SonicBoomEffect(void)
{
	SetPosPlayingEffekseer3DEffect(effectSonicPlayId_, transform_.pos.x, transform_.pos.y + 200.0f, transform_.pos.z + 1000);
	//�p�x�̓���
	VECTOR angles = transform_.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(effectSonicPlayId_, angles.x, angles.y, angles.z);
	// �傫��
	float scale = 10.0f;
	SetScalePlayingEffekseer3DEffect(effectSonicPlayId_, scale, scale, scale);
}

void Bike::SyncBoostEffect(Transform player)
{
	VECTOR pos = player.pos;

	pos = Quaternion::PosAxis(transform_.quaRot, RELATIVE_P2EB_POS);
	pos = VAdd(player.pos, pos);
	SetPosPlayingEffekseer3DEffect(effectBoostPlayId_, pos.x, pos.y, pos.z);

	//�p�x�̓���
	//VECTOR angles = transform_.quaRot.ToEuler();
	VECTOR angles = player.quaRot.ToEuler();
	SetRotationPlayingEffekseer3DEffect(effectBoostPlayId_, angles.x, angles.y, angles.z);

	// �傫��
	float boostSCALE = 30.0f;
	SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, boostSCALE, boostSCALE, boostSCALE);
}
