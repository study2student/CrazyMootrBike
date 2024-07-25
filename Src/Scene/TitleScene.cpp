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

TitleScene::TitleScene(void)
{
	imgPush_ = -1;
	imgTitle_ = -1;
	skyDome_ = nullptr;
	animationController_ = nullptr;
	stepBikeDeparture_ = 0.0f;
	stepFlash_ = 0.0f;
	isInvisible_ = false;
	effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_FIRST_POS_Y;
}

TitleScene::~TitleScene(void)
{
	delete animationController_;

	StopEffekseer3DEffect(effectBurnoutPlayId_);
	StopEffekseer3DEffect(effectStartPlayId_);
}

void TitleScene::Init(void)
{

	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// 背景
	spaceDomeTran_.pos = MyUtility::VECTOR_ZERO;
	skyDome_ = std::make_unique<SkyDome>(spaceDomeTran_);
	skyDome_->Init();

	float size;

	// メインステージ
	mainStage_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));
	mainStage_.pos = { -20150.0f, -12000.0f, 6100.0f };
	mainStage_.quaRot = Quaternion();
	size = 1.0f;
	mainStage_.scl = { size * 3.5f, size * 2.5f, size * 4.5f };
	mainStage_.Update();

	// ビル
	sity_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));
	sity_.pos = { 250.0f, -100.0f, 21100.0f };
	size = 1.0f;
	sity_.scl = { size, size, size };
	sity_.quaRotLocal = Quaternion::Euler(
		0.0f, MyUtility::Deg2RadF(-90.0f), 0.0f);
	sity_.Update();



	//バイク
	bike.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BIKE));
	bike.pos = { -150.0f, -100.0f, -100.0f };
	size = 1.3f;
	bike.scl = { size, size, size };
	bike.quaRotLocal = Quaternion::Euler(
		0.0f, MyUtility::Deg2RadF(180.0f), 0.0f);
	bike.Update();

	// キャラ
	charactor_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	//charactor_.pos = { -150.0f, -42.0f, -105.0f };
	charactor_.pos = { -150.0f, -50.0f, -109.0f };
	size = 1.0f;
	charactor_.scl = { size, size, size };
	charactor_.quaRot = Quaternion::Euler(
		0.0f, MyUtility::Deg2RadF(180.0f), 0.0f);
	charactor_.Update();

	// フロントタイヤ
	frontTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	//frontTyre_.pos = { -150.0f, -32.0f, -55.0f };
	frontTyre_.pos = VAdd(bike.pos, BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE);
	size = 1.3f;
	frontTyre_.scl = { size, size, size };
	frontTyre_.quaRot = Quaternion::Euler(
		0.0f, MyUtility::Deg2RadF(0.0f), 0.0f);
	frontTyre_.Update();

	// リアタイヤ
	rearTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	//rearTyre_.pos = { -155.0f, -42.0f, -257.0f };
	rearTyre_.pos = VAdd(bike.pos, BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE);
	size = 1.7f;
	rearTyre_.scl = { size, size, size };
	rearTyre_.quaRot = Quaternion::Euler(
		0.0f, MyUtility::Deg2RadF(0.0f), 0.0f);
	rearTyre_.Update();

	// アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(charactor_.modelId);
	// 無理やりアニメーション
	animationController_->Add(0, path + "Sit.mv1",1.0f);
	animationController_->Play(0,true,33.0f,36.0f);

	// 定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//待機状態
	state_ = STATE::IDLE;
	isBikeDeparture_ = false;

	//エフェクト読み込み
	InitEffect();

	//スコアリセット
	score_.ResetScore();

	//データリセット
	data_.ResetData();

	// BGMを再生
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

	//アニメーションループ
	animationController_->SetEndLoop(33.0f, 36.0f, 1.0f);


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
	//ボタンが押されたら表示しない
	if(state_==STATE::IDLE)
	{
		DrawExtendGraph(160, 150, Application::SCREEN_SIZE_X - 160,Application::SCREEN_SIZE_Y, imgTitle_, true);

		//点滅表示
		float maxStepFlash = 0.80f;
		float minStepFlash = 0.0f;
		if (!isInvisible_)
		{
			stepFlash_ += SceneManager::GetInstance().GetDeltaTime();
			if (stepFlash_ >= maxStepFlash)
			{
				isInvisible_ = true;
			}
		}
		else
		{
			stepFlash_ -= SceneManager::GetInstance().GetDeltaTime();
			if (stepFlash_ <= minStepFlash)
			{
				isInvisible_ = false;
			}
		}

		if(!isInvisible_)
		{
			DrawGraph(Application::SCREEN_SIZE_X / 2 - 145, 700, imgPush_, true);
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
	StartEffect();
}

void TitleScene::UpdateIdle(void)
{
	//タイヤ回転
	BikeTyreRot();

	//待機中エフェクト(スタート前)
	BurnoutIdleEffect();

	//演出スタート
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || static_cast<bool>(GetJoypadInputState(DX_INPUT_PAD1) & PAD_INPUT_A))
	{
		ChangeState(STATE::START);
	}

	//スキップデバッグ
	if (ins.IsTrgDown(KEY_INPUT_S))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	// キャラアニメーション
	animationController_->Update();

	skyDome_->Update();
}

