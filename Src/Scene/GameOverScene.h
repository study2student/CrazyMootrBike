#pragma once
#include <map>
#include "SceneBase.h"
class Score;

class GameOverScene : public SceneBase
{
public:
	// �R���X�g���N�^
	GameOverScene(void);

	// �f�X�g���N�^
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	std::unique_ptr<Score> score_;
};

