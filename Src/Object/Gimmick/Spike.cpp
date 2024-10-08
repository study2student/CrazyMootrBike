#include <string>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Stage/Planet.h"
#include "../Rider/Bike.h"
#include "Spike.h"

#pragma region �萔�錾

//��]���x
const float SPEED_ROT = 20.0f;

//���x
const float SPEED_MOVE = 60.0f;

//�ꏊ�\������
const float PLACE_DRAW_MAX_TIME = 3.0f;

//������܂ł̎���
const float TO_DELETE_MAX_TIME = 4.5f;

//�Ƃ���������
const float SPIKE_REMAKE_MAX_TIME = 6.0f;

//�E���瓊����ʒu(��������)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_SIDE = { -900.0f,100.0f,13500.0f };

//�E���瓊����ʒu(�����΂߂�)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { -750.0f,100.0f,11000.0f };

//�E���瓊����ʒu(�傫���΂߂�)
const VECTOR RIGHT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { -600.0f,100.0f,7000.0f };

//�����瓊����ʒu(��������)
const VECTOR LEFT_THROW_LOCAL_POS_TO_SIDE = { 600.0f,100.0f,7000.0f };

//�����瓊����ʒu(�����΂߂�)
const VECTOR LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { 770.0f,100.0f,12800.0f };

//�����瓊����ʒu(�傫���΂߂�)
const VECTOR LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { 900.0f,100.0f,13500.0f };

//�E�̏ꍇ�̏o������X���W
const float MAKE_RIGHT_POS_X = 2499.0f;

//���̏ꍇ�̏o������X���W
const float MAKE_LEFT_POS_X = 909.0f;

//���[�J���ҋ@���W
const VECTOR SPIKE_IDLE_ROCAL_POS = { 0.0f,100.0f,15000.0f };

//�������W
const VECTOR INIT_POS = { 1500.0f, 1000.0f, 3000.0f };

// �����傫��
const VECTOR INIT_SCL = { 120.0f,120.0f,120.0f };

// ������]
const VECTOR INIT_ROT = { 0.0f, 0.0f, 0.0f };

// �������[�J����]
const VECTOR INIT_LOCAL_ROT = { 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 30.0f, 0.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 10.0f, 0.0f };

//�J�v�Z�����a
const float CAPSULE_RADIUS = 130.0f;

//�����G�t�F�N�g�傫��
const float MAKE_EFFECT_SCL = 130.0f;

//�����G�t�F�N�g���[�J�����W
const VECTOR MKAE_EFFECT_LOCAL_POS = { 0.0f,300.0f,0.0f };

//�����G�t�F�N�g�傫��
const float BOMB_EFFECT_SCL = 30.0f;

#pragma endregion


Spike::Spike()
	:
	transformTarget_(Transform()),
	state_(STATE::IDLE),
	targetDir_({}),
	targetDirSave_({}),
	rotX_(Quaternion()),
	isCol_(false),
	stepPlaceDrawTime_(0.0f),
	stepToDeleteTime_(0.0f),
	stepSpikeDestroy_(0.0f),
	makeEffectResId_(-1),
	makeEffectPlayId_(-1),
	bombEffectResId_(-1),
	bombEffectPlayId_(-1)
{
	//�ʒu��]�傫��
	initScl_ = INIT_SCL;
	initRotEuler_ = INIT_ROT;
	initLocalRotEuler_ = INIT_LOCAL_ROT;
	initPos_ = INIT_POS;

	//�J�v�Z��
	capsulePosTop_ = CAPSULE_LOCAL_POS_TOP;
	capsulePosDown_ = CAPSULE_LOCAL_POS_DOWN;
	capsuleRadius_ = CAPSULE_RADIUS;

	//��]
	rotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;
}

Spike::~Spike(void)
{
}

