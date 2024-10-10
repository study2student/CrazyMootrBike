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
class DataSave;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		IDLE,
		START
	};

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
	// �v�b�V�������摜
	int imgPush_;

	// �X�J�C�h�[���p
	Transform spaceDomeTran_;

	// �X�J�C�h�[��(�w�i)
	std::unique_ptr<SkyDome> skyDome_;

	//���H
	Transform mainStage_;

	//�r��
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
	//�J�n�G�t�F�N�g
	int effectStartResId_;
	int effectStartPlayId_;

	// �o�[���A�E�g�G�t�F�N�g
	int effectBurnoutResId_;
	int effectBurnoutPlayId_;
	float effectBurnoutPosY_;

	// �G�t�F�N�g������
	void InitEffect(void);

	//�o�C�N�̔��Ԍo�ߎ���
	float stepBikeDeparture_;

	//�摜�_��
	//�_�Ŏ���
	float stepFlash_;

	//�����Ă��邩�ǂ���
	bool isInvisible_;

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

	//�X�^�[�g�G�t�F�N�g
	void StartEffect(void);

	//�o�[���A�E�g�G�t�F�N�g(�X�^�[�g�����O)
	void BurnoutIdleEffect(void);

	//�o�[���A�E�g�G�t�F�N�g(�X�^�[�g��������)
	void BurnoutMoveEffect(void);


};
