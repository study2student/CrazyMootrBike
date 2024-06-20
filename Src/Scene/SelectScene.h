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


	enum class STATE
	{
		ONE_PERSON,		//1人で
		FOUR_PERSON		//4人で
	};

	//1人でボタンの横の長さ
	static constexpr int ONE_PERSON_FONT_LENGTH = 60;

	//1人でボタンの高さ
	static constexpr int ONE_PERSON_FONT_HEIGHT = 20;

	//4人でボタンの横の長さ
	static constexpr int FOUR_PERSON_FONT_LENGTH = 60;

	//4人でボタンの高さ
	static constexpr int FOUR_PERSON_FONT_HEIGHT = 20;

	//選択肢数
	static constexpr int SELECT_MAX_NUM = 2;

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

	//カーソルが文字にあってるかどうか
	bool isCursorHit_;

	//現カーソル位置
	int nowCursor_;

	//状態
	STATE state_;

	//マウス操作
	void MouseProcess(void);

	//キー操作
	void KeyProcess(void);

	//状態遷移
	void ChangeState(STATE state);

	//現カーソル番号から状態を変化させる
	void CursorToState(int cursor);


};


