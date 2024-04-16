#pragma once
#include "SceneBase.h"
class Stage;
class SkyDome;
class Player;
class Enemy;
class Bike;

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
	Player* player_;

	//	敵
	Enemy* enemy_;

	// バイク
	Bike* bike_;

};
