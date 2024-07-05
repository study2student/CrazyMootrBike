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
#include "../Scene/SelectScene.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
	//bike_ = nullptr;
	enemy_ = nullptr;
	stage_ = nullptr;
	enemyBike_ = nullptr;
	helicopter_ = nullptr;
	score_ = nullptr;

	nowCursor_ = 0;
}

GameScene::~GameScene(void)
{
	delete enemy_;
}

void GameScene::Init(void)
{
	if (playNumber == 1)
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
	}
	else
	{
		mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2);
	}
	
	// プレイヤー
	for (int i = 0; i < 4; i++) {
		bikes_.emplace_back(std::make_shared<Bike>(200.0f * (i + 1), i));
	}
	
	for (auto& bike : bikes_) {
		bike->Init();
	}


	//enemy_->Init();
	/*enemyBike_ = new EnemyBike(enemy_);
	enemyBike_->Init();*/

	//スコア
	score_ = std::make_shared<Score>();
	score_->Init();

	// 敵
	for (auto& bike : bikes_) {
		enemy_ = new EnemyBase(bikes_, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f });
	}

	selectScene_ = std::make_unique<SelectScene>();
	selectScene_->Init();
	
	//各プレイヤーのカメラを生成
	if (playNumber == 1)
	{
		cameras_.push_back(std::make_shared<Camera>());
	}
	else if (playNumber == 4)
	{
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
		cameras_.push_back(std::make_shared<Camera>()); // 各プレイヤーのカメラを作成
	}



	for (int i = 0; i < cameras_.size(); i++) {
		cameras_[i]->Init();
		cameras_[i]->ChangeMode(Camera::MODE::FOLLOW);
		cameras_[i]->SetFollow(&bikes_[i]->GetTransform()); // 各プレイヤーのバイクを追従
	}

	//ヘリコプター
	helicopter_ = std::make_unique<Helicopter>();
	helicopter_->Init();

	// ステージ
	stage_ = std::make_unique<Stage>(bikes_, enemy_,helicopter_->GetBomb(), this);
	stage_->Init();

	// ステージの初期設定
	stage_->ChangeStage(Stage::NAME::MAIN_PLANET);

	// スカイドーム
	for (auto& bike : bikes_) {
		auto sky = std::make_unique<SkyDome>(bike->GetTransform());
		sky->Init();
		skyDomes_.emplace_back(std::move(sky));
	}

	//for (auto& bike : bikes_) {
	//	SceneManager::GetInstance().GetCamera()->SetFollow(&bike->GetTransform());
	//}
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FOLLOW);
	
	// エフェクト初期化
	InitEffect();

	//エンカウントリセット
	enCounter = 0;

	//敵が生成されたか
	isCreateEnemy_ = false;

	hitStopDuration = 6000.0f;
	hitStopTimer = 0.0f;
	isHitStop = false;

	//ポーズメニュー
	//色
	reStartFontColor_ = GetColor(255, 255, 255);
	reTryFontColor_ = GetColor(255, 255, 255);
	endFontColor_ = GetColor(255, 255, 255);

	//左上の再開ポジション
	reStartFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 - 20 };

	//左上のリトライポジション
	reTryFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 + 120 };

	//左上の終わるポジション
	endFontBasePos_ = { Application::SCREEN_SIZE_X / 2 + 210 , Application::SCREEN_SIZE_Y / 2 + 260 };

	isPause_ = false;
	isCursorHit_ = false;
	stepPauseKeyHit_ = 0.0f;

	// ゲームスタート時のカウント
	startCount_ = 0.0f;
	isStart_ = false;

	//ゲームBGM
	isBGM_ = false;
}

