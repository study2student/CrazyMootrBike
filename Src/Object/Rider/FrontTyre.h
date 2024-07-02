#pragma once
#include <map>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;

class FrontTyre : public ActorBase
{
public:

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//��]���x
	static constexpr float SPEED_ROT = 20.0f;

	// ���
	enum class STATE
	{
		NONE,
		ROT
	};


	// �R���X�g���N�^
	FrontTyre();

	// �f�X�g���N�^
	~FrontTyre(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�e�o�C�N�̏��ݒ�
	void SetTransform(Transform& transformParent);

private:

	Transform transformParent_;

	// ��ԊǗ�
	STATE state_;

	// �X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion rotX_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStateRot(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateRot(void);

	// �`��n
	void DrawDebug(void);

	// ����
	void ProcessDebug(void);//�f�o�b�O�p

	// ��]
	void Rotate(void);

	//�ʒu���킹
	void SyncParent(void);
};









