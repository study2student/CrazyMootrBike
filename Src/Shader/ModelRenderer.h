#pragma once
#include "../Common/Vector2.h"
#include "ModelMaterial.h"

class ModelRenderer
{

public:

	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// コンストラクタ
	ModelRenderer(int modelId, ModelMaterial& modelMaterial);

	//デストラクタ
	~ModelRenderer(void);

	// 描画
	void Draw(void);

private:

	// モデルのハンドルID
	int modelId_;

	ModelMaterial& modelMaterial_;

	// 座標
	Vector2 pos_;
	// 描画サイズ
	Vector2 size_;

	// シェーダ設定(頂点)
	void SetReserveVS(void);

	// シェーダ設定(ピクセル)
	void SetReservePS(void);

};

