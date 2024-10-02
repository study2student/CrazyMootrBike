#pragma once
#include <map>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class Spike : public ActorBase
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

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;

	//�o�C�N�̏��ݒ�
	void SetTransform(Transform transformTarget);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	//�����ɓ����������ݒ�
	void SetIsCol(bool isCol);

	//�����������擾
	bool GetIsCol(void);

	//�ҋ@��Ԃ��ǂ���
	bool IsIdle(void);

private:

	Transform transformTarget_;

	// ��ԊǗ�
	STATE state_;

	//�^�[�Q�b�g�Ɍ���������
	VECTOR targetDir_;

	//�^�[�Q�b�g�Ɍ����������ۑ��p
	VECTOR targetDirSave_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	// ��]
	Quaternion rotX_;

	//����������
	bool isCol_;

	//�ꏊ�\������
	float stepPlaceDrawTime_;

	//�^�C�����o�����Ă��������܂ł̎���
	float stepToDeleteTime_;

	//�^�C���������Ă���ǂ̂��炢���Ԍo�߂�����
	float stepSpikeDestroy_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �����G�t�F�N�g
	int makeEffectResId_;
	int makeEffectPlayId_;

	//�����G�t�F�N�g
	int bombEffectResId_;
	int bombEffectPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);

	// �G�t�F�N�g
	void MakeEffect(void);
	void BombEffect(void);

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

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);
};
