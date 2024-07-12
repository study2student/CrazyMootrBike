#pragma once
#include <DxLib.h>
#include <memory>
#include "../Common/Quaternion.h"
class Transform;
class GameScene;

class Camera
{

public:

	// カメラスピード(度)
	static constexpr float SPEED = 1.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// 追従位置からカメラ位置までの相対座標
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 300.0f, -400.0f };
	//static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 300.0f, -900.0f };

	// 追従位置からカメラ位置までの相対座標(ブースト)
	static constexpr VECTOR BOOST_LOCAL_F2C_MAX_POS = { 0.0f, 300.0f, -1000.0f };

	// 追従位置から注視点までの相対座標
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };

	// 追従対象からカメラ位置までの相対座標(ブースト開始時)
	static constexpr VECTOR RELATIVE_F2C_POS_BOOST_START = { 0.0f, 40.0f, 150.0f };

	// ブースト使用時のカメラ速度増分
	static constexpr float ADD_CAMERA_SPEED = 0.32f;

	// ブースト使用時のカメラ速度上限値
	static constexpr float CAMERA_SPEED_MAX = 3.0f;

	// 回転完了までの時間
	static constexpr float TIME_ROT = 0.6f;

	// カメラのX回転上限度角
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		SELF_SHOT
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	// カメラ位置
	VECTOR GetPos(void) const;
	// カメラの操作角度
	VECTOR GetAngles(void) const;
	// カメラの注視点
	VECTOR GetTargetPos(void) const;

	// カメラ角度
	Quaternion GetQuaRot(void) const;
	// X回転を抜いたカメラ角度
	Quaternion GetQuaRotOutX(void) const;
	// カメラの前方方向
	VECTOR GetForward(void) const;

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	//ポーズ中かどうか設定
	void SetIsPause(bool isPause);

	//カメラを正面に戻すかどうか設定(ステージ外にでた場合)
	void SetIsCameraReset(bool isCameraReset);

	//ブースト中かどうか設定
	void SetIsBoost(bool isBoost);


private:


	// カメラが追従対象とするTransform
	const Transform* followTransform_;

	//ポーズ中かどうか
	bool isPause_;

	//カメラを正面に戻すかどうか(ステージ外にでた場合)
	bool isCameraReset_;

	//ブースト中か
	bool isBoost_;

	//ブースト手前に行ったときの位置
	VECTOR boostLocalPos_;

	//手前にくるまでの経過時間
	float stepMoveFront_;

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// X軸回転が無い角度
	Quaternion rotOutX_;

	// カメラ角度
	Quaternion rot_;

	//カメラ速度
	float velocity_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	float stepRotTime_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象との位置同期を取る
	void SyncFollow(void);

	// カメラ操作
	void ProcessRot(void);

	// モード別更新ステップ
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawSelfShot(void);


};

