#pragma once
#include <queue>
#include "Common/Transform.h"
class ResourceManager;
class Planet;
class Bike;
class Enemy;
class CoinBase;
class LoopStage;
class GameScene;
class Bomb;
class City;
class Goal;
class TyreThrow;

class Stage
{

public:

	// ステージの切り替え間隔
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	//ステージの初期位置
	static constexpr VECTOR STAGE_START_POS = { -12600.0f, -5000.0f, 0.0f };

	//カーブの初期位置
	static constexpr VECTOR CURVE_START_POS = { 4000.0f,-100.0f, 4000.0f };

	//町の初期位置
	static constexpr VECTOR CITY_START_POS = { 3000.0f,-3000.0f, 1000.0f };

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
	Stage(const std::vector<std::shared_ptr<Bike>>& bikes, CoinBase* coin, Bomb* bomb, TyreThrow* throwTyre, GameScene* gameScene);

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

	//ループステージのサイズ
	int GetLoopStageSize(void);

	//ゴールしたかどうか
	bool GetIsGoal(void);


private:

	//ゲームシーンポインタ
	GameScene* gameScene_;

	// シングルトン参照
	ResourceManager& resMng_;

	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;

	CoinBase* coin_;
	//std::shared_ptr<Bomb> bomb_;
	Bomb* bomb_;
	TyreThrow* throwTyre_;
	std::deque<std::shared_ptr<City>> city_;

	//ゴール
	std::unique_ptr<Goal> goal_;

	// ステージアクティブになっている惑星の情報
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	//ループ用のステージ
	//Transform loopStage_;
	//ループ用のステージ
	//std::vector<LoopStage*> loopStage_;

	//deque使ってもいいよ
	//std::queue<LoopStage*> loopStage_;
	std::deque<std::shared_ptr<LoopStage>> loopStage_;


	// 惑星
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// 空のPlanet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	float step_;

	//ゴールしたかどうか
	bool isGoal_;

	//ループ用のステージが生成されたかどうか
	bool isMakeLoopStage_;

	// 最初の惑星
	void MakeMainStage(void);

	//ループ用のステージ(最初)
	void MakeLoopStage(void);

	void AddStage(std::shared_ptr<LoopStage> newStage);

	// sizeはloopStage_のサイズを指す
	int sizeS = 0;

	//町生成
	void MakeCity(void);

};
