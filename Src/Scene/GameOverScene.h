#pragma once
#include <map>
#include "SceneBase.h"
class Score;
class Bike;

class GameOverScene : public SceneBase
{
public:
	// コンストラクタ
	GameOverScene(void);

	// デストラクタ
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;
	//std::shared_ptr<Score> score_;

	//スコア描画の背景
	int img_BG;
};

