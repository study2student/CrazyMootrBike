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

		DEMO_STAGE,//���X�e�[�W
		JUMP_RAMP,//�W�����v��
		CURVE,	//�J�[�u
		CITY,	//�w�i�X����

		ENEMY_SHORT,		//���G
		ENEMY_LONG,
		ENEMY_MAGIC,
		HELICOPTER,
		HELICOPTER_ROTOR,
		BOMB,		//���e
		BIKE,		//�o�C�N
		TYRE,		//�^�C��
		BURNOUT_EFFECT, //�o�[���A�E�g�G�t�F�N�g(�o�C�N���ԑO)

		W_SWORD,//�o�X�^�[�\�[�h

		//�G�t�F�N�g�֘A
		HITEFFECT,//�q�b�g�������̃G�t�F�N�g
		SONICEFFECT,//�\�j�b�N�G�t�F�N�g

		//�T�E���h�֘A
		SND_BGM,//�Q�[��BGM
		SND_COIN,//�R�C�����W���̉�
		SND_MOTOR//���[�^�[��
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
