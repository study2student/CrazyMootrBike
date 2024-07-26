#include <Dxlib.h>

#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "ModelMaterial.h"
#include "ModelRenderer.h"


ModelRenderer::ModelRenderer(int modelId, ModelMaterial& modelMaterial) : modelMaterial_(modelMaterial)
{
	modelId_ = modelId;
}

ModelRenderer::~ModelRenderer(void)
{
}


void ModelRenderer::Draw(void)
{

	MV1SetUseOrigShader(true);

	// 頂点シェーダの設定
	SetReserveVS();

	// ピクセルシェーダの設定
	SetReservePS();

	// テクスチャアドレスタイプの取得
	auto texA = modelMaterial_.GetTextureAddress();
	int texAType = static_cast<int>(texA);

	// テクスチャアドレスタイプを変更
	SetTextureAddressModeUV(texAType, texAType);

	MV1DrawModel(modelId_);

	SetTextureAddressModeUV(DX_TEXADDRESS_CLAMP, DX_TEXADDRESS_CLAMP);

	// テクスチャ解除
	const auto textures = modelMaterial_.GetTextures();
	size_t size = textures.size();
	if (size == 0)
	{
		SetUseTextureToShader(0, 1);
	}
	else
	{
		for (const auto& tex : textures)
		{
			SetUseTextureToShader(tex.first, -1);
		}
	}

	// 頂点シェーダ解除
	SetUseVertexShader(-1);

	// ピクセルシェーダ解除
	SetUsePixelShader(-1);

	MV1SetUseOrigShader(false);

}


void ModelRenderer::SetReserveVS(void)
{
	// 頂点シェーダ設定
	SetUseVertexShader(modelMaterial_.GetShaderVS());

	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufVS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsVS();

	size_t size = constBufs.size();

	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 定数バッファをピクセルシェーダ用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_VERTEX, CONSTANT_BUF_SLOT_BEGIN_VS);

	// 頂点シェーダ設定
	SetUseVertexShader(modelMaterial_.GetShaderVS());
}

void ModelRenderer::SetReservePS(void)
{
	SetUsePixelShader(modelMaterial_.GetShaderPS());

	size_t size;

	// ピクセルシェーダにテクスチャを転送
	const auto& textures = modelMaterial_.GetTextures();
	size = textures.size();
	if (size == 0)
	{
		SetUseTextureToShader(0, -1);
	}
	else
	{
		for (const auto tex : textures)
		{
			SetUseTextureToShader(tex.first, tex.second);
		}
	}


	// 定数バッファハンドル
	int constBuf = modelMaterial_.GetConstBufPS();

	FLOAT4* constBufsPtr = (FLOAT4*)GetBufferShaderConstantBuffer(constBuf);
	const auto& constBufs = modelMaterial_.GetConstBufsPS();

	size = constBufs.size();

	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			constBufsPtr++;
		}
		constBufsPtr->x = constBufs[i].x;
		constBufsPtr->y = constBufs[i].y;
		constBufsPtr->z = constBufs[i].z;
		constBufsPtr->w = constBufs[i].w;
	}

	// 定数バッファを更新して書き込んだ内容を反映する
	UpdateShaderConstantBuffer(constBuf);

	// 定数バッファをピクセルシェーダ用定数バッファレジスタにセット
	SetShaderConstantBuffer(
		constBuf, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS
	);

	// ピクセルシェーダ設定
	SetUsePixelShader(modelMaterial_.GetShaderPS());
}
