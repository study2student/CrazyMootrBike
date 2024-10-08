#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>
#include "../Utility/MyUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/Stage/SkyDome.h"
#include "../Object/Stage/Stage.h"
#include "../Object/Stage/Planet.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Coin/CoinBase.h"
#include "../Object/Coin/GoldCoin.h"
#include "../Object/Coin/SilverCoin.h"
#include "../Object/Coin/CopperCoin.h"
#include "../Object/Gimmick/Bomb.h"
#include "../Object/Gimmick/Helicopter.h"
#include "../Object/Gimmick/Spike.h"
#include "../Object/DataSave.h"
#include "../Object/Score.h"
#include "../Application.h"
#include "GameScene.h"

#pragma region �萔�錾

//4�l�ΐ�p�̃S�[�������ʒu
//�v���C���[1
const Vector2 FINISH_FONT_POS_MULTI_P1 = { 450,250 };

//�v���C���[2
const Vector2 FINISH_FONT_POS_MULTI_P2 = { 1400,250 };

//�v���C���[3
const Vector2 FINISH_FONT_POS_MULTI_P3 = { 450,750 };

//�v���C���[4
const Vector2 FINISH_FONT_POS_MULTI_P4 = { 1400,750 };

//��ʕ����g���̔����̑���
const int HALF_BORDER_SIZE = 5;

//���S���w�i�̍���ƉE�����W�ƕ����̈ʒusx / 2 + halfBorderSize, sy / 2 + halfBorderSize, sx, sy
//�v���C���[1
const Vector2 DEAD_BACK_BOX_MIN_POS_P1 = { 0, 0 };
const Vector2 DEAD_BACK_BOX_MAX_POS_P1 = { Application::SCREEN_SIZE_X / 2 - HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y / 2 - HALF_BORDER_SIZE };
const Vector2 DEAD_FONT_POS_P1 = { 350,200 };

//�v���C���[2
const Vector2 DEAD_BACK_BOX_MIN_POS_P2 = { Application::SCREEN_SIZE_X / 2 + HALF_BORDER_SIZE, 0 };
const Vector2 DEAD_BACK_BOX_MAX_POS_P2 = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y / 2 - HALF_BORDER_SIZE };
const Vector2 DEAD_FONT_POS_P2 = { 1300,200 };

//�v���C���[3
const Vector2 DEAD_BACK_BOX_MIN_POS_P3 = { 0, Application::SCREEN_SIZE_Y / 2 + HALF_BORDER_SIZE };
const Vector2 DEAD_BACK_BOX_MAX_POS_P3 = { Application::SCREEN_SIZE_X / 2 - HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y};
const Vector2 DEAD_FONT_POS_P3 = { 350,700 };

//�v���C���[4
const Vector2 DEAD_BACK_BOX_MIN_POS_P4 = { Application::SCREEN_SIZE_X / 2 + HALF_BORDER_SIZE, Application::SCREEN_SIZE_Y / 2 + HALF_BORDER_SIZE };
const Vector2 DEAD_BACK_BOX_MAX_POS_P4 = { Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y };
const Vector2 DEAD_FONT_POS_P4 = { 1300,700 };

//���S�����傫��
const double DEAD_FONT_EXRATE = 6;

//���S�����傫��
const std::string DEAD_FONT = "DEAD";

#pragma endregion


GameScene::GameScene(void)
{
	coin_ = nullptr;
	stage_ = nullptr;
	helicopter_ = nullptr;
	spike_ = nullptr;
	//score_ = nullptr;

	nowCursor_ = 0;
	stepGoalAfter_ = 0.0f;
}

GameScene::~GameScene(void)
{
	delete coin_;
}

