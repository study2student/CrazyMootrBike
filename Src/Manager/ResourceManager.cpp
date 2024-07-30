#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{

	// �������܂��񂪁A�ǂ����Ă��g����������
	using RES = Resource;
	using RES_T = RES::TYPE;
	static std::string PATH_IMG = Application::PATH_IMAGE;
	static std::string PATH_MDL = Application::PATH_MODEL;
	static std::string PATH_EFF = Application::PATH_EFFECT;
	static std::string PATH_SND = Application::PATH_SOUND;

	Resource* res;

	// �^�C�g���摜
	res = new RES(RES_T::IMG, PATH_IMG + "Game_title.png");
	resourcesMap_.emplace(SRC::TITLE, res);

	// PushSpace
	res = new RES(RES_T::IMG, PATH_IMG + "push.png");
	resourcesMap_.emplace(SRC::PUSH_SPACE, res); 

	// �����o��
	res = new RES(RES_T::IMG, PATH_IMG + "SpeechBalloon.png");
	resourcesMap_.emplace(SRC::SPEECH_BALLOON, res);

	// �x���}�[�N
	res = new RES(RES_T::IMG, PATH_IMG + "warning.png");
	resourcesMap_.emplace(SRC::WARNING, res);

	// �Q�[���I�[�o�[�V�[���Z���N�g�w�i
	res = new RES(RES_T::IMG, PATH_IMG + "selectBG.png");
	resourcesMap_.emplace(SRC::IMG_GAMEOVER_SELECT_BG, res);

	// �|�[�Y�w�i
	res = new RES(RES_T::IMG, PATH_IMG + "pause.png");
	resourcesMap_.emplace(SRC::PAUSE, res);

	// FINISH�摜
	res = new RES(RES_T::IMG, PATH_IMG + "finish.png");
	resourcesMap_.emplace(SRC::IMG_FINISH, res);

	// �v���C���[
	res = new RES(RES_T::MODEL, PATH_MDL + "Player/playerA.mv1");
	resourcesMap_.emplace(SRC::PLAYER, res);

	// �v���C���[�e
	res = new RES(RES_T::IMG, PATH_IMG + "Shadow.png");
	resourcesMap_.emplace(SRC::PLAYER_SHADOW, res);

	// �X�J�C�h�[��
	res = new RES(RES_T::MODEL, PATH_MDL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKY_DOME, res);

	// �ŏ��̘f��
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Stage1.mv1");
	resourcesMap_.emplace(SRC::MAIN_PLANET, res);

	// ���Ƃ����̘f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FallPlanet.mv1");
	resourcesMap_.emplace(SRC::FALL_PLANET, res);

	// ���R�Șf��01
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet01.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_01, res);

	// ���R�Șf��02
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/FlatPlanet02.mv1");
	resourcesMap_.emplace(SRC::FLAT_PLANET_02, res);

	// �Ō�̘f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/LastPlanet.mv1");
	resourcesMap_.emplace(SRC::LAST_PLANET, res);

	// ���ʂȘf��
	res = new RES(RES_T::MODEL, PATH_MDL + "Planet/RoadPlanet.mv1");
	resourcesMap_.emplace(SRC::SPECIAL_PLANET, res);

	// ����
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Smoke/Smoke.efkefc");
	resourcesMap_.emplace(SRC::FOOT_SMOKE, res);

	// ���[�v�X�^�[���f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Star/star.mv1");
	resourcesMap_.emplace(SRC::WARP_STAR, res);

	// ���[�v�X�^�[�p��]�G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "StarDust/StarDust.efkefc");
	resourcesMap_.emplace(SRC::WARP_STAR_ROT_EFF, res);

	// ���[�v�̋O�Ր�
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Warp/WarpOrbit.efkefc");
	resourcesMap_.emplace(SRC::WARP_ORBIT, res);

	// �u���b�N�z�[��
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BlackHole/BlackHole.efkefc");
	resourcesMap_.emplace(SRC::BLACK_HOLE, res);

	// �S�[���X�^�[
	res = new RES(RES_T::MODEL, PATH_MDL + "GoalStar/GoalStar.mv1");
	resourcesMap_.emplace(SRC::GOAL_STAR, res);

	// Clear
	res = new RES(RES_T::IMG, PATH_IMG + "Congratulations.png");
	resourcesMap_.emplace(SRC::CLEAR, res);

	// �^���N
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Body.mv1");
	resourcesMap_.emplace(SRC::TANK_BODY, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Wheel.mv1");
	resourcesMap_.emplace(SRC::TANK_WHEEL, res);
	res = new RES(RES_T::MODEL, PATH_MDL + "Tank/Barrel.mv1");
	resourcesMap_.emplace(SRC::TANK_BARREL, res);

	// �Z���N�g�V�[���̔w�i
	res = new RES(RES_T::IMG, PATH_IMG + "Background.png");
	resourcesMap_.emplace(SRC::IMG_BACKGROUND, res);

	// �Z���N�g�ЂƂ��
	res = new RES(RES_T::IMG, PATH_IMG + "Alone.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_ALONE, res);
	
	// �Z���N�g�݂�Ȃ�
	res = new RES(RES_T::IMG, PATH_IMG + "Everyone.png");
	resourcesMap_.emplace(SRC::IMG_SELECT_EVERYONE, res);

	// �X�R�A�`��̔w�i
	res = new RES(RES_T::IMG, PATH_IMG + "Score_BG.png");
	resourcesMap_.emplace(SRC::IMG_SCORE, res);

	//�����ʃX�e�B�b�N
	res = new RES(RES_T::IMG, PATH_IMG + "Operation.png");
	resourcesMap_.emplace(SRC::IMG_OPERATION, res);

	//�R�C���摜
	res = new RES(RES_T::IMG, PATH_IMG + "coin.png");
	resourcesMap_.emplace(SRC::IMG_COIN, res);

	//�o�C�N�摜
	res = new RES(RES_T::IMG, PATH_IMG + "bike.png");
	resourcesMap_.emplace(SRC::IMG_BIKE, res);

	// ���X�e�[�W
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Stage1.mv1");
	resourcesMap_.emplace(SRC::DEMO_STAGE, res);

	//�J�[�u
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/Curve.mv1");
	resourcesMap_.emplace(SRC::CURVE, res);

	// �W�����v��
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/jampRanp/janpudai.mv1");
	resourcesMap_.emplace(SRC::JUMP_RAMP, res);

	//�w�i�X����
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/city/city.mv1");
	resourcesMap_.emplace(SRC::CITY, res);

	//�S�[��
	res = new RES(RES_T::MODEL, PATH_MDL + "demoStage/GoalPost/Goal.mv1");
	resourcesMap_.emplace(SRC::GOAL, res);

	//// �ߋ����G
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemy.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_SHORT, res);

	//// �������G
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemyLong.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_LONG, res);

	//// ���e�G
	//res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/enemyBomb.mv1");
	//resourcesMap_.emplace(SRC::ENEMY_MAGIC, res);

	//�w��
	res = new RES(RES_T::MODEL, PATH_MDL + "Helicopter/Aircraft04.mv1");
	resourcesMap_.emplace(SRC::HELICOPTER, res);

	//�w���H
	res = new RES(RES_T::MODEL, PATH_MDL + "Helicopter/AirCraft04_Rotor.mv1");
	resourcesMap_.emplace(SRC::HELICOPTER_ROTOR, res);

	// �G�����Ƃ����e
	res = new RES(RES_T::MODEL, PATH_MDL + "Enemy/bomb.mv1");
	resourcesMap_.emplace(SRC::BOMB, res);

	//�����_��
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/GoldCoin.mv1");
	resourcesMap_.emplace(SRC::GOLD_COIN, res);

	//�⃁�_��
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/SilverCoin.mv1");
	resourcesMap_.emplace(SRC::SILVER_COIN, res);

	// �����_��
	res = new RES(RES_T::MODEL, PATH_MDL + "Medal/CopperCoin.mv1");
	resourcesMap_.emplace(SRC::COPPER_COIN, res);


	// �o�C�N���f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Bike/bike.mv1");
	resourcesMap_.emplace(SRC::BIKE, res);

	// �^�C�����f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Bike/tyre.mv1");
	resourcesMap_.emplace(SRC::TYRE, res);

	// �Ƃ���
	res = new RES(RES_T::MODEL, PATH_MDL + "Throw/Spikeball.mv1");
	resourcesMap_.emplace(SRC::SPIKE_BALL, res);

	// ���탂�f��
	res = new RES(RES_T::MODEL, PATH_MDL + "Weapon/BusterSword/sword.mv1");
	resourcesMap_.emplace(SRC::W_SWORD, res);

	// �o�[���A�E�g���̃G�t�F�N�g(�o�C�N���ԑO)
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BikeStart/burnoutEffect.efkefc");
	resourcesMap_.emplace(SRC::BURNOUT_EFFECT, res);

	// �Q�[���J�n�G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Start/startEffect.efkefc");
	resourcesMap_.emplace(SRC::START_EFFECT, res);

	// �q�b�g�������̃G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "HitSprite/HitSprite.efkefc");
	resourcesMap_.emplace(SRC::HITEFFECT, res);

	// �������m�����̃G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Throw/makeThrow.efkefc");
	resourcesMap_.emplace(SRC::THROW_MAKE_EFFECT, res);

	// �����G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Bomb/bombEffect.efkefc");
	resourcesMap_.emplace(SRC::BOMB_EFFECT, res);

	// ���e�ꏊ�̃G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BombPlace/bombPlace.efkefc");
	resourcesMap_.emplace(SRC::BOMB_PLACE_EFFECT, res);


	// �������m�����̃G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Throw/makeThrow.efkefc");
	resourcesMap_.emplace(SRC::THROW_MAKE_EFFECT, res);

	// �����G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Bomb/bombEffect.efkefc");
	resourcesMap_.emplace(SRC::BOMB_EFFECT, res);

	// ���e�ꏊ�̃G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "BombPlace/bombPlace.efkefc");
	resourcesMap_.emplace(SRC::BOMB_PLACE_EFFECT, res);


	// �\�j�b�N�G�t�F�N�g
	//res = new RES(RES_T::EFFEKSEER, PATH_EFF + "HitSprite/HitSprite.efkefc");
	//res = new RES(RES_T::EFFEKSEER, PATH_EFF + "SonicBoom/SonicBoom.efkefc");
	//resourcesMap_.emplace(SRC::SonicEffect, res);

	// �u�[�X�g�G�t�F�N�g
	res = new RES(RES_T::EFFEKSEER, PATH_EFF + "Boost/boostEffect.efkefc");
	resourcesMap_.emplace(SRC::BOOST_EFFECT, res);

	// �^�C�g��BGM
	res = new RES(RES_T::SOUND, PATH_SND + "�r��̎�.mp3");
	resourcesMap_.emplace(SRC::SND_TITLE_BGM, res);

	// �Q�[��BGM
	res = new RES(RES_T::SOUND, PATH_SND + "BURNING_MY_HEART.mp3");
	resourcesMap_.emplace(SRC::SND_GAME_BGM, res);

	//�R�C�����W���̉�
	res = new RES(RES_T::SOUND, PATH_SND + "collectcoin.mp3");
	resourcesMap_.emplace(SRC::SND_COIN, res);

	//�u�[�X�Ǝg�p���̉�
	res = new RES(RES_T::SOUND, PATH_SND + "Boost.mp3");
	resourcesMap_.emplace(SRC::SND_BOOST, res);

	//���[�^�[��
	res = new RES(RES_T::SOUND, PATH_SND + "motor.mp3");
	resourcesMap_.emplace(SRC::SND_MOTOR, res);

	//�Z���N�g�V�[���̑I�����̉�
	res = new RES(RES_T::SOUND, PATH_SND + "select.mp3");
	resourcesMap_.emplace(SRC::SND_SELECT, res);

	//�Z���N�g���莞�̉�
	res = new RES(RES_T::SOUND, PATH_SND + "gameStart.mp3");
	resourcesMap_.emplace(SRC::SND_START, res);

	//�x����
	res = new RES(RES_T::SOUND, PATH_SND + "Warning.mp3");
	resourcesMap_.emplace(SRC::SND_WARNING, res);

	//������
	res = new RES(RES_T::SOUND, PATH_SND + "Explosion.mp3");
	resourcesMap_.emplace(SRC::SND_EXPLOSION, res);

	//�S�[����
	res = new RES(RES_T::SOUND, PATH_SND + "Goal.mp3");
	resourcesMap_.emplace(SRC::SND_GOAL, res);
}


void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second.Release();
	}

	loadedMap_.clear();
}

void ResourceManager::Destroy(void)
{
	Release();
	for (auto& res : resourcesMap_)
	{
		res.second->Release();
		delete res.second;
	}
	resourcesMap_.clear();
	delete instance_;
}

const Resource& ResourceManager::Load(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return dummy_;
	}
	return res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource& res = _Load(src);
	if (res.type_ == Resource::TYPE::NONE)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res.handleId_);
	res.duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

Resource& ResourceManager::_Load(SRC src)
{

	// ���[�h�ς݃`�F�b�N
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return *resourcesMap_.find(src)->second;
	}

	// ���\�[�X�o�^�`�F�b�N
	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return dummy_;
	}

	// ���[�h����
	rPair->second->Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	loadedMap_.emplace(src, *rPair->second);

	return *rPair->second;

}
