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
#include "FrontTyre.h"

FrontTyre::FrontTyre()
{
	state_ = STATE::NONE;


	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	rotX_ = Quaternion();

}

FrontTyre::~FrontTyre(void)
{
}

void FrontTyre::Init(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::TYRE));
	float scale = 1.5f;
	transform_.scl = { scale / 1.40f, scale, scale };
	transform_.pos = { transformParent_.pos.x + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.x,
		transformParent_.pos.y + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.y,
		transformParent_.pos.z + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(0.0f), 0.0f });
	transform_.Update();

	// �������
	ChangeState(STATE::ROT);
}

void FrontTyre::Update(void)
{
	switch (state_)
	{
	case FrontTyre::STATE::NONE:
		UpdateNone();
		break;
	case FrontTyre::STATE::ROT:
		UpdateRot();
		break;
	}

	// ���f������X�V
	transform_.Update();
}

void FrontTyre::Draw(void)
{
	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �f�o�b�O�`��
	DrawDebug();
}

void FrontTyre::SetTransform(Transform& transformParent)
{
	transformParent_ = transformParent;
}

void FrontTyre::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case FrontTyre::STATE::NONE:
		ChangeStateNone();
		break;
	case FrontTyre::STATE::ROT:
		ChangeStateRot();
		break;
	}
}

void FrontTyre::ChangeStateNone(void)
{
}

void FrontTyre::ChangeStateRot(void)
{
}

void FrontTyre::UpdateNone(void)
{
	//�ʒu���킹
	SyncParent();
}

void FrontTyre::UpdateRot(void)
{
	// �f�o�b�O�p
	ProcessDebug();

	// �ړ������ɉ�������]
	Rotate();

	//�ʒu���킹
	SyncParent();
}

void FrontTyre::DrawDebug(void)
{
	DrawFormatString(840, 300, 0xffffff, "frontPos = {%f, %f, %f}", transform_.pos.x,transform_.pos.y,transform_.pos.z);
}

void FrontTyre::ProcessDebug(void)
{
}

void FrontTyre::Rotate(void)
{
	auto& ins = InputManager::GetInstance();

	//�t�����g�^�C���̉�]
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

void FrontTyre::SyncParent(void)
{
	//�e�o�C�N�̈ʒu�ɍ��킹��
	//���W����������
	//transform_.pos = { transformParent_.pos.x,transformParent_.pos.y,transformParent_.pos.z };
	transform_.pos = { transformParent_.pos.x + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.x,
		transformParent_.pos.y + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.y,
		transformParent_.pos.z + Bike::BIKE_TO_FRONT_TYRE_LOCALPOS.z };
	//transform_.pos = VAdd(transform_.pos, Bike::BIKE_TO_FRONT_TYRE_LOCALPOS);
	transform_.quaRot = transformParent_.quaRot;
	//transform_.quaRot.Mult(rotX_, transformParent_.quaRot);
	//// ���f������X�V
	//transform_.Update();
}
