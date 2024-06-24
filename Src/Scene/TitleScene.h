#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <memory>
#include <vector>
class SceneManager;
class SkyDome;
class AnimationController;
class Bike;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		IDLE,
		START
	};

	//�o�[���A�E�g�G�t�F�N�g��������
	static constexpr float BURNOUT_EFFECT_FIRST_POS_Y = -290.0f;

	//�o�[���A�E�g�G�t�F�N�g�ő卂��
	static constexpr float BURNOUT_EFFECT_MAX_POS_Y = -180.0f;

	//�{�^����������Ă���̃o�C�N�̑ҋ@�ő厞��
	static constexpr float BIKE_IDLE__MAX_TIME = 3.0f;

	//�o�C�N�����Ԃ��Ă��玟�̃V�[���܂ł̍ő厞��
	static constexpr float BIKE_DEPARTURE_TO_NEXT_MAX_TIME = 4.5f;


	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// �摜
	int imgTitle_;
	int imgPush_;

	// �X�J�C�h�[���p
	Transform spaceDomeTran_;

	// �X�J�C�h�[��(�w�i)
	SkyDome* skyDome_;

	//���H
	Transform mainStage_;

	//�r��
	/*std::vector<Transform*> sity_;*/
	Transform sity_;

	// �L�����N�^�[
	Transform charactor_;

	// �A�j���[�V����
	AnimationController* animationController_;

	//�o�C�N
	Transform bike;

	//�G�t�F�N�g�n
	// �o�[���A�E�g�G�t�F�N�g
	int effectBurnoutResId_;
	int effectBurnoutPlayId_;
	float effectBurnoutPosY_;

	// �G�t�F�N�g������
	void InitEffect(void);

	//�o�C�N�̔��Ԍo�ߎ���
	float stepBikeDeparture_;

	//���
	STATE state_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateStart(void);

	// �X�V�X�e�b�v
	void UpdateIdle(void);
	void UpdateStart(void);

	//�o�C�N���ԉ��o
	void BikeDeparture(void);

	//�o�[���A�E�g�G�t�F�N�g
	void BurnoutEffect(void);

};
