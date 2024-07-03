#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// 推奨しませんが、どうしても使いたい方は
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;

	Resource* res;

	// タイトル画像
	res = new RES(RES_T::IMG, PATH_IMG + "Title.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	// PushSpace
	res = new RES(RES_T::IMG, PATH_IMG + "PushSpace.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, res);

	// 吹き出し
	res = new RES(RES_T::IMG, PATH_IMG + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, res);

	// プレイヤー
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/Great Sword Slash.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// プレイヤー影
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	// スカイドーム
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	// 最初の惑星
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Stage1.mv1");
	resourcesMap_.emplace(SRC::MAIN_PLANET, res);

	// 落とし穴の惑星
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FallPlanet.mv1");
	resourcesMap_.emplace(SRC::FALL_PLANET, res);

	// 平坦な惑星01
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet01.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_01, res);

	// 平坦な惑星02
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet02.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_02, res);

	// 最後の惑星
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/LastPlanet.mv1");
	resourcesMap_.emplace(SRC::LAST_PLANET, res);

	// 特別な惑星
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/RoadPlanet.mv1");
	resourcesMap_.emplace(SRC::SPECIAL_PLANET, res);

	// 足煙
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, res);

	// ワープスターモデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Star/star.mv1");
	resourcesMap_.emplace(SRC::WARP_STAR, res);

	// ワープスター用回転エフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "StarDust/StarDust.efkefc");
	resourcesMap_.emplace(SRC::WARP_STAR_ROT_EFF, res);

	// ワープの軌跡線
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Warp/WarpOrbit.efkefc");
	resourcesMap_.emplace(SRC::WARP_ORBIT, res);

	// ブラックホール
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BlackHole/BlackHole.efkefc");
	resourcesMap_.emplace(SRC::BLACK_HOLE, res);

	// ゴールスター
	res = new RES(RES_T::MODEL, PATH_MDL + "GoalStar/GoalStar.mv1");
	resourcesMap_.emplace(SRC::GOAL_STAR, res);

	// Clear
	res = new RES(RES_T::IMG, PATH_IMG + "Congratulations.png");
	resourcesMap_.emplace(SRC::CLEAR, res);

	// タンク
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Body.mv1");
	resourcesMap_.emplace(SRC::TANK_BODY, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Wheel.mv1");
	resourcesMap_.emplace(SRC::TANK_WHEEL, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Barrel.mv1");
	resourcesMap_.emplace(SRC::TANK_BARREL, res);

	// 仮ステージ
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Stage1.mv1");
	resourcesMap_.emplace(SRC::DEMO_STAGE, res);

	//カーブ
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Curve.mv1");
	resourcesMap_.emplace(SRC::CURVE, res);
	
	// ジャンプ台
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/jampRanp/janpudai.mv1");
	resourcesMap_.emplace(SRC::JUMP_RAMP, res);

	//背景街並み
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/city/city.mv1");
	resourcesMap_.emplace(SRC::CITY, res);

	//ゴール
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/GoalPost/Goal.mv1");
	resourcesMap_.emplace(SRC::GOAL, res);

	//// 近距離敵
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemy.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_SHORT, res);

	//// 遠距離敵
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemyLong.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_LONG, res);

	//// 爆弾敵
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemyBomb.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_MAGIC, res);

	//ヘリ
	res = new RES(RES_T::MODEL, PATH_MDL + "Helicopter/Aircraft04.mv1");
	resourcesMap_.emplace(SRC::HELICOPTER, res);

	//ヘリ羽
	res = new RES(RES_T::MODEL, PATH_MDL + "Helicopter/AirCraft04_Rotor.mv1");
	resourcesMap_.emplace(SRC::HELICOPTER_ROTOR, res);

	// 敵が落とす爆弾
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/bomb.mv1");
	resourcesMap_.emplace(SRC::BOMB, res);

	//金メダル
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/GoldCoin.mv1");
	resourcesMap_.emplace(SRC::GOLD_MEDAL, res);

	//銀メダル
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/SilverCoin.mv1");
	resourcesMap_.emplace(SRC::SILVER_MEDAL, res);

	// 銅メダル
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/CopperCoin.mv1");
	resourcesMap_.emplace(SRC::COPPER_MEDAL, res);


	// バイクモデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Bike/bike.mv1");
	resourcesMap_.emplace(SRC::BIKE, res);

	// タイヤモデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Bike/tyre.mv1");
	resourcesMap_.emplace(SRC::TYRE, res);

	// 武器モデル
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/BusterSword/sword.mv1");
	resourcesMap_.emplace(SRC::W_SWORD, res);

	// バーンアウト時のエフェクト(バイク発車前)
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BikeStart/burnoutEffect.efkefc");
	resourcesMap_.emplace(SRC::BURNOUT_EFFECT, res);

	// ゲーム開始エフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Start/startEffect.efkefc");
	resourcesMap_.emplace(SRC::START_EFFECT, res);

	// ヒットした時のエフェクト
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "HitSprite/HitSprite.efkefc");
	resourcesMap_.emplace(SRC::HitEffect, res);

	// ソニックエフェクト
	//res = new RES(RES_T::EFFEKSEER, PATH_EFF + "HitSprite/HitSprite.efkefc");
	//res = new RES(RES_T::EFFEKSEER, PATH_EFF + "SonicBoom/SonicBoom.efkefc");
	//resourcesMap_.emplace(SRC::SonicEffect, res);
	
}


void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ロード済みチェック
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// リソース登録チェック
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// 登録されていない
		return dummy_;
	}

	// ロード処理
	rPair->second->Load();

	// 念のためコピーコンストラクタ
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
