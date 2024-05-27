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
class MagicEnemy;
class Enemy;
class EnemyBike;
class Helicopter;
class Score;

class GameScene : public SceneBase
{

public:

	//エンカウント値
	static constexpr int ENCOUNT = 350;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//敵情報
	std::vector<EnemyBase*> GetEnemys(void);
	std::vector<EnemyBike*> GetEnemyBikes(void);

	//敵が追加されたタイミングかどうか
	bool GetIsCreateEnemy(void);
 
private:

	// ステージ
	Stage* stage_;

	//スコア
	Score* score_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Rider* rider_;

	//// プレイヤー
	//Player* player_;

	// プレイヤー
	Bike* bike_;

	//ヘリコプター
	Helicopter* helicopter_;

	////	敵
	//Enemy* enemy_;

	//敵
	EnemyBase* enemy_;
	EnemyBike* enemyBike_;

	//複数の敵
	std::vector<EnemyBase*>enemys_;	
	std::vector<EnemyBike*>enemyBikes_;

	//敵の発生頻度
	int enCounter;

	//敵が追加されたタイミングかどうか
	bool isCreateEnemy_;

	void DrawDubg(void);
	
	//衝突判定
	void Collision(void);
};
