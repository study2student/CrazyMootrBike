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

#pragma region 変数宣言
	//ボタンが押されてからのバイクの待機最大時間
	const float BIKE_IDLE__MAX_TIME = 3.0f;

	//バイクが発車してから次のシーンまでの最大時間
	const float BIKE_DEPARTURE_TO_NEXT_MAX_TIME = 4.5f;

	// メインステージ座標
	const VECTOR MAINSTAGE_POS = { -20150.0f, -12000.0f, 6100.0f };
	// メインステージ大きさ
	const VECTOR MAINSTAGE_SIZE = { 3.5f, 2.5f, 4.5f };

	// ビル群の座標,大きさ,角度
	const VECTOR SITY_POS = { 250.0f, -100.0f, 21100.0f };
	const VECTOR SITY_SIZE = { 1.0f,1.0f,1.0f };
	const VECTOR SITY_ROT = { 0.0f, MyUtility::Deg2RadF(-90.0f), 0.0f };

	// バイクの座標、大きさ、角度
	const VECTOR BIKE_POS = { -150.0f, -100.0f, -100.0f };
	const VECTOR BIKE_SIZE = { 1.3f,1.3f,1.3f };
	const VECTOR BIKE_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// プレイヤーの座標、大きさ、角度
	const VECTOR CHARACTOR_POS = { -150.0f, -50.0f, -109.0f };
	const VECTOR CHARACTOR_SIZE = { 1.0f, 1.0f, 1.0f };
	const VECTOR CHARACTOR_ROT = { 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f };

	// バイクからフロントタイヤ相対座標
	const VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE = { 10.0f,68.0f,45.0f };
	// フロントタイヤの大きさ,角度
	const VECTOR FRONT_TYRE_SIZE = { 1.3f,1.3f,1.3f };
	const VECTOR FRONT_TYRE_ROT = { 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f };

	// バイクからリアタイヤ相対座標
	const VECTOR BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE = { -5.0f,58.0f,-157.0f };
	// リアタイヤの大きさ、角度
	const VECTOR REAR_TYRE_SIZE = { 1.7f,1.7f,1.7f };
	const VECTOR REAR_TYRE_ROT = { 0.0f, MyUtility::Deg2RadF(0.0f), 0.0f };

	// アニメーションスピード
	const float ANIMATION_SPEED = 1.0f;
	//アニメーションの再生位置
	const float ANIMATION_START_STEP = 33.0f;
	const float ANIMATION_END_STEP = 36.0f;

	// 点滅表示
	const float MAX_STEP_FLASH = 0.8f;

	// プッシュ文字画像の座標
	const VECTOR IMG_PUSH = { Application::SCREEN_SIZE_X / 2 - 145, 700 };

	// バイクのスピード
	const float BIKE_SPEED = 60.0f;

	// タイヤの回転数(IDLE状態)
	const float IDLE_TYRE_ROT_SPEED = 10.0f;

	// タイヤの回転数(START状態)
	const float START_TYRE_ROT_SPEED = 20.0f;

	// スタート時のエフェクトの大きさ
	const float START_EFFECT_SIZE = 50.0f;

	// スタート時のエフェクトの相対位置
	const float START_EFFECT_LOCAL_POS = 100.0f;

	// スタート前の待機中エフェクトの大きさ
	const float BURNOUT_IDLE_EFFECT_SIZE = 60.0f;

	// スタート前の待機中エフェクトのZ座標の相対位置
	const float BURNOUT_IDLE_EFFECT_LOCALPOS_Z = 80.0f;

	// スタート時のバーンエフェクトの大きさ
	const float BURNOUT_MOVE_EFFECT_SIZE = 60.0f;

	// 徐々に高さを上げる
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

	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// 背景
	spaceDomeTran_.pos = MyUtility::VECTOR_ZERO;
	skyDome_ = std::make_unique<SkyDome>(spaceDomeTran_);
	skyDome_->Init();

	// メインステージ
	mainStage_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));
	mainStage_.pos = {MAINSTAGE_POS};
	mainStage_.quaRot = Quaternion();
	mainStage_.scl = { MAINSTAGE_SIZE };
	mainStage_.Update();

	// ビル
	sity_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));
	sity_.pos = { SITY_POS };
	sity_.scl = { SITY_SIZE };
	sity_.quaRotLocal = Quaternion::Euler(SITY_ROT);
	sity_.Update();


	//バイク
	bike.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BIKE));
	bike.pos = { BIKE_POS };
	bike.scl = { BIKE_SIZE };
	bike.quaRotLocal = Quaternion::Euler(BIKE_ROT);
	bike.Update();

	// キャラ
	charactor_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	charactor_.pos = { CHARACTOR_POS };
	charactor_.scl = { CHARACTOR_SIZE };
	charactor_.quaRot = Quaternion::Euler(CHARACTOR_ROT);
	charactor_.Update();

	// フロントタイヤ
	frontTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	frontTyre_.pos = VAdd(bike.pos, BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE);
	frontTyre_.scl = { FRONT_TYRE_SIZE };
	frontTyre_.quaRot = Quaternion::Euler(
		FRONT_TYRE_ROT);
	frontTyre_.Update();

	// リアタイヤ
	rearTyre_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::TYRE));
	rearTyre_.pos = VAdd(bike.pos, BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE);
	rearTyre_.scl = { REAR_TYRE_SIZE };
	rearTyre_.quaRot = Quaternion::Euler(
		REAR_TYRE_ROT);
	rearTyre_.Update();

	// アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(charactor_.modelId);
	// 無理やりアニメーション
	animationController_->Add(0, path + "Sit.mv1",ANIMATION_SPEED);
	animationController_->Play(0, true, ANIMATION_START_STEP, ANIMATION_END_STEP);

	// 定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	//待機状態
	state_ = STATE::IDLE;

	//エフェクト読み込み
	InitEffect();

	effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_FIRST_POS_Y;

	//スコアリセット
	score_.ResetScore();

	//データリセット
	data_.ResetData();

	 //BGMを再生
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
	//ボタンが押されたら表示しない
	if(state_==STATE::IDLE)
	{
		DrawExtendGraph(160, 150, Application::SCREEN_SIZE_X - 160,Application::SCREEN_SIZE_Y, imgTitle_, true);

		//点滅表示
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
	//エンジン音
	PlaySoundMem(ResourceManager::GetInstance().Load(
		ResourceManager::SRC::SND_MOTOR).handleId_, DX_PLAYTYPE_LOOP, false);

	//スタートエフェクト
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

	////スキップデバッグ
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

	//タイヤの回転
	//速度変更
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

	//徐々に高さを上げる
	effectBurnoutPosY_ += stepBikeDeparture_ * GRADUALLY_INCREASE_HEIGHT;
	if (effectBurnoutPosY_ >= Bike::BURNOUT_EFFECT_MAX_POS_Y)
	{
		//高さ制限
		effectBurnoutPosY_ = Bike::BURNOUT_EFFECT_MAX_POS_Y;
	}
	SetPosPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.pos.x, bike.pos.y + effectBurnoutPosY_, bike.pos.z);
	SetRotationPlayingEffekseer3DEffect(effectBurnoutPlayId_, bike.rot.x, bike.rot.y, bike.rot.z);
}
