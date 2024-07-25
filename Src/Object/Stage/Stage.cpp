#include <vector>
#include <DxLib.h>
#include "../../Utility/MyUtility.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../Common/Collider.h"
#include "../Common/Capsule.h"
#include "../Common/Transform.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Coin/CoinBase.h"
#include "../../Object/Gimmick/Bomb.h"
#include "../../Object//Gimmick/TyreThrow.h"
#include "../../Object/DataSave.h"
#include "../../Scene/GameScene.h"
#include "../Stage/Goal.h"
#include "../Stage/City.h"
#include "Planet.h"
#include "LoopStage.h"
#include "Stage.h"

Stage::Stage(const std::vector<std::shared_ptr<Bike>>& bikes, CoinBase* coin, Bomb* bomb, TyreThrow* throwTyre, GameScene* gameScene)
	: resMng_(ResourceManager::GetInstance()), bikes_(bikes)
{
	gameScene_ = gameScene;

	//bike_ = bike;
	coin_ = coin;
	bomb_ = bomb;
	throwTyre_ = throwTyre;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;

	isMakeLoopStage_ = false;

	isGoal_ = false;

}

Stage::~Stage(void)
{

	// 惑星
	planets_.clear();

	loopStage_.clear();
}

void Stage::Init(void)
{
	MakeMainStage();
	MakeLoopStage();
	MakeCity();

	goal_ = std::make_unique<Goal>();
	goal_->Init();

	step_ = -1.0f;
}

void Stage::Update(void)
{

	//町
	for (const auto& ct : city_)
	{
		ct->Update();
	}

	// 惑星
	for (const auto& s : planets_)
	{
		s.second->Update();
	}

	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		ls->Update();
	}

	//ステージを生成する
	MakeLoopStage();
	//MakeCurveStage();
	MakeCity();

	//ゴール
	goal_->Update();

	//誰かがゴールゾーンを超えたらクリア
	if (gameScene_->GetPlayNum() == 1)
	{
		if (bikes_[0]->GetTransform().pos.z >= goal_->GetTransform().pos.z)
		{
			
			isGoal_ = true;
			// ゴール音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_GOAL).handleId_, DX_PLAYTYPE_BACK, false);
		}
		else
		{
			isGoal_ = false;
		}
	}
	else
	{
		for (const auto& bike : bikes_)
		{
			if (bike->GetTransform().pos.z >= goal_->GetTransform().pos.z)
			{
				//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				isGoal_ = true;
				bike->SetIsGoal(true);
				// ゴール音を再生
				PlaySoundMem(ResourceManager::GetInstance().Load(
					ResourceManager::SRC::SND_GOAL).handleId_, DX_PLAYTYPE_BACK, false);
			}
			else
			{
				bike->SetIsGoal(false);
			}
		}
	}

	//ステージを一定数生成したらゴールが出現
	if (loopStage_.size() >= Goal::STAGE_NUM_MAX_FOR_GOAL)
	{
		goal_->SetPosZ(5000.0f * (Goal::STAGE_NUM_MAX_FOR_GOAL + 1));
	}


}

void Stage::Draw(void)
{

	//町
	for (const auto& ct : city_)
	{
		ct->Draw();
	}

	// 惑星
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		ls->Draw();
	}

	goal_->Draw();

}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// 対象のステージを取得する
	activePlanet_ = GetPlanet(activeName_);

	for (const auto& bike : bikes_)
	{
		bike->ClearCollider();
		bike->AddCollider(activePlanet_.lock()->GetTransform().collider);
	}

	// ステージの当たり判定設定
	bomb_->ClearCollider();
	bomb_->AddCollider(activePlanet_.lock()->GetTransform().collider);

	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		for (const auto& bike : bikes_)
		{
			bike->AddCollider(ls->GetTransform().collider);
		}
		bomb_->AddCollider(ls->GetTransform().collider);

	}

	coin_->ClearCollider();
	coin_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		std::vector<CoinBase*>coins_ = gameScene_->GetEnemys();
		for (int i = 0; i < coins_.size(); i++)
		{
			coins_[i]->AddCollider(ls->GetTransform().collider);
		}
	}

	step_ = TIME_STAGE_CHANGE;

}

std::weak_ptr<Planet> Stage::GetPlanet(NAME type)
{
	if (planets_.count(type) == 0)
	{
		return nullPlanet;
	}

	return planets_[type];
}

bool Stage::GetIsMakeLoopStage(void)
{
	return isMakeLoopStage_;
}

void Stage::SetMakeLoopStage(bool value)
{
	isMakeLoopStage_ = value;
}

