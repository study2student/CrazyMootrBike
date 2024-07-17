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
	static constexpr int ONE_PERSON_FONT_LENGTH = 400;

	//1人でボタンの高さ
	static constexpr int ONE_PERSON_FONT_HEIGHT = 392;

	//4人でボタンの横の長さ
	static constexpr int FOUR_PERSON_FONT_LENGTH = 400;

	//4人でボタンの高さ
	static constexpr int FOUR_PERSON_FONT_HEIGHT = 360;

	//ひとりプレイ選択画像最大サイズ
	static constexpr float SELECT_IMG_MAX_SCALE = 1.50f;

	//ひとりプレイ選択画像最小サイズ
	static constexpr float SELECT_IMG_MIN_SCALE = 1.30f;

	//四人プレイ選択画像最大サイズ
	static constexpr float SELECT_FOUR_IMG_MAX_SCALE = 1.50f;

	//四人プレイ選択画像最小サイズ
	static constexpr float SELECT_FOUR_IMG_MIN_SCALE = 1.30f;

	//選択画像大きさ変化量
	static constexpr float SELECT_IMG_CHANGE_SCALE = 0.006f;

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

	// 背景画像
	int background_;

	// ひとりで選択画像
	int aloneImg_;

	// みんなで選択画像
	int everyoneImg_;

	// ひとりプレイ選択画像の大きさ
	float selectAloneImgScale_;
	// 四人プレイ選択画像の大きさ
	float selectFourImgScale_;
	//最大サイズかどうか
	bool isMaxSelectScale_;

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


