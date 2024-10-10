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
class DataSave;

class TitleScene : public SceneBase
{

public:

	enum class STATE
	{
		IDLE,
		START
	};

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
	// プッシュ文字画像
	int imgPush_;

	// スカイドーム用
	Transform spaceDomeTran_;

	// スカイドーム(背景)
	std::unique_ptr<SkyDome> skyDome_;

	//道路
	Transform mainStage_;

	//ビル
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
	//開始エフェクト
	int effectStartResId_;
	int effectStartPlayId_;

	// バーンアウトエフェクト
	int effectBurnoutResId_;
	int effectBurnoutPlayId_;
	float effectBurnoutPosY_;

	// エフェクト初期化
	void InitEffect(void);

	//バイクの発車経過時間
	float stepBikeDeparture_;

	//画像点滅
	//点滅時間
	float stepFlash_;

	//消えているかどうか
	bool isInvisible_;

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

	//スタートエフェクト
	void StartEffect(void);

	//バーンアウトエフェクト(スタート押す前)
	void BurnoutIdleEffect(void);

	//バーンアウトエフェクト(スタート押した後)
	void BurnoutMoveEffect(void);


};
