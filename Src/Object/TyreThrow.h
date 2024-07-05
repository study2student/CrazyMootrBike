#pragma once
#include <map>
#include <DxLib.h>
#include "ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;

class TyreThrow : public ActorBase
{
public:

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//��]���x
	static constexpr float SPEED_ROT = 20.0f;

	//���x
	static constexpr float SPEED_MOVE = 30.0f;

	//�ꏊ�\������
	static constexpr float PLACE_DRAW_MAX_TIME = 3.0f;

	//������܂ł̎���
	static constexpr float TO_DELETE_MAX_TIME = 3.5f;

	//�^�C����������
	static constexpr float TYRE_REMAKE_MAX_TIME = 9.0f;

	//�^�C���̓�����ʒu(��������)
	static constexpr VECTOR THROW_LOCAL_POS_TO_SIDE = { 0.0f,0.0f,4000.0f };

	//�^�C���̓�����ʒu(�����΂߂�)
	static constexpr VECTOR THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { 0.0f,0.0f,3000.0f };

	//�^�C���̓�����ʒu(�傫���΂߂�)
	static constexpr VECTOR THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { 0.0f,0.0f,2000.0f };


	// ���
	enum class STATE
	{
		IDLE,
		THROW_MOVE,
		DESTROY
	};

	//���������
	enum class DIR
	{
		SIDE,				//����
		SLIGHTLY_OBLIPUE,	//�����΂߂�
		LARGE_OBLIPUE,		//�傫���΂߂�
		MAX
	};


	// �R���X�g���N�^
	TyreThrow();

	// �f�X�g���N�^
	~TyreThrow(void);

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

	//���e�������ɓ����������ݒ�
	void SetIsCol(bool isCol);

	//���e�������������擾
	bool GetIsCol(void);

private:

	Transform transformTarget_;

	// ��ԊǗ�
	STATE state_;

	// �X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	//�^�[�Q�b�g�Ɍ���������
	VECTOR targetDir_;

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
	float stepTyreDestroy_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateIdle(void);
	void ChangeStateThrow(void);
	void ChangeStateDestroy(void);

	// �X�V�X�e�b�v
	void UpdateIdle(void);
	void UpdateThrowMove(void);
	void UpdateDestroy(void);

	// �`��n
	void DrawDebug(void);

	// ����
	void ProcessDebug(void);//�f�o�b�O�p

	// ��]
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

};











