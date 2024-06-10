#pragma once
#include <map>
#include <queue>
#include "Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class Player;
class Bike;
class Enemy;
class JampRamp;
class EnemyBase;
class LoopStage;
class GameScene;
class Bomb;

class Stage
{

public:

	// ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	//static constexpr VECTOR STAGE_START_POS = { -5000.0f, -5600.0f, 0.0f };
	//ステージの初期位置
	static constexpr VECTOR STAGE_START_POS = { -12600.0f, -5000.0f, 0.0f };

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
	Stage(std::shared_ptr<Bike> bike, EnemyBase* enemy,std::shared_ptr<Bomb> bomb, GameScene* gameScene);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// ステージ変更
	void ChangeStage(NAME type);

	// 対象ステージを取得
	std::weak_ptr<Planet> GetPlanet(NAME type);

	//ループ用のステージが生成されたかどうか取得
	bool GetIsMakeLoopStage(void);
	void SetMakeLoopStage(bool value);

	//先頭のループ用ステージの座標を取得
	VECTOR GetForwardLoopPos(void);

	// ジャンプ台に当たった時の処理
	void Jump(void);

	//ループステージのサイズ
	int GetLoopStageSize(void);
private:

	//ゲームシーンポインタ
	GameScene* gameScene_;

	// シングルトン参照
	ResourceManager& resMng_;

	std::shared_ptr<Bike> bike_;
	EnemyBase* enemy_;
	std::shared_ptr<Bomb> bomb_;

	// ステージアクティブになっている惑星の情報
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	//ループ用のステージ
	//Transform loopStage_;
	//ループ用のステージ
	//std::vector<LoopStage*> loopStage_;

	//deque使ってもいいよ
	//std::queue<LoopStage*> loopStage_;
	std::deque<LoopStage*> loopStage_;

	JampRamp* jampRamp_;

	// 惑星
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// ワープスター
	std::vector<WarpStar*> warpStars_;

	// 空のPlanet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	float step_;

	//ジャンプ台に当たったか
	bool isJamp_;

	//ループ用のステージが生成されたかどうか
	bool isMakeLoopStage_;

	// 最初の惑星
	void MakeMainStage(void);

	//ループ用のステージ(最初)
	void MakeLoopStage(void);

	void AddStage(LoopStage* newStage);

	// sizeはloopStage_のサイズを指す
	int sizeS = 0;

	// ワープスター
	void MakeWarpStar(void);

};
