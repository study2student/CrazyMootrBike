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
	Rider* rider_;

	//// �v���C���[
	//Player* player_;

	// �v���C���[
	Bike* bike_;

	//	�G
	Enemy* enemy_;

};