void GameScene::Update(void)
{
	
	InputManager& ins = InputManager::GetInstance();

	//ポーズメニュー
	if (ins.IsTrgDown(KEY_INPUT_C))
	{
		//ポーズ状態
		isPause_ = true;
	}
	//ポーズ中はカメラの回転をさせないために情報を渡しておく
	SceneManager::GetInstance().GetCamera()->SetIsPause(isPause_);

	//ポーズ中は他処理中断
	if (isPause_)
	{
		Pause();
		return;
	}

	if (isBGM_)
	{
		PlaySoundMem(ResourceManager::GetInstance().Load(
			ResourceManager::SRC::SND_BGM).handleId_, DX_PLAYTYPE_BACK, false);
	}

	// シーン遷移
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
	if (stage_->GetLoopStageSize() >= STAGE_COUNT)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

	// スタート時のカウントダウンを減らす
	if (startCount_ > 0.0f)
	{
		startCount_ -= 1 / 60.0f;
	}
	else if (startCount_ <= 0.0f)
	{
		isStart_ = true;
	}

	if (isStart_)
	{
		float deltaTime = hitStopDuration;

		//BGMを鳴らす
		isBGM_ = true;

		for (auto& skyDome : skyDomes_)
		{
			skyDome->Update();
		}

		if (!isHitStop)
		{

			//bikes_[0]->Update();
			if (playNumber == 1)
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
			// ヒットストップ中の場合、タイマーを更新
			hitStopTimer -= deltaTime;
			if (hitStopTimer <= 0.f) {
				isHitStop = false;
			}

			// ヒットエフェクト
			float scale = 50.0f;
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);
			SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
		}
		for (auto& bike : bikes_)
		{
			enemy_->SetBikeTrans(bike->GetTransform());
		}
		//enemy_->SetBikeTrans(bike_->GetTransform());
		helicopter_->SetBikeTrans(bikes_[3]->GetTransform());

		//カメラ
		for (int i = 0; i < cameras_.size(); i++) {
			cameras_[i]->Update();
		}

		//敵
		size_t sizeE = enemys_.size();
		for (int i = 0; i < sizeE; i++)
		{
			enemys_[i]->Update();
			if (enemys_[i]->GetIsAddScore())
			{
				isHitStop = true;
				//スコア加算
				//score_->AddScore();

			}
		}

		//敵バイク
		size_t sizeEb = enemyBikes_.size();
		for (int t = 0; t < sizeEb; t++)
		{
			enemyBikes_[t]->Update();
		}


		if (isHitStop == true)
		{

		}

		//衝突判定
		Collision();

		// プレイヤー同士の当たり判定
		BikeCollision();

		//ステージが生成されたら敵を配置する
		if (stage_->GetIsMakeLoopStage())
		{

			stage_->SetMakeLoopStage(false);

			//センター方向からの横の移動幅
			float shiftX_ = {};


			//道のランダムな場所に生成(3パターン)
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

			//縦に敵を生成する
			for (int i = 0; i < EnemyBase::MAX_MAKE_NUM; i++)
			{
				//縦に並ぶ敵と敵の距離
				float len = EnemyBase::X_LEN;

				//敵の生成
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


				////デバッグ
				//TRACE("%d:(%d,%d)\n", randDir, randPos.x, randPos.y);

				////座標の設定
				//e->setPos(randPos.ToVector2F());

				//可変長配列に要素を追加
				enemys_.push_back(e);
				enemyBikes_.push_back(eB);
			}
		}
		else
		{
			stage_->Update();
			isCreateEnemy_ = false;
		}


		helicopter_->Update();
	}
}

