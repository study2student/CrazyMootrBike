#include <DxLib.h>
#include <string>
#include<EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "../../Scene/GameScene.h"
#include "GoldCoin.h"


GoldCoin::GoldCoin(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene ,VECTOR loopStagePos, VECTOR localPos) : CoinBase(bikes,gameScene, loopStagePos, localPos)
{
	makePos_ = loopStagePos;
	localPos_ = localPos;
}

void GoldCoin::SetParam(void)
{
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOLD_COIN));
	float SCL = 200.0f;
	transform_.scl = { SCL,SCL,SCL };
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, 700.0f, makePos_.z + localPos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();

	// �������
	ChangeState(STATE::PLAY);
}

void GoldCoin::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case CoinBase::STATE::NONE:
		UpdateNone();
		break;
	case CoinBase::STATE::PLAY:
		UpdatePlay();
		break;
	case CoinBase::STATE::FLIPED:
		UpdateFliped();
		break;
	case CoinBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// ���f������X�V
	transform_.Update();

}

void GoldCoin::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	//��]
	RotY();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	// ��]������
	transform_.quaRot = enemyRotY_;
}

void GoldCoin::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//�Փ˔���(�G�ƃv���C���[)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = MyUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//�͈͂ɓ�����
			isBikeCol_ = true;
			isAddScore_ = true;

			// �q�b�g�G�t�F�N�g
			HitEffect();

			int playNum = gameScene_->GetPlayNum();
			if (playNum == 1)
			{
				if (!gameScene_->OnePersonIsGoal())
				{
					AddScoreToPlayer(bike->GetPlayerID(), 10);

					// �R�C�����W���̉����Đ�
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}
			else
			{
				//�S�[�����ĂȂ��v���C���[�ɂ���
				if (!bike->GetIsGoal())
				{
					AddScoreToPlayer(bike->GetPlayerID(), 10);

					// �R�C�����W���̉����Đ�
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}
			
			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}

		}
		else
		{
			isAddScore_ = false;

		}
	}

	//�폜����
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}


}
