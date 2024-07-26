#pragma once
#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class ModelMaterial
{
public:

	// 設定できる最大テクスチャの数
	static constexpr int MAX_TETURES = 4;

	// テクスチャアドレスタイプ
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//コンストラクタ
	ModelMaterial(std::string shaderaafileNameVS, int constBufFloat4SizeVS,
		std::string shaderaafileNamePS, int constBufFloat4SizePS);

	//デストラクタ
	~ModelMaterial(void);

	// 定数バッファを追加
	void AddConstBufVS(const FLOAT4& constBuf);
	void AddConstBufPS(const FLOAT4& constBuf);

	// 定数バッファを更新
	void SetConstBufVS(int idx, const FLOAT4& constBuf);
	void SetConstBufPS(int idx, const FLOAT4& constBuf);

	// テクスチャバッファを追加
	void AddTextureBuf(int slot, int handle);

	// テクスチャを更新
	void SetTextureBuf(int slot, int handle);

	// シェーダ
	int GetShaderVS(void) const;
	int GetShaderPS(void) const;

	// 定数バッファハンドル
	int GetConstBufVS(void) const;
	int GetConstBufPS(void) const;

	// 定数バッファ
	const std::vector<FLOAT4>& GetConstBufsVS(void) const;
	const std::vector<FLOAT4>& GetConstBufsPS(void) const;

	// 画像
	const std::map<int, int>& GetTextures(void) const;

	// テクスチャアドレスを取得
	TEXADDRESS GetTextureAddress(void) const;

	// テクスチャアドレスを設定
	void SetTextureAddress(TEXADDRESS texA);

private:

	// 頂点シェーダ
	int shaderVS_;

	//ピクセルシェーダ
	int shaderPS_;

	// 定数バッファの確保サイズ
	int constBufFloat4SizeVS_;

	// 定数バッファハンドル
	int constBufVS_;

	// 定数バッファ
	std::vector<FLOAT4> constBufsVS_;

	// ピクセル:定数バッファの確保サイズ
	int constBufFloat4SizePS_;

	// ピクセル:定数バッファハンドル
	int constBufPS_;

	// ピクセル:定数バッファ
	std::vector<FLOAT4> constBufsPS_;

	// テクスチャアドレス
	TEXADDRESS texAddres_;

	// gazou(スロット、ハンドルIO)
	std::map<int, int> textures_;
};



