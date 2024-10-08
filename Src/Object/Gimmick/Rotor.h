#pragma once
#include <map>
#include <DxLib.h>
#include "GimmickBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Rotor : public GimmickBase
{

public:

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		DEAD,
		END
	};

	// �A�j���[�V�������
	enum class ANIM_TYPE
	{
		IDLE,
		RUN,
		FAST_RUN,
		JUMP,
		WARP_PAUSE,
		FLY,
		FALLING,
		VICTORY
	};

	// �R���X�g���N�^
	Rotor();

	// �f�X�g���N�^
	~Rotor(void);

	void Update(void) override;
	void Draw(void) override;

	//�e�w���R�v�^�[�̏��ݒ�
	void SetTransform(Transform& transformParent);

private:

	Transform transformParent_;

	// ��ԊǗ�
	STATE state_;

	// �̗�
	int hp_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdatePlay(void);

	// �`��n
	void DrawDebug(void);

	// ����
	void ProcessMove(void);//�ړ�
	void ProcessDebug(void);//�f�o�b�O�p

	// ��]
	void SetGoalRotate(float rotRad);
	void SetGoalRotateZ(float rotRad);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ������(�O���ǂݍ���)
	void InitLoad(void) override;

	// ������(���㏈��)
	void InitPost(void) override;

};







