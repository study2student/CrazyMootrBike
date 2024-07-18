#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <queue>
#include <DxLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/ResourceManager.h"
#include "WarpStar.h"
#include "../Object/JampRamp.h"
#include "../Object/City.h"
#include "../Object/Rider/Player.h"
#include "../Object/Rider/Bike.h"
#include "../Object/Rider/Enemy.h"
#include "../Object/Rider/EnemyBase.h"
#include "../Object/Bomb.h"
#include "Planet.h"
#include "LoopStage.h"
#include "Common/Collider.h"
#include "Common/Capsule.h"
#include "Common/Transform.h"
#include "../Scene/GameScene.h"
#include "../Object/StageCurve.h"
#include "../Object/Goal.h"
#include "../Object/TyreThrow.h"
#include "../Object/DataSave.h"
#include "Stage.h"

Stage::Stage(const std::vector<std::shared_ptr<Bike>>& bikes, EnemyBase* enemy, Bomb* bomb, TyreThrow* throwTyre, GameScene* gameScene)
	: resMng_(ResourceManager::GetInstance()), bikes_(bikes)
{
	gameScene_ = gameScene;

	//bike_ = bike;
	enemy_ = enemy;
	bomb_ = bomb;
	throwTyre_ = throwTyre;
	activeName_ = NAME::MAIN_PLANET;
	step_ = 0.0f;

	isMakeLoopStage_ = false;

	isJamp_ = false;
	isGoal_ = false;

	//activePlanet_ = nullptr;

	jampRamp_ = nullptr;

	//ループ用のステージ
	/*loopStage_.modelId = resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE);
	loopStage_.pos = { -5000.0f, -5600.0f, 6500.0f };
	loopStage_.scl = { 1.0f,1.0f,1.0f };
	loopStage_.quaRot = Quaternion();
	loopStage_.MakeCollider(Collider::TYPE::STAGE);
	loopStage_.Update();*/

}

Stage::~Stage(void)
{

	//// ワープスター
	//for (auto star : warpStars_)
	//{
	//	delete star;
	//}
	//warpStars_.clear();

	// 惑星
	planets_.clear();

	////ループ用のステージ
	//for (auto loop : loopStage_)
	//{
	//	delete loop;
	//}
	loopStage_.clear();

	// ループ用のステージを削除
	//while (!loopStage_->empty())
	//{
	//	auto loop = loopStage_->front();
	//	loopStage_->pop();
	//	delete loop;
	//}

}

void Stage::Init(void)
{
	MakeMainStage();
	MakeLoopStage();
	MakeWarpStar();
	//MakeCurveStage();
	MakeCity();

	jampRamp_ = std::make_unique<JampRamp>();
	jampRamp_->Init();

	goal_ = std::make_unique<Goal>();
	goal_->Init();

	step_ = -1.0f;
}

void Stage::Update(void)
{

	// ワープスター
	//for (const auto& s : warpStars_)
	//{
	//	s->Update();
	//}

	////カーブ
	//for (const auto& cs : curve_)
	//{
	//	cs->Update();
	//}

	//町
	for (const auto& ct : city_)
	{
		ct->Update();
	}

	// 惑星
	for (const auto& s : planets_)
	{
		s.second->Update();
	}

	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		ls->Update();
	}

	//while (!loopStage_.empty())
	//{
	//	LoopStage* ls = loopStage_.front();
	//	loopStage_.pop();
	//	ls->Update();
	//}

	//ステージを生成する
	MakeLoopStage();
	//MakeCurveStage();
	MakeCity();

	for (const auto& bike : bikes_)
	{

		//バイクとジャンプ台の当たり判定
		auto bikeCap = bike->GetCapsule();
		auto jumpRampCap = jampRamp_->GetCapsule();

		VECTOR diff = VSub(jumpRampCap.lock()->GetCenter(), bikeCap.lock()->GetCenter());
		float  dis = AsoUtility::SqrMagnitudeF(diff);
		if (dis < bikeCap.lock()->GetRadius() * jumpRampCap.lock()->GetRadius())
		{
			isJamp_ = true;
			Jump();
		}
		else
		{
			isJamp_ = false;

		}
	}
	//jampRamp_->Update();

	//ゴール
	goal_->Update();

	//誰かがゴールゾーンを超えたらクリア
	if (gameScene_->GetPlayNum() == 1)
	{
		if (bikes_[0]->GetTransform().pos.z >= goal_->GetTransform().pos.z)
		{
			//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
			isGoal_ = true;
		}
		else
		{
			isGoal_ = false;
		}
	}
	else
	{
		for (const auto& bike : bikes_)
		{
			if (bike->GetTransform().pos.z >= goal_->GetTransform().pos.z)
			{
				//SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
				isGoal_ = true;
				bike->SetIsGoal(true);
			}
			else
			{
				isGoal_ = false;
				bike->SetIsGoal(false);
			}
		}
	}

	//ステージを一定数生成したらゴールが出現
	if (loopStage_.size() >= Goal::STAGE_NUM_MAX_FOR_GOAL)
	{
		goal_->SetPosZ(5000.0f * (Goal::STAGE_NUM_MAX_FOR_GOAL + 1));
	}


}