void GameScene::Init(void)
{
	//�v���C���[�l��
	playNumber_ = data_.GetData().playerNum_;
	if (playNumber_ == 1)
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
	}
	else
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	}

	// �v���C���[
	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(PLAYER_WIDTH * (i + 1), i));
	}

	for (auto& bike : bikes_) {
		bike->Init();
	}

	// �R�C��
	for (auto& bike : bikes_) {
		coin_ = new CoinBase(bikes_,this, { 0.0f,800.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	//�w���R�v�^�[
	helicopter_ = std::make_shared<Helicopter>(this);
	helicopter_->Init();

	//�����g�Q
	spike_ = std::make_shared<Spike>();
	spike_->Init();

	// �X�e�[�W
	stage_ = std::make_shared<Stage>(bikes_, coin_, helicopter_->GetBomb(), spike_, this);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	for (auto& bike : bikes_) {
		auto sky = std::make_unique<SkyDome>(bike->GetTransform());
		sky->Init();
		skyDomes_.emplace_back(std::move(sky));
	}

	if (playNumber_ == 1)
	{
		cameras_.push_back(std::make_shared<Camera>());
	}
	else
	{
		cameras_.push_back(std::make_shared<Camera>()); // �e�v���C���[�̃J�������쐬
		cameras_.push_back(std::make_shared<Camera>()); // �e�v���C���[�̃J�������쐬
		cameras_.push_back(std::make_shared<Camera>()); // �e�v���C���[�̃J�������쐬
		cameras_.push_back(std::make_shared<Camera>()); // �e�v���C���[�̃J�������쐬
	}


	for (int i = 0; i < cameras_.size(); i++) {
		cameras_[i]->Init();
		cameras_[i]->ChangeMode(Camera::MODE::FOLLOW);
		cameras_[i]->SetFollow(&bikes_[i]->GetTransform()); // �e�v���C���[�̃o�C�N��Ǐ]
	}

	// �G�t�F�N�g������
	InitEffect();

	//�G���J�E���g���Z�b�g
	enCounter = 0;

	//�R�C�����������ꂽ��
	isCreateCoin_ = false;

	hitStopDuration = 6000.0f;
	hitStopTimer = 0.0f;
	isHitStop = false;

	//�|�[�Y���j���[
	//�F
	reStartFontColor_ = GetColor(255, 255, 255);
	reTryFontColor_ = GetColor(255, 255, 255);
	endFontColor_ = GetColor(255, 255, 255);

	//����̍ĊJ�|�W�V����
	int addX = -55;
	reStartFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 - 130 };

	//����̃��g���C�|�W�V����
	addX = -95;
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 10 };

	//����̏I���|�W�V����
	addX = -70;
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + addX , Application::SCREEN_SIZE_Y / 2 + 150 };

	//FINISH�����̏����ʒu
	int addPosX = 0;
	Vector2 finishStartPos = { Application::SCREEN_SIZE_X / 2 + addPosX ,-40 };
	finishFontMovePos_ = finishStartPos;

	isPause_ = false;
	isCursorHit_ = false;
	stepPauseKeyHit_ = 0.0f;

	onePersonIsGoal_ = false;

	//�摜
	imgWarning_= resMng_.Load(ResourceManager::SRC::WARNING).handleId_;
	warningImgScale_ = WARNING_IMG_MIN_SCALE;

	imgPause_ = resMng_.Load(ResourceManager::SRC::PAUSE).handleId_;
	imgFinish_= resMng_.Load(ResourceManager::SRC::IMG_FINISH).handleId_;
	imgCoin_= resMng_.Load(ResourceManager::SRC::IMG_COIN).handleId_;


	//�X�R�A���Z�b�g
	score_.ResetScore();

	// �Q�[���X�^�[�g���̃J�E���g
	startCount_ = 3.0f;
	isStart_ = false;

	//�G���W����
	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false);
}

void GameScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();

	//�|�[�Y���j���[
	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) -1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}

		if (ins.IsTrgDown(KEY_INPUT_C) || ins.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START))
		{
			//�|�[�Y���
			isPause_ = true;
		}
	}
	
	//�|�[�Y���̓J�����̉�]�������Ȃ����߂ɏ���n���Ă���
	SceneManager::GetInstance().GetCamera()->SetIsPause(isPause_);

	//�|�[�Y���͑��������f
	if (isPause_)
	{
		Pause();
		return;
	}

	//�ŏ��̐l���S�[��������
	if (stage_->GetIsGoal())
	{
		GoalProcess();
	}
	
	//1�l�v���C���S�[���������ǂ����Z�b�g
	onePersonIsGoal_ = stage_->GetIsGoal();

	// �X�^�[�g���̃J�E���g�_�E�������炷
	if (startCount_ > 0.0f)
	{
		startCount_ -= 1 / 60.0f;
	}


	if (startCount_ <= 0.0f)
	{
		float deltaTime = hitStopDuration;

		//BGM���Đ�
		ChangeVolumeSoundMem(125,PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_GAME_BGM).handleId_, DX_PLAYTYPE_LOOP, false));

		for (auto& skyDome : skyDomes_)
		{
			skyDome->Update();
		}

		if (!isHitStop)
		{
			if (playNumber_ == 1)
			{
				bikes_[0]->Update();
			}
			else
			{
				for (auto& bike : bikes_) {
					bike->Update();
				}
			}

		}
		else
		{
			// �q�b�g�X�g�b�v���̏ꍇ�A�^�C�}�[���X�V
			hitStopTimer -= deltaTime;
			if (hitStopTimer <= 0.f) {
				isHitStop = false;
			}
		}

		//�w��
		//�擪(���W)�̗v�f�ԍ��擾
		size_t posZMaxIndex = GetBikeMaxPosZIndex();
		helicopter_->SetBikeTrans(bikes_[posZMaxIndex]->GetTransform());
		helicopter_->SetBikeIsOutside(bikes_[0]->GetIsOutSide());

		//�������m
		spike_->Update();
		spike_->SetTransform(bikes_[posZMaxIndex]->GetTransform());


		//�R�C��
		size_t sizeE = coins_.size();
		for (int i = 0; i < sizeE; i++)
		{
			coins_[i]->Update();
			if (coins_[i]->GetIsAddScore())
			{
				isHitStop = true;
			}
		}

		//�Փ˔���
		Collision();

		// �o�C�N���m�̏Փ˔���
		BikeCollision();

		//�X�e�[�W���������ꂽ��R�C����z�u����
		if (stage_->GetIsMakeLoopStage())
		{
			CoinPlace();
		}
		else
		{
			stage_->Update();
			isCreateCoin_ = false;
		}

		helicopter_->Update();

	}
}

