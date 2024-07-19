#include <string>
#include<EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Utility/MyUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "../../Scene/GameScene.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const std::vector<std::shared_ptr<Bike>>& bikes,GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos)
{
	bikes_ = bikes;
	gameScene_ = gameScene;

	animationController_ = nullptr;
	state_ = STATE::NONE;

	speed_ = 0.0f;
	moveDir_ = MyUtility::VECTOR_ZERO;
	movePow_ = MyUtility::VECTOR_ZERO;
	movedPos_ = MyUtility::VECTOR_ZERO;

	makePos_ = loopStagePos;
	localPos_ = localPos;

	enemyRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = MyUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	// 衝突チェック
	gravHitPosDown_ = MyUtility::VECTOR_ZERO;
	gravHitPosUp_ = MyUtility::VECTOR_ZERO;

	isBikeCol_ = false;

	isEnemyCol_ = false;

	isAtk_ = false;
	isAddScore_ = false;
	toAtkStep_ = 0.0f;

	imgShadow_ = -1;

	capsule_ = nullptr;

	isCollGround_ = false;

	stepMade_ = 0.0f;
}

EnemyBase::~EnemyBase(void)
{
	delete animationController_;
}

void EnemyBase::Init(void)
{

	// モデルの基本設定
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COPPER_MEDAL));
	transform_.scl = MyUtility::VECTOR_ONE;
	//transform_.pos = { 700.0f, -800.0f, -2500.0f };
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, -800.0f, makePos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, MyUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();


	// アニメーションの設定
	InitAnimation();

	// カプセルコライダ
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	// 丸影画像
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// 初期状態
	ChangeState(STATE::PLAY);

	//敵キャラ個別のパラメータ設定
	SetParam();

	// エフェクト初期化
	InitEffect();
}

void EnemyBase::SetParam(void)
{

}

void EnemyBase::Update(void)
{
	// 更新ステップ
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBase::STATE::PLAY:
		UpdatePlay();
		break;
	case EnemyBase::STATE::FLIPED:
		UpdateFliped();
		break;
	case EnemyBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// モデル制御更新
	transform_.Update();

	// アニメーション再生
	animationController_->Update();
}

void EnemyBase::Draw(void)
{
	//死亡状態は描画しない
	if (state_ == STATE::DEAD)
	{
		return;
	}

	// モデルの描画
	MV1DrawModel(transform_.modelId);

	// 丸影描画
	DrawShadow();

	////Hp表示
	//DrawHpBar();

	DrawLine3D(fowardPos_, backPos_, 0x0000ff);

	//capsule_->Draw();
}

void EnemyBase::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EnemyBase::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> EnemyBase::GetCapsule(void) const
{
	return capsule_;
}

void EnemyBase::SetBikeTrans(Transform bikeTrans)
{
	Transform bikeTrans_;
	bikeTrans_ = bikeTrans;
}

void EnemyBase::SetSpeed(float speed)
{
	speed_ = speed;
}

void EnemyBase::SetIsEnemyCol(bool isEnemyCol)
{
	isEnemyCol_ = isEnemyCol;
}

void EnemyBase::Flip(VECTOR dir)
{
	flipDir_ = dir;
	flipSpeed_ = 10.0f;
	ChangeState(STATE::FLIPED);
}

bool EnemyBase::GetIsBikeCol(void)
{
	return isBikeCol_;
}

bool EnemyBase::GetIsAddScore(void)
{
	return isAddScore_;
}

EnemyBase::STATE EnemyBase::GetState(void)
{
	return state_;
}

bool EnemyBase::IsDestroy(void)
{
	return state_ == STATE::DEAD;
}

void EnemyBase::Destroy(void)
{
	ChangeState(STATE::DEAD);
}

void EnemyBase::AddScoreToPlayer(int playerId, int score)
{
	if (playerId >= 0 && playerId < bikes_.size())
	{
		bikes_[playerId]->AddScore(score);
	}
}

void EnemyBase::InitEffect(void)
{
	// ヒットエフェクト
	effectHitResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::HITEFFECT).handleId_;

}

void EnemyBase::HitEffect()
{
	effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

	float scale = 50.0f;

	SetPosPlayingEffekseer3DEffect(effectHitPlayId_, transform_.pos.x, transform_.pos.y, transform_.pos.z + EFF_POS_Z);
	SetRotationPlayingEffekseer3DEffect(effectHitPlayId_, transform_.rot.x, transform_.rot.y, transform_.rot.z);
	SetScalePlayingEffekseer3DEffect(effectHitPlayId_, scale, scale, scale);
	
}