void Spike::Update(void)
{
	switch (state_)
	{
	case Spike::STATE::IDLE:
		UpdateIdle();
		break;
	case Spike::STATE::THROW_MOVE:
		UpdateThrowMove();
		break;
	case Spike::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void Spike::Draw(void)
{

	//�����Ă��鎞�ȊO�͕`�悵�Ȃ�
	if (!(state_ == STATE::THROW_MOVE))
	{
		return;
	}

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);
}

void Spike::SetTransform(Transform transformTarget)
{
	transformTarget_ = transformTarget;
}

void Spike::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

const bool& Spike::GetIsCol(void) const
{
	return isCol_;
}

const bool Spike::IsIdle(void)
{
	return state_ == STATE::IDLE;
}

void Spike::InitEffect(void)
{
	makeEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::THROW_MAKE_EFFECT).handleId_;

	bombEffectResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void Spike::PlayMakeEffect(void)
{
	makeEffectPlayId_ = PlayEffekseer3DEffect(makeEffectResId_);
	SetScalePlayingEffekseer3DEffect(makeEffectPlayId_, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL);
	VECTOR localPos = MKAE_EFFECT_LOCAL_POS;
	SetPosPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.pos.x, transform_.pos.y + localPos.y, transform_.pos.z);
	SetRotationPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void Spike::PlayBombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//�����ɓ����������͔���
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, BOMB_EFFECT_SCL, BOMB_EFFECT_SCL, BOMB_EFFECT_SCL);
}

void Spike::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case Spike::STATE::IDLE:
		ChangeStateIdle();
		break;
	case Spike::STATE::THROW_MOVE:
		ChangeStateThrow();
		break;
	case Spike::STATE::DESTROY:
		ChangeStateDestroy();
		break;
	}
}

void Spike::ChangeStateIdle(void)
{
	isCol_ = false;
}


void Spike::ChangeStateThrow(void)
{

	transform_.pos = VAdd(transformTarget_.pos, SPIKE_IDLE_ROCAL_POS);

	//�ǂ����瓊���邩�����_���Ō��߂�
	int randDir = GetRand(static_cast<int>(Spike::DIR::MAX) - 1);
	Spike::DIR dir = static_cast<Spike::DIR>(randDir);

	switch (dir)
	{
	case Spike::DIR::LEFT:
		transform_.pos.x = MAKE_LEFT_POS_X;
		break;
	case Spike::DIR::RIGHT:
		transform_.pos.x = MAKE_RIGHT_POS_X;
		break;
	case Spike::DIR::MAX:
		break;
	}

	//�����G�t�F�N�g
	PlayMakeEffect();

	//�����_����3�p�^�[���̓���
	int randAngle = GetRand(static_cast<int>(Spike::ANGLE::MAX) - 1);
	Spike::ANGLE angle = static_cast<Spike::ANGLE>(randAngle);
	VECTOR targetPos;
	VECTOR throwLocalPos;

	switch (angle)
	{
	case Spike::ANGLE::SIDE:
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
	case Spike::ANGLE::SLIGHTLY_OBLIPUE:
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
	case Spike::ANGLE::LARGE_OBLIPUE:
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


void Spike::ChangeStateDestroy(void)
{
	//�����G�t�F�N�g
	PlayBombEffect();
}

void Spike::UpdateIdle(void)
{
	//�v���C���[�̏�����őҋ@
	transform_.pos = VAdd(transformTarget_.pos, SPIKE_IDLE_ROCAL_POS);

	//3�b��ɓ�����
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//���e����������ԂɈڍs
		ChangeState(STATE::THROW_MOVE);
		stepPlaceDrawTime_ = 0.0f;
	}
}

void Spike::UpdateThrowMove(void)
{

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

void Spike::UpdateDestroy(void)
{
	//8�b��ɕ���
	stepSpikeDestroy_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepSpikeDestroy_ >= SPIKE_REMAKE_MAX_TIME)
	{
		//���e�������ˑO��ԂɈڍs
		ChangeState(STATE::IDLE);
		stepSpikeDestroy_ = 0.0f;
	}
}

void Spike::Rotate(void)
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

void Spike::InitLoad(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::SPIKE_BALL));

	//�G�t�F�N�g�ǂݍ���
	InitEffect();
}

void Spike::InitPost(void)
{
	// �������
	ChangeState(STATE::DESTROY);
}