void GameScene::Draw(void)
{
	if (playNumber_ == 1)
	{

		DrawObject(0);

		// �e�o�C�N��`��
		bikes_[0]->Draw();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
		UpdateEffekseer3D();

		// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
		DrawEffekseer3D();

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, mainScreen_, false);
		//�X�R�A�`��
		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2, 0, 2, 2, 0xff0000, "Player %d       :%d", 1, bikes_[0]->GetScore());
		int coinX_ = 240;
		int coinY_ = 20;
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2 + coinX_, coinY_, 0.15, 0.0, imgCoin_, true);
		//HP�`��
		using ap = Application;
		int sc_x = ap::SCREEN_SIZE_X - 500;
		int sc_y = ap::SCREEN_SIZE_Y - 100;

		// HP�o�[�̕�
		int HP_BAR_WIDTH = ap::SCREEN_SIZE_X - HP_BER - sc_x;
		// HP�o�[�̍���
		int HP_BAR_HEIGHT = ap::SCREEN_SIZE_Y - HP_BER;
		// HP�o�[��`��
		DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HP�o�[�̔w�i
		DrawBox(sc_x, sc_y, sc_x + (bikes_[0]->GetHP() * HP_BAR_WIDTH) / Bike::MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HP�o�[

		// HP�̍��g
		DrawBoxAA((float)sc_x, (float)sc_y,
			(float)ap::SCREEN_SIZE_X - HP_BER, (float)HP_BAR_HEIGHT,
			0x000000, false, 13.0f);

		// �X�^�[�g���̃J�E���g�����炷
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 50 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2 -95, 15, 15, 0xffffff, "%.f", startCount_);
		}


	}
	else
	{
		//�X�N���[���T�C�Y
		int sx = Application::SCREEN_SIZE_X;
		int sy = Application::SCREEN_SIZE_Y;

		for (int i = 0; i < cameras_.size(); i++)
		{

			DrawObject(i);

			// �e�o�C�N��`��
			for (auto& bike : bikes_) {
				bike->Draw();
				
			}

			// Effekseer�ɂ��Đ����̃G�t�F�N�g���X�V����B
			UpdateEffekseer3D();

			// Effekseer�ɂ��Đ����̃G�t�F�N�g��`�悷��B
			DrawEffekseer3D();

			for (int p = 0; p < bikes_.size(); p++) {
				//int score = bikes_[p]->GetScore();

				SetDrawScreen(DX_SCREEN_BACK);

				//1�g�̉��̑傫��
				int width = 600;

				//�R�C���������W
				int coinX = 140;
				int coinY = 31;

				switch (i)
				{
				case 0:
					DrawGraph(0, 0, mainScreen_, false);
					DrawUI(sx / 2 - width, 0, 0);
					CoinImgDraw(sx / 2 - coinX, coinY);

					//�G�t�F�N�g�̍Đ�
					if (bikes_[0]->GetIsBoost())
					{
						bikes_[0]->SyncBoostEffect(bikes_[0]->GetTransform());
					}

					//�S�[������
					if (bikes_[0]->GetIsGoal())
					{
						//FINISH�����`��
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P1);
					}

					//���S����
					if (bikes_[0]->GetHP() <= 0)
					{
						//���S�����`��
						DeadAfterDraw(DEAD_FONT_POS_P1, DEAD_BACK_BOX_MIN_POS_P1, DEAD_BACK_BOX_MAX_POS_P1);
					}
					break;
				case 1:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawUI(sx - width, 0, 1);
					CoinImgDraw(sx - coinX, coinY);

					//�G�t�F�N�g�̍Đ�
					if (bikes_[1]->GetIsBoost())
					{
						bikes_[1]->SyncBoostEffect(bikes_[1]->GetTransform());
					}

					//�S�[������
					if (bikes_[1]->GetIsGoal())
					{
						//FINISH�����`��
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P2);
					}

					//���S����
					if (bikes_[1]->GetHP() <= 0)
					{
						//���S�����`��
						DeadAfterDraw(DEAD_FONT_POS_P2, DEAD_BACK_BOX_MIN_POS_P2, DEAD_BACK_BOX_MAX_POS_P2);
					}
					break;
				case 2:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawUI(sx / 2 - width, sy / 2, 2);
					CoinImgDraw(sx / 2 - coinX, sy / 2 + coinY);

					//�G�t�F�N�g�̍Đ�
					if (bikes_[2]->GetIsBoost())
					{
						bikes_[2]->SyncBoostEffect(bikes_[2]->GetTransform());
					}

					//�S�[������
					if (bikes_[2]->GetIsGoal())
					{
						//FINISH�����`��
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P3);
					}

					//���S����
					if (bikes_[2]->GetHP() <= 0)
					{
						//���S�����`��
						DeadAfterDraw(DEAD_FONT_POS_P3, DEAD_BACK_BOX_MIN_POS_P3, DEAD_BACK_BOX_MAX_POS_P3);
					}
					break;
				case 3:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawUI(sx - width, sy / 2, 3);
					CoinImgDraw(sx - coinX, sy / 2 + coinY);

					//�G�t�F�N�g�̍Đ�
					if (bikes_[3]->GetIsBoost())
					{
						bikes_[3]->SyncBoostEffect(bikes_[3]->GetTransform());
					}

					//�S�[������
					if (bikes_[3]->GetIsGoal())
					{
						//FINISH�����`��
						GoalAfterDraw(playNumber_, FINISH_FONT_POS_MULTI_P4);
					}

					//���S����
					if (bikes_[3]->GetHP() <= 0)
					{
						//���S�����`��
						DeadAfterDraw(DEAD_FONT_POS_P4, DEAD_BACK_BOX_MIN_POS_P4, DEAD_BACK_BOX_MAX_POS_P4);
					}
					break;
				}
			}
		}

		// ���̐�
		DrawLine(0, sy / 2, sx, sy / 2, 0xffff00, 8);
		// �c�̐�
		DrawLine(sx / 2, 0, sx / 2, sy, 0xffff00, 8);

		// �X�^�[�g���̃J�E���g�����炷
		if (startCount_ >= 0.0f)
		{

			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 50 - GetDrawFormatStringWidth("%.f", startCount_), Application::SCREEN_SIZE_Y / 2 -95, 15, 15, 0xffffff, "%.f", startCount_);

			//DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 400, Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);

		}
		
	}
	// �w���v
	//DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	//DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	//DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	//DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
	//DrawDubg();


	//�S�[�������當���o��
	if (playNumber_ == 1)
	{
		if (stage_->GetIsGoal())
		{
			Vector2 noUse = {};
			GoalAfterDraw(playNumber_, noUse);
		}
		else
		{
			//�x��
			WarningDraw();
		}
	}
	else
	{
		bool allBikeGoal = true;
		for (auto& bike : bikes_)
		{
			// �v���C���[��HP��0�łȂ��o�C�N�������S�[���̃`�F�b�N�ΏۂƂ���
			if (bike->GetHP() > 0)
			{
				// ������ł��S�[�����Ă��Ȃ��o�C�N������΃t���O���X�V
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // �S�[�����Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
				}
			}
		}

		// �S�����S�[��������x����~
		if (!allBikeGoal)
		{
			//�x��
			WarningDraw();
		}

	}

	//�|�[�Y��
	if (isPause_)
	{
		PauseDraw();
	}

	
}

