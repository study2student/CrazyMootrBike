#include <EffekseerForDXLib.h>
#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../Common/Transform.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../Stage/Planet.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "Bomb.h"

#pragma region �萔�錾

//���e�ꏊ�\������
const float PLACE_DRAW_MAX_TIME = 5.0f;

//���e������������
const float RESERVE_MAX_TIME = 2.5f;

//���e��������
const float BOMB_REMAKE_MAX_TIME = 0.8f;

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_TOP = { 0.0f, 30.0f, 0.0f };

//�J�v�Z�����[�J�����W��
const VECTOR CAPSULE_LOCAL_POS_DOWN = { 0.0f, 10.0f, 0.0f };

//�J�v�Z�����a
const float CAPSULE_RADIUS = 100.0f;

//�G�t�F�N�g�傫��
const float EFFECT_SCL = 30.0f;

// �X�s�[�h
const float SPEED = 30.0f;

// �����傫��
const VECTOR INIT_SCL = { 0.15f,0.15f,0.15f };

// ������]
const VECTOR INIT_ROT = { 0.0f, 0.0f, 0.0f };

// �������[�J����]
const VECTOR INIT_LOCAL_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

#pragma endregion


Bomb::Bomb()
	:
	heliTrans_(Transform()),
	state_(STATE::NONE),
	stepPlaceDrawTime_(0.0f),
	stepReserveTime_(0.0f),
	stepBombBlast_(0.0f),
	isCol_(false),
	bombEffectResId_(-1),
	bombEffectPlayId_(-1),
	bombTargetPos_({})
{
	//�ʒu��]�傫��
	initScl_ = INIT_SCL;
	initRotEuler_ = INIT_ROT;
	initLocalRotEuler_ = INIT_LOCAL_ROT;
  	initPos_ = heliTrans_.pos;

	//�J�v�Z��
	capsulePosTop_ = CAPSULE_LOCAL_POS_TOP;
	capsulePosDown_ = CAPSULE_LOCAL_POS_DOWN;
	capsuleRadius_ = CAPSULE_RADIUS;
}

Bomb::~Bomb(void)
{
	StopEffekseer3DEffect(bombEffectPlayId_);
}

void Bomb::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case Bomb::STATE::NONE:
		UpdateNone();
		break;
	case Bomb::STATE::IDLE:
		UpdateIdle();
		break;
	case Bomb::STATE::RESERVE:
		UpdateReserve();
		break;
	case Bomb::STATE::BLAST:
		UpdateBlast();
		break;
	}
}

void Bomb::Draw(void)
{

	// �X�V�X�e�b�v
	switch (state_)
	{
	case Bomb::STATE::NONE:
		break;
	case Bomb::STATE::IDLE:
		DrawBombPlace();
		break;
	case Bomb::STATE::RESERVE:
		MV1DrawModel(transform_.modelId);
		break;
	case Bomb::STATE::BLAST:
		break;
	}

}

void Bomb::SetHeliTrans(const Transform& heliTrans)
{
	heliTrans_ = heliTrans;
}

void Bomb::SetIsCol(bool isCol)
{
	isCol_ = isCol;
}

const bool& Bomb::GetIsCol(void) const
{
	return isCol_;
}

const Bomb::STATE& Bomb::GetState(void) const
{
	return state_;
}

void Bomb::InitEffectLoad(void)
{
	bombEffectResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOMB_EFFECT).handleId_;
}

void Bomb::PlayBombEffect(void)
{
	bombEffectPlayId_ = PlayEffekseer3DEffect(bombEffectResId_);

	//�����ɓ����������͔���
	SetPosPlayingEffekseer3DEffect(bombEffectPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z);
	SetScalePlayingEffekseer3DEffect(bombEffectPlayId_, EFFECT_SCL, EFFECT_SCL, EFFECT_SCL);
}

void Bomb::ChangeState(STATE state)
{
	//��ԕύX
	state_ = state;

	// �X�V�X�e�b�v
	switch (state_)
	{
	case Bomb::STATE::NONE:
		ChangeStateNone();
		break;
	case Bomb::STATE::IDLE:
		ChangeStateIdle();
		break;
	case Bomb::STATE::RESERVE:
		ChangeStateReserve();
		break;
	case Bomb::STATE::BLAST:
		ChangeStateBlast();
		break;
	}
}

void Bomb::ChangeStateNone(void)
{
}

void Bomb::ChangeStateIdle(void)
{
	isCol_ = false;
}

void Bomb::ChangeStateReserve(void)
{
}

void Bomb::ChangeStateBlast(void)
{
	//�����G�t�F�N�g
	PlayBombEffect();
}

void Bomb::UpdateNone(void)
{
}

void Bomb::UpdateIdle(void)
{
	//�w���ɏ悹�Ƃ�
	transform_.pos = heliTrans_.pos;

	//5�b��ɐݒu
	stepPlaceDrawTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepPlaceDrawTime_ >= PLACE_DRAW_MAX_TIME)
	{
		//���e����������ԂɈڍs
		ChangeState(STATE::RESERVE);
		stepPlaceDrawTime_ = 0.0f;
	}
}

void Bomb::UpdateReserve(void)
{
	// �e���ړ�������
	VECTOR movePow;
	VECTOR targetDir = VNorm(VSub(bombTargetPos_, transform_.pos));
	movePow = VScale(targetDir, SPEED);

	// �ړ�����
	transform_.pos = VAdd(transform_.pos, movePow);

	// �d�͂ɂ��ړ���
	CalcGravityPow();
	// �Փ˔���
	Collision();
	transform_.Update();

	//3�b��ɔ���
	stepReserveTime_ += SceneManager::GetInstance().GetDeltaTime();

	//�����ɓ������Ă���������
	if (stepReserveTime_ >= RESERVE_MAX_TIME || isCol_)
	{
		//���e������ԂɈڍs
		ChangeState(STATE::BLAST);
		stepReserveTime_ = 0.0f;
	}

}

void Bomb::UpdateBlast(void)
{
	// �d�͂ɂ��ړ���
	CalcGravityPow();
	// �Փ˔���
	Collision();
	transform_.Update();

	//1.5�b��ɕ���
	stepBombBlast_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepBombBlast_ >= BOMB_REMAKE_MAX_TIME)
	{
		//���e�������ˑO��ԂɈڍs
		ChangeState(STATE::IDLE);
		stepBombBlast_ = 0.0f;
	}
}

void Bomb::DrawBombPlace(void)
{
	VECTOR localPos1 = { 0.0f,-600.0f,-390.0f };
	VECTOR localPos2 = { 0.0f,-590.0f,-390.0f };
	VECTOR pos1 = VAdd(heliTrans_.pos, localPos1);
	VECTOR pos2 = VAdd(heliTrans_.pos, localPos2);
	DrawCapsule3D(pos1, pos2, 100.0f, 1, 0xff0000, 0xffffff, 0);

	//�����ڕW�̐ݒ�
	bombTargetPos_ = pos1;
	transform_.pos = bombTargetPos_;
	transform_.Update();
}

void Bomb::CalcGravityPow(void)
{
	// �d�͕���
	VECTOR dirGravity = MyUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

}

void Bomb::InitLoad(void)
{
	//���f���ǂݍ���
	transform_.modelId = MV1DuplicateModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BOMB));

	//�����G�t�F�N�g�ǂݍ���
	InitEffectLoad();
}

void Bomb::InitPost(void)
{
	//�ҋ@���
	ChangeState(STATE::IDLE);
}
