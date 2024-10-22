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
#include "../../Object/Stage/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "../../Scene/GameScene.h"
#include "GoldCoin.h"


GoldCoin::GoldCoin(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene ,VECTOR loopStagePos, VECTOR localPos) 
	:
	CoinBase(bikes,gameScene, loopStagePos, localPos)
{
	makePos_ = loopStagePos;
	localPos_ = localPos;
}

void GoldCoin::SetParam(void)
{
	// ÉÇÉfÉãÇÃäÓñ{ê›íË
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::GOLD_COIN));
	transform_.scl = { SCL,SCL,SCL };
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, INIT_POS_Y, makePos_.z + localPos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(INIT_LOCAL_ROT_Y), 0.0f });
	transform_.Update();

	// èâä˙èÛë‘
	ChangeState(STATE::PLAY);
}