void Stage::Draw(void)
{

	// ワープスター
	//for (const auto& s : warpStars_)
	//{
	//	s->Draw();
	//}

	////カーブ
	//for (const auto& cs : curve_)
	//{
	//	cs->Draw();
	//}

	//町
	for (const auto& ct : city_)
	{
		ct->Draw();
	}

	// 惑星
	for (const auto& s : planets_)
	{
		s.second->Draw();
	}

	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		ls->Draw();
	}

	//loopStage_.emplace();
	//while (!loopStage_.empty())
	//{
	//	LoopStage* ls = loopStage_.front();
	//	loopStage_.pop();
	//	ls->Draw();
	//}

	//jampRamp_->Draw();
	goal_->Draw();

	DrawFormatString(0, 200, 0xff0000, "IsJump:%d", isJamp_);
	//DrawFormatString(840, 520, 0xffffff, "loopstageNum : %d", loopStage_.size());
}

void Stage::ChangeStage(NAME type)
{

	activeName_ = type;

	// 対象のステージを取得する
	activePlanet_ = GetPlanet(activeName_);

	for (const auto& bike : bikes_)
	{
		bike->ClearCollider();
		bike->AddCollider(activePlanet_.lock()->GetTransform().collider);
	}

	// ステージの当たり判定設定
	//bike_->ClearCollider();
	//bike_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	jampRamp_->ClearCollider();
	jampRamp_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	bomb_->ClearCollider();
	bomb_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	throwTyre_->ClearCollider();
	throwTyre_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		for (const auto& bike : bikes_)
		{
			bike->AddCollider(ls->GetTransform().collider);
		}
		//bike_->AddCollider(ls->GetTransform().collider);
		jampRamp_->AddCollider(ls->GetTransform().collider);
		bomb_->AddCollider(ls->GetTransform().collider);
		throwTyre_->AddCollider(ls->GetTransform().collider);
	}

	//while (!loopStage_.empty())
	//{
	//	LoopStage* ls = loopStage_.front();
	//	loopStage_.pop();
	//	bike_->AddCollider(ls->GetTransform().collider);
	//	jampRamp_->AddCollider(ls->GetTransform().collider);
	//	bomb_->AddCollider(ls->GetTransform().collider);
	//}

	enemy_->ClearCollider();
	enemy_->AddCollider(activePlanet_.lock()->GetTransform().collider);
	//ループ用のステージ
	for (const auto& ls : loopStage_)
	{
		//enemy_->AddCollider(ls->GetTransform().collider);
		std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
		for (int i = 0; i < enemys_.size(); i++)
		{
			enemys_[i]->AddCollider(ls->GetTransform().collider);
		}
	}

	//loopStage_.emplace();
	//while (!loopStage_.empty())
	//{
	//	LoopStage* ls = loopStage_.front();
	//	loopStage_.pop();
	//	
	//	std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
	//	for (int i = 0; i < enemys_.size(); i++)
	//	{
	//		enemys_[i]->AddCollider(ls->GetTransform().collider);
	//	}
	//}

	step_ = TIME_STAGE_CHANGE;

}

std::weak_ptr<Planet> Stage::GetPlanet(NAME type)
{
	if (planets_.count(type) == 0)
	{
		return nullPlanet;
	}

	return planets_[type];
}

bool Stage::GetIsMakeLoopStage(void)
{
	return isMakeLoopStage_;
}

void Stage::SetMakeLoopStage(bool value)
{
	isMakeLoopStage_ = value;
}

VECTOR Stage::GetForwardLoopPos(void)
{
	//先頭ループステージの座標を取得
	int size = (int)loopStage_.size();
	return loopStage_[size - 1]->GetPos();
	//loopStage_.emplace();
	//while (!loopStage_.empty())
	//{
	//	return loopStage_.back()->GetPos();
	//}
}

