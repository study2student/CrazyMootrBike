#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
class Camera;
class Stage;
class SkyDome;
class Rider;
class Bike;
class EnemyBase;
class ShortDisEnemy;
class LongDisEnemy;
class MagicEnemy;
class EnemyBike;
class Helicopter;
class Score;
class TyreThrow;

class GameScene : public SceneBase
{

public:

	enum class PAUSE_STATE
	{
		RESTART,//再開
		RETRY,	//リトライ
		END		//終わる
	};

	//再開ボタンの横の長さ
	static constexpr int RESTART_FONT_LENGTH = 40;

	//再開ボタンの高さ
	static constexpr int RESTART_FONT_HEIGHT = 20;

	//リトライボタンの横の長さ
	static constexpr int RETRY_FONT_LENGTH = 80;

	//リトライボタンの高さ
	static constexpr int RETRY_FONT_HEIGHT = 20;

	//終わるボタンの横の長さ
	static constexpr int END_FONT_LENGTH = 60;

	//終わるボタンの高さ
	static constexpr int END_FONT_HEIGHT = 20;

	//選択肢数
	static constexpr int SELECT_MAX_NUM = 3;

	//警告画像最大サイズ
	static constexpr double WARNING_IMG_MAX_SCALE = 0.40;

	//警告画像最小サイズ
	static constexpr double WARNING_IMG_MIN_SCALE = 0.20;

	//警告画像大きさ変化量
	static constexpr double WARNING_IMG_CHANGE_SCALE = 0.005;

	//ポーズキー入力からもう一度押せるようになるまでの時間
	static constexpr float PAUSE_KEY_HIT_MAX_TIME = 0.25f;

	//ゴールしてから次のシーンになるまでの時間
	static constexpr float GOAL_TO_NEXT_SCENE = 2.5f;

	//エンカウント値
	static constexpr int ENCOUNT = 350;

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//敵情報
	std::vector<EnemyBase*> GetEnemys(void);
	std::vector<EnemyBike*> GetEnemyBikes(void);

	//敵が追加されたタイミングかどうか
	bool GetIsCreateEnemy(void);

	//プレイヤー人数取得
	int GetPlayNum(void);

private:
	int mainScreen_;

	// ゲームスタート時のカウント
	float startCount_;
	// スタートフラグ
	bool isStart_;

	// 画像
	int imgWarning_;
	//大きさ
	double warningImgScale_;
	//最大サイズかどうか
	bool isMaxWarningScale_;

	// カメラ
	std::vector<std::shared_ptr<Camera>> cameras_;

	// ステージ
	std::shared_ptr<Stage> stage_;

	////スコア
	//std::shared_ptr<Score> score_;

	// スカイドーム
	std::vector<std::unique_ptr<SkyDome>> skyDomes_;

	//タイヤ
	TyreThrow* throwTyre_;


	// プレイヤー
	//std::shared_ptr<Bike> bike_;

	// 複数プレイヤー
	std::vector<std::shared_ptr<Bike>> bikes_;
	////プレイ人数
	//int playNumber = 4;

	//ヘリコプター
	std::shared_ptr<Helicopter> helicopter_;

	////	敵
	//Enemy* enemy_;

	//敵
	EnemyBase* enemy_;
	EnemyBike* enemyBike_;

	//複数の敵
	std::vector<EnemyBase*>enemys_;
	std::vector<EnemyBike*>enemyBikes_;

	//敵の発生頻度
	int enCounter;

	//敵が追加されたタイミングかどうか
	bool isCreateEnemy_;

	//デバッグ表示
	void DrawDubg(void);

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
	void MouseProcess(void);

	//キー操作
	void KeyProcess(void);

	//状態遷移
	void ChangePState(PAUSE_STATE pState);

	//現カーソル番号から状態を変化させる
	void CursorToPState(int cursor);

	//ポーズ
	void Pause(void);

	//警告
	void WarningDraw(void);

	//ポーズ文字描画
	void PauseFontDraw(void);

	//ゴールしたあとの処理
	void GoalAfterDraw(void);
};
