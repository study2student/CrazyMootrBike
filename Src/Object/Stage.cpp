#include <vector>
#include <map>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "WarpStar.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "Planet.h"
#include "LoopStage.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "../Scene/GameScene.h"
#include "Stage.h"

Stage::Stage(Bike* bike, EnemyBase* enemy, GameScene* gameScene)
	: resMng_(ResourceManager::GetInstance())
{
	gameScene_ = gameScene;

	bike_ = bike;
	enemy_ = enemy;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;

	isMakeLoopStage_ = false;

	//ループ用のステージ
	/*loopStage_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE);
	loopStage_.pos = { -5000.0f, -5600.0f, 6500.0f };
	loopStage_.scl = { 1.0f,1.0f,1.0f };
	loopStage_.quaRot = Quaternion();
	loopStage_.MakeCollider(Collider::TYPE::STAGE);
	loopStage_.Update();*/

}

Stage::~Stage(void)
{

	// ワープスター
	for (auto star : warpStars_)
	{
		delete star;
	}
	warpStars_.clear();

	// 惑星
	for (auto pair : planets_)
	{
		delete pair.second;
	}
	planets_.clear();

	//ループ用のステージ
	for (auto loop : loopStage_)
	{
		delete loop;
	}
	loopStage_.clear();

}

void Stage::Init(void)
{
	MakeMainStage();
	MakeLoopStage();
	MakeWarpStar();



	step_ = -1.0f;
}

void Stage::Update(void)
{

	// ワープスター
	for (const auto& s : warpStars_)
	{
		s->Update();
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


}

void Stage::Draw(void)
{

	// ワープスター
	for (const auto& s : warpStars_)
	{
		s->Draw();
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


}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// 対象のステージを取得する
	activePlanet_ = GetPlanet(activeName_);

	// ステージの当たり判定をプレイヤーに設定
	bike_->ClearCollider();
	bike_->AddCollider(activePlanet_->GetTransform().collider);
	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		bike_->AddCollider(ls->GetTransform().collider);
	}


	// ステージのあたり判定を敵に設定
	enemy_->ClearCollider();
	enemy_->AddCollider(activePlanet_->GetTransform().collider);
	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		//enemy_->AddCollider(ls->GetTransform().collider);

		std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
		for (int i = 0; i < enemys_.size(); i++)
		{
			enemys_[i]->AddCollider(ls->GetTransform().collider);
		}
	}

	step_ = TIME_STAGE_CHANGE;

}

Planet* Stage::GetPlanet(NAME type)
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
			std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
			for (int i = 0; i < enemys_.size(); i++)
			{
				enemys_[i]->AddCollider(activePlanet_->GetTransform().collider);
				enemys_[i]->AddCollider(ls->GetTransform().collider);
			}
		}
	}

	Transform loopTrans;
	LoopStage* stage;


	float z = bike_->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)loopStage_.size();

	//一定の距離超えたら
	if (size <= mapZ)
	{

		loopTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));



		float scale = 1.0f;
		loopTrans.scl = { scale * 2.5f,scale,scale };
		loopTrans.quaRot = Quaternion();
		loopTrans.pos = { STAGE_START_POS.x,  STAGE_START_POS.y,  STAGE_START_POS.z + 5000.0f * (size + 1) };

		// 当たり判定(コライダ)作成
		loopTrans.MakeCollider(Collider::TYPE::STAGE);
		loopTrans.Update();

		//すり抜けるためここでプレイヤーのコライダーも追加しとく
		for (const auto& ls : loopStage_)
		{
			bike_->AddCollider(ls->GetTransform().collider);
		}

		stage = new LoopStage(bike_, loopTrans);
		stage->Init();
		loopStage_.push_back(stage);
		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//後ろのステージを削除
	if (loopStage_.size() >= 6)
	{
		LoopStage* tailLoop = loopStage_[size - 5];
		tailLoop->Destroy();
	}


}

void Stage::MakeWarpStar(void)
{

	//Transform trans;
	//WarpStar* star;

	//// 落とし穴惑星へのワープスター
	////------------------------------------------------------------------------------
	//trans.pos = { -910.0f, 200.0f, 894.0f };
	//trans.scl = { 0.6f, 0.6f, 0.6f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(-25.0f),
	//	AsoUtility::Deg2RadF(-50.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);

 // 	star = new WarpStar(bike_, trans);
	//star->Init();
	//warpStars_.push_back(star);
	////------------------------------------------------------------------------------

}
