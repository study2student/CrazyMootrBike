#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;

class FrontTyre : public ActorBase
{
public:

	// 回転完了までの時間
	static constexpr float TIME_ROT = 1.0f;

	//回転速度
	static constexpr float SPEED_ROT = 20.0f;

	// 状態
	enum class STATE
	{
		NONE,
		ROT
	};


	// コンストラクタ
	FrontTyre();

	// デストラクタ
	~FrontTyre(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//親バイクの情報設定
	void SetTransform(Transform& transformParent);

private:

	Transform transformParent_;

	// 状態管理
	STATE state_;

	// スピード
	float speed_;

	// 移動方向
	VECTOR moveDir_;

	// 移動量
	VECTOR movePow_;

	// 移動後の座標
	VECTOR movedPos_;

	// 回転
	Quaternion rotX_;

	// 状態遷移
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateRot(void);

	// 更新ステップ
	void UpdateNone(void);
	void UpdateRot(void);

	// 描画系
	void DrawDebug(void);

	// 操作
	void ProcessDebug(void);//デバッグ用

	// 回転
	void Rotate(void);

	//位置合わせ
	void SyncParent(void);
};









