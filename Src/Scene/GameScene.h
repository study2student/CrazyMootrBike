#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Camera;
class Stage;
class SkyDome;
class Bike;
class CoinBase;
class GoldCoin;
class SilverCoin;
class CopperCoin;
class Helicopter;
class Score;
class Spike;

class GameScene : public SceneBase
{

public:

	enum class PAUSE_STATE
	{
		RESTART,//再開
		RETRY,	//リトライ
		END		//終わる
	};

	// プレイヤー同士の初期幅
	static constexpr float PLAYER_WIDTH = 200.0f;

	//パッド最大数
	static constexpr int PAD_MAX = 4;

	//再開ボタンの横の長さ
	static constexpr int RESTART_FONT_LENGTH = 100;

	//再開ボタンの高さ
	static constexpr int RESTART_FONT_HEIGHT = 48;

	//リトライボタンの横の長さ
	static constexpr int RETRY_FONT_LENGTH = 200;

	//リトライボタンの高さ
	static constexpr int RETRY_FONT_HEIGHT = 48;

	//終わるボタンの横の長さ
	static constexpr int END_FONT_LENGTH = 150;

	//終わるボタンの高さ
	static constexpr int END_FONT_HEIGHT = 48;

	//選択肢数
	static constexpr int SELECT_MAX_NUM = 3;

	//警告画像最大サイズ
	static constexpr float WARNING_IMG_MAX_SCALE = 0.40f;

	//警告画像最小サイズ
	static constexpr float WARNING_IMG_MIN_SCALE = 0.20f;

	//警告画像大きさ変化量
	static constexpr float WARNING_IMG_CHANGE_SCALE = 0.005f;

	//警告画像Y座標
	static constexpr float WARNING_POS_Y = 120.0f;

	//ポーズキー入力からもう一度押せるようになるまでの時間
	static constexpr float PAUSE_KEY_HIT_MAX_TIME = 1.25f;

	//ゴールしてから次のシーンになるまでの時間
	static constexpr float GOAL_TO_NEXT_SCENE = 2.5f;

	// HPバーの幅と高さ
	static constexpr int HP_BER = 10;

	// コントローラーの揺れ値
	static constexpr int CE_SWING_VALUE = 1000;

	// コントローラーの揺れ時間
	static constexpr int CE_SWING_TIME = 700;

	//エンカウント値
	static constexpr int ENCOUNT = 350;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//コイン情報
	std::vector<CoinBase*> GetEnemys(void);

	//コインが追加されたタイミングかどうか
	bool GetIsCreateEnemy(void);

	//プレイヤー人数取得
	int GetPlayNum(void);

	//1人プレイ時ゴールしたかどうか
	bool OnePersonIsGoal(void);

	//先頭(座標)のバイクの要素番号を取得
	size_t GetBikeMaxPosZIndex(void);
	

private:
	int mainScreen_;

	//1人プレイ時ゴールしたかどうか
	bool onePersonIsGoal_;

	// ゲームスタート時のカウント
	float startCount_;
	// スタートフラグ
	bool isStart_;

	// 警告画像
	int imgWarning_;
	//大きさ
	float warningImgScale_;
	//最大サイズかどうか
	bool isMaxWarningScale_;

	// ポーズ背景画像
	int imgPause_;

	//FINISH画像
	int imgFinish_;

	//コイン画像
	int imgCoin_;

	// カメラ
	std::vector<std::shared_ptr<Camera>> cameras_;

	// ステージ
	std::shared_ptr<Stage> stage_;

	// スカイドーム
	std::vector<std::unique_ptr<SkyDome>> skyDomes_;

	//トゲ
	std::shared_ptr<Spike> spike_;

	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;

	//ヘリコプター
	std::shared_ptr<Helicopter> helicopter_;

	//コイン
	CoinBase* coin_;

	//複数のコイン
	std::vector<CoinBase*>coins_;

	//コインの発生頻度
	int enCounter;

	//コインが追加されたタイミングかどうか
	bool isCreateCoin_;

	// カメラやスクリーンの表示の関数化
	void DrawObject(int playerID);
	//デバッグ表示
	void DrawDubg(void);
	// UI表示
	void DrawUI(int x, int y, int playerID);

	// ゴールした時の処理
	void GoalProcess(void);

	// コインの配置
	void CoinPlace(void);

	//衝突判定
	void Collision(void);

	// バイク同士の衝突判定
	void BikeCollision(void);

	float hitStopDuration;  // ヒットストップの持続時間（秒）
	float hitStopTimer;      // ヒットストップのタイマー
	bool isHitStop;        // ヒットストップ中かどうかのフラグ

	// ヒットエフェクト
	int effectHitResId_;
	int effectHitPlayId_;

	// エフェクト初期化
	void InitEffect(void);
	// Hitエフェクトの位置
	void HitEffect(void);

	//プレイヤー人数
	int playNumber_;

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

	//FINISH文字の位置
	Vector2 finishFontPos_;

	//ゴールしてからの経過時間
	float stepGoalAfter_;

	//状態
	PAUSE_STATE pState_;

	//マウス操作
	void DecideProcess(void);

	//キー操作
	void SelectProcess(void);

	//状態遷移
	void ChangePState(PAUSE_STATE pState);

	//現カーソル番号から状態を変化させる
	void CursorToPState(int cursor);

	//ポーズ
	void Pause(void);

	//警告
	void WarningDraw(void);

	//ポーズ文字描画
	void PauseDraw(void);

	//ゴールしたあとの処理
	void GoalAfterDraw(void);

	//コイン画像描画
	void CoinImgDraw(int x, int y);
};
