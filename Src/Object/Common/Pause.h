#pragma once
#include <string>
#include <DxLib.h>
#include <memory>
#include "Transform.h"
#include "../../Common/Vector2.h"
#include "../../Scene/GameScene.h"
class SceneManager;
class GameScene;
class Vector2;

class Pause
{

public:

	enum class STATE
	{
		RESTART,//再開
		RETRY,	//リトライ
		END		//終わる
	};

	// コンストラクタ
	Pause(void);

	// デストラクタ
	~Pause(void);

	void Init(void);
	void Update(void);
	void Draw(void);

	//ポーズ状態かどうか取得
	const bool& GetIsPause(void) const;

	//ポーズ前
	void PausePrevious(void);

	//ポーズ中
	void PauseMidst(void);

	//ポーズ画像設定
	void SetImgHandle(const int& handle);

private:

	// ポーズ背景画像
	int imgPause_;

	//左上の再開ポジション
	Vector2 reStartFontBasePos_;

	//左上のリトライポジション
	Vector2 reTryFontBasePos_;

	//左上の終わるポジション
	Vector2 endFontBasePos_;

	//再開文字色
	int reStartFontColor_;

	//リトライ文字色
	int reTryFontColor_;

	//終わる文字色
	int endFontColor_;

	//現カーソル位置
	int nowCursor_;

	//ポーズメニュー中かどうか
	bool isPause_;

	//カーソルが文字にあってるかどうか
	bool isCursorHit_;

	//ポーズキー入力からの時間
	float stepPauseKeyHit_;

	//状態
	STATE state_;

	//マウス操作
	void DecideProcess(void);

	//キー操作
	void SelectProcess(void);

	//状態遷移
	void ChangeState(STATE pState);

	//現カーソル番号から状態を変化させる
	void CursorToState(int cursor);

	//ポーズ文字描画
	void PauseDraw(void);
};

