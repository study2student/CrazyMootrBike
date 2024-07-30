#pragma once
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;
class Bike;
class Score;
class GameScene;

class CoinBase : public ActorBase
{

public:

	//半径
	static constexpr float RADIUS = 200.0f;

	//当たり判定用コライダーの上座標
	static constexpr VECTOR COLLIDER_POS_TOP = { 0.0f, 110.0f, 0.0f };

	//当たり判定用コライダーの下座標
	static constexpr VECTOR COLLIDER_POS_DOWN = { 0.0f, 30.0f, 0.0f };

	//回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//1ループステージあたりの敵の生成数
	static constexpr int MAX_MAKE_NUM = 3;

	//敵の生成時のセンター方向からの横幅
	static constexpr float DIR_LEN = 500.0f;

	//縦の敵間距離
	static constexpr float X_LEN = 400.0f;

	//敵生成時の横の調整用
	static constexpr float ADJUST_POS_X = 14300.0f;

	//倒した時のスコア増分
	static constexpr int SCORE_INCREMENT = 100;

	//地面衝突後の座標
	static constexpr float COLL_AFTER_POS_Y = -150.0f;

	//回転スピード
	static constexpr float SPEED_ROT = 7.0f;

	//死亡状態になるまでの最大時間
	static constexpr float TO_DEAD_TIME_MAX = 6.0f;

	// 志望状態になるY座標
	static constexpr float DEAD_POS_Y = -500.0f;

	// エフェクト出現位置Z座標
	static constexpr int EFF_POS_Z = 500;

	// 状態
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		DEAD,
	};

	//敵の種類
	enum class TYPE
	{
		GOLD,
		SILVER,
		COPPER,
		MAX
	};

	//敵発生場所
	enum class DIR
	{
		LEFT,
		CENTER,
		RIGHT,
		MAX

	};

	// コンストラクタ
	CoinBase(const std::vector<std::shared_ptr<Bike>>& bikes, GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

	// デストラクタ
	virtual ~CoinBase(void);

	virtual void Init(void) override;
	virtual void SetParam(void);
	virtual void Update(void) override;
	virtual void Draw(void) override;

	virtual void UpdatePlay(void);
	virtual void ProcessMove(void);

	// 衝突判定に用いられるコライダ制御
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// 衝突用カプセルの取得
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	// 敵に吹っ飛ばされた
	void Flip(VECTOR dir);

	// プレイヤーと当たったか
	bool GetIsBikeCol(void);

	//スコアを加算してよいか取得
	bool GetIsAddScore(void);

	//状態取得
	STATE GetState(void);

	//死亡状態か
	bool IsDestroy(void);

	//死亡状態へ
	void Destroy(void);

	// スコア加算メソッド
	void AddScoreToPlayer(int playerId, int score);
protected:

	//バイク情報
	std::vector< std::shared_ptr<Bike>> bikes_;

	//ゲームシーン
	GameScene* gameScene_;

	// 状態管理
	STATE state_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;
	
	// 移動後の座標
	VECTOR movedPos_;

	//敵の発生座標座標
	VECTOR makePos_;

	//先頭の敵から次の敵までの相対座標
	VECTOR localPos_;

	//地面と衝突したかどうか
	bool isCollGround_;

	//生成されてからの秒数
	float stepMade_;

	// 回転
	Quaternion enemyRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// ジャンプ量
	VECTOR jumpPow_;

	// 衝突判定に用いられるコライダ
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// 衝突チェック
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	//プレイヤー(バイク)とあたっているかどうか
	bool isBikeCol_;

	//攻撃用
	VECTOR fowardPos_;
	VECTOR backPos_;

	//スコアを加算してもよいか
	bool isAddScore_;

	float flipSpeed_;
	VECTOR flipDir_;

	// ヒットエフェクト
	int effectHitResId_;
	int effectHitPlayId_;

	// エフェクト初期化
	void InitEffect(void);
	// Hitエフェクトの位置
	void HitEffect();

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);
	void ChangeStateDead(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateFliped(void);
	void UpdateDead(void);

	// 回転
	void SetGoalRotate(float rotRad);
	void RotY(void);

	// 衝突判定
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// 移動量の計算
	void CalcGravityPow(void);

};





