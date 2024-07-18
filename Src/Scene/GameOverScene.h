#pragma once
#include <map>
#include "SceneBase.h"
class Score;
class Bike;

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
	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;
	//std::shared_ptr<Score> score_;

	//�X�R�A�`��̔w�i
	int img_BG;
};