void EnemyBase::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Enemy/";
	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::SHORT, path + "Boxing.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::LONG, path + "Sword Slash.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::BOMB, path + "Throw In.mv1", 30.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EnemyBase::ChangeState(STATE state)
{
	// 状態変更
	state_ = state;

	// 各状態遷移の初期処理
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		ChangeStateNone();
		break;
	case EnemyBase::STATE::PLAY:
		ChangeStatePlay();
		break;
	case EnemyBase::STATE::FLIPED:
		ChangeStateFliped();
		break;
	case EnemyBase::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void EnemyBase::ChangeStateNone(void)
{
}

void EnemyBase::ChangeStatePlay(void)
{
}

void EnemyBase::ChangeStateFliped(void)
{
}

void EnemyBase::ChangeStateDead(void)
{
}

void EnemyBase::UpdateNone(void)
{
}

void EnemyBase::UpdateFliped(void)
{

	// 移動処理
	ProcessMove();

	// 吹っ飛ばされる
	flipSpeed_ -= 0.16f;
	if (flipSpeed_ < 0.0f)
	{
		flipSpeed_ = 0.0f;
		ChangeState(STATE::PLAY);
	}
	movePow_ = VAdd(movePow_, VScale(flipDir_, flipSpeed_));

	// ジャンプ処理
	ProcessJump();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	isAtk_ = false;

	// 回転させる
	transform_.quaRot = enemyRotY_;

}

void EnemyBase::UpdateDead(void)
{
	isAddScore_ = false;
}

void EnemyBase::UpdatePlay(void)
{
	// 移動処理
	ProcessMove();

	// ジャンプ処理
	ProcessJump();

	//回転
	RotY();

	// 重力による移動量
	CalcGravityPow();

	// 衝突判定
	Collision();

	isAtk_ = false;

	// 回転させる
	transform_.quaRot = enemyRotY_;
}

void EnemyBase::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3] = { VERTEX3D(), VERTEX3D(), VERTEX3D() };
	VECTOR SlideVec;
	int ModelHandle;

	// ライティングを無効にする
	SetUseLighting(FALSE);

	// Ｚバッファを有効にする
	SetUseZBuffer3D(TRUE);

	// テクスチャアドレスモードを CLAMP にする( テクスチャの端より先は端のドットが延々続く )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// 影を落とすモデルの数だけ繰り返し
	for (const auto c : colliders_)
	{

		// チェックするモデルは、jが0の時はステージモデル、1以上の場合はコリジョンモデル
		ModelHandle = c->modelId_;

		// プレイヤーの直下に存在する地面のポリゴンを取得
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

		// 頂点データで変化が無い部分をセット
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// 球の直下に存在するポリゴンの数だけ繰り返し
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++)
		{
			// ポリゴンの座標は地面ポリゴンの座標
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ちょっと持ち上げて重ならないようにする
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// ポリゴンの不透明度を設定する
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			// ＵＶ値は地面ポリゴンとプレイヤーの相対座標から割り出す
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// 影ポリゴンを描画
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ライティングを有効にする
	SetUseLighting(TRUE);

	// Ｚバッファを無効にする
	SetUseZBuffer3D(FALSE);

}

void EnemyBase::DrawHpBar(void)
{
	Transform hpTrans_;

	// 対象物の姿勢から見た、上方向に3D座標をずらす
	VECTOR relativePos = transform_.quaRot.PosAxis({ 0,200,0 });
	VECTOR pos = VAdd(transform_.pos, relativePos);
	hpTrans_.pos = ConvWorldPosToScreenPos(pos);
	VECTOR hpPos = hpTrans_.pos;

	//カメラから外れていたら表示しない
	if (hpPos.z > 0.0f && hpPos.z < 1.0f)
	{
		//HPの表示
		// HPの黒枠
		DrawBoxAA(hpPos.x - 50, hpPos.y,
			hpPos.x + 30, hpPos.y + 10,
			0x000000, false, 5.0f);

		// HPゲージ
		DrawBox(hpPos.x - 50, hpPos.y,
			hpPos.x + 30, hpPos.y + 10,
			0xff000, true);
	}

}

