#pragma once
#include <map>
#include <queue>
#include "Common/Transform.h"
class ResourceManager;
class WarpStar;
class Planet;
class Player;
class Bike;
class Enemy;
class JampRamp;
class CoinBase;
class LoopStage;
class GameScene;
class StageCurve;
class Bomb;
class City;
class Goal;
class TyreThrow;

class Stage
{

public:

	// �X�e�[�W�̐؂�ւ��Ԋu
	static constexpr float TIME_STAGE_CHANGE = 1.0f;

	//static constexpr VECTOR STAGE_START_POS = { -5000.0f, -5600.0f, 0.0f };
	//�X�e�[�W�̏����ʒu
	static constexpr VECTOR STAGE_START_POS = { -12600.0f, -5000.0f, 0.0f };

	//�J�[�u�̏����ʒu
	static constexpr VECTOR CURVE_START_POS = { 4000.0f,-100.0f, 4000.0f };
	//static constexpr VECTOR CURVE_START_POS = { 6000.0f,-1100.0f, 4500.0f };

	//���̏����ʒu
	static constexpr VECTOR CITY_START_POS = { 3000.0f,-3000.0f, 1000.0f };

	//���ǂɓ����������̖@���x�N�g��
	static constexpr VECTOR LEFT_NORMAL_VEC = { 1.0f,0.0f,0.0f };

	//�E�ǂɓ����������̖@���x�N�g��
	static constexpr VECTOR RIGHT_NORMAL_VEC = { -1.0f,0.0f,0.0f };

	//���̃X�e�[�W�����̍ő���W
	static constexpr float STAGE_LEFT_POS_X_MAX = 910.0f;

	//���̃X�e�[�W�E���̍ő���W
	static constexpr float STAGE_RIGHT_POS_X_MAX = 2500.0f;

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
	//Stage(const std::vector<std::shared_ptr<Bike>>& bikes, EnemyBase* enemy,std::shared_ptr<Bomb> bomb, GameScene* gameScene);
	Stage(const std::vector<std::shared_ptr<Bike>>& bikes, CoinBase* enemy, Bomb* bomb, TyreThrow* throwTyre, GameScene* gameScene);

	// �f�X�g���N�^
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	// �X�e�[�W�ύX
	void ChangeStage(NAME type);

	// �ΏۃX�e�[�W���擾
	std::weak_ptr<Planet> GetPlanet(NAME type);

	//���[�v�p�̃X�e�[�W���������ꂽ���ǂ����擾
	bool GetIsMakeLoopStage(void);
	void SetMakeLoopStage(bool value);

	//�擪�̃��[�v�p�X�e�[�W�̍��W���擾
	VECTOR GetForwardLoopPos(void);

	// �W�����v��ɓ����������̏���
	void Jump(void);

	//���[�v�X�e�[�W�̃T�C�Y
	int GetLoopStageSize(void);

	//�S�[���������ǂ���
	bool GetIsGoal(void);


private:

	//�Q�[���V�[���|�C���^
	GameScene* gameScene_;

	// �V���O���g���Q��
	ResourceManager& resMng_;

	//std::shared_ptr<Bike> bike_;

	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;

	CoinBase* coin_;
	//std::shared_ptr<Bomb> bomb_;
	Bomb* bomb_;
	TyreThrow* throwTyre_;
	std::deque<std::shared_ptr<StageCurve>> curve_;
	std::deque<std::shared_ptr<City>> city_;

	//�S�[��
	std::unique_ptr<Goal> goal_;

	// �X�e�[�W�A�N�e�B�u�ɂȂ��Ă���f���̏��
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	//���[�v�p�̃X�e�[�W
	//Transform loopStage_;
	//���[�v�p�̃X�e�[�W
	//std::vector<LoopStage*> loopStage_;

	//deque�g���Ă�������
	//std::queue<LoopStage*> loopStage_;
	std::deque<std::shared_ptr<LoopStage>> loopStage_;

	std::unique_ptr<JampRamp> jampRamp_;

	// �f��
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// ���[�v�X�^�[
	//std::vector<WarpStar*> warpStars_;

	// ���Planet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	float step_;

	//�S�[���������ǂ���
	bool isGoal_;

	//�W�����v��ɓ���������
	bool isJamp_;

	//���[�v�p�̃X�e�[�W���������ꂽ���ǂ���
	bool isMakeLoopStage_;

	// �ŏ��̘f��
	void MakeMainStage(void);

	//���[�v�p�̃X�e�[�W(�ŏ�)
	void MakeLoopStage(void);

	void AddStage(std::shared_ptr<LoopStage> newStage);

	// size��loopStage_�̃T�C�Y���w��
	int sizeS = 0;

	// ���[�v�X�^�[
	void MakeWarpStar(void);

	//�J�[�u����
	void MakeCurveStage(void);

	//������
	void MakeCity(void);

};
