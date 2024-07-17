#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Object/Common/Capsule.h"
#include "../Object/Common/Collider.h"
#include "../Object/SkyDome.h"
#include "../Object/Stage.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/EnemyBike.h"
#include "../Object/Rider/Rider.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Rider/ShortDisEnemy.h"
#include "../Object/Rider/LongDisEnemy.h"
#include "../Object/Rider/MagicEnemy.h"
#include "../Object/Planet.h"
#include "../Object/Bomb.h"
#include "../Object/Score.h"
#include "../Object/Helicopter.h"
#include "../Object/TyreThrow.h"
#include "../Object/DataSave.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	enemy_ = nullptr;
	stage_ = nullptr;
	enemyBike_ = nullptr;
	helicopter_ = nullptr;
	throwTyre_ = nullptr;
	//score_ = nullptr;

	nowCursor_ = 0;
	stepGoalAfter_ = 0.0f;
}

GameScene::~GameScene(void)
{
	delete enemy_;
	delete throwTyre_;
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
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	for (int i = 0; i < 4; ++i) {
		bikes_.emplace_back(std::make_shared<Bike>(200.0f * (i + 1), i));
	}

	for (auto& bike : bikes_) {
		bike->Init();
	}

	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	////�X�R�A
	//score_ = std::make_shared<Score>();
	//score_->Init();

	// �G
	for (auto& bike : bikes_) {
		enemy_ = new EnemyBase(bikes_,this, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	//�w���R�v�^�[
	helicopter_ = std::make_shared<Helicopter>();
	helicopter_->Init();

	//�����^�C��
	throwTyre_ = new TyreThrow();
	throwTyre_->Init();

	// �X�e�[�W
	stage_ = std::make_shared<Stage>(bikes_, enemy_, helicopter_->GetBomb(), throwTyre_, this);
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

	//for (auto& bike : bikes_) {
	//	SceneManager::GetInstance().GetCamera()->SetFollow(&bike->GetTransform());
	//}
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

	// �G�t�F�N�g������
	InitEffect();

	//�G���J�E���g���Z�b�g
	enCounter = 0;

	//�G���������ꂽ��
	isCreateEnemy_ = false;

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
	Vector2 finishStartPos = { Application::SCREEN_SIZE_X / 2 - 220 ,-40 };
	finishFontPos_ = finishStartPos;

	isPause_ = false;
	isCursorHit_ = false;
	stepPauseKeyHit_ = 0.0f;

	onePersonIsGoal_ = false;

	//�摜
	imgWarning_= resMng_.Load(ResourceManager::SRC::WARNING).handleId_;
	warningImgScale_ = WARNING_IMG_MIN_SCALE;

	imgPause_ = resMng_.Load(ResourceManager::SRC::PAUSE).handleId_;


	//�X�R�A���Z�b�g
	score_.ResetScore();

	// �Q�[���X�^�[�g���̃J�E���g
	startCount_ = 3.0f;
	isStart_ = false;
}

void GameScene::Update(void)
{
	InputManager& ins = InputManager::GetInstance();
	//�|�[�Y���j���[
	if (ins.IsTrgDown(KEY_INPUT_C))
	{
		//�|�[�Y���
		isPause_ = true;
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
				// ������ł��S�[�����Ă��Ȃ��o�C�N������΃t���O���X�V
				if (!bike->GetIsGoal())
				{
					allBikeGoal = false;
					break;  // �S�[�����Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
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

					// ������ł��S�[�����Ă��Ȃ��o�C�N������΃t���O���X�V
					if (!bike->GetIsGoal())
					{
						allBikeGoal = false;
						break;  // �S�[�����Ă��Ȃ��o�C�N�������������_�Ń��[�v�𔲂���
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
	//1�l�v���C���S�[���������ǂ����Z�b�g
	onePersonIsGoal_ = stage_->GetIsGoal();

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	// �X�^�[�g���̃J�E���g�_�E�������炷
	if (startCount_ > 0.0f)
	{
		startCount_ -= 1 / 60.0f;
	}


	if (startCount_ <= 0.0f)
	{
		float deltaTime = hitStopDuration;

		// BGM���Đ�
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_BGM).handleId_, DX_PLAYTYPE_LOOP, false);

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

			// �q�b�g�G�t�F�N�g
			float scale = 50.0f;
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
		}
		for (auto& bike : bikes_)
		{
			enemy_->SetBikeTrans(bike->GetTransform());
		}

		//�w��
		helicopter_->SetBikeTrans(bikes_[0]->GetTransform());
		helicopter_->SetBikeIsOutside(bikes_[0]->GetIsOutSide());

		//�������m
		throwTyre_->Update();
		throwTyre_->SetTransform(bikes_[0]->GetTransform());

		//throwTyre_->SetTransform(bikes_[3]->GetTransform());

		//�G
		size_t sizeE = enemys_.size();
		for (int i = 0; i < sizeE; i++)
		{
			enemys_[i]->Update();
			if (enemys_[i]->GetIsAddScore())
			{
				isHitStop = true;
				//�X�R�A���Z
				//score_.AddScore();
			}
		}

		//�Փ˔���
		Collision();

		// �o�C�N���m�̏Փ˔���
		BikeCollision();

		//�X�e�[�W���������ꂽ��G��z�u����
		if (stage_->GetIsMakeLoopStage())
		{

			stage_->SetMakeLoopStage(false);

			//�Z���^�[��������̉��̈ړ���
			float shiftX_ = {};

			//���̃����_���ȏꏊ�ɐ���(3�p�^�[��)
			int randDir = GetRand(static_cast<int>(EnemyBase::DIR::MAX) - 1);
			EnemyBase::DIR dir = static_cast<EnemyBase::DIR>(randDir);

			Vector2 randPos;
			switch (dir)
			{
			case EnemyBase::DIR::LEFT:
				shiftX_ = -EnemyBase::DIR_LEN;
				break;
			case EnemyBase::DIR::CENTER:
				shiftX_ = 0.0f;
				break;
			case EnemyBase::DIR::RIGHT:
				shiftX_ = EnemyBase::DIR_LEN;
				break;
			}

			//�c�ɓG�𐶐�����
			for (int i = 0; i < EnemyBase::MAX_MAKE_NUM; i++)
			{
				//�c�ɕ��ԓG�ƓG�̋���
				float len = EnemyBase::X_LEN;

				//�G�̐���
				EnemyBase* e = nullptr;
				int eType = GetRand(static_cast<int>(EnemyBase::TYPE::MAX) - 1);
				EnemyBase::TYPE type = static_cast<EnemyBase::TYPE>(eType);
				for (auto& bike : bikes_) {
					switch (type)
					{
					case EnemyBase::TYPE::SHORT_DIS:
						e = new ShortDisEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					case EnemyBase::TYPE::LONG_DIS:
						e = new LongDisEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					case EnemyBase::TYPE::BOMB:
						e = new MagicEnemy(bikes_,this, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
						break;
					}
				}
				e->Init();

				EnemyBike* eB = nullptr;
				eB = new EnemyBike(e);
				eB->Init();
				isCreateEnemy_ = true;


				////�f�o�b�O
				//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

				////���W�̐ݒ�
				//e->setPos(randPos.ToVector2F());

				//�ϒ��z��ɗv�f��ǉ�
				enemys_.push_back(e);
			}
		}
		else
		{
			stage_->Update();
			isCreateEnemy_ = false;
		}


		helicopter_->Update();
		//throwTyre_->Update();

	}
}

void GameScene::Draw(void)
{
	if (playNumber_ == 1)
	{
		SetDrawScreen(mainScreen_);

		// ��ʂ�������
		ClearDrawScreen();

		cameras_[0]->SetBeforeDraw(); // �e�v���C���[�̎��_��ݒ�

		// �w�i
		skyDomes_[0]->Draw();

		stage_->Draw();

		//bike_->Draw();
		helicopter_->Draw();

		throwTyre_->Draw();

		//�G�`��
		size_t sizeE = enemys_.size();
		for (int i = 0; i < sizeE; i++)
		{
			if (!enemys_[i]->IsDestroy())
			{
				enemys_[i]->Draw();
			}
		}

		size_t sizeEb = enemyBikes_.size();
		for (int i = 0; i < sizeEb; i++)
		{
			if (!enemys_[i]->IsDestroy())
			{
				enemyBikes_[i]->Draw();
			}
		}

		// �e�o�C�N��`��
		bikes_[0]->Draw();

		int score = bikes_[0]->GetScore();

		SetDrawScreen(DX_SCREEN_BACK);

		DrawGraph(0, 0, mainScreen_, false);
		DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, score);

		// �X�^�[�g���̃J�E���g�����炷
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
		}
	}
	else
	{
		for (int i = 0; i < cameras_.size(); i++)
		{
			SetDrawScreen(mainScreen_);

			// ��ʂ�������
			ClearDrawScreen();

			cameras_[i]->SetBeforeDraw(); // �e�v���C���[�̎��_��ݒ�

			// �w�i
			skyDomes_[i]->Draw();

			stage_->Draw();

			//bike_->Draw();
			helicopter_->Draw();
			throwTyre_->Draw();

			//�G�`��
			size_t sizeE = enemys_.size();
			for (int i = 0; i < sizeE; i++)
			{
				if (!enemys_[i]->IsDestroy())
				{
					enemys_[i]->Draw();
				}
			}

			size_t sizeEb = enemyBikes_.size();
			for (int i = 0; i < sizeEb; i++)
			{
				if (!enemys_[i]->IsDestroy())
				{
					enemyBikes_[i]->Draw();
				}
			}

			// �e�o�C�N��`��
			for (auto& bike : bikes_) {
				bike->Draw();
			}

			for (int p = 0; p < bikes_.size(); p++) {
				//int score = bikes_[p]->GetScore();

				SetDrawScreen(DX_SCREEN_BACK);

				int sx = Application::SCREEN_SIZE_X;
				int sy = Application::SCREEN_SIZE_Y;

				switch (i)
				{
				case 0:
					DrawGraph(0, 0, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, bikes_[0]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 1:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawExtendFormatString(sx - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 2, bikes_[1]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 2:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 3, bikes_[2]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				case 3:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 4, bikes_[3]->GetScore());
					if (stage_->GetIsGoal())
					{
						GoalAfterDraw();
					}
					break;
				}
			}

			// �X�^�[�g���̃J�E���g�����炷
			if (startCount_ >= 0.0f)
			{
				DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
			}
		}
	}
	// �w���v
	//DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	//DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	//DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	//DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
	DrawDubg();


	////�S�[������������
	//if (stage_->GetIsGoal())
	//{
	//	GoalAfterDraw();
	//}
	//else
	//{
	//	//�x��
	//	WarningDraw();
	//}

	//�|�[�Y��
	if (isPause_)
	{
		PauseDraw();
	}

	//�S�[�������當���o��
	if (playNumber_ == 1)
	{
		if (stage_->GetIsGoal())
		{
			GoalAfterDraw();
		}
	}

	if(!stage_->GetIsGoal())
	{
		//�x��
		WarningDraw();
	}
}

std::vector<EnemyBase*> GameScene::GetEnemys(void)
{
	return enemys_;
}

std::vector<EnemyBike*> GameScene::GetEnemyBikes(void)
{
	return enemyBikes_;
}

bool GameScene::GetIsCreateEnemy(void)
{
	return isCreateEnemy_;
}

int GameScene::GetPlayNum(void)
{
	return playNumber_;
}

bool GameScene::OnePersonIsGoal(void)
{
	return onePersonIsGoal_;
}

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000, "DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000, "FPS:%f", GetFPS());
	//DrawFormatString(840, 420, 0x000000,"�J�E���^ = %d", helicopter_.use_count());
	//DrawFormatString(840, 420, 0x000000,"bombPosY = %f", helicopter_->GetBomb().lock()->GetTransform().pos.y);
	DrawFormatString(0, 140, 0x000000, "IsHitStop:%d", isHitStop);
}

void GameScene::Collision(void)
{
	//�G���m�̓����蔻��(�e��)
	size_t sizeEb = enemyBikes_.size();
	for (int b1 = 0; b1 < sizeEb; b1++)
	{

		for (int b2 = 0; b2 < sizeEb; b2++)
		{

			if (enemyBikes_[b1] == enemyBikes_[b2])
			{
				continue;
			}

			auto b1Pos = enemyBikes_[b1]->GetCapsule().lock()->GetCenter();
			auto b2Pos = enemyBikes_[b2]->GetCapsule().lock()->GetCenter();

			VECTOR diff = VSub(b1Pos, b2Pos);
			float  dis = AsoUtility::SqrMagnitudeF(diff);
			if (dis < EnemyBase::RADIUS * EnemyBase::RADIUS)
			{

				// �͈͂ɓ�������A���݂���e��
				auto flipDirB1 = VNorm(VSub(b1Pos, b2Pos));
				flipDirB1.y = 0.0f;
				flipDirB1 = VNorm(flipDirB1);
				auto flipDirB2 = VNorm(VSub(b2Pos, b1Pos));
				flipDirB2.y = 0.0f;
				flipDirB2 = VNorm(flipDirB2);

				enemys_[b1]->Flip(flipDirB1);
				enemys_[b2]->Flip(flipDirB2);
			}


		}

	}


	//���e�ƃv���C���[�̓����蔻��A�������m�ƃv���C���[�̔���
	//HP�����葱���Ă��܂��̂œ����������͏������f
	/*if (helicopter_->GetBomb()->GetIsCol() || throwTyre_->GetIsCol())
	if (helicopter_->GetBomb()->GetIsCol() || throwTyre_->GetIsCol())	{
		return;
	}*/

	//auto bombCap = helicopter_->GetBomb()->GetCapsule();
	//auto bikeCap = bikes_[3]->GetCapsule();

	//VECTOR diff = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
	//float  dis = AsoUtility::SqrMagnitudeF(diff);
	//if (dis < bombCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
	//{
	//	//�v���C���[�Ƀ_���[�W
	//	bikes_[3]->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

	//	//��������
	//	helicopter_->GetBomb()->SetIsCol(true);
	//}

	for (const auto& bike : bikes_)
	{
		auto bikeCap = bike->GetCapsule();
		if(!helicopter_->GetBomb()->GetIsCol())
		{
			//���e
			auto bombCap = helicopter_->GetBomb()->GetCapsule();

			VECTOR diffB = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disB = AsoUtility::SqrMagnitudeF(diffB);
			if (disB < bombCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//�S�[�����ĂȂ��ꍇ
					if (!stage_->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

					}
				}
				else
				{
					//�S�[�����ĂȂ��v���C���[�ɂ����_���[�W
					if (!bike->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);
					}
				}

				//��������
				helicopter_->GetBomb()->SetIsCol(true);
			}
		}

		if (!throwTyre_->GetIsCol())
		{

			//�������m
			auto throwCap = throwTyre_->GetCapsule();

			VECTOR diffT = VSub(throwCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
			float  disT = AsoUtility::SqrMagnitudeF(diffT);
			if (disT < throwCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
			{
				if (playNumber_ == 1)
				{
					//�S�[�����ĂȂ��ꍇ
					if (!stage_->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(throwTyre_->THROW_DAMAGE);

					}
				}
				else
				{
					//�S�[�����ĂȂ��v���C���[�ɂ����_���[�W
					if (!bike->GetIsGoal())
					{
						//�v���C���[�Ƀ_���[�W
						bike->Damage(throwTyre_->THROW_DAMAGE);
					}
				}

				//��������
				throwTyre_->SetIsCol(true);
			}
		}

		//�������m
		auto throwCap = throwTyre_->GetCapsule();

		VECTOR diffT = VSub(throwCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
		float  disT = AsoUtility::SqrMagnitudeF(diffT);
		if (disT < throwCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
		{
			//�v���C���[�Ƀ_���[�W
			bike->Damage(throwTyre_->THROW_DAMAGE);

			//��������
			throwTyre_->SetIsCol(true);
		}

		//�Q�[���I�[�o�[����
		if (bike->GetHP() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		}
	}
}

void GameScene::BikeCollision(void)
{
	//�G���m�̓����蔻��(�e��)
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
			float  dis = AsoUtility::SqrMagnitudeF(diff);
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

void GameScene::HitEffect(void)
{
	for (auto& bike : bikes_) {
		const auto pPos = bike->GetTransform();
		SetPosPlayingEffekseer3DEffect(effectHitPlayId_, pPos.pos.x, pPos.pos.y, pPos.pos.z + 500);
		SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, pPos.rot.x, pPos.rot.y, pPos.rot.z);
	}
}

void GameScene::MouseProcess(void)
{

	auto& ins_ = InputManager::GetInstance();

	//�}�E�X���W
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
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
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
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
		if (ins_.IsTrgDown(KEY_INPUT_C) && isPause_)
		{
			//�|�[�Y����
			stepPauseKeyHit_ = 0.0f;
			isPause_ = false;
		}
	}


}

void GameScene::KeyProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//�J�[�\���ԍ��ɂ��㉺����
	if (ins_.IsTrgDown(KEY_INPUT_UP))
	{
		nowCursor_--;
		if (nowCursor_ <= 0)
		{
			nowCursor_ = 0;
		}
	}
	if (ins_.IsTrgDown(KEY_INPUT_DOWN))
	{
		nowCursor_++;
		if (nowCursor_ >= SELECT_MAX_NUM - 1)
		{
			nowCursor_ = SELECT_MAX_NUM - 1;
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
	MouseProcess();

	//�L�[����
	KeyProcess();


}

void GameScene::WarningDraw(void)
{
	//�������m���ҋ@��Ԃ̂Ƃ��ɕ`��
	if (throwTyre_->IsIdle())
	{
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

		DrawRotaGraphFastF(Application::SCREEN_SIZE_X / 2, 100, warningImgScale_, 0.0, imgWarning_, true);
	}
	else
	{
		warningImgScale_ = WARNING_IMG_MIN_SCALE;
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

void GameScene::GoalAfterDraw(void)
{
	//FINISH�����F
	int finishFontColor_ = GetColor(255, 165, 0);

	//���W
	if(!isPause_)
	{
		float addPosY = 10;
		finishFontPos_.y += addPosY;
	}

	int stopPosY = Application::SCREEN_SIZE_Y / 2 - 120;
	if (finishFontPos_.y >= stopPosY)
	{
		finishFontPos_.y = stopPosY;
	}

	//�X�R�A
	DrawExtendFormatString(finishFontPos_.x, finishFontPos_.y, 9, 9, finishFontColor_, "FINISH");

}
