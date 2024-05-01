#pragma once
#include <map>
#include "Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class Player;
class Bike;
class Enemy;
class EnemyBase;
class LoopStage;
class GameScene;

class Stage
{

public:

	// �X�e�[�W�̐؂�ւ��Ԋu
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	//static constexpr VECTOR STAGE_START_POS = { -5000.0f, -5600.0f, 0.0f };
	//�X�e�[�W�̏����ʒu
	static constexpr VECTOR STAGE_START_POS = { -5600.0f, -5000.0f, 0.0f };

	// �X�e�[�W��
	enum class NAME
	{
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_BASE,
		FLAT_PLANET_ROT01,
		FLAT_PLANET_ROT02,
		FLAT_PLANET_ROT03,
		FLAT_PLANET_ROT04,
		FLAT_PLANET_FIXED01,
		FLAT_PLANET_FIXED02,
		PLANET10,
		LAST_STAGE,
		SPECIAL_STAGE,
		DEMO_STAGE_1 //���X�e�[�W
	};

	// �R���X�g���N�^
	Stage(Bike* bike, EnemyBase* enemy,GameScene* gameScene);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �X�e�[�W�ύX
	void ChangeStage(NAME type);

	// �ΏۃX�e�[�W���擾
	Planet* GetPlanet(NAME type);

private:

	//�Q�[���V�[���|�C���^
	GameScene* gameScene_;

	// �V���O���g���Q��
	ResourceManager& resMng_;

	Bike* bike_;
	EnemyBase* enemy_;

	// �X�e�[�W�A�N�e�B�u�ɂȂ��Ă���f���̏��
	NAME activeName_;
	Planet* activePlanet_;

	//���[�v�p�̃X�e�[�W
	//Transform loopStage_;
	//���[�v�p�̃X�e�[�W
	std::vector<LoopStage*> loopStage_;

	// �f��
	std::map<NAME, Planet*> planets_;

	// ���[�v�X�^�[
	std::vector<WarpStar*> warpStars_;

	// ���Planet
	Planet* nullPlanet = nullptr;

	float step_;

	// �ŏ��̘f��
	void MakeMainStage(void);

	//���[�v�p�̃X�e�[�W(�ŏ�)
	void MakeLoopStage(void);

	// ���[�v�X�^�[
	void MakeWarpStar(void);

};
