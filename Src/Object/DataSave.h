#pragma once

class DataSave
{

public:


	//データ
	struct Data
	{
		//プレイヤー数
		int playerNum_;
	};

	enum class DataType
	{
		PLAYER_NUM	//プレイヤー人数設定
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static DataSave& GetInstance(void);

	void Init(void);

	//データ種類設定(なんのデータを設定するか,設定値)
	void SetData(DataType dataType,int num);

	//データ取得
	const Data& GetData(void) const;

	//データリセット
	void ResetData(void);

private:

	// 静的インスタンス
	static DataSave* instance_;

	//データ
	Data data_;

	
};



