#pragma once
#include <map>
#include "SceneBase.h"
class Score;

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
	std::unique_ptr<Score> score_;
};

