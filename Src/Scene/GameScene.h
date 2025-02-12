#pragma once
#include "SceneBase.h"
#include <vector>
#include <list>
#include <memory>
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
class Pause;

class GameScene : public SceneBase
{

public:

	enum class PAUSE_STATE
	{
		RESTART,//再開
		RETRY,	//リトライ
		END		//終わる
	};


	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//コイン情報
	std::vector<CoinBase*> GetCoins(void);

	//コインが追加されたタイミングかどうか
	bool GetIsCreateCoin(void);

	//プレイヤー人数取得
	int GetPlayNum(void);

	//1人プレイ時ゴールしたかどうか
	bool OnePersonIsGoal(void);

	//先頭(座標)のバイクの要素番号を取得
	size_t GetBikeMaxPosZIndex(void);
	

private:

	//メインスクリーン
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

	// ヒットストップの持続時間（秒）
	float hitStopDuration;
	// ヒットストップのタイマー
	float hitStopTimer;
	// ヒットストップ中かどうかのフラグ
	bool isHitStop;

	//プレイヤー人数
	int playNumber_;

	//ポーズメニュー中かどうか
	bool isPause_;

	//FINISH文字の位置
	Vector2 finishImgMovePos_;

	//ゴールしてからの経過時間
	float stepGoalAfter_;

	//ポーズ
	std::unique_ptr<Pause> pause_;

	// カメラやスクリーンの表示の関数化
	void DrawObject(int playerID);
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

	//警告
	void WarningDraw(void);

	//ゴールしたあとの描画処理
	void GoalAfterDraw(int playNum, Vector2 drawPos);

	//死亡したあとの描画処理
	void DeadAfterDraw(Vector2 drawPos,Vector2 boxMinPos, Vector2 boxMaxPos);

	//コイン画像描画
	void CoinImgDraw(int x, int y);
};
