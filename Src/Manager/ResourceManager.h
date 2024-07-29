#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		TITLE,
		PUSH_SPACE,
		SPEECH_BALLOON,
		WARNING,	//警告
		PAUSE,		//ポーズ背景
		PLAYER,
		PLAYER_SHADOW,
		SKY_DOME,
		MAIN_PLANET,
		FALL_PLANET,
		FLAT_PLANET_01,
		FLAT_PLANET_02,
		LAST_PLANET,
		SPECIAL_PLANET,
		FOOT_SMOKE,
		WARP_STAR,
		WARP_STAR_ROT_EFF,
		WARP_ORBIT,
		BLACK_HOLE,
		GOAL_STAR,
		CLEAR,
		TANK_BODY,
		TANK_BARREL,
		TANK_WHEEL,

		//画像関連
		IMG_BACKGROUND,// セレクトシーンの背景
		IMG_SELECT_ALONE,//ひとりで
		IMG_SELECT_EVERYONE,//みんなで
		IMG_SCORE,//スコアの背景
		IMG_OPERATION,//操作説明画面
		IMG_GAMEOVER_SELECT_BG,	//ゲームオーバーシーンセレクト背景
		IMG_FINISH,		//フィニッシュ文字画像

		DEMO_STAGE,//仮ステージ
		JUMP_RAMP,//ジャンプ台
		CURVE,	//カーブ
		CITY,	//背景街並み
		GOAL,	//ゴール

		//ENEMY_SHORT,		//仮敵
		//ENEMY_LONG,
		//ENEMY_MAGIC,
		HELICOPTER,
		HELICOPTER_ROTOR,
		BOMB,		//爆弾
		BIKE,		//バイク
		TYRE,		//タイヤ
		BURNOUT_EFFECT, //バーンアウトエフェクト(バイク発車前)
		START_EFFECT, //ゲーム開始エフェクト

		GOLD_COIN,		//金メダル
		SILVER_COIN,	//銀メダル
		COPPER_COIN,	//銅メダル

		W_SWORD,//バスターソード
		SPIKE_BALL, //とげ球

		HITEFFECT,//ヒットした時のエフェクト
		SONICEFFECT,//ソニックエフェクト
		BOOST_EFFECT, //ブーストエフェクト
		THROW_MAKE_EFFECT,	//投げモノ発生エフェクト
		BOMB_EFFECT,		//爆発エフェクト
		BOMB_PLACE_EFFECT,	//爆弾場所エフェクト

		//サウンド関連
		SND_TITLE_BGM,//タイトルBGM
		SND_GAME_BGM,//ゲームBGM
		SND_COIN,//コイン収集時の音
		SND_BOOST,//ブースと使用時の音
		SND_MOTOR,//モーター音
		SND_SELECT,//セレクトシーンの選択時の音
		SND_START,//セレクト決定時の音
		SND_WARNING,//警告音
		SND_EXPLOSION,//爆発音
		SND_GOAL,//ゴール音
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースの完全破棄
	void Destroy(void);

	// リソースのロード
	const Resource& Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource*> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource&> loadedMap_;

	Resource dummy_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);
	ResourceManager(const ResourceManager& manager) = default;
	~ResourceManager(void) = default;

	// 内部ロード
	Resource& _Load(SRC src);

};
