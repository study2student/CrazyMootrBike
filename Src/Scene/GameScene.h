#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Stage;
class SkyDome;
class Rider;
//class Player;
class Bike;
class EnemyBase;
class ShortDisEnemy;
class LongDisEnemy;
class BombEnemy;
class Enemy;

class GameScene : public SceneBase
{

public:

	//エンカウント値
	static constexpr int ENCOUNT = 150;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//敵情報
	std::vector<EnemyBase*> GetEnemys(void);

private:

	// ステージ
	Stage* stage_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Rider* rider_;

	//// プレイヤー
	//Player* player_;

	// プレイヤー
	Bike* bike_;

	////	敵
	//Enemy* enemy_;

	//敵
	EnemyBase* enemy_;

	//複数の敵
	std::vector<EnemyBase*>enemys_;	

	//敵の発生頻度
	int enCounter;

};
