#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "Manager/InputManager.h"
#include "Manager/ResourceManager.h"
#include "Manager/SceneManager.h"
#include "Object/Score.h"
#include "Object/DataSave.h"
#include "Application.h"

Application* Application::instance_ = nullptr;

const std::string Application::PATH_IMAGE = "Data/Image/";
const std::string Application::PATH_MODEL = "Data/Model/";
const std::string Application::PATH_EFFECT = "Data/Effect/";
const std::string Application::PATH_SOUND = "Data/Sound/";
const std::string Application::PATH_SHADER = "Data/Shader/";

void Application::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Application();
	}
	instance_->Init();
}

Application& Application::GetInstance(void)
{
	return *instance_;
}

void Application::Init(void)
{

	// アプリケーションの初期設定
	SetWindowText("Crazy MotorBiker");

	// ウィンドウサイズ
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 32);
	ChangeWindowMode(true);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);
	isInitFail_ = false;
	if (DxLib_Init() == -1)
	{
		isInitFail_ = true;
		return;
	}

	// Effekseerの初期化
	InitEffekseer();

	// キー制御初期化
	SetUseDirectInputFlag(true);
	InputManager::CreateInstance();

	// リソース管理初期化
	ResourceManager::CreateInstance();

	//スコア初期化
	Score::CreateInstance();

	//データ初期化
	DataSave::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	isGameFinishKey_ = false;

}

void Application::Run(void)
{

	auto& inputManager = InputManager::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	// ゲームループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0 && !isGameFinishKey_)
	{

		Sleep(1);		//システムに処理を返す

		currentTime = GetNowCount();

		// 現在の時刻が、前回のフレーム時より
		// 1/60秒経過していたら処理を実行する
		if (currentTime - lastFrameTime >= FRAME_RATE)
		{
			// フレーム実行時間を更新
			lastFrameTime = currentTime;

			// フレーム数をカウント
			frameCnt++;

			inputManager.Update();
			sceneManager.Update();

			sceneManager.Draw();

			// フレームレート計算
			CalcFrameRate();

			// フレームレート表示
			DrawFrameRate();

			ScreenFlip();
		}

	}

}

void Application::Destroy(void)
{

	InputManager::GetInstance().Destroy();
	ResourceManager::GetInstance().Destroy();
	SceneManager::GetInstance().Destroy();

	// Effekseerを終了する。
	Effkseer_End();

	// DxLib終了
	if (DxLib_End() == -1)
	{
		isReleaseFail_ = true;
	}

	delete instance_;

}

bool Application::IsInitFail(void) const
{
	return isInitFail_;
}

bool Application::IsReleaseFail(void) const
{
	return isReleaseFail_;
}

void Application::SetIsGameFinishKey(bool isGameFinishKey)
{
	isGameFinishKey_ = isGameFinishKey;
}

Application::Application(void)
{
	isInitFail_ = false;
	isReleaseFail_ = false;
}

void Application::InitEffekseer(void)
{
	if (Effekseer_Init(8000) == -1)
	{
		DxLib_End();
	}

	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
}

void Application::CalcFrameRate()
{
	// 前回のフレームレート更新からの経過時間を求める
	int nDirTime = currentTime - updateFrameRateTime;

	// 前回のフレームレートを更新から
	// 1秒以上経過していたらフレームレートを更新する
	if (nDirTime > 1000)
	{
		// フレーム回数をミリ秒に合わせる
		//少数まで出したのでfloatにキャスト
		float fFrameCnt = (float)(frameCnt * 1000);

		// フレームレートを求める
		// 理想通りなら60000 / 1000 で 60となる
		frameRate = fFrameCnt / nDirTime;

		// フレームカウントをクリア
		frameCnt = 0;

		// フレームレート更新時間を更新
		updateFrameRateTime = currentTime;
	}
}

void Application::DrawFrameRate()
{
	DrawFormatString(SCREEN_SIZE_X - 90, 0, GetColor(255, 30, 30), "FPS[%.2f]", frameRate);
}