void EnemyBase::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//// 移動量をゼロ
	//movePow_ = MyUtility::VECTOR_ZERO;

	// X軸回転を除いた、重力方向に垂直なカメラ角度(XZ平面)を取得
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// 回転したい角度
	double rotRad = 0;

	//VECTOR dir = MyUtility::DIR_F;
	VECTOR dir;// = MyUtility::DIR_F;

	//// カメラ方向に前進したい
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	rotRad = MyUtility::Deg2RadD(0.0);
	//	dir = cameraRot.GetForward();
	//}

	//// カメラ方向から後退したい
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	rotRad = MyUtility::Deg2RadD(180.0);
	//	dir = cameraRot.GetBack();
	//}

	//// カメラ方向から右側へ移動したい
	//if (ins.IsNew(KEY_INPUT_D))
	//{
	//	rotRad = MyUtility::Deg2RadD(90.0);
	//	dir = cameraRot.GetRight();
	//}

	//// カメラ方向から左側へ移動したい
	//if (ins.IsNew(KEY_INPUT_A))
	//{
	//	rotRad = MyUtility::Deg2RadD(270.0);
	//	dir = cameraRot.GetLeft();
	//}

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//プレイヤーへに向けた方向取得
	/*VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	dir = VNorm(len);*/


	/*if (!MyUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {*/

	// 移動処理
	//speed_ = SPEED_MOVE;
	//衝突判定(敵とプレイヤー)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = MyUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//範囲に入った
			speed_ = 0;
			//スコア加算
			isAddScore_ = true;
			//衝突判定
			isBikeCol_ = true;

			// ヒットエフェクト
			HitEffect();
			effectHitPlayId_ = PlayEffekseer3DEffect(effectHitResId_);

			//コイン収集時の音を再生
			PlaySoundMem(ResourceManager::GetInstance().Load(
				ResourceManager::SRC::SND_COIN).handleId_, DX_PLAYTYPE_BACK, true);

			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			if (!isEnemyCol_)
			{
				speed_ = SPEED_MOVE;
				isBikeCol_ = false;
				isAddScore_ = false;
			}
		}
	}
	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/

	//向いてる方向
	/*moveDir_ = dir;
	movePow_ = VScale(dir, speed_);*/

	// 回転処理(プレイヤーの方向を向かせる)
	//VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	//double subDeg = atan2(subVec.x, subVec.z);
	//SetGoalRotate(subDeg);

	if (!isJump_ && IsEndLanding())
	{

		animationController_->Play((int)ANIM_TYPE::FAST_RUN);

	}

	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/

	//削除処理
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}
}

void EnemyBase::ProcessJump(void)
{
	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	// ジャンプ
	if (isHit && (isJump_ || IsEndLanding()))
	{

		if (!isJump_)
		{
			// 制御無しジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP);
			// ループしないジャンプ
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false);
			// 切り取りアニメーション
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);
			// 無理やりアニメーション
			animationController_->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		isJump_ = true;

		// ジャンプの入力受付時間をヘラス
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(MyUtility::DIR_U, POW_JUMP);
		}

	}

	// ボタンを離したらジャンプ力に加算しない
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}
}

void EnemyBase::SetGoalRotate(double rotRad)
{
	//VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, MyUtility::AXIS_Y);

	// 現在設定されている回転との角度差を取る
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// しきい値
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void EnemyBase::RotY(void)
{
	//地面と衝突したら回転
	if (isCollGround_)
	{
		//回転
		// デグリーからラジアン(変換)
		float rad = MyUtility::Deg2RadF(SPEED_ROT);

		// ラジアンからクォータニオン(指定軸を指定角分回転させる)
		Quaternion rotPow = Quaternion::AngleAxis(rad, MyUtility::AXIS_Y);

		//クォータニオン(回転)の合成
		enemyRotY_ = enemyRotY_.Mult(rotPow);
	}
}

void EnemyBase::Collision(void)
{
	// 現在座標を起点に移動後座標を決める
	movedPos_ = VAdd(transform_.pos, movePow_);

	//y座標での死亡判定
	if (transform_.pos.y <= -500.0f)
	{
		ChangeState(STATE::DEAD);
	}

	// 衝突(カプセル)
	CollisionCapsule();

	// 衝突(重力)
	CollisionGravity();

	// 移動
	transform_.pos = movedPos_;
}

void EnemyBase::CollisionGravity(void)
{

	// ジャンプ量を加算
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力方向の反対
	VECTOR dirUpGravity = MyUtility::DIR_U;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{

		// 地面との衝突
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// 最初は上の行のように実装して、木の上に登ってしまうことを確認する
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			// 衝突地点から、少し上に移動
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// ジャンプリセット
			jumpPow_ = MyUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// 着地モーション
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			isJump_ = false;
			isCollGround_ = true;

		}

	}
}

void EnemyBase::CollisionCapsule(void)
{
	// カプセルを移動させる
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// カプセルとの衝突判定
	for (const auto c : colliders_)
	{

		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// カプセルを移動させる
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;

			}

		}

		// 検出した地面ポリゴン情報の後始末
		MV1CollResultPolyDimTerminate(hits);

	}



}

void EnemyBase::CalcGravityPow(void)
{
	//1回当たったら中断
	if (isCollGround_)
	{
		transform_.pos.y = COLL_AFTER_POS_Y;
		return;
	}

	// 重力方向
	VECTOR dirGravity = MyUtility::DIR_D;

	// 重力の強さ
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// 重力
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// 最初は実装しない。地面と突き抜けることを確認する。
	// 内積
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// 重力方向と反対方向(マイナス)でなければ、ジャンプ力を無くす
		jumpPow_ = gravity;
	}
}

bool EnemyBase::IsEndLanding(void)
{
	bool ret = true;

	// アニメーションがジャンプではない
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	// アニメーションが終了しているか
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

bool EnemyBase::IsAtkStart(void)
{


	return false;
}