void GameScene::DrawObject(int playerID)
{
	SetDrawScreen(mainScreen_);

	// ��ʂ�������
	ClearDrawScreen();

	cameras_[playerID]->SetBeforeDraw(); // �e�v���C���[�̎��_��ݒ�

	// �w�i
	skyDomes_[playerID]->Draw();

	stage_->Draw();

	helicopter_->Draw();
	spike_->Draw();

	//�R�C���`��
	size_t sizeC = coins_.size();
	for (int i = 0; i < sizeC; i++)
	{
		if (!coins_[i]->IsDestroy())
		{
			coins_[i]->Draw();
		}
	}
}

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000, "DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000, "FPS:%f", GetFPS());
	DrawFormatString(0, 140, 0x000000, "IsHitStop:%d", isHitStop);
}

void GameScene::DrawUI(int x, int y, int playerID)
{

	using ap = Application;
	int sc_x = x - 300;
	int sc_y = y + 50;

	// HP�o�[�̕�
	int HP_BAR_WIDTH = x - HP_BER - sc_x;
	// HP�o�[�̍���
	int HP_BAR_HEIGHT = y + HP_BER;
	// HP�o�[��`��
	DrawBox(sc_x, sc_y, sc_x + HP_BAR_WIDTH, HP_BAR_HEIGHT, 0x999999, true); // HP�o�[�̔w�i
	DrawBox(sc_x, sc_y, sc_x + (bikes_[playerID]->GetHP() * HP_BAR_WIDTH) / Bike::MAX_HP, HP_BAR_HEIGHT, 0x00aeef, true); // HP�o�[

	// HP�̍��gg
	DrawBox(sc_x, sc_y,
		x - HP_BER, HP_BAR_HEIGHT,
		0x000000, false);

	//DrawBoxAA(sc_x, sc_y,
	//	x - 10, HP_BAR_HEIGHT,
	//	0x000000, false, 13.0f);

	// �X�R�A�`��
	int addX = 250;
	DrawExtendFormatString(x + addX, y + 20, 2, 2, 0xff0000, "Player %d     :%d", playerID + 1, bikes_[playerID]->GetScore());
}

