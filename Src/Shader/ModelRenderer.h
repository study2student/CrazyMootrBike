#pragma once
#include "../Common/Vector2.h"
#include "ModelMaterial.h"

class ModelRenderer
{

public:

	static constexpr int CONSTANT_BUF_SLOT_BEGIN_VS = 7;

	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// �R���X�g���N�^
	ModelRenderer(int modelId, ModelMaterial& modelMaterial);

	//�f�X�g���N�^
	~ModelRenderer(void);

	// �`��
	void Draw(void);

private:

	// ���f���̃n���h��ID
	int modelId_;

	ModelMaterial& modelMaterial_;

	// ���W
	Vector2 pos_;
	// �`��T�C�Y
	Vector2 size_;

	// �V�F�[�_�ݒ�(���_)
	void SetReserveVS(void);

	// �V�F�[�_�ݒ�(�s�N�Z��)
	void SetReservePS(void);

};

