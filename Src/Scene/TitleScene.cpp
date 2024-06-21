#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/Common/AnimationController.h"
#include "../Object/SkyDome.h"
#include "../Object/Common/Transform.h"
#include "../Object/Rider/Bike.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgPush_ = -1;
	imgTitle_ = -1;
	skyDome_ = nullptr;
	animationController_ = nullptr;
}

TitleScene::~TitleScene(void)
{
	delete skyDome_;
	delete animationController_;
}

void TitleScene::Init(void)
{

	// 画像読み込み
	imgTitle_ = resMng_.Load(ResourceManager::SRC::TITLE).handleId_;
	imgPush_ = resMng_.Load(ResourceManager::SRC::PUSH_SPACE).handleId_;

	// 背景
	spaceDomeTran_.pos = AsoUtility::VECTOR_ZERO;
	skyDome_ = new SkyDome(spaceDomeTran_);
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
		0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f);
	sity_.Update();

	////2つ分ビル
	//sity_[0]->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));
	//sity_[0]->pos = {250.0f, -100.0f, 21100.0f};
	//size = 1.0f;
	//sity_[0]->scl = {size, size, size};
	//sity_[0]->quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f);
	//sity_[0]->Update();
	//
	//sity_[1]->SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));
	//sity_[1]->pos = { 250.0f, -100.0f, 21100.0f };
	//size =1.0f;
	//sity_[1]->scl = { size, size, size };
	//sity_[1]->quaRotLocal = Quaternion::Euler(
	//	0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f);
	//sity_[1]->Update();


	//バイク
	bike.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::BIKE));
	bike.pos = { -150.0f, -100.0f, -100.0f };
	size = 1.3f;
	bike.scl = { size, size, size };
	bike.quaRotLocal = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	bike.Update();

	// キャラ
	charactor_.SetModel(resMng_.LoadModelDuplicate(ResourceManager::SRC::PLAYER));
	charactor_.pos = { -150.0f, -32.0f, -105.0f };
	size = 1.0f;
	charactor_.scl = { size, size, size };
	charactor_.quaRot = Quaternion::Euler(
		0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f);
	charactor_.Update();

	// アニメーションの設定
	std::string path = Application::PATH_MODEL + "Player/";
	animationController_ = new AnimationController(charactor_.modelId);
	animationController_->Add(0, path + "Run.mv1", 20.0f);
	animationController_->Play(0);

	// 定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT);
	}

	// キャラアニメーション
	animationController_->Update();

	skyDome_->Update();

}

void TitleScene::Draw(void)
{

	skyDome_->Draw();

	MV1DrawModel(charactor_.modelId);
	MV1DrawModel(bike.modelId);
	MV1DrawModel(mainStage_.modelId);
	MV1DrawModel(sity_.modelId);
	/*MV1DrawModel(sity_[0]->modelId);
	MV1DrawModel(sity_[1]->modelId);*/
	

	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 250, 1.0, 0.0, imgTitle_, true);
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, 500, 1.0, 0.0, imgPush_, true);

}
