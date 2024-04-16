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

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �X�e�[�W
	Stage* stage_;

	// �X�J�C�h�[��
	SkyDome* skyDome_;

	// �v���C���[
	Player* player_;

	//	�G
	Enemy* enemy_;

	// �o�C�N
	Bike* bike_;

};
