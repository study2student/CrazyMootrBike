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

#pragma region �ϐ��錾
	// �v���C���[�̃��f���̑傫��
	const float PLAYER_SCALE = 1.3f;

	// �����ʒu
	const VECTOR INIT_POS = { 1270.0f , -260.0f ,0.0f };
	const VECTOR INIT_LOCAL_POS = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// �����蔻��p�R���C�_�[�̏���W
	const VECTOR COLLIDER_POS_TOP = { 0.0f, 130.0f, 0.0f };
	// �����蔻��p�R���C�_�[�̉����W
	const VECTOR COLLIDER_POS_DOWN = { 0.0f, 130.0f, -150.0f };

	// �X�s�[�h
	const float SPEED_MOVE = 100.0f;

	// �v���C���[�̌X��
	const float SLOPE = 45.0f;

	// �u�[�X�g�g�p���̉������x
	const float ADD_SPEED_BOOST = 50.0f;
	// ���u�[�X�g�g����悤�ɂȂ�܂ł̃J�E���g
	const float DELEY_BOOST_MAX = 220.0f;
	// �u�[�X�g���������邽�߂�HP�����
	const int BOOST_USE_HP = 40;

	// ���ړ��̃X�s�[�h
	const float SPEED_MOVE_X = 18.0f;

	// ��]�����܂ł̎���
	const float TIME_ROT = 1.0f;

	// ���h��}�b�N�X����
	const float SWAY_CURRENT_MAX_TIME = 70.0f;

#pragma endregion

Bike::Bike(float localpos, int playerID) :
	
	animationController_(nullptr),
	state_(STATE::NONE),
	isOutSide_(false),
	playerID_(playerID),
	moveSpeed_(0.0f),
	moveDir_({}),
	movePow_({}),
	movedPos_({}),
	localPosX_(localpos),
	playerRotY_({}),
	goalQuaRot_({}),
	stepRotTime_(0.0f),
	currentTime_(0.0f),
	jumpPow_({}),
	stepJump_(0.0f),
	stepJumpSecond_(0.0f),
	colliders_(0),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	hp_(0),
	score_(0),
	flipSpeed_(0.0f),
	flipDir_({}),
	speedBoost_(0.0f),
	deleyBoost_(0.0f),
	isBoost_(false),
	isGoal_(false),
	effectBoostResId_(-1),
	effectBoostPlayId_(-1)
{
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
	transform_.scl = { PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE };
	transform_.pos = { INIT_POS.x + localPosX_, INIT_POS.y, INIT_POS.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler(INIT_LOCAL_POS);

	// ���f���̊�{�ݒ�
	transformPlayer_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::PLAYER));
	transformPlayer_.scl = { PLAYER_SCALE, PLAYER_SCALE, PLAYER_SCALE };
	transformPlayer_.pos = VAdd(transform_.pos, RELATIVE_P2B_POS);
	//transformPlayer_.quaRot = Quaternion();
	transformPlayer_.quaRot = transform_.quaRot;
	transformPlayer_.quaRotLocal =
		Quaternion::Euler(INIT_LOCAL_POS);
	transform_.Update();
	transformPlayer_.Update();

	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �G�t�F�N�g������
	InitEffect();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ COLLIDER_POS_TOP });
	capsule_->SetLocalPosDown({ COLLIDER_POS_DOWN });
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
	case STATE::CRASH:
		UpdateCrash();
		break;
	case Bike::STATE::DEAD:
		UpdateDead();
		break;
	}

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
	ChangeState(STATE::CRASH);

	//HP�����l
	if (hp_ <= MIN_HP)
	{
		hp_ = MIN_HP;
 		ChangeState(STATE::DEAD);
	}
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
	if (state_ == STATE::DEAD || state_ == STATE::CRASH)
	{
		return;
	}

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
	case STATE::NONE:
		ChangeStateNone();
		break;
	case STATE::PLAY:
		ChangeStatePlay();
		break;
	case STATE::FLIPED:
		ChangeStateFliped();
		break;
	case STATE::CRASH:
		ChangeStateCrash();
		break;
	case Bike::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void Bike::ChangeStateNone(void)
{
}

void Bike::ChangeStatePlay(void)
{
	currentTime_ = 0.0f;
}

void Bike::ChangeStateFliped(void)
{
}

void Bike::ChangeStateCrash(void)
{
	moveSpeed_ = 10.0f;
}

void Bike::ChangeStateDead(void)
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

void Bike::UpdateCrash(void)
{

	if (currentTime_ >= SWAY_CURRENT_MAX_TIME)
	{
		transform_.quaRotLocal =Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
		transformPlayer_.quaRotLocal =Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
		ChangeState(STATE::PLAY);
	}
	else
	{
		// Y���N�_�ɗh���ǉ�
		float swayAngle = MyUtility::Deg2RadF(45.0f);  // �h��̊p�x�͈̔�
		float swaySpeed = 0.1f;  // �h��̑��x
		float sway = swayAngle * sinf(swaySpeed * currentTime_);

		// Y������̗h���ݒ�
		transform_.quaRotLocal = Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f) + sway, 0.0f });
		transformPlayer_.quaRotLocal = Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f) + sway, 0.0f });

		currentTime_ += 2.0f;
	}

	// ���̍X�V����
	transform_.Update();
}