void GameScene::Draw(void)
{
	if (playNumber == 1)
	{
		SetDrawScreen(mainScreen_);

		// 画面を初期化
		ClearDrawScreen();


		cameras_[0]->SetBeforeDraw(); // 各プレイヤーの視点を設定

		// スタート時のカウントを減らす
		if (startCount_ >= 0.0f)
		{
			DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
		}

		// 背景
		skyDomes_[0]->Draw();

		stage_->Draw();

		//bike_->Draw();
		helicopter_->Draw();

		//敵描画
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

		// 各バイクを描画
		bikes_[0]->Draw();

		for (int p = 0; p < bikes_.size(); p++) {
			int score = bikes_[0]->GetScore();

			SetDrawScreen(DX_SCREEN_BACK);

			int sx = Application::SCREEN_SIZE_X;
			int sy = Application::SCREEN_SIZE_Y;

			DrawGraph(0, 0, mainScreen_, false);
		}
	}
	else
	{
		for (int i = 0; i < cameras_.size(); i++)
		{
			SetDrawScreen(mainScreen_);

			// 画面を初期化
			ClearDrawScreen();


			cameras_[i]->SetBeforeDraw(); // 各プレイヤーの視点を設定

			// スタート時のカウントを減らす
			if (startCount_ >= 0.0f)
			{
				DrawExtendFormatString(Application::SCREEN_SIZE_X / 2 - GetDrawFormatStringWidth("%.f"), Application::SCREEN_SIZE_Y / 2, 15, 15, 0xffffff, "%.f", startCount_);
			}

			// 背景
			skyDomes_[i]->Draw();

			stage_->Draw();

			//bike_->Draw();
			helicopter_->Draw();

			//敵描画
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

			// 各バイクを描画
			for (auto& bike : bikes_) {
				bike->Draw();
			}

			for (int p = 0; p < bikes_.size(); p++) {
				int score = bikes_[p]->GetScore();

				SetDrawScreen(DX_SCREEN_BACK);

				int sx = Application::SCREEN_SIZE_X;
				int sy = Application::SCREEN_SIZE_Y;

				switch (i)
				{
				case 0:
					DrawGraph(0, 0, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 1, bikes_[0]->GetScore());
					break;
				case 1:
					DrawGraph(sx / 2, 0, mainScreen_, false);
					DrawExtendFormatString(sx - 400, 0, 2, 2, 0xff0000, "Player %d Score:%d", 2, bikes_[1]->GetScore());
					break;
				case 2:
					DrawGraph(0, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx / 2 - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 3, bikes_[2]->GetScore());
					break;
				case 3:
					DrawGraph(sx / 2, sy / 2, mainScreen_, false);
					DrawExtendFormatString(sx - 400, sy / 2, 2, 2, 0xff0000, "Player %d Score:%d", 4, bikes_[3]->GetScore());
					break;
				}
			}
		}
	}

	// ヘルプ
	//DrawFormatString(840, 20, 0x000000, "移動　　：WASD");
	//DrawFormatString(840, 40, 0x000000, "カメラ　：矢印キー");
	//DrawFormatString(840, 60, 0x000000, "ダッシュ：右Shift");
	//DrawFormatString(840, 80, 0x000000, "ジャンプ：＼(バクスラ)");
	DrawDubg();

	//ポーズ中
	if (isPause_)
	{
		//文字表示
		DrawString(reStartFontBasePos_.x, reStartFontBasePos_.y, "再開", reStartFontColor_);
		DrawString(reTryFontBasePos_.x, reTryFontBasePos_.y, "リトライ", reTryFontColor_);
		DrawString(endFontBasePos_.x, endFontBasePos_.y, "終わる", endFontColor_);

	}
	
	//SetDrawArea(0, 0, Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y);
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
	DrawFormatString(0, 140, 0x000000, "IsHitStop:%d", isHitStop);
}

