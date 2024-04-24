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

	//�G���J�E���g�l
	static constexpr int ENCOUNT = 150;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�G���
	std::vector<EnemyBase*> GetEnemys(void);

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

	////	�G
	//Enemy* enemy_;

	//�G
	EnemyBase* enemy_;

	//�����̓G
	std::vector<EnemyBase*>enemys_;	

	//�G�̔����p�x
	int enCounter;

};
