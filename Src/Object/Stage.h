#pragma once
#include <map>
#include "Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class Player;
class Bike;
class Enemy;
class EnemyBase;
class LoopStage;
class GameScene;

class Stage
{

public:

	// ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	//static constexpr VECTOR STAGE_START_POS = { -5000.0f, -5600.0f, 0.0f };
	//ステージの初期位置
	static constexpr VECTOR STAGE_START_POS = { -5600.0f, -5000.0f, 0.0f };

	// ステージ名
	enum class NAME
	{
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_BASE,
		FLAT_PLANET_ROT01,
		FLAT_PLANET_ROT02,
		FLAT_PLANET_ROT03,
		FLAT_PLANET_ROT04,
		FLAT_PLANET_FIXED01,
		FLAT_PLANET_FIXED02,
		PLANET10,
		LAST_STAGE,
		SPECIAL_STAGE,
		DEMO_STAGE_1 //仮ステージ
	};

	// コンストラクタ
	Stage(Bike* bike, EnemyBase* enemy,GameScene* gameScene);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// ステージ変更
	void ChangeStage(NAME type);

	// 対象ステージを取得
	Planet* GetPlanet(NAME type);

private:

	//ゲームシーンポインタ
	GameScene* gameScene_;

	// シングルトン参照
	ResourceManager& resMng_;

	Bike* bike_;
	EnemyBase* enemy_;

	// ステージアクティブになっている惑星の情報
	NAME activeName_;
	Planet* activePlanet_;

	//ループ用のステージ
	//Transform loopStage_;
	//ループ用のステージ
	std::vector<LoopStage*> loopStage_;

	// 惑星
	std::map<NAME, Planet*> planets_;

	// ワープスター
	std::vector<WarpStar*> warpStars_;

	// 空のPlanet
	Planet* nullPlanet = nullptr;

	float step_;

	// 最初の惑星
	void MakeMainStage(void);

	//ループ用のステージ(最初)
	void MakeLoopStage(void);

	// ワープスター
	void MakeWarpStar(void);

};
