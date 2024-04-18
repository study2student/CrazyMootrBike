#pragma once
#include "SceneBase.h"
class Stage;
class SkyDome;
class Rider;
//class Player;
class Bike;
class Enemy;

class GameScene : public SceneBase
{

public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

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

	//	敵
	Enemy* enemy_;

};