void GameScene::GoalProcess(void)
{
	//�Q�[���I�[�o�[�V�[���ŕ`�悷�邽�ߕۑ����Ă���
	if (playNumber_ == 1)
	{

		//���e�͏o�����Ȃ�
		helicopter_->ChangeState(Helicopter::STATE::MOVE);
	}
	else
	{
		bool allBikeGoal = true;
		for (auto& bike : bikes_)
		{

			// �v���C���[��HP��0�łȂ��o�C�N�������S�[���̃`�F�b�N�ΏۂƂ���
			if (bike->GetHP() > 0)
			{
				// ������ł��S�[�����Ă��Ȃ��o�C�N������΃t���O���X�V
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // �S�[�����Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
				}
			}
		}

		// �S�����S�[��������w����~
		if (allBikeGoal)
		{
			//���e�͏o�����Ȃ�
			helicopter_->ChangeState(Helicopter::STATE::MOVE);
		}
	}

	stepGoalAfter_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepGoalAfter_ >= GOAL_TO_NEXT_SCENE)
	{
		//�Q�[���I�[�o�[�V�[���ŕ`�悷�邽�ߕۑ����Ă���
		if (playNumber_ == 1)
		{
			score_.ScoreSet(bikes_[0]->GetScore());
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		}
		else
		{
			bool allBikeGoal = true;

			for (auto& bike : bikes_)
			{

				score_.ScoreSetArray(bike->GetScore());

				// �v���C���[��HP��0�łȂ��o�C�N�������S�[���̃`�F�b�N�ΏۂƂ���
				if (bike->GetHP() > 0)
				{
					// ������ł��S�[�����Ă��Ȃ��o�C�N������΃t���O���X�V
					if (!bike->GetIsGoal())
					{
						allBikeGoal = false;
						break;  // �S�[�����Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
					}
				}
			}

			// �S�����S�[�������ꍇ�ɃV�[����؂�ւ���
			if (allBikeGoal)
			{
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
		}

	}
}

