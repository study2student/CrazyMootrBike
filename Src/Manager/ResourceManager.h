#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		SPEECH_BALLOON,
		WARNING,	//�x��
		PAUSE,		//�|�[�Y�w�i
		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_01,
		FLAT_PLANET_02,
		LAST_PLANET,
		SPECIAL_PLANET,
		FOOT_SMOKE,
		WARP_STAR,
		WARP_STAR_ROT_EFF,
		WARP_ORBIT,
		BLACK_HOLE,
		GOAL_STAR,
		CLEAR,
		TANK_BODY,
		TANK_BARREL,
		TANK_WHEEL,

		//�摜�֘A
		IMG_BACKGROUND,// �Z���N�g�V�[���̔w�i
		IMG_SELECT_ALONE,//�ЂƂ��
		IMG_SELECT_EVERYONE,//�݂�Ȃ�
		IMG_SCORE,//�X�R�A�̔w�i
		IMG_OPERATION,//����������
		IMG_GAMEOVER_SELECT_BG,	//�Q�[���I�[�o�[�V�[���Z���N�g�w�i
		IMG_FINISH,		//�t�B�j�b�V�������摜

		DEMO_STAGE,//���X�e�[�W
		JUMP_RAMP,//�W�����v��
		CURVE,	//�J�[�u
		CITY,	//�w�i�X����
		GOAL,	//�S�[��

		//ENEMY_SHORT,		//���G
		//ENEMY_LONG,
		//ENEMY_MAGIC,
		HELICOPTER,
		HELICOPTER_ROTOR,
		BOMB,		//���e
		BIKE,		//�o�C�N
		TYRE,		//�^�C��
		BURNOUT_EFFECT, //�o�[���A�E�g�G�t�F�N�g(�o�C�N���ԑO)
		START_EFFECT, //�Q�[���J�n�G�t�F�N�g

		GOLD_COIN,		//�����_��
		SILVER_COIN,	//�⃁�_��
		COPPER_COIN,	//�����_��

		W_SWORD,//�o�X�^�[�\�[�h
		SPIKE_BALL, //�Ƃ���

		HITEFFECT,//�q�b�g�������̃G�t�F�N�g
		SONICEFFECT,//�\�j�b�N�G�t�F�N�g
		BOOST_EFFECT, //�u�[�X�g�G�t�F�N�g
		THROW_MAKE_EFFECT,	//�������m�����G�t�F�N�g
		BOMB_EFFECT,		//�����G�t�F�N�g
		BOMB_PLACE_EFFECT,	//���e�ꏊ�G�t�F�N�g

		//�T�E���h�֘A
		SND_TITLE_BGM,//�^�C�g��BGM
		SND_GAME_BGM,//�Q�[��BGM
		SND_COIN,//�R�C�����W���̉�
		SND_BOOST,//�u�[�X�Ǝg�p���̉�
		SND_MOTOR,//���[�^�[��
		SND_SELECT,//�Z���N�g�V�[���̑I�����̉�
		SND_START,//�Z���N�g���莞�̉�
		SND_WARNING,//�x����
		SND_EXPLOSION,//������
		SND_GOAL,//�S�[����
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̊��S�j��
	void Destroy(void);

	// ���\�[�X�̃��[�h
	const Resource& Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource*> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// �������[�h
	Resource& _Load(SRC src);

};
