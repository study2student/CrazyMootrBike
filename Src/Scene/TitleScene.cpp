#include <string>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/MyUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Common/Transform.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Score.h"
#include "../Object/DataSave.h"
#include "TitleScene.h"

#pragma region �ϐ��錾
	//�{�^����������Ă���̃o�C�N�̑ҋ@�ő厞��
	const float BIKE_IDLE__MAX_TIME = 3.0f;

	//�o�C�N�����Ԃ��Ă��玟�̃V�[���܂ł̍ő厞��
	const float BIKE_DEPARTURE_TO_NEXT_MAX_TIME = 4.5f;

	// ���C���X�e�[�W���W
	const VECTOR MAINSTAGE_POS = { -20150.0f, -12000.0f, 6100.0f };
	// ���C���X�e�[�W�傫��
	const VECTOR MAINSTAGE_SIZE = { 3.5f, 2.5f, 4.5f };

	// �r���Q�̍��W,�傫��,�p�x
	const VECTOR SITY_POS = { 250.0f, -100.0f, 21100.0f };
	const VECTOR SITY_SIZE = { 1.0f,1.0f,1.0f };
	const VECTOR SITY_ROT = { 0.0f, MyUtility::Deg2RadF(-90.0f), 0.0f };

	// �o�C�N�̍��W�A�傫���A�p�x
	const VECTOR BIKE_POS = { -150.0f, -100.0f, -100.0f };
	const VECTOR BIKE_SIZE = { 1.3f,1.3f,1.3f };
	const VECTOR BIKE_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// �v���C���[�̍��W�A�傫���A�p�x
	const VECTOR CHARACTOR_POS = { -150.0f, -50.0f, -109.0f };
	const VECTOR CHARACTOR_SIZE = { 1.0f, 1.0f, 1.0f };
	const VECTOR CHARACTOR_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// �o�C�N����t�����g�^�C�����΍��W
	const VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE = { 10.0f,68.0f,45.0f };
	// �t�����g�^�C���̑傫��,�p�x
	const VECTOR FRONT_TYRE_SIZE = { 1.3f,1.3f,1.3f };
	const VECTOR FRONT_TYRE_ROT = { 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f };

	// �o�C�N���烊�A�^�C�����΍��W
	const VECTOR BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE = { -5.0f,58.0f,-157.0f };
	// ���A�^�C���̑傫���A�p�x
	const VECTOR REAR_TYRE_SIZE = { 1.7f,1.7f,1.7f };
	const VECTOR REAR_TYRE_ROT = { 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f };

	// �A�j���[�V�����X�s�[�h
	const float ANIMATION_SPEED = 1.0f;
	//�A�j���[�V�����̍Đ��ʒu
	const float ANIMATION_START_STEP = 33.0f;
	const float ANIMATION_END_STEP = 36.0f;

	// �_�ŕ\��
	const float MAX_STEP_FLASH = 0.8f;

	// �v�b�V�������摜�̍��W
	const VECTOR IMG_PUSH = { Application::SCREEN_SIZE_X / 2 - 145, 700 };

	// �o�C�N�̃X�s�[�h
	const float BIKE_SPEED = 60.0f;

	// �^�C���̉�]��(IDLE���)
	const float IDLE_TYRE_ROT_SPEED = 10.0f;

	// �^�C���̉�]��(START���)
	const float START_TYRE_ROT_SPEED = 20.0f;

	// �X�^�[�g���̃G�t�F�N�g�̑傫��
	const float START_EFFECT_SIZE = 50.0f;

	// �X�^�[�g���̃G�t�F�N�g�̑��Έʒu
	const float START_EFFECT_LOCAL_POS = 100.0f;

	// �X�^�[�g�O�̑ҋ@���G�t�F�N�g�̑傫��
	const float BURNOUT_IDLE_EFFECT_SIZE = 60.0f;

	// �X�^�[�g�O�̑ҋ@���G�t�F�N�g��Z���W�̑��Έʒu
	const float BURNOUT_IDLE_EFFECT_LOCALPOS_Z = 80.0f;

	// �X�^�[�g���̃o�[���G�t�F�N�g�̑傫��
	const float BURNOUT_MOVE_EFFECT_SIZE = 60.0f;

	// ���X�ɍ������グ��
	const float GRADUALLY_INCREASE_HEIGHT = 1.1f;
#pragma endregion

TitleScene::TitleScene(void)
	:
	imgTitle_(-1),
	imgPush_(-1),
	skyDome_(nullptr),
	animationController_(nullptr),
	isBikeDeparture_(false),
	tyreRotX_({}),
	effectStartResId_(-1),
	effectStartPlayId_(-1),
	effectBurnoutResId_(-1),
	effectBurnoutPlayId_(-1),
	effectBurnoutPosY_(0.0f),
	stepBikeDeparture_(0.0f),
	stepFlash_(0.0f),
	isInvisible_(false),
	state_(STATE::IDLE)
{
}

