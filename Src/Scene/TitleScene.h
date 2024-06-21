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

	//�o�C�N
	Transform bike;


	// �A�j���[�V����
	AnimationController* animationController_;

};
