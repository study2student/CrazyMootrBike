#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include <memory>
#include <vector>
class SceneManager;
class SkyDome;
class AnimationController;
class Bike;

class TitleScene : public SceneBase
{

public:

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

	//バイク
	Transform bike;


	// アニメーション
	AnimationController* animationController_;

};
