#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
#include "../Common/Vector2.h"
class SceneManager;
class SkyDome;
class Vector2;

class SelectScene : public SceneBase
{

public:


	//1人でボタンの横の長さ
	static constexpr int ONE_PERSON_FONT_LENGTH = 160;

	//1人でボタンの高さ
	static constexpr int ONE_PERSON_FONT_HEIGHT = 50;

	//4人でボタンの横の長さ
	static constexpr int FOUR_PERSON_FONT_LENGTH = 160;

	//4人でボタンの高さ
	static constexpr int FOUR_PERSON_FONT_HEIGHT = 50;

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:

	// スカイドーム用
	Transform spaceDomeTran_;

	// スカイドーム(背景)
	SkyDome* skyDome_;

	//左上の1人でポジション
	Vector2 onePersonFontBasePos_;

	//左上の4人でポジション
	Vector2 fourPersonFontBasePos_;

	//1人で文字色
	int onePersonFontColor_;

	//4人で文字色
	int fourPersonFontColor_;

	//マウス操作
	void MouseProcess(void);


};


