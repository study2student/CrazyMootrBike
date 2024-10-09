#pragma once
#include <map>
#include "SceneBase.h"
class Score;
class Bike;

class GameOverScene : public SceneBase
{
public:

	enum class PAUSE_STATE
	{
		RETRY,	//リトライ
		END		//終わる
	};

	// コンストラクタ
	GameOverScene(void);

	// デストラクタ
	~GameOverScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;

	//スコア描画の背景
	int img_BG;
	int imgPush_;

	//セレクト背景
	int imgSelectBG_;

	//状態
	PAUSE_STATE pState_;

	//左上のリトライポジション
	Vector2 reTryFontBasePos_;

	//左上の終わるポジション
	Vector2 endFontBasePos_;

	//リトライ文字色
	int reTryFontColor_;

	//終わる文字色
	int endFontColor_;

	//現カーソル位置
	int nowCursor_;

	//カーソルが文字にあってるかどうか
	bool isCursorHit_;

	//マウス操作
	void DecideProcess(void);

	//キー操作
	void SelectProcess(void);

	//状態遷移
	void ChangePState(PAUSE_STATE pState);

	//現カーソル番号から状態を変化させる
	void CursorToPState(int cursor);
};

