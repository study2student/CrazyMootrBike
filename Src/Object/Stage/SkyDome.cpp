#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/MyUtility.h"
#include "../Common/Transform.h"
#include "SkyDome.h"

#pragma region �萔�錾

//�傫���֘A
const float STAGE_SCALE = 100.0f;
const VECTOR SCALES = { STAGE_SCALE, STAGE_SCALE, STAGE_SCALE };

//�������Ή�]Y
const float INIT_LOCAL_ROT_Y = 180.0f;

#pragma endregion


SkyDome::SkyDome(const Transform& syncTransform)
	: 
	syncTransform_(syncTransform),
	state_(STATE::NONE)
{
}

SkyDome::~SkyDome(void)
{
}

void SkyDome::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		resMng_.LoadModelDuplicate(
			ResourceManager::SRC::SKY_DOME));
	transform_.scl = SCALES;
	transform_.pos = MyUtility::VECTOR_ZERO;
	transform_.quaRot = Quaternion::Euler(
		0.0f, 
		MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

	// ��ԑJ��
	auto sceneId = scnMng_.GetSceneID();
	if(sceneId == SceneManager::SCENE_ID::TITLE)
	{
		ChangeState(STATE::STAY);
	}
	else
	{
		ChangeState(STATE::FOLLOW);
	}

}

void SkyDome::Update(void)
{

	// �X�V�X�e�b�v
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		UpdateNone();
		break;
	case SkyDome::STATE::STAY:
		UpdateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		UpdateFollow();
		break;
	}

}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SkyDome::ChangeState(STATE state)
{

	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case SkyDome::STATE::NONE:
		ChangeStateNone();
		break;
	case SkyDome::STATE::STAY:
		ChangeStateStay();
		break;
	case SkyDome::STATE::FOLLOW:
		ChangeStateFollow();
		break;
	}

}

void SkyDome::ChangeStateNone(void)
{
}

void SkyDome::ChangeStateStay(void)
{
}

void SkyDome::ChangeStateFollow(void)
{
	transform_.pos = syncTransform_.pos;
	transform_.Update();
}

void SkyDome::UpdateNone(void)
{
}

void SkyDome::UpdateStay(void)
{
}

void SkyDome::UpdateFollow(void)
{
	transform_.pos = syncTransform_.pos;
	transform_.Update();
}
