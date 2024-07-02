#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <memory>
#include <vector>
class SceneManager;
class SkyDome;
class AnimationController;
class Bike;
class Score;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		IDLE,
		START
	};

	//ボタンが押されてからのバイクの待機最大時間
	static constexpr float BIKE_IDLE__MAX_TIME = 3.0f;

	//バイクが発車してから次のシーンまでの最大時間
	static constexpr float BIKE_DEPARTURE_TO_NEXT_MAX_TIME = 4.5f;

	//バイクからフロントタイヤ相対座標
	static constexpr VECTOR BIKE_TO_FRONT_TYRE_LOCALPOS_FOR_TITLE = { 10.0f,68.0f,45.0f };

	//バイクからリアタイヤ相対座標
	static constexpr VECTOR BIKE_TO_REAR_TYRE_LOCALPOS_FOR_TITLE = { -5.0f,58.0f,-157.0f };

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// 画像
	int imgTitle_;
	int imgPush_;

	// スカイドーム用
	Transform spaceDomeTran_;

	// スカイドーム(背景)
	SkyDome* skyDome_;

	//道路
	Transform mainStage_;

	//ビル
	/*std::vector<Transform*> sity_;*/
	Transform sity_;

	// キャラクター
	Transform charactor_;

	//フロントタイヤ
	Transform frontTyre_;

	//リアタイヤ
	Transform rearTyre_;

	// アニメーション
	AnimationController* animationController_;

	//バイク
	Transform bike;

	//バイクが発車したかどうか
	bool isBikeDeparture_;

	//回転
	Quaternion tyreRotX_;

	//エフェクト系
	// バーンアウトエフェクト
	int effectBurnoutResId_;
	int effectBurnoutPlayId_;
	float effectBurnoutPosY_;

	// エフェクト初期化
	void InitEffect(void);

	//バイクの発車経過時間
	float stepBikeDeparture_;

	//状態
	STATE state_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateStart(void);

	// 更新ステップ
	void UpdateIdle(void);
	void UpdateStart(void);

	//バイク発車演出
	void BikeDeparture(void);

	//タイヤ回転
	void BikeTyreRot(void);

	//バーンアウトエフェクト
	void BurnoutEffect(void);

};