void Stage::Jump(void)
{
	//bike_->Jump();
}

int Stage::GetLoopStageSize(void)
{
	return loopStage_.size();
}

bool Stage::GetIsGoal(void)
{
	return isGoal_;
}

void Stage::MakeMainStage(void)
{

	// 最初の惑星
	//------------------------------------------------------------------------------
	Transform planetTrans;
	planetTrans.SetModel(
		resMng_.LoadModelDuplicate(ResourceManager::SRC::MAIN_PLANET));
	float scale = 1.0f;
	planetTrans.scl = { scale * 2.5f,scale,scale };
	planetTrans.quaRot = Quaternion();
	planetTrans.pos = { STAGE_START_POS };

	// 当たり判定(コライダ)作成
	planetTrans.MakeCollider(Collider::TYPE::STAGE);

	planetTrans.Update();

	NAME name = NAME::MAIN_PLANET;
	Planet* planet =
		new Planet(
			name, Planet::TYPE::GROUND, planetTrans);
	planet->Init();
	planets_.emplace(name, planet);
	//------------------------------------------------------------------------------

}

void Stage::MakeLoopStage(void)
{

	//すり抜けるためここでバイクと敵のコライダーも追加しとく
	if (gameScene_->GetIsCreateEnemy())
	{
		for (const auto& ls : loopStage_)
		{
			std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
			for (int i = 0; i < enemys_.size(); i++)
			{
				enemys_[i]->AddCollider(activePlanet_.lock()->GetTransform().collider);
				enemys_[i]->AddCollider(ls->GetTransform().collider);
			}
		}

		//while (!loopStage_.empty())
		//{
		//	LoopStage* ls = loopStage_.front();
		//	loopStage_.pop();

		//	std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
		//	for (int i = 0; i < enemys_.size(); i++)
		//	{
		//		enemys_[i]->AddCollider(activePlanet_->GetTransform().collider);
		//		enemys_[i]->AddCollider(ls->GetTransform().collider);
		//	}
		//}
	}


	Transform loopTrans;
	std::shared_ptr<LoopStage> stage;


	//pos.z が最大の要素のインデックスを取得する
	size_t posZMaxIndex = gameScene_->GetBikeMaxPosZIndex();

	//先頭のバイクの座標
	float z = bikes_[posZMaxIndex]->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)loopStage_.size();

	//一定の距離超えたら
	if (size <= mapZ)
	{

		loopTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::DEMO_STAGE));



		float scale = 1.0f;
		loopTrans.scl = { scale * 2.5f,scale ,scale };
		loopTrans.quaRot = Quaternion();
		loopTrans.pos = { STAGE_START_POS.x,  STAGE_START_POS.y,  STAGE_START_POS.z + 5000.0f * (size + 1) };

		// 当たり判定(コライダ)作成
		loopTrans.MakeCollider(Collider::TYPE::STAGE);
		loopTrans.Update();

		//すり抜けるためここでプレイヤーのコライダーも追加しとく
		for (const auto& ls : loopStage_)
		{
			for (const auto& bike : bikes_)
			{
				bike->AddCollider(ls->GetTransform().collider);
			}
			//bike_->AddCollider(ls->GetTransform().collider);
			bomb_->AddCollider(ls->GetTransform().collider);
			throwTyre_->AddCollider(ls->GetTransform().collider);
		}


		stage = std::make_shared<LoopStage>(bikes_.front(), loopTrans);
		stage->Init();
		loopStage_.push_back(stage);
		//loopStage_.emplace(stage);
		//while (!loopStage_.empty())
		//{
		//	LoopStage* ls = loopStage_.front();
		//	//loopStage_.pop();
		//	bike_->AddCollider(ls->GetTransform().collider);
		//	bomb_->AddCollider(ls->GetTransform().collider);
		//}
		//AddStage(stage);
		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//後ろのステージを削除
	if (loopStage_.size() >= 12)
	{
		//std::queue<int> lt;

		//for (size_t i = 0; i < loopStage_.size(); ++i) {
		//	lt.push(i);
		//}

		//while (!lt.empty() && loopStage_.size() >= 6)
		//{
			//int index = lt.front();
			//lt.pop();

			// ステージを削除する

			std::shared_ptr<LoopStage> tailLoop = loopStage_[size-11];
			tailLoop->Destroy();

		//int size = static_cast<int>(loopStage_.size());
		//if (size < 5) {
		//	throw std::runtime_error("Queue does not contain enough elements");
		//}

		//// 最後から5番目の要素にアクセスするための準備
		//std::queue<LoopStage*> tempQueue = loopStage_; // 元のキューをコピー
		//LoopStage* tailLoop = nullptr;

		//// 後ろから5番目の要素までポップする
		//for (int i = 0; i < size - 4; ++i) {
		//	tailLoop = tempQueue.front();
		//	tempQueue.pop();
		//}

		//// 目的の要素を操作
		//if (tailLoop != nullptr) {
		//	tailLoop->Destroy();
		//}

		//// 条件に一致するイテレータを削除
		//loopStage_.erase(
		//	std::remove(
		//		loopStage_.begin(),
		//		loopStage_.end(),
		//		tailLoop
		//	),
		//	loopStage_.end()
		//);

	}


	//tailLoop->Destroy();
	//std::_Vector_iterator<std::_Vector_val<std::_Simple_types<LoopStage*>>> it;


	//for (const auto& ls : loopStage_)
	//{
	//		it = std::remove_if(loopStage_.begin(), loopStage_.end(), [=]() {
	//			return ls->GetState() == LoopStage::STATE::BACK;
	//			});
	//}

	//// 削除する条件をラムダ式で定義
	//auto it = std::remove_if(loopStage_.begin(), loopStage_.end(),
	//	[](LoopStage* ls) {
	//		return ls->GetState() == LoopStage::STATE::BACK;
	//	});

	//// ベクターから要素を削除
	//loopStage_.erase(it, loopStage_.end());

}

