#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Stage;
class SkyDome;
class Rider;
class Bike;
class EnemyBase;
class ShortDisEnemy;
class LongDisEnemy;
class MagicEnemy;
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
	std::unique_ptr<Stage> stage_;

	//�X�R�A
	std::shared_ptr<Score> score_;

	// �X�J�C�h�[��
	std::unique_ptr<SkyDome> skyDome_;


	// �v���C���[
	std::shared_ptr<Bike> bike_;

	//�w���R�v�^�[
	std::unique_ptr<Helicopter> helicopter_;

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

	float hitStopDuration;  // �q�b�g�X�g�b�v�̎������ԁi�b�j
	float hitStopTimer;      // �q�b�g�X�g�b�v�̃^�C�}�[
	bool isHitStop;        // �q�b�g�X�g�b�v�����ǂ����̃t���O

	// �q�b�g�G�t�F�N�g
	int effectHitResId_;
	int effectHitPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);
	// Hit�G�t�F�N�g�̈ʒu
	void FireBlessEffect(void);
};
