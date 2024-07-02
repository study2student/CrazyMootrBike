#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <memory>
#include <vector>
class SceneManager;
class SkyDome;
class AnimationController;
class Bike;
class Score;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		IDLE,
		START
	};

	//�{�^����������Ă���̃o�C�N�̑ҋ@�ő厞��
	static constexpr float BIKE_IDLE__MAX_TIME = 3.0f;

	//�o�C�N�����Ԃ��Ă��玟�̃V�[���܂ł̍ő厞��
	static constexpr float BIKE_DEPARTURE_TO_NEXT_MAX_TIME = 4.5f;

	//�o�C�N����t�����g�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE = { 10.0f,68.0f,45.0f };

	//�o�C�N���烊�A�^�C�����΍��W
	static constexpr VECTOR BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE = { -5.0f,58.0f,-157.0f };

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

	//�t�����g�^�C��
	Transform frontTyre_;

	//���A�^�C��
	Transform rearTyre_;

	// �A�j���[�V����
	AnimationController* animationController_;

	//�o�C�N
	Transform bike;

	//�o�C�N�����Ԃ������ǂ���
	bool isBikeDeparture_;

	//��]
	Quaternion tyreRotX_;

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

	//�^�C����]
	void BikeTyreRot(void);

	//�o�[���A�E�g�G�t�F�N�g
	void BurnoutEffect(void);

};