TitleScene::~TitleScene(void)
{
	delete animationController_;

	StopEffekseer3DEffect(effectBurnoutPlayId_);
	StopEffekseer3DEffect(effectStartPlayId_);
}

void TitleScene::Init(void)
{

	// �摜�ǂݍ���
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// �w�i
	spaceDomeTran_.pos = MyUtility::VECTOR_ZERO;
	skyDome_ = std::make_unique<SkyDome>(spaceDomeTran_);
	skyDome_->Init();

	// ���C���X�e�[�W
	mainStage_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));
	mainStage_.pos = {MAINSTAGE_POS};
	mainStage_.quaRot = Quaternion();
	mainStage_.scl = { MAINSTAGE_SIZE };
	mainStage_.Update();

	// �r��
	sity_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));
	sity_.pos = { SITY_POS };
	sity_.scl = { SITY_SIZE };
	sity_.quaRotLocal = Quaternion::Euler(SITY_ROT);
	sity_.Update();


	//�o�C�N
	bike.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BIKE));
	bike.pos = { BIKE_POS };
	bike.scl = { BIKE_SIZE };
	bike.quaRotLocal = Quaternion::Euler(BIKE_ROT);
	bike.Update();

	// �L����
	charactor_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	charactor_.pos = { CHARACTOR_POS };
	charactor_.scl = { CHARACTOR_SIZE };
	charactor_.quaRot = Quaternion::Euler(CHARACTOR_ROT);
	charactor_.Update();

	// �t�����g�^�C��
	frontTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	frontTyre_.pos = VAdd(bike.pos, BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE);
	frontTyre_.scl = { FRONT_TYRE_SIZE };
	frontTyre_.quaRot = Quaternion::Euler(
		FRONT_TYRE_ROT);
	frontTyre_.Update();

	// ���A�^�C��
	rearTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	rearTyre_.pos = VAdd(bike.pos, BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE);
	rearTyre_.scl = { REAR_TYRE_SIZE };
	rearTyre_.quaRot = Quaternion::Euler(
		REAR_TYRE_ROT);
	rearTyre_.Update();

	// �A�j���[�V�����̐ݒ�
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(charactor_.modelId);
	// �������A�j���[�V����
	animationController_->Add(0, path + "Sit.mv1",ANIMATION_SPEED);
	animationController_->Play(0, true, ANIMATION_START_STEP, ANIMATION_END_STEP);

	// ��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//�ҋ@���
	state_ = STATE::IDLE;

	//�G�t�F�N�g�ǂݍ���
	InitEffect();

	effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_FIRST_POS_Y;

	//�X�R�A���Z�b�g
	score_.ResetScore();

	//�f�[�^���Z�b�g
	data_.ResetData();

	 //BGM���Đ�
	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_TITLE_BGM).handleId_, DX_PLAYTYPE_LOOP, false);

}

void TitleScene::Update(void)
{
	switch (state_)
	{
	case TitleScene::STATE::IDLE:
		UpdateIdle();
		break;
	case TitleScene::STATE::START:
		UpdateStart();
		break;
	}

	//�A�j���[�V�������[�v
	animationController_->SetEndLoop(ANIMATION_START_STEP, ANIMATION_START_STEP, ANIMATION_SPEED);

}

void TitleScene::Draw(void)
{

	skyDome_->Draw();

	MV1DrawModel(charactor_.modelId);
	MV1DrawModel(bike.modelId);
	MV1DrawModel(mainStage_.modelId);
	MV1DrawModel(sity_.modelId);
	MV1DrawModel(frontTyre_.modelId);
	MV1DrawModel(rearTyre_.modelId);
	//�{�^���������ꂽ��\�����Ȃ�
	if(state_==STATE::IDLE)
	{
		DrawExtendGraph(160, 150, Application::SCREEN_SIZE_X - 160,Application::SCREEN_SIZE_Y, imgTitle_, true);

		//�_�ŕ\��
		if (!isInvisible_)
		{
			stepFlash_ += SceneManager::GetInstance().GetDeltaTime();
			if (stepFlash_ >= MAX_STEP_FLASH)
			{
				isInvisible_ = true;
			}
		}
		else
		{
			stepFlash_ -= SceneManager::GetInstance().GetDeltaTime();
			if (stepFlash_ <= 0.0f)
			{
				isInvisible_ = false;
			}
		}

		if(!isInvisible_)
		{
			DrawGraph(IMG_PUSH.x, IMG_PUSH.y, imgPush_, true);
		}
	}

}

void TitleScene::InitEffect(void)
{
	effectBurnoutResId_=ResourceManager::GetInstance().Load(
			ResourceManager::SRC::BURNOUT_EFFECT).handleId_;

	effectStartResId_= ResourceManager::GetInstance().Load(
		ResourceManager::SRC::START_EFFECT).handleId_;
}

void TitleScene::ChangeState(STATE state)
{
	state_ = state;

	switch (state_)
	{
	case TitleScene::STATE::IDLE:
		ChangeStateIdle();
		break;
	case TitleScene::STATE::START:
		ChangeStateStart();
		break;
	}
}