// ステージを追加する関数
void Stage::AddStage(std::shared_ptr<LoopStage> newStage)
{

	loopStage_.push_back(newStage);
	//stageQueue(newStage);
	sizeS++;

	// 6以上のステージがある場合は古いステージを削除
	if (loopStage_.size() > 5) {
		std::shared_ptr<LoopStage> oldStage = loopStage_.front();
		loopStage_.pop_front();

		oldStage->Destroy();

		// 古いステージをloopStage_から削除
		//loopStage_.erase(
		//	std::remove(loopStage_.begin(), loopStage_.end(), oldStage),
		//	loopStage_.end()
		//);
	}
}

void Stage::MakeWarpStar(void)
{

	//Transform trans;
	//WarpStar* star;

	//// 落とし穴惑星へのワープスター
	////------------------------------------------------------------------------------
	//trans.pos = { -910.0f, 200.0f, 894.0f };
	//trans.scl = { 0.6f, 0.6f, 0.6f };
	//trans.quaRot = Quaternion::Euler(
	//	AsoUtility::Deg2RadF(-25.0f),
	//	AsoUtility::Deg2RadF(-50.0f),
	//	AsoUtility::Deg2RadF(0.0f)
	//);

 // 	star = new WarpStar(bike_, trans);
	//star->Init();
	//warpStars_.push_back(star);
	////------------------------------------------------------------------------------

}

void Stage::MakeCurveStage(void)
{
	//if (gameScene_->GetIsCreateEnemy())
	//{
	//	for (const auto& cs : curve_)
	//	{
	//		std::vector<EnemyBase*>enemys_ = gameScene_->GetEnemys();
	//		for (int i = 0; i < enemys_.size(); i++)
	//		{
	//			enemys_[i]->AddCollider(activePlanet_.lock()->GetTransform().collider);
	//			enemys_[i]->AddCollider(cs->GetTransform().collider);
	//		}
	//	}

	//}

	//Transform curveTrans;
	//std::shared_ptr<StageCurve> curve;


	//float z = bikes_.front()->GetTransform().pos.z;

	//int mapZ = (int)((z + 6000.0f) / 5000.0f);
	//int size = (int)curve_.size();

	////���̋�����������
	//if (size <= mapZ)
	//{

	//	curveTrans.SetModel(
	//		resMng_.LoadModelDuplicate(ResourceManager::SRC::CURVE));

	//	float scale = 1.0f;
	//	curveTrans.scl = { scale * 2.5f,scale,scale * 2.5f };
	//	curveTrans.quaRot = Quaternion();
	//	curveTrans.pos = { CURVE_START_POS.x,  CURVE_START_POS.y,  CURVE_START_POS.z + 5000.0f * (size + 1) };

	//	// �����蔻��(�R���C�_)�쐬
	//	curveTrans.MakeCollider(Collider::TYPE::STAGE);
	//	curveTrans.Update();

	//	//���蔲���邽�߂����Ńv���C���[�̃R���C�_�[���ǉ����Ƃ�
	//	for (const auto& cs : curve_)
	//	{
	//		for (const auto& bike : bikes_)
	//		{
	//			bike->AddCollider(cs->GetTransform().collider);
	//		}
	//		//bike_->AddCollider(cs->GetTransform().collider);
	//		bomb_->AddCollider(cs->GetTransform().collider);
	//	}


	//	curve = std::make_shared<StageCurve>(bikes_.front(), curveTrans);
	//	curve->Init();
	//	curve_.push_back(curve);
	//	//loopStage_.emplace(curve);
	//	//while (!loopStage_.empty())
	//	//{
	//	//	LoopStage* ls = loopStage_.front();
	//	//	//loopStage_.pop();
	//	//	bike_->AddCollider(ls->GetTransform().collider);
	//	//	bomb_->AddCollider(ls->GetTransform().collider);
	//	//}
	//	//AddStage(stage);
	//	isMakeLoopStage_ = true;

	//}
	//else
	//{
	//	isMakeLoopStage_ = false;
	//}


	////���̃X�e�[�W���폜
	//if (curve_.size() >= 6)
	//{
	//	//std::queue<int> lt;

	//	//for (size_t i = 0; i < loopStage_.size(); ++i) {
	//	//	lt.push(i);
	//	//}

	//	//while (!lt.empty() && loopStage_.size() >= 6)
	//	//{
	//		//int index = lt.front();
	//		//lt.pop();

	//		// �X�e�[�W���폜����
	//	std::shared_ptr<StageCurve> tailLoop = curve_[size - 5];
	//	tailLoop->Destroy();
	//}

}

