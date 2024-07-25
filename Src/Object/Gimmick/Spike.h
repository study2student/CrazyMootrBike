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

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	//��]���x
	static constexpr float SPEED_ROT = 20.0f;

	//���x
	static constexpr float SPEED_MOVE = 60.0f;

	//�ꏊ�\������
	static constexpr float PLACE_DRAW_MAX_TIME = 3.0f;

	//������܂ł̎���
	static constexpr float TO_DELETE_MAX_TIME = 4.5f;

	//�^�C����������
	static constexpr float TYRE_REMAKE_MAX_TIME = 8.0f;

	//�E���瓊����ʒu(��������)
	static constexpr VECTOR RIGHT_THROW_LOCAL_POS_TO_SIDE = { -900.0f,100.0f,13500.0f };

	//�E���瓊����ʒu(�����΂߂�)
	static constexpr VECTOR RIGHT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { -750.0f,100.0f,11000.0f };

	//�E���瓊����ʒu(�傫���΂߂�)
	static constexpr VECTOR RIGHT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { -600.0f,100.0f,7000.0f };

	//�����瓊����ʒu(��������)
	//static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_SIDE = { 900.0f,100.0f,13500.0f };
	static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_SIDE = { 600.0f,100.0f,7000.0f };

	//�����瓊����ʒu(�����΂߂�)
	//static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { 750.0f,100.0f,11000.0f };
	static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_SLIGHTLY_OBLIPUE = { 770.0f,100.0f,12800.0f };

	//�����瓊����ʒu(�傫���΂߂�)
	//static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { 600.0f,100.0f,7000.0f };
	static constexpr VECTOR LEFT_THROW_LOCAL_POS_TO_LARGE_OBLIPUE = { 900.0f,100.0f,13500.0f };

	//�E�̏ꍇ�̏o������X���W
	static constexpr float MAKE_RIGHT_POS_X = 2499.0f;

	//���̏ꍇ�̏o������X���W
	static constexpr float MAKE_LEFT_POS_X = 909.0f;

	//���[�J���ҋ@���W
	static constexpr VECTOR TYRE_IDLE_ROCAL_POS = { 0.0f,100.0f,15000.0f };

	//�_���[�W
	static constexpr int THROW_DAMAGE = 20;


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

	// �X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

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
	float stepTyreDestroy_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �����G�t�F�N�g
	int effectMakeResId_;
	int effectMakePlayId_;

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
