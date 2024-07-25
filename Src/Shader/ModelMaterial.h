#pragma once
#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>

class ModelMaterial
{
public:

	// �ݒ�ł���ő�e�N�X�`���̐�
	static constexpr int MAX_TETURES = 4;

	// �e�N�X�`���A�h���X�^�C�v
	enum class TEXADDRESS
	{
		NONE = 0,
		WRAP,
		MIRROR,
		CLAMP,
		BORDER,
		MAX
	};

	//�R���X�g���N�^
	ModelMaterial(std::string shaderaafileNameVS, int constBufFloat4SizeVS,
		std::string shaderaafileNamePS, int constBufFloat4SizePS);

	//�f�X�g���N�^
	~ModelMaterial(void);

	// �萔�o�b�t�@��ǉ�
	void AddConstBufVS(const FLOAT4& constBuf);
	void AddConstBufPS(const FLOAT4& constBuf);

	// �萔�o�b�t�@���X�V
	void SetConstBufVS(int idx, const FLOAT4& constBuf);
	void SetConstBufPS(int idx, const FLOAT4& constBuf);

	// �e�N�X�`���o�b�t�@��ǉ�
	void AddTextureBuf(int slot, int handle);

	// �e�N�X�`�����X�V
	void SetTextureBuf(int slot, int handle);

	// �V�F�[�_
	int GetShaderVS(void) const;
	int GetShaderPS(void) const;

	// �萔�o�b�t�@�n���h��
	int GetConstBufVS(void) const;
	int GetConstBufPS(void) const;

	// �萔�o�b�t�@
	const std::vector<FLOAT4>& GetConstBufsVS(void) const;
	const std::vector<FLOAT4>& GetConstBufsPS(void) const;

	// �摜
	const std::map<int, int>& GetTextures(void) const;

	// �e�N�X�`���A�h���X���擾
	TEXADDRESS GetTextureAddress(void) const;

	// �e�N�X�`���A�h���X��ݒ�
	void SetTextureAddress(TEXADDRESS texA);

private:

	// ���_�V�F�[�_
	int shaderVS_;

	//�s�N�Z���V�F�[�_
	int shaderPS_;

	// �萔�o�b�t�@�̊m�ۃT�C�Y
	int constBufFloat4SizeVS_;

	// �萔�o�b�t�@�n���h��
	int constBufVS_;

	// �萔�o�b�t�@
	std::vector<FLOAT4> constBufsVS_;

	// �s�N�Z��:�萔�o�b�t�@�̊m�ۃT�C�Y
	int constBufFloat4SizePS_;

	// �s�N�Z��:�萔�o�b�t�@�n���h��
	int constBufPS_;

	// �s�N�Z��:�萔�o�b�t�@
	std::vector<FLOAT4> constBufsPS_;

	// �e�N�X�`���A�h���X
	TEXADDRESS texAddres_;

	// gazou(�X���b�g�A�n���h��IO)
	std::map<int, int> textures_;
};



