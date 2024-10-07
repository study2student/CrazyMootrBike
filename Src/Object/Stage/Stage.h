#pragma once
#include <queue>
#include<map>
#include <memory>
#include "../Common/Transform.h"
class ResourceManager;
class Planet;
class Bike;
class CoinBase;
class LoopStage;
class GameScene;
class Bomb;
class City;
class Goal;
class Spike;

class Stage
{

public:

	//左壁に当たった時の法線ベクトル
	static constexpr VECTOR LEFT_NORMAL_VEC = { 1.0f,0.0f,0.0f };

	//右壁に当たった時の法線ベクトル
	static constexpr VECTOR RIGHT_NORMAL_VEC = { -1.0f,0.0f,0.0f };

	//下のステージ左側の最大座標
	static constexpr float STAGE_LEFT_POS_X_MAX = 910.0f;

	//下のステージ右側の最大座標
	static constexpr float STAGE_RIGHT_POS_X_MAX = 2500.0f;


	// ステージ名
	enum class NAME
	{
		MAIN_PLANET,
		LAST_STAGE,
		SPECIAL_STAGE,
		DEMO_STAGE_1 //仮ステージ
	};

	// コンストラクタ
	Stage(const std::vector<std::shared_ptr<Bike>>& bikes, CoinBase* coin, Bomb* bomb, std::shared_ptr<Spike>& throwTyre, GameScene* gameScene);

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
	const bool& GetIsMakeLoopStage(void) const;
	void SetMakeLoopStage(bool value);

	//先頭のループ用ステージの座標を取得
	const VECTOR& GetForwardLoopPos(void) const;

	//ループステージのサイズ
	const int& GetLoopStageSize(void) const;

	//ゴールしたかどうか
	const bool& GetIsGoal(void) const;

private:

	//ゲームシーンポインタ
	GameScene* gameScene_;

	// シングルトン参照
	ResourceManager& resMng_;

	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;

	CoinBase* coin_;
	Bomb* bomb_;
	std::shared_ptr<Spike> spike_;
	std::deque<std::shared_ptr<City>> city_;

	//ゴール
	std::unique_ptr<Goal> goal_;

	// ステージアクティブになっている惑星の情報
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	std::deque<std::shared_ptr<LoopStage>> loopStage_;

	// 惑星
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// 空のPlanet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	//ステージの切り替え間隔
	float step_;

	//ゴールしたかどうか
	bool isGoal_;

	//ゴール音が再生されたかどうか
	bool hasPlayedGoalSound_;

	//ループ用のステージが生成されたかどうか
	bool isMakeLoopStage_;

	// loopStage_のサイズ
	int sizeS_;

	// 最初のステージ
	void MakeMainStage(void);

	//ループ用のステージ(最初)
	void MakeLoopStage(void);

	void AddStage(std::shared_ptr<LoopStage> newStage);

	//背景生成
	void MakeCity(void);

};