void GameScene::CoinPlace(void)
{
	stage_->SetMakeLoopStage(false);

	//�Z���^�[��������̉��̈ړ���
	float shiftX_ = {};

	//���̃����_���ȏꏊ�ɐ���(3�p�^�[��)
	int randDir = GetRand(static_cast<int>(CoinBase::DIR::MAX) - 1);
	CoinBase::DIR dir = static_cast<CoinBase::DIR>(randDir);

	Vector2 randPos;
	switch (dir)
	{
	case CoinBase::DIR::LEFT:
		shiftX_ = -CoinBase::DIR_LEN;
		break;
	case CoinBase::DIR::CENTER:
		shiftX_ = 0.0f;
		break;
	case CoinBase::DIR::RIGHT:
		shiftX_ = CoinBase::DIR_LEN;
		break;
	}

	//�c�ɃR�C���𐶐�����
	for (int i = 0; i < CoinBase::MAX_MAKE_NUM; i++)
	{
		//�c�ɕ��ԃR�C���ƃR�C���̋���
		float len = CoinBase::X_LEN;

		//�R�C���̐���
		CoinBase* c = nullptr;
		int eType = GetRand(static_cast<int>(CoinBase::TYPE::MAX) - 1);
		CoinBase::TYPE type = static_cast<CoinBase::TYPE>(eType);
		for (auto& bike : bikes_) {
			switch (type)
			{
			case CoinBase::TYPE::GOLD:
				c = new GoldCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::SILVER:
				c = new SilverCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			case CoinBase::TYPE::COPPER:
				c = new CopperCoin(bikes_, this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
				break;
			}
		}
		c->Init();

		isCreateCoin_ = true;

		//�ϒ��z��ɗv�f��ǉ�
		coins_.push_back(c);
	}
}

std::vector<CoinBase*> GameScene::GetEnemys(void)
{
	return coins_;
}


bool GameScene::GetIsCreateEnemy(void)
{
	return isCreateCoin_;
}

int GameScene::GetPlayNum(void)
{
	return playNumber_;
}

bool GameScene::OnePersonIsGoal(void)
{
	return onePersonIsGoal_;
}

size_t GameScene::GetBikeMaxPosZIndex(void)
{
	//pos.z����ԑ傫��Bike�v�f���擾
	auto posZMaxElementIt = std::max_element(bikes_.begin(), bikes_.end(), [](const std::shared_ptr<Bike>& a, const std::shared_ptr<Bike>& b) {
		return a->GetTransform().pos.z < b->GetTransform().pos.z; });  // ��r�̊�Ƃ���pos.z���g��

	//pos.z ���ő�̗v�f�̃C���f�b�N�X���擾����
	size_t posZMaxIndex = std::distance(bikes_.begin(), posZMaxElementIt);

	return posZMaxIndex;
}

void GameScene::Collision(void)
{

	//���e�ƃv���C���[�̓����蔻��A�������m�ƃv���C���[�̔���
	//HP�����葱���Ă��܂��̂œ����������͏������f

	for (const auto& bike : bikes_)
	{
		auto bikeCap = bike->GetCapsule();
		if(!(helicopter_->GetBomb()->GetState()== Bomb::STATE::IDLE))
		{
			if (!helicopter_->GetBomb()->GetIsCol())
			{
				//���e
				auto bombCap = helicopter_->GetBomb()->GetCapsule();

				VECTOR diffB = VSub(bombCap.GetCenter(), bikeCap.lock()->GetCenter());
				float  disB = MyUtility::SqrMagnitudeF(diffB);
				if (disB < bombCap.GetRadius() * bikeCap.lock()->GetRadius())
				{
					if (playNumber_ == 1)
					{
						//�S�[�����ĂȂ��ꍇ
						if (!stage_->GetIsGoal())
						{
							//�v���C���[�Ƀ_���[�W
							bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

							//�R���g���[���[�U��
							StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

						}
					}
					else
					{
						//�S�[�����ĂȂ��v���C���[�ɂ����_���[�W
						if (!bike->GetIsGoal())
						{
							//�v���C���[�Ƀ_���[�W
							bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);


							//�Ԃ������v���C���[�̃R���g���[���[��U��
							// �Փ˂����o�C�N�̃C���f�b�N�X���擾
							auto it = std::find(bikes_.begin(), bikes_.end(), bike);
							int bikeIndex = std::distance(bikes_.begin(), it);
							int pad = DX_INPUT_PAD1 + bikeIndex;
							StartJoypadVibration(pad, CE_SWING_VALUE, CE_SWING_TIME, -1);
						}
					}

					//��������
					helicopter_->GetBomb()->SetIsCol(true);


					// ���ʉ��Đ�
					PlaySoundMem(ResourceManager::GetInstance().Load(
						ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, true);
				}
			}
		}

		if (!spike_->GetIsCol())
		{

			//�������m
			auto throwCap = spike_->GetCapsule();

			VECTOR diffT = VSub(throwCap.GetCenter(), bikeCap.lock()->GetCenter());
			float  disT = MyUtility::SqrMagnitudeF(diffT);
			if (disT < throwCap.GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//�S�[�����ĂȂ��ꍇ
					if (!stage_->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(spike_->SPIKE_DAMAGE);

						//�R���g���[���[�U��
						StartJoypadVibration(DX_INPUT_PAD1, CE_SWING_VALUE, CE_SWING_TIME, -1);

					}
				}
				else
				{
					//�S�[�����ĂȂ��v���C���[�ɂ����_���[�W
					if (!bike->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(spike_->SPIKE_DAMAGE);

						//�Ԃ������v���C���[�̃R���g���[���[��U��
						// �Փ˂����o�C�N�̃C���f�b�N�X���擾
						auto it = std::find(bikes_.begin(), bikes_.end(), bike);
						int bikeIndex = std::distance(bikes_.begin(), it);
						int pad = DX_INPUT_PAD1 + bikeIndex;
						StartJoypadVibration(pad, CE_SWING_VALUE, CE_SWING_TIME, -1);
					}
				}

				//��������
				spike_->SetIsCol(true);

				// ���ʉ��Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(
					ResourceManager::SRC::SND_EXPLOSION).handleId_, DX_PLAYTYPE_BACK, true);
			}
		}

		
		//���S����
		//�Q�[���I�[�o�[�V�[���ŕ`�悷�邽�ߕۑ����Ă���
		if (playNumber_ == 1)
		{
			//�Q�[���I�[�o�[����
			if (bikes_[0]->GetHP() <= 0)
			{
				//�X�R�A��ێ�
				score_.ScoreSet(bikes_[0]->GetScore());
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
		}
		else
		{
			bool allBikeDead = true;
			for (auto& bike : bikes_)
			{
			
				// ������ł����S���Ă��Ȃ��o�C�N������΃t���O���X�V
				if (!(bike->GetHP() <= 0))
				{
					allBikeDead = false;
					break;  // ���S���Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
				}
			}

			// �S�������S�����ꍇ�ɃV�[����؂�ւ���
			if (allBikeDead)
			{
				for (auto& bike : bikes_)
				{
					//�X�R�A��ێ�
					score_.ScoreSetArray(bike->GetScore());
				}
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			}
		}
	}
}

void GameScene::BikeCollision(void)
{
	//�o�C�N���m�̓����蔻��(�e��)
	size_t sizeBb = bikes_.size();
	for (int b1 = 0; b1 < sizeBb; b1++)
	{

		for (int b2 = 0; b2 < sizeBb; b2++)
		{

			if (bikes_[b1] == bikes_[b2])
			{
				continue;
			}

			auto b1Pos = bikes_[b1]->GetCapsule().lock()->GetCenter();
			auto b2Pos = bikes_[b2]->GetCapsule().lock()->GetCenter();

			VECTOR diff = VSub(b1Pos, b2Pos);
			float  dis = MyUtility::SqrMagnitudeF(diff);
			if (dis < Bike::RADIUS * Bike::RADIUS)
			{

				// �͈͂ɓ�������A���݂���e��
				auto flipDirB1 = VNorm(VSub(b1Pos, b2Pos));
				flipDirB1.y = 0.0f;
				flipDirB1 = VNorm(flipDirB1);
				auto flipDirB2 = VNorm(VSub(b2Pos, b1Pos));
				flipDirB2.y = 0.0f;
				flipDirB2 = VNorm(flipDirB2);

				bikes_[b1]->Flip(flipDirB1);
				bikes_[b2]->Flip(flipDirB2);
			}


		}

	}
}