void TitleScene::ChangeStateIdle(void)
{
}

void TitleScene::ChangeStateStart(void)
{
	//�G���W����
	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false);

	//�X�^�[�g�G�t�F�N�g
	StartEffect();
}

void TitleScene::UpdateIdle(void)
{
	//�^�C����]
	BikeTyreRot();

	//�ҋ@���G�t�F�N�g(�X�^�[�g�O)
	BurnoutIdleEffect();

	//���o�X�^�[�g
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
	{
		ChangeState(STATE::START);
	}

	////�X�L�b�v�f�o�b�O
	if (ins.IsTrgDown(KEY_INPUT_S))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// �L�����A�j���[�V����
	animationController_->Update();

	skyDome_->Update();
}

void TitleScene::UpdateStart(void)
{
	//�^�C����]
	BikeTyreRot();

	// �V�[���J��
	//���Ԍo��
	stepBikeDeparture_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepBikeDeparture_ <= BIKE_IDLE__MAX_TIME)
	{
		//�ҋ@���G�t�F�N�g(�X�^�[�g����)
		BurnoutMoveEffect();
	}
	else
	{
		//�o�C�N����
		BikeDeparture();
	}
	if (stepBikeDeparture_ >= BIKE_DEPARTURE_TO_NEXT_MAX_TIME)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		stepBikeDeparture_ = 0.0f;
	}

	// �L�����A�j���[�V����
	animationController_->Update();
}

void TitleScene::BikeDeparture(void)
{
	//�o�C�N��O��������
	//�o�C�N�X�s�[�h
	bike.pos = VAdd(bike.pos, VScale(bike.GetForward(), BIKE_SPEED));
	bike.Update();
	charactor_.pos.z = bike.pos.z;
	charactor_.Update();
	frontTyre_.pos = VAdd(bike.pos, BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE);
	rearTyre_.pos = VAdd(bike.pos, BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE);
	frontTyre_.Update();
	rearTyre_.Update();
	isBikeDeparture_ = true;
}

void TitleScene::BikeTyreRot(void)
{
	auto& ins = InputManager::GetInstance();

	//�^�C���̉�]
	//���x�ύX
	float speedRot = 0.0f;
	switch (state_)
	{
	case TitleScene::STATE::IDLE:
		speedRot = IDLE_TYRE_ROT_SPEED;
		break;
	case TitleScene::STATE::START:
		speedRot = START_TYRE_ROT_SPEED;
		break;
	}
	// �f�O���[���烉�W�A��(�ϊ�)
	float rad = MyUtility::Deg2RadF(speedRot);

	// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_X);

	//�N�H�[�^�j�I��(��])�̍���
	tyreRotX_ = tyreRotX_.Mult(rotPow);

	// ��]������
	//��֋쓮�̂��ߍŏ��̓��A�^�C��������
	rearTyre_.quaRot = tyreRotX_;
	rearTyre_.Update();
	if(isBikeDeparture_)
	{
		//���Ԃ�����t�����g�^�C����
		frontTyre_.quaRot = tyreRotX_;
		frontTyre_.Update();
	}

}

void TitleScene::StartEffect(void)
{
	effectStartPlayId_ = PlayEffekseer3DEffect(effectStartResId_);
	float scale = START_EFFECT_SIZE;
	SetScalePlayingEffekseer3DEffect(effectStartPlayId_, scale, scale, scale);
	SetPosPlayingEffekseer3DEffect(effectStartPlayId_, bike.pos.x, bike.pos.y + START_EFFECT_LOCAL_POS, bike.pos.z - START_EFFECT_LOCAL_POS);
	SetRotationPlayingEffekseer3DEffect(effectStartPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}

void TitleScene::BurnoutIdleEffect(void)
{
	effectBurnoutPlayId_ = PlayEffekseer3DEffect(effectBurnoutResId_);
	float scale = BURNOUT_IDLE_EFFECT_SIZE;
	SetScalePlayingEffekseer3DEffect(effectBurnoutPlayId_, scale / 2, scale, scale);
	SetPosPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.pos.x, Bike::IDLE_EFFECT_POS_Y, bike.pos.z - BURNOUT_IDLE_EFFECT_LOCALPOS_Z);
	SetRotationPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}

void TitleScene::BurnoutMoveEffect(void)
{
	effectBurnoutPlayId_ = PlayEffekseer3DEffect(effectBurnoutResId_);
	float scale = BURNOUT_MOVE_EFFECT_SIZE;
	SetScalePlayingEffekseer3DEffect(effectBurnoutPlayId_, scale / 2, scale, scale);

	//���X�ɍ������グ��
	effectBurnoutPosY_ += stepBikeDeparture_ * GRADUALLY_INCREASE_HEIGHT;
	if (effectBurnoutPosY_ >= Bike::BURNOUT_EFFECT_MAX_POS_Y)
	{
		//��������
		effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_MAX_POS_Y;
	}
	SetPosPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.pos.x, bike.pos.y + effectBurnoutPosY_, bike.pos.z);
	SetRotationPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}