VECTOR Stage::GetForwardLoopPos(void)
{
	//先頭ループステージの座標を取得
	int size = (int)loopStage_.size();
	return loopStage_[size - 1]->GetPos();
}

int Stage::GetLoopStageSize(void)
{
	return loopStage_.size();
}

bool Stage::GetIsGoal(void)
{
	return isGoal_;
}

void Stage::MakeMainStage(void)
{

	// 最初の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	float scale = 1.0f;
	planetTrans.scl = { scale * 2.5f,scale,scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { STAGE_START_POS };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet =
		new Planet(
			name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planets_.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeLoopStage(void)
{

	//すり抜けるためここでバイクと敵のコライダーも追加しとく
	if (gameScene_->GetIsCreateEnemy())
	{
		for (const auto& ls : loopStage_)
		{
			std::vector<CoinBase*>coins_ = gameScene_->GetEnemys();
			for (int i = 0; i < coins_.size(); i++)
			{
				coins_[i]->AddCollider(activePlanet_.lock()->GetTransform().collider);
				coins_[i]->AddCollider(ls->GetTransform().collider);
			}
		}
	}


	Transform loopTrans;
	std::shared_ptr<LoopStage> stage;


	//pos.z が最大の要素のインデックスを取得する
	size_t posZMaxIndex = gameScene_->GetBikeMaxPosZIndex();

	//先頭のバイクの座標
	float z = bikes_[posZMaxIndex]->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)loopStage_.size();

	//一定の距離超えたら
	if (size <= mapZ)
	{

		loopTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));



		float scale = 1.0f;
		loopTrans.scl = { scale * 2.5f,scale ,scale };
		loopTrans.quaRot = Quaternion();
		loopTrans.pos = { STAGE_START_POS.x,  STAGE_START_POS.y,  STAGE_START_POS.z + 5000.0f * (size + 1) };

		// 当たり判定(コライダ)作成
		loopTrans.MakeCollider(Collider::TYPE::STAGE);
		loopTrans.Update();

		//すり抜けるためここでプレイヤーのコライダーも追加しとく
		for (const auto& ls : loopStage_)
		{
			for (const auto& bike : bikes_)
			{
				bike->AddCollider(ls->GetTransform().collider);
			}
			bomb_->AddCollider(ls->GetTransform().collider);
		}


		stage = std::make_shared<LoopStage>(bikes_.front(), loopTrans);
		stage->Init();
		loopStage_.push_back(stage);
		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//後ろのステージを削除
	if (loopStage_.size() >= 12)
	{

		// ステージを削除する

		std::shared_ptr<LoopStage> tailLoop = loopStage_[size-11];
		tailLoop->Destroy();

	}

}

// ステージを追加する関数
void Stage::AddStage(std::shared_ptr<LoopStage> newStage)
{

	loopStage_.push_back(newStage);
	//stageQueue(newStage);
	sizeS++;

	// 6以上のステージがある場合は古いステージを削除
	if (loopStage_.size() > 5) {
		std::shared_ptr<LoopStage> oldStage = loopStage_.front();
		loopStage_.pop_front();

		oldStage->Destroy();

		// 古いステージをloopStage_から削除
		//loopStage_.erase(
		//	std::remove(loopStage_.begin(), loopStage_.end(), oldStage),
		//	loopStage_.end()
		//);
	}
}

void Stage::MakeCity(void)
{
	Transform cityTrans;
	std::shared_ptr<City> city;


	//pos.z が最大の要素のインデックスを取得する
	size_t posZMaxIndex = gameScene_->GetBikeMaxPosZIndex();

	//先頭のバイクに合わせる
	float z = bikes_[posZMaxIndex]->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)city_.size();

	//一定の距離超えたら
	if (size <= mapZ)
	{

		cityTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));



		float scale = 1.0f;
		cityTrans.scl = { scale,scale,scale };
		cityTrans.quaRot = Quaternion();
		cityTrans.quaRotLocal =
			Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(-90.0f), 0.0f });
		cityTrans.pos = { CITY_START_POS.x,  CITY_START_POS.y,  CITY_START_POS.z + 5000.0f * (size + 1) };

		cityTrans.Update();


		city = std::make_shared<City>(bikes_.front(), cityTrans);
		city->Init();
		city_.push_back(city);

		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//後ろのステージを削除
	if (city_.size() >= 12)
	{
		// ステージを削除する
		std::shared_ptr<City> tailLoop = city_[size - 11];
		tailLoop->Destroy();
	}
}
