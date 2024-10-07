#pragma once
#include <queue>
#include<map>
#include <memory>
#include "../Common/Transform.h"
class ResourceManager;
class Planet;
class Bike;
class CoinBase;
class LoopStage;
class GameScene;
class Bomb;
class City;
class Goal;
class Spike;

class Stage
{

public:

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
		LAST_STAGE,
		SPECIAL_STAGE,
		DEMO_STAGE_1 //���X�e�[�W
	};

	// �R���X�g���N�^
	Stage(const std::vector<std::shared_ptr<Bike>>& bikes, CoinBase* coin, Bomb* bomb, std::shared_ptr<Spike>& throwTyre, GameScene* gameScene);

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
	const bool& GetIsMakeLoopStage(void) const;
	void SetMakeLoopStage(bool value);

	//�擪�̃��[�v�p�X�e�[�W�̍��W���擾
	const VECTOR& GetForwardLoopPos(void) const;

	//���[�v�X�e�[�W�̃T�C�Y
	const int& GetLoopStageSize(void) const;

	//�S�[���������ǂ���
	const bool& GetIsGoal(void) const;

private:

	//�Q�[���V�[���|�C���^
	GameScene* gameScene_;

	// �V���O���g���Q��
	ResourceManager& resMng_;

	// �����v���C���[
	std::vector<std::shared_ptr<Bike>> bikes_;

	CoinBase* coin_;
	Bomb* bomb_;
	std::shared_ptr<Spike> spike_;
	std::deque<std::shared_ptr<City>> city_;

	//�S�[��
	std::unique_ptr<Goal> goal_;

	// �X�e�[�W�A�N�e�B�u�ɂȂ��Ă���f���̏��
	NAME activeName_;
	std::weak_ptr<Planet> activePlanet_;

	std::deque<std::shared_ptr<LoopStage>> loopStage_;

	// �f��
	std::map<NAME, std::shared_ptr<Planet>> planets_;

	// ���Planet
	std::shared_ptr<Planet> nullPlanet = nullptr;

	//�X�e�[�W�̐؂�ւ��Ԋu
	float step_;

	//�S�[���������ǂ���
	bool isGoal_;

	//�S�[�������Đ����ꂽ���ǂ���
	bool hasPlayedGoalSound_;

	//���[�v�p�̃X�e�[�W���������ꂽ���ǂ���
	bool isMakeLoopStage_;

	// loopStage_�̃T�C�Y
	int sizeS_;

	// �ŏ��̃X�e�[�W
	void MakeMainStage(void);

	//���[�v�p�̃X�e�[�W(�ŏ�)
	void MakeLoopStage(void);

	void AddStage(std::shared_ptr<LoopStage> newStage);

	//�w�i����
	void MakeCity(void);

};
