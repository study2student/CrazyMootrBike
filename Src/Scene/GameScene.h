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

	//�G���J�E���g�l
	static constexpr int ENCOUNT = 350;

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�G���
	std::vector<EnemyBase*> GetEnemys(void);
	std::vector<EnemyBike*> GetEnemyBikes(void);

	//�G���ǉ����ꂽ�^�C�~���O���ǂ���
	bool GetIsCreateEnemy(void);
 
private:

	// �X�e�[�W
	Stage* stage_;

	//�X�R�A
	Score* score_;

	// �X�J�C�h�[��
	SkyDome* skyDome_;

	// �v���C���[
	Rider* rider_;

	//// �v���C���[
	//Player* player_;

	// �v���C���[
	Bike* bike_;

	//�w���R�v�^�[
	Helicopter* helicopter_;

	////	�G
	//Enemy* enemy_;

	//�G
	EnemyBase* enemy_;
	EnemyBike* enemyBike_;

	//�����̓G
	std::vector<EnemyBase*>enemys_;	
	std::vector<EnemyBike*>enemyBikes_;

	//�G�̔����p�x
	int enCounter;

	//�G���ǉ����ꂽ�^�C�~���O���ǂ���
	bool isCreateEnemy_;

	void DrawDubg(void);
	
	//�Փ˔���
	void Collision(void);
};