void GameScene::Collision(void)
{
	//敵同士の当たり判定(弾く)
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

				// 範囲に入ったら、お互いを弾く
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


	//爆弾とプレイヤーの当たり判定
	//HPが減り続けてしまうので当たった時は処理中断
	if (helicopter_->GetBomb()->GetIsCol())
	{
		return;
	}

	auto heliCap = helicopter_->GetBomb()->GetCapsule();

	for (auto& bike : bikes_) {
		auto bikeCap = bike->GetCapsule();


		VECTOR diff = VSub(heliCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
		float  dis = AsoUtility::SqrMagnitudeF(diff);
		if (dis < heliCap.lock()->GetRadius() * bikeCap.lock()->GetRadius())
		{
			//プレイヤーにダメージ
			bike->Damage(helicopter_->GetBomb()->BOMB_DAMAGE);

			//当たった
			helicopter_->GetBomb()->SetIsCol(true);
		}
	}
}

void GameScene::BikeCollision(void)
{
	//敵同士の当たり判定(弾く)q
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

				// 範囲に入ったら、お互いを弾く
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
	// ヒットエフェクト
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
	//auto& ins_ = InputManager::GetInstance();

	////マウス座標
	//Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	////再開ボタン
	//Vector2 reStartFontLenPos_ = { reStartFontBasePos_.x + RESTART_FONT_LENGTH ,reStartFontBasePos_.y + RESTART_FONT_HEIGHT };
	//if (mousePos_.x >= reStartFontBasePos_.x && mousePos_.x <= reStartFontLenPos_.x
	//	&& mousePos_.y >= reStartFontBasePos_.y && mousePos_.y <= reStartFontLenPos_.y || pState_ == PAUSE_STATE::RESTART)
	//{
	//	//ボタンにふれている場合
	//	reStartFontColor_ = GetColor(0, 0, 255);
	//	if (GetMouseInput() & MOUSE_INPUT_LEFT || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	//	{
	//		//ポーズ解除
	//		isPause_ = false;
	//	}
	//}
	//else
	//{
	//	//ボタンにふれいない場合
	//	reStartFontColor_ = GetColor(255, 255, 255);
	//}



	////リトライボタン
	//Vector2 reTryFontLenPos_ = { reTryFontBasePos_.x + RETRY_FONT_LENGTH ,reTryFontBasePos_.y + RETRY_FONT_HEIGHT };
	//if (mousePos_.x >= reTryFontBasePos_.x && mousePos_.x <= reTryFontLenPos_.x
	//	&& mousePos_.y >= reTryFontBasePos_.y && mousePos_.y <= reTryFontLenPos_.y || pState_ == PAUSE_STATE::RETRY)
	//{
	//	
	//	//ボタンにふれている場合
	//	reTryFontColor_ = GetColor(0, 0, 255);
	//	if (GetMouseInput() & MOUSE_INPUT_LEFT || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	//	{
	//		//左クリックまたはスペースキーでリトライ
	//		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	//	}
	//}
	//else
	//{
	//	//ボタンにふれいない場合
	//	reTryFontColor_ = GetColor(255, 255, 255);
	//}



	//
	//Vector2 endFontLenPos_ = { endFontBasePos_.x + END_FONT_LENGTH ,endFontBasePos_.y + END_FONT_HEIGHT };
	//if (mousePos_.x >= endFontBasePos_.x && mousePos_.x <= endFontLenPos_.x
	//	&& mousePos_.y >= endFontBasePos_.y && mousePos_.y <= endFontLenPos_.y || pState_ == PAUSE_STATE::END)
	//{
	//
	//	//ボタンにふれている場合
	//	endFontColor_ = GetColor(0, 0, 255);
	//	if (GetMouseInput() & MOUSE_INPUT_LEFT || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
	//	{
	//		//左クリックまたはスペースキーでタイトルへ
	//		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	//	}
	//}
	//else
	//{
	//	//ボタンにふれいない場合
	//	endFontColor_ = GetColor(255, 255, 255);
	//}



	auto& ins_ = InputManager::GetInstance();

	//マウス座標
	Vector2 mousePos_ = InputManager::GetInstance().GetMousePos();

	//再開ボタン
	//カーソルが当たっている
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
		//ボタンにふれている場合
		reStartFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//ポーズ解除
			stepPauseKeyHit_ = 0.0f;
			isPause_ = false;
		}
	}
	else
	{
		//ボタンにふれいない場合
		reStartFontColor_ = GetColor(255, 255, 255);
	}



	//リトライボタン
	//カーソルが当たっている
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
		//ボタンにふれている場合
		reTryFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでリトライ
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
		}
	}
	else
	{
		//ボタンにふれいない場合
		reTryFontColor_ = GetColor(255, 255, 255);
	}



	//終わるボタン
	//カーソルが当たっている
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
		//ボタンにふれている場合
		endFontColor_ = GetColor(0, 0, 255);
		if (GetMouseInput() & MOUSE_INPUT_LEFT && isCursorHit_ || ins_.GetInstance().IsTrgDown(KEY_INPUT_SPACE))
		{
			//左クリックまたはスペースキーでタイトルへ
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
		}
	}
	else
	{
		//ボタンにふれいない場合
		endFontColor_ = GetColor(255, 255, 255);
	}


	//ポーズキーがもう一度押せるようになるまで
	stepPauseKeyHit_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepPauseKeyHit_ >= PAUSE_KEY_HIT_MAX_TIME)
	{
		//もう一度ポーズキー押下でポーズメニュー解除
		if (ins_.IsTrgDown(KEY_INPUT_C) && isPause_)
		{
			//ポーズ解除
			stepPauseKeyHit_ = 0.0f;
			isPause_ = false;
		}
	}

	
}

void GameScene::KeyProcess(void)
{
	auto& ins_ = InputManager::GetInstance();

	//カーソル番号による上下操作
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

	//現カーソルから状態を変化
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

	//マウス操作
	MouseProcess();

	//キー操作
	KeyProcess();

	
}