void GameScene::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;
}

void GameScene::DecideProcess(void)
{

	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();


	
	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}


		//�ĊJ�{�^��
		//�J�[�\�����������Ă���
		Vector2 reStartFontLenPos_ = { reStartFontBasePos_.x + RESTART_FONT_LENGTH ,reStartFontBasePos_.y + RESTART_FONT_HEIGHT };
		if (mousePos_.x >= reStartFontBasePos_.x && mousePos_.x <= reStartFontLenPos_.x
			&& mousePos_.y >= reStartFontBasePos_.y && mousePos_.y <= reStartFontLenPos_.y)
		{
			nowCursor_ = (int)PAUSE_STATE::RESTART;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;

		}

		if (pState_ == PAUSE_STATE::RESTART)
		{
			//�{�^���ɂӂ�Ă���ꍇ
			reStartFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//�|�[�Y����
				stepPauseKeyHit_ = 0.0f;
				isPause_ = false;
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			reStartFontColor_ = GetColor(255, 255, 255);
		}



		//���g���C�{�^��
		//�J�[�\�����������Ă���
		Vector2 reTryFontLenPos_ = { reTryFontBasePos_.x + RETRY_FONT_LENGTH ,reTryFontBasePos_.y + RETRY_FONT_HEIGHT };
		if (mousePos_.x >= reTryFontBasePos_.x && mousePos_.x <= reTryFontLenPos_.x
			&& mousePos_.y >= reTryFontBasePos_.y && mousePos_.y <= reTryFontLenPos_.y)
		{
			nowCursor_ = (int)PAUSE_STATE::RETRY;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;
		}

		if (pState_ == PAUSE_STATE::RETRY)
		{
			//�{�^���ɂӂ�Ă���ꍇ
			reTryFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE)
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//���N���b�N�܂��̓X�y�[�X�L�[�Ń��g���C
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			reTryFontColor_ = GetColor(255, 255, 255);
		}



		//�I���{�^��
		//�J�[�\�����������Ă���
		Vector2 endFontLenPos_ = { endFontBasePos_.x + END_FONT_LENGTH ,endFontBasePos_.y + END_FONT_HEIGHT };
		if (mousePos_.x >= endFontBasePos_.x && mousePos_.x <= endFontLenPos_.x
			&& mousePos_.y >= endFontBasePos_.y && mousePos_.y <= endFontLenPos_.y)
		{
			nowCursor_ = (int)PAUSE_STATE::END;
			isCursorHit_ = true;
		}
		else
		{
			isCursorHit_ = false;

		}

		if (pState_ == PAUSE_STATE::END)
		{
			//�{�^���ɂӂ�Ă���ꍇ
			endFontColor_ = GetColor(0, 0, 255);
			if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE) 
				|| ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::DOWN))
			{
				// ���莞�̉����Đ�
				PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_START).handleId_, DX_PLAYTYPE_BACK, true);

				//���N���b�N�܂��̓X�y�[�X�L�[�Ń^�C�g����
				SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
			}
		}
		else
		{
			//�{�^���ɂӂꂢ�Ȃ��ꍇ
			endFontColor_ = GetColor(255, 255, 255);
		}


		//�|�[�Y�L�[��������x������悤�ɂȂ�܂�
		stepPauseKeyHit_ += SceneManager::GetInstance().GetDeltaTime();
		if (stepPauseKeyHit_ >= PAUSE_KEY_HIT_MAX_TIME)
		{

			//������x�|�[�Y�L�[�����Ń|�[�Y���j���[����
			if (ins_.IsTrgDown(KEY_INPUT_C) || ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::START) && isPause_)
			{
				//�|�[�Y����
				stepPauseKeyHit_ = 0.0f;
				isPause_ = false;
			}

		}
	}


}

void GameScene::SelectProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//PC
	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_UP) && !(nowCursor_== 0))
	{
		// �I�����̉����Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
	{
		// �I�����̉����Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
		}
	}


	//PAD
	InputManager::JOYPAD_NO padNum[PAD_MAX];
	for (int i = 0; i < PAD_MAX; i++)
	{
		switch (i)
		{
		case static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD1) - 1] = InputManager::JOYPAD_NO::PAD1;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD2) - 1] = InputManager::JOYPAD_NO::PAD2;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD3) - 1] = InputManager::JOYPAD_NO::PAD3;
			break;
		case static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1:
			padNum[static_cast<int>(InputManager::JOYPAD_NO::PAD4) - 1] = InputManager::JOYPAD_NO::PAD4;
			break;
		}

		//�J�[�\���ԍ��ɂ��㉺����
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_UP) && !(nowCursor_ == 0))
		{
			// �I�����̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_--;
			if (nowCursor_ <= 0)
			{
				nowCursor_ = 0;
			}
		}
		if (ins_.IsPadBtnTrgDown(padNum[i], InputManager::JOYPAD_BTN::L_STICK_DOWN) && !(nowCursor_ == SELECT_MAX_NUM - 1))
		{
			// �I�����̉����Đ�
			PlaySoundMem(ResourceManager::GetInstance().Load(ResourceManager::SRC::SND_SELECT).handleId_, DX_PLAYTYPE_BACK, true);

			nowCursor_++;
			if (nowCursor_ >= SELECT_MAX_NUM - 1)
			{
				nowCursor_ = SELECT_MAX_NUM - 1;
			}
		}
	}
	
	//���J�[�\�������Ԃ�ω�
	CursorToPState(nowCursor_);
}