void Bike::UpdateDead(void)
{
}

void Bike::DrawDebug(void)
{
	capsule_->Draw();
	//DrawLine3D(gravHitPosUp_, gravHitPosDown_, 0x00ffff);

	DrawFormatString(0, 40, 0xffffff,
		"�o�C�N�̉�]�F%f,%f,%f",
		MyUtility::Rad2DegF(transform_.rot.x),
		MyUtility::Rad2DegF(transform_.quaRot.ToEuler().y),
		MyUtility::Deg2RadF(transform_.quaRotLocal.ToEuler().z));

	DrawFormatString(0, 80, 0xffffff, "bikePos : %f, %f, %f", transform_.pos.x, transform_.pos.y, transform_.pos.z);

}

void Bike::ProcessMove(void)
{
	if (state_ == STATE::DEAD)
	{
		return;
	}

	auto& ins = InputManager::GetInstance();

	// �ړ��ʂ��[��
	movePow_ = MyUtility::VECTOR_ZERO;


	std::array<PlayerInput, 4> playerInputs = { {
	{ DX_INPUT_PAD1, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 1
	{ DX_INPUT_PAD2, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 2
	{ DX_INPUT_PAD3, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }, // Player 3
	{ DX_INPUT_PAD4, JoypadButton::UP, JoypadButton::DOWN, JoypadButton::LEFT, JoypadButton::RIGHT, JoypadButton::ACTION }  // Player 4
} };

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	float rotRad = 0.0f;
	float rotRadZ = 0.0f;

	VECTOR dir = MyUtility::VECTOR_ZERO;

	//�O�ɐi��
	VECTOR movePowF_ = VScale(cameraRot.GetForward(), SPEED_MOVE + speedBoost_);

	// �v���C���[���Ƃ̓��͏���
	const auto& input = playerInputs[playerID_];
	int padState = GetJoypadInputState(input.padId);

	//�E
	if (padState & static_cast<int>(input.right)) {
		rotRadZ = MyUtility::Deg2RadF(-SLOPE);
		dir = cameraRot.GetRight();
	}

	//��
	if (padState & static_cast<int>(input.left)) {
		rotRadZ = MyUtility::Deg2RadF(SLOPE);
		dir = cameraRot.GetLeft();
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

			// �J������������E���ֈړ�������
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
				dir = cameraRot.GetRight();
			}

			// �J�����������獶���ֈړ�������
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
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

			// �J������������E���ֈړ�������
			if (ins.IsNew(KEY_INPUT_D))
			{
				rotRadZ = MyUtility::Deg2RadF(-SLOPE);
				dir = cameraRot.GetRight();
			}

			// �J�����������獶���ֈړ�������
			if (ins.IsNew(KEY_INPUT_A))
			{
				rotRadZ = MyUtility::Deg2RadF(SLOPE);
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
		moveSpeed_ = SPEED_MOVE;


		if (ins.IsNew(KEY_INPUT_A) || ins.IsNew(KEY_INPUT_D) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) &  PAD_INPUT_LEFT || PAD_INPUT_RIGHT))
		{
			moveSpeed_ = SPEED_MOVE_X;
		}

		// ��]����
		SetGoalRotate(rotRad);
		SetGoalRotateZ(rotRadZ);

	}
	else
	{
		//�X�����ςɂȂ�̂Ŋp�x���Z�b�g���Ă���
		rotRad = MyUtility::Deg2RadF(0.0f);
		dir = cameraRot.GetForward();

		// ��]����
		SetGoalRotateZ(rotRadZ);
	}

	//�O�֐i�ރx�N�g���Ɖ��ɋȂ���x�N�g������������
	moveDir_ = dir;
	movePow_ = VAdd(VScale(dir, moveSpeed_), movePowF_);

}

bool Bike::IsBoostPush(void)
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

	return ins.IsTrgDown(KEY_INPUT_E) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP || padState & static_cast<int>(input.action) && deleyBoost_ <= 0 && hp_ > BOOST_USE_HP;
}


void Bike::ProcessBoost(void)
{

	//E�L�[��������pad�̃A�N�V�����{�^���������ƁA�u�[�X�g����
	if (IsBoostPush())
	{
		// �u�[�X�g�����true��
		isBoost_ = true;

		//HP������Ĕ���(�u�[�X�g�Ŏ��ȂȂ��悤��40�ȏ�̏ꍇ�̂�)
		hp_ -= BOOST_USE_HP;

		//�����Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_BOOST).handleId_, DX_PLAYTYPE_BACK, true);

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
	//�u�[�X�g�G�t�F�N�g
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST_EFFECT).handleId_;
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