void Stage::MakeCity(void)
{
	Transform cityTrans;
	std::shared_ptr<City> city;


	//pos.z が最大の要素のインデックスを取得する
	size_t posZMaxIndex = gameScene_->GetBikeMaxPosZIndex();

	//先頭のバイクに合わせる
	float z = bikes_[posZMaxIndex]->GetTransform().pos.z;

	int mapZ = (int)((z + 6000.0f) / 5000.0f);
	int size = (int)city_.size();

	//一定の距離超えたら
	if (size <= mapZ)
	{

		cityTrans.SetModel(
			resMng_.LoadModelDuplicate(ResourceManager::SRC::CITY));



		float scale = 1.0f;
		cityTrans.scl = { scale,scale,scale };
		cityTrans.quaRot = Quaternion();
		cityTrans.quaRotLocal =
			Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(-90.0f), 0.0f });
		cityTrans.pos = { CITY_START_POS.x,  CITY_START_POS.y,  CITY_START_POS.z + 5000.0f * (size + 1) };

		cityTrans.Update();


		city = std::make_shared<City>(bikes_.front(), cityTrans);
		city->Init();
		city_.push_back(city);
		//loopStage_.emplace(stage);
		//while (!loopStage_.empty())
		//{
		//	LoopStage* ls = loopStage_.front();
		//	//loopStage_.pop();
		//	bike_->AddCollider(ls->GetTransform().collider);
		//	bomb_->AddCollider(ls->GetTransform().collider);
		//}
		//AddStage(stage);
		isMakeLoopStage_ = true;

	}
	else
	{
		isMakeLoopStage_ = false;
	}


	//後ろのステージを削除
	if (city_.size() >= 12)
	{
		//std::queue<int> lt;

		//for (size_t i = 0; i < loopStage_.size(); ++i) {
		//	lt.push(i);
		//}

		//while (!lt.empty() && loopStage_.size() >= 6)
		//{
			//int index = lt.front();
			//lt.pop();



		// ステージを削除する
		std::shared_ptr<City> tailLoop = city_[size - 11];
		tailLoop->Destroy();




		//int size = static_cast<int>(loopStage_.size());
		//if (size < 5) {
		//	throw std::runtime_error("Queue does not contain enough elements");
		//}

		//// 最後から5番目の要素にアクセスするための準備
		//std::queue<LoopStage*> tempQueue = loopStage_; // 元のキューをコピー
		//LoopStage* tailLoop = nullptr;

		//// 後ろから5番目の要素までポップする
		//for (int i = 0; i < size - 4; ++i) {
		//	tailLoop = tempQueue.front();
		//	tempQueue.pop();
		//}

		//// 目的の要素を操作
		//if (tailLoop != nullptr) {
		//	tailLoop->Destroy();
		//}

		//// 条件に一致するイテレータを削除
		//loopStage_.erase(
		//	std::remove(
		//		loopStage_.begin(),
		//		loopStage_.end(),
		//		tailLoop
		//	),
		//	loopStage_.end()
		//);

	}
}
