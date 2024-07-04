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
#include "GameScene.h"

GameScene::GameScene(void)
{
	bike_ = nullptr;
	enemy_ = nullptr;
	skyDome_ = nullptr;
	stage_ = nullptr;
	enemyBike_ = nullptr;
	helicopter_ = nullptr;
	//score_ = nullptr;

	nowCursor_ = 0;
}

GameScene::~GameScene(void)
{
	delete enemy_;
}

void GameScene::Init(void)
{

	// �v���C���[
	//rider_ = new Rider();
	//rider_->Init();

	//player_ = new Player();
	//player_->Init();

	bike_ = std::make_shared<Bike>(100,3);
	bike_->Init();

	for (int i = 0; i < 4; ++i) {
		bikes_.push_back(std::make_shared<Bike>(200.0f * (i + 1), i));
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
		enemy_ = new EnemyBase(bikes_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	//�w���R�v�^�[
	helicopter_ = std::make_shared<Helicopter>();
	helicopter_->Init();

	// �X�e�[�W
	stage_ = std::make_shared<Stage>(bikes_, enemy_,helicopter_->GetBomb(), this);
	stage_->Init();

	// �X�e�[�W�̏����ݒ�
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// �X�J�C�h�[��
	for (auto& bike : bikes_) {
		skyDome_ = std::make_unique<SkyDome>(bike->GetTransform());
	}
	skyDome_->Init();

	for (auto& bike : bikes_) {
		SceneManager::GetInstance().GetCamera()->SetFollow(&bike->GetTransform());
	}
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);

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
	reStartFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 - 20 };

	//����̃��g���C�|�W�V����
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 + 120 };

	//����̏I���|�W�V����
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 + 260 };

	isPause_ = false;
	isCursorHit_ = false;
	stepPauseKeyHit_ = 0.0f;

	//�X�R�A���Z�b�g
	score_.ResetScore();

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

	// �V�[���J��
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	/*if (stage_->GetLoopStageSize() >= 35)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}*/


	float deltaTime = hitStopDuration;
	skyDome_->Update();
	if (!isHitStop)
	{
		bike_->Update();

		for (auto& bike : bikes_) {
			bike->Update();
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
	enemy_->SetBikeTrans(bike_->GetTransform());
	helicopter_->SetBikeTrans(bikes_[3]->GetTransform());
	helicopter_->SetBikeIsOutside(bikes_[3]->GetIsOutSide());

	//�G
	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		enemys_[i]->Update();
		if (enemys_[i]->GetIsAddScore())
		{
			isHitStop = true;
			//�X�R�A���Z
			score_.AddScore();
		}
	}
	
	if (isHitStop == true)
	{

	}

	//�Փ˔���
	Collision();

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
					e = new ShortDisEnemy(bikes_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
					break;
				case EnemyBase::TYPE::LONG_DIS:
					e = new LongDisEnemy(bikes_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
					break;
				case EnemyBase::TYPE::BOMB:
					e = new MagicEnemy(bikes_, stage_->GetForwardLoopPos(), { shiftX_,0.0f,i * len });
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
	score_.Update();
}

void GameScene::Draw(void)
{
	
	

	// �w�i
	skyDome_->Draw();
	stage_->Draw();

	bike_->Draw();
	helicopter_->Draw();
	score_.Draw();

	//enemy_->Draw();
	//enemyBike_->Draw();
	
	for (auto& bike : bikes_) {
		bike->Draw();
	}


	size_t sizeE = enemys_.size();
	for (int i = 0; i < sizeE; i++)
	{
		if (!enemys_[i]->IsDestroy())
		{
			enemys_[i]->Draw();
		}
	}


	// �w���v
	//DrawFormatString(840, 20, 0x000000, "�ړ��@�@�FWASD");
	//DrawFormatString(840, 40, 0x000000, "�J�����@�F���L�[");
	//DrawFormatString(840, 60, 0x000000, "�_�b�V���F�EShift");
	//DrawFormatString(840, 80, 0x000000, "�W�����v�F�_(�o�N�X��)");
	DrawDubg();

	//�|�[�Y��
	if (isPause_)
	{
		//�����\��
		DrawString(reStartFontBasePos_.x, reStartFontBasePos_.y, "�ĊJ", reStartFontColor_);
		DrawString(reTryFontBasePos_.x, reTryFontBasePos_.y, "���g���C", reTryFontColor_);
		DrawString(endFontBasePos_.x, endFontBasePos_.y, "�I���", endFontColor_);

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

void GameScene::DrawDubg(void)
{
	DrawFormatString(840, 100, 0x000000,"DrawCall:%d", GetDrawCallCount());
	DrawFormatString(840, 120, 0x000000,"FPS:%f", GetFPS());
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


	//���e�ƃv���C���[�̓����蔻��
	//HP�����葱���Ă��܂��̂œ����������͏������f
	if (helicopter_->GetBomb()->GetIsCol())
	{
		return;
	}

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
		auto bombCap = helicopter_->GetBomb()->GetCapsule();
		auto bikeCap = bike->GetCapsule();

		VECTOR diff = VSub(bombCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
		float  dis = AsoUtility::SqrMagnitudeF(diff);
		if (dis < bombCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
		{
			//�v���C���[�Ƀ_���[�W
			bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

			//��������
			helicopter_->GetBomb()->SetIsCol(true);
		}

		//�Q�[���I�[�o�[����
		if (bike->GetHP() <= 0)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
		}
	}
}

void GameScene::InitEffect(void)
{
	// �q�b�g�G�t�F�N�g
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HitEffect).handleId_;
}

void GameScene::HitEffect(void)
{
	for (auto& bike : bikes_) {
		auto pPos = bike->GetTransform();
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

	if(pState_== PAUSE_STATE::RESTART)
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

	if(pState_== PAUSE_STATE::RETRY)
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

	if(pState_== PAUSE_STATE::END)
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
