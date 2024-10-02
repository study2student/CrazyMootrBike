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

//�Ƃ��̑傫��
const float SCL = 120.0f;

//�������W
const VECTOR INIT_POS = { 1500.0f, 1000.0f, 3000.0f };

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
	movePow_({}),
	movedPos_({}),
	rotX_(Quaternion()),
	isCol_(false),
	stepPlaceDrawTime_(0.0f),
	stepToDeleteTime_(0.0f),
	stepSpikeDestroy_(0.0f),
	colliders_({}),
	capsule_(nullptr),
	gravHitPosDown_({}),
	gravHitPosUp_({}),
	makeEffectResId_(-1),
	makeEffectPlayId_(-1),
	bombEffectResId_(-1),
	bombEffectPlayId_(-1)
{
}

Spike::~Spike(void)
{
}

void Spike::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::SPIKE_BALL));
	transform_.scl = { SCL , SCL, SCL };
	transform_.pos = INIT_POS;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop(CAPSULE_LOCAL_POS_TOP);
	capsule_->SetLocalPosDown(CAPSULE_LOCAL_POS_DOWN);
	capsule_->SetRadius(CAPSULE_RADIUS);

	//�G�t�F�N�g
	InitEffect();

	// �������
	ChangeState(STATE::DESTROY);
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

void Spike::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void Spike::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> Spike::GetCapsule(void) const
{
	return capsule_;
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

void Spike::MakeEffect(void)
{
	makeEffectPlayId_ = PlayEffekseer3DEffect(makeEffectResId_);
	SetScalePlayingEffekseer3DEffect(makeEffectPlayId_, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL, MAKE_EFFECT_SCL);
	VECTOR localPos = MKAE_EFFECT_LOCAL_POS;
	SetPosPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.pos.x, transform_.pos.y + localPos.y, transform_.pos.z);
	SetRotationPlayingEffekseer3DEffect(makeEffectPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
}

void Spike::BombEffect(void)
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
	MakeEffect();

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
	BombEffect();
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

void Spike::Collision(void)
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

void Spike::CollisionGravity(void)
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

void Spike::CollisionCapsule(void)
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
