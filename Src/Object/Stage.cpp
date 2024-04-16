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
#include "Planet.h"
#include "Common/Collider.h"
#include "Common/Transform.h"
#include "Stage.h"

Stage::Stage(Player* player, Bike* bike, Enemy* enemy)
	: resMng_(ResourceManager::GetInstance())
{
	player_ = player;
	bike_ = bike;
	enemy_ = enemy;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;
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

}

void Stage::Init(void)
{
	MakeMainStage();
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

}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// 対象のステージを取得する
	activePlanet_ = GetPlanet(activeName_);

	// ステージの当たり判定をプレイヤーに設定
	player_->ClearCollider();
	player_->AddCollider(activePlanet_->GetTransform().collider);

	// ステージのあたり判定をバイクに設定
	bike_->ClearCollider();
	bike_->AddCollider(activePlanet_->GetTransform().collider);

	// ステージのあたり判定を敵に設定
	enemy_->ClearCollider();
	enemy_->AddCollider(activePlanet_->GetTransform().collider);

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

void Stage::MakeMainStage(void)
{

	// 最初の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	float scale = 1.0f;
	planetTrans.scl = { scale,scale,scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { -5000.0f, -5600.0f, 0.0f };

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

void Stage::MakeWarpStar(void)
{

	Transform trans;
	WarpStar* star;

	// 落とし穴惑星へのワープスター
	//------------------------------------------------------------------------------
	trans.pos = { -910.0f, 200.0f, 894.0f };
	trans.scl = { 0.6f, 0.6f, 0.6f };
	trans.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(-25.0f),
		AsoUtility::Deg2RadF(-50.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	star = new WarpStar(player_, trans);
	star->Init();
	warpStars_.push_back(star);
	//------------------------------------------------------------------------------

}
