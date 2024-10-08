#pragma once
#include <map>
#include "GimmickBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Spike : public GimmickBase
{
public:

	//�_���[�W
	static constexpr int SPIKE_DAMAGE = 25;


	// ���
	enum class STATE
	{
		IDLE,
		THROW_MOVE,
		DESTROY
	};

	//������p�x
	enum class ANGLE
	{
		SIDE,				//����
		SLIGHTLY_OBLIPUE,	//�����΂߂�
		LARGE_OBLIPUE,		//�傫���΂߂�
		MAX
	};

	//�ǂ����瓊���邩
	enum class DIR
	{
		LEFT,
		RIGHT,
		MAX
	};


	// �R���X�g���N�^
	Spike();

	// �f�X�g���N�^
	~Spike(void);

	void Update(void) override;
	void Draw(void) override;

	//�o�C�N�̏��ݒ�
	void SetTransform(Transform transformTarget);

	//�����ɓ����������ݒ�
	void SetIsCol(bool isCol);

	//�����������擾
	const bool& GetIsCol(void) const;

	//�ҋ@��Ԃ��ǂ���
	const bool IsIdle(void);

private:

	//�ڕW(�v���C���[)�̈ʒu��]���
	Transform transformTarget_;

	// ��ԊǗ�
	STATE state_;

	//�^�[�Q�b�g�Ɍ���������
	VECTOR targetDir_;

	//�^�[�Q�b�g�Ɍ����������ۑ��p
	VECTOR targetDirSave_;

	// ��]
	Quaternion rotX_;

	//����������
	bool isCol_;

	//�ꏊ�\������
	float stepPlaceDrawTime_;

	//�Ƃ����o�����Ă��������܂ł̎���
	float stepToDeleteTime_;

	//�Ƃ��������Ă���ǂ̂��炢���Ԍo�߂�����
	float stepSpikeDestroy_;

	// �����G�t�F�N�g
	int makeEffectResId_;
	int makeEffectPlayId_;

	//�����G�t�F�N�g
	int bombEffectResId_;
	int bombEffectPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g
	void PlayMakeEffect(void);
	void PlayBombEffect(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateThrow(void);
	void ChangeStateDestroy(void);

	// �X�V�X�e�b�v
	void UpdateIdle(void);
	void UpdateThrowMove(void);
	void UpdateDestroy(void);

	// ��]
	void Rotate(void);

	// ������(�O���ǂݍ���)
	void InitLoad(void) override;

	// ������(���㏈��)
	void InitPost(void) override;
};
