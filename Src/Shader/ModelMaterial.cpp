#include <Dxlib.h>
#include "../Application.h"
#include "ModelMaterial.h"

ModelMaterial::ModelMaterial(std::string shaderaafileNameVS, int constBufFloat4SizeVS, std::string shaderaafileNamePS, int constBufFloat4SizePS)
{
	// ���_�V�F�[�_�̃��[�h
	shaderVS_ = LoadVertexShader(
		(Application::PATH_SHADER + shaderaafileNameVS).c_str());

	// �萔�o�b�t�@�̊m�ۃT�C�Y
	constBufFloat4SizeVS_ = constBufFloat4SizeVS;

	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@���쐬
	constBufVS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizeVS);


	// �s�N�Z���V�F�[�_�̃��[�h
	shaderPS_ = LoadPixelShader(
		(Application::PATH_SHADER + shaderaafileNamePS).c_str());

	// �萔�o�b�t�@�̊m�ۃT�C�Y
	constBufFloat4SizePS_ = constBufFloat4SizePS;

	// �s�N�Z���V�F�[�_�p�̒萔�o�b�t�@���쐬
	constBufPS_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4SizePS);

	texAddres_ = TEXADDRESS::CLAMP;
}

ModelMaterial::~ModelMaterial(void)
{
	DeleteShader(shaderVS_);
	DeleteShaderConstantBuffer(constBufVS_);
	DeleteShader(shaderPS_);
	DeleteShaderConstantBuffer(constBufPS_);
}

void ModelMaterial::AddConstBufVS(const FLOAT4& constBuf)
{
	if (constBufFloat4SizeVS_ > constBufsVS_.size())
	{
		constBufsVS_.emplace_back(constBuf);
	}
}

void ModelMaterial::AddConstBufPS(const FLOAT4& constBuf)
{
	if (constBufFloat4SizePS_ > constBufsPS_.size())
	{
		constBufsPS_.emplace_back(constBuf);
	}
}

void ModelMaterial::SetConstBufVS(int idx, const FLOAT4& constBuf)
{
	if (idx >= constBufsVS_.size())
	{
		return;
	}

	constBufsVS_[idx] = constBuf;
}

void ModelMaterial::SetConstBufPS(int idx, const FLOAT4& constBuf)
{
	if (idx >= constBufsPS_.size())
	{
		return;
	}

	constBufsPS_[idx] = constBuf;
}

void ModelMaterial::AddTextureBuf(int slot, int handle)
{
	if (textures_.count(slot) == 0)
	{
		textures_.emplace(slot, handle);
	}
}

void ModelMaterial::SetTextureBuf(int slot, int handle)
{
	if (textures_.count(slot) != 0)
	{
		//textures_.emplace(slot, handle);
		textures_[slot] = handle;
	}
}

int ModelMaterial::GetShaderVS(void) const
{
	return shaderVS_;
}

int ModelMaterial::GetShaderPS(void) const
{
	return shaderPS_;
}

int ModelMaterial::GetConstBufVS(void) const
{
	return constBufVS_;
}

int ModelMaterial::GetConstBufPS(void) const
{
	return constBufPS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsVS(void) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return constBufsVS_;
}

const std::vector<FLOAT4>& ModelMaterial::GetConstBufsPS(void) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return constBufsPS_;
}

const std::map<int, int>& ModelMaterial::GetTextures(void) const
{
	// TODO: return �X�e�[�g�����g�������ɑ}�����܂�
	return textures_;
}

ModelMaterial::TEXADDRESS ModelMaterial::GetTextureAddress(void) const
{
	return texAddres_;
}

void ModelMaterial::SetTextureAddress(TEXADDRESS texA)
{
	texAddres_ = texA;
}