void GameScene::ChangePState(PAUSE_STATE pState)
{
	pState_ = pState;
}

void GameScene::CursorToPState(int cursor)
{
	switch (cursor)
	{
	case (int)PAUSE_STATE::RESTART:
		ChangePState(PAUSE_STATE::RESTART);
		break;
	case (int)PAUSE_STATE::RETRY:
		ChangePState(PAUSE_STATE::RETRY);
		break;
	case (int)PAUSE_STATE::END:
		ChangePState(PAUSE_STATE::END);
		break;
	}
}

void GameScene::Pause(void)
{

	//�}�E�X����
	DecideProcess();

	//�L�[����
	SelectProcess();


}

void GameScene::WarningDraw(void)
{
	//�������m���ҋ@��Ԃ̂Ƃ��ɕ`��
	if (spike_->IsIdle())
	{
		//�x����
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_WARNING).handleId_, DX_PLAYTYPE_LOOP, false);

		//�|�[�Y�̎��͎~�߂�
		if(!isPause_)
		{
			//�g��k��
			if (warningImgScale_ > WARNING_IMG_MAX_SCALE)
			{
				isMaxWarningScale_ = true;
			}
			else if (warningImgScale_ < WARNING_IMG_MIN_SCALE)
			{
				isMaxWarningScale_ = false;
			}

			if (isMaxWarningScale_)
			{
				warningImgScale_ -= WARNING_IMG_CHANGE_SCALE;
			}
			else
			{
				warningImgScale_ += WARNING_IMG_CHANGE_SCALE;
			}
		}

		DrawRotaGraphFastF((float)Application::SCREEN_SIZE_X / 2, WARNING_POS_Y, warningImgScale_, 0.0, imgWarning_, true);
	}
	else
	{
		warningImgScale_ = WARNING_IMG_MIN_SCALE;
		StopSoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_WARNING).handleId_);
	}
}

void GameScene::PauseDraw(void)
{
	//�|�[�Y�w�i�摜
	float imgScl = 3.0f;
	DrawRotaGraphFastF(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, imgScl, 0.0f, imgPause_, true);

	//�����\��
	float fontScl = 3.0f;
	DrawExtendFormatString(reStartFontBasePos_.x, reStartFontBasePos_.y, fontScl, fontScl, reStartFontColor_, "�ĊJ");
	DrawExtendFormatString(reTryFontBasePos_.x, reTryFontBasePos_.y, fontScl, fontScl, reTryFontColor_, "���g���C");
	DrawExtendFormatString(endFontBasePos_.x, endFontBasePos_.y, fontScl, fontScl, endFontColor_, "�I���");
}

void GameScene::GoalAfterDraw(int playNum, Vector2 drawPos)
{
	//1�l�p
	if (playNum == 1)
	{
		//���W
		if (!isPause_)
		{
			int addPosY = 10;
			finishFontMovePos_.y += addPosY;
		}

		float stopPosY = Application::SCREEN_SIZE_Y / 2 - 40;
		if (finishFontMovePos_.y >= stopPosY)
		{
			finishFontMovePos_.y = stopPosY;
		}

		//FINISH�����`��
		DrawRotaGraph(finishFontMovePos_.x, finishFontMovePos_.y, 1.5, 0.0, imgFinish_, true);
	}
	//4�l�p
	else
	{
		DrawRotaGraph(drawPos.x, drawPos.y, 1.0, 0.0, imgFinish_, true);
	}

}

void GameScene::DeadAfterDraw(Vector2 drawPos, Vector2 boxMinPos, Vector2 boxMaxPos)
{
	//�����w�i
	DrawBox(boxMinPos.x, boxMinPos.y, boxMaxPos.x, boxMaxPos.y, 0x000000, true);

	//���S�����F
	int deadFontColor_ = GetColor(255, 0, 0);

	//���S����
	DrawExtendFormatString(drawPos.x, drawPos.y, DEAD_FONT_EXRATE, DEAD_FONT_EXRATE, deadFontColor_, DEAD_FONT.c_str());
}

void GameScene::CoinImgDraw(int x, int y)
{
	DrawRotaGraph(x, y, 0.15, 0.0, imgCoin_, true);
}