void TitleScene::UpdateStart(void)
{
	//タイヤ回転
	BikeTyreRot();

	// シーン遷移
	//時間経過
	stepBikeDeparture_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepBikeDeparture_ <= BIKE_IDLE__MAX_TIME)
	{
		//待機中エフェクト(スタート直後)
		BurnoutMoveEffect();
	}
	else
	{
		//バイク発車
		BikeDeparture();
	}
	if (stepBikeDeparture_ >= BIKE_DEPARTURE_TO_NEXT_MAX_TIME)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
		stepBikeDeparture_ = 0.0f;
	}

	// キャラアニメーション
	animationController_->Update();
}

void TitleScene::BikeDeparture(void)
{
	//バイクを前方方向へ
	//バイクスピード
	float bikeSpeed_ = 40.0f;
	bike.pos = VAdd(bike.pos, VScale(bike.GetForward(), bikeSpeed_ * 1.5f));
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

	//タイヤの回転
	//速度変更
	float speedRot = 0.0f;
	switch (state_)
	{
	case TitleScene::STATE::IDLE:
		speedRot = 10.0f;
		break;
	case TitleScene::STATE::START:
		speedRot = 20.0f;
		break;
	}
	// デグリーからラジアン(変換)
	float rad = MyUtility::Deg2RadF(speedRot);

	// ラジアンからクォータニオン(指定軸を指定角分回転させる)
	Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_X);

	//クォータニオン(回転)の合成
	tyreRotX_ = tyreRotX_.Mult(rotPow);

	// 回転させる
	//後輪駆動のため最初はリアタイヤだけ回す
	rearTyre_.quaRot = tyreRotX_;
	rearTyre_.Update();
	if(isBikeDeparture_)
	{
		//発車したらフロントタイヤ回す
		frontTyre_.quaRot = tyreRotX_;
		frontTyre_.Update();
	}

}

void TitleScene::StartEffect(void)
{
	effectStartPlayId_ = PlayEffekseer3DEffect(effectStartResId_);
	float scale = 50.0f;
	SetScalePlayingEffekseer3DEffect(effectStartPlayId_, scale, scale, scale);
	SetPosPlayingEffekseer3DEffect(effectStartPlayId_, bike.pos.x, bike.pos.y + 100.0f , bike.pos.z -100.0f);
	SetRotationPlayingEffekseer3DEffect(effectStartPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}

void TitleScene::BurnoutIdleEffect(void)
{
	effectBurnoutPlayId_ = PlayEffekseer3DEffect(effectBurnoutResId_);
	float scale = 60.0f;
	SetScalePlayingEffekseer3DEffect(effectBurnoutPlayId_, scale / 2, scale, scale);
	float localPosZ = 80.0f;
	SetPosPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.pos.x, Bike::IDLE_EFFECT_POS_Y, bike.pos.z - localPosZ);
	SetRotationPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}

void TitleScene::BurnoutMoveEffect(void)
{
	effectBurnoutPlayId_ = PlayEffekseer3DEffect(effectBurnoutResId_);
	float scale = 60.0f;
	SetScalePlayingEffekseer3DEffect(effectBurnoutPlayId_, scale / 2, scale, scale);

	//徐々に高さを上げる
	effectBurnoutPosY_ += stepBikeDeparture_ * 1.1f;
	if (effectBurnoutPosY_ >= Bike::BURNOUT_EFFECT_MAX_POS_Y)
	{
		//高さ制限
		effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_MAX_POS_Y;
	}
	SetPosPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.pos.x, bike.pos.y + effectBurnoutPosY_, bike.pos.z);
	SetRotationPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}
