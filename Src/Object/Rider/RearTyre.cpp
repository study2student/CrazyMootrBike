#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../Planet.h"
#include "Bike.h"
#include "RearTyre.h"

RearTyre::RearTyre()
{
	state_ = STATE::NONE;


	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();
}

RearTyre::~RearTyre(void)
{
}

void RearTyre::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TYRE));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 1670.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// �������
	ChangeState(STATE::ROT);
}

void RearTyre::Update(void)
{
	switch (state_)
	{
	case RearTyre::STATE::NONE:
		UpdateNone();
		break;
	case RearTyre::STATE::ROT:
		UpdateRot();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void RearTyre::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
	DrawDebug();
}

void RearTyre::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void RearTyre::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case RearTyre::STATE::NONE:
		ChangeStateNone();
		break;
	case RearTyre::STATE::ROT:
		ChangeStateRot();
		break;
	}
}

void RearTyre::ChangeStateNone(void)
{
}

void RearTyre::ChangeStateRot(void)
{
}

void RearTyre::UpdateNone(void)
{
	//�ʒu���킹
	SyncParent();
}

void RearTyre::UpdateRot(void)
{
	// �f�o�b�O�p
	ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	//�ʒu���킹
	SyncParent();
}

void RearTyre::DrawDebug(void)
{
}

void RearTyre::ProcessDebug(void)
{
}

void RearTyre::Rotate(void)
{
	auto& ins = InputManager::GetInstance();

	//�H�̉�]
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

void RearTyre::SyncParent(void)
{
	//�e�o�C�N�̈ʒu�ɍ��킹��
	//���W����������
	//transform_.pos = transformParent_.pos;
	transform_.pos = { transformParent_.pos.x, 0.0f, transformParent_.pos.z };
	transform_.pos = VAdd(transform_.pos, LOCAL_POS);
	transform_.quaRot = transformParent_.quaRot;
	transform_.quaRot.Mult(transform_.quaRot, transformParent_.quaRot);
}
