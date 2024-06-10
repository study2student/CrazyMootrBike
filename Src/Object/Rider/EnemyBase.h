#pragma once
#include <map>
//#include <vector>
#include <DxLib.h>
#include "../ActorBase.h"
#include <vector>
class AnimationController;
class Collider;
class Capsule;
class Bike;
class Score;

class EnemyBase : public ActorBase
{

public:

	// �X�s�[�h
	static constexpr float SPEED_MOVE = 5.0f;
	static constexpr float SPEED_RUN = 10.0f;

	//���a
	static constexpr float RADIUS = 200.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 1.0f;

	// �W�����v��
	static constexpr float POW_JUMP = 35.0f;

	// �W�����v��t����
	static constexpr float TIME_JUMP_IN = 0.5f;

	//1���[�v�X�e�[�W������̓G�̐�����
	static constexpr int MAX_MAKE_NUM = 3;

	//�G�̐������̃Z���^�[��������̉���
	static constexpr float DIR_LEN = 500.0f;

	//�c�̓G�ԋ���
	static constexpr float X_LEN = 400.0f;

	//�G�������̉��̒����p
	static constexpr float ADJUST_POS_X = 14300.0f;

	//�|�������̃X�R�A����
	static constexpr int SCORE_INCREMENT = 100;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		WARP_RESERVE,
		WARP_MOVE,
		DEAD,
		VICTORY,
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
		VICTORY,
		SHORT,
		LONG,
		BOMB

	};

	//�G�̎��
	enum class TYPE
	{
		SHORT_DIS,
		LONG_DIS,
		BOMB,
		MAX
	};

	//�G�����ꏊ
	enum class DIR
	{
		LEFT,
		CENTER,
		RIGHT,
		MAX

	};

	// �R���X�g���N�^
	EnemyBase(std::shared_ptr<Bike> bike, VECTOR loopStagePos, VECTOR localPos);

	// �f�X�g���N�^
	virtual ~EnemyBase(void);

	virtual void Init(void) override;
	virtual void SetParam(void);
	virtual void Update(void) override;
	virtual void Draw(void) override;

	virtual void UpdatePlay(void);
	virtual void ProcessMove(void);

	// �Փ˔���ɗp������R���C�_����
	void AddCollider(std::shared_ptr<Collider> collider);
	void ClearCollider(void);

	// �Փ˗p�J�v�Z���̎擾
	const Capsule* GetCapsule(void) const;

	//�v���C���[(�o�C�N)�̏��ݒ�
	void SetBikeTrans(Transform bikeTrans);

	//�X�s�[�h�ݒ�
	void SetSpeed(float speed);

	//�G���m�������������ݒ�
	void SetIsEnemyCol(bool isEnemyCol);

	// �G�ɐ�����΂��ꂽ
	void Flip(VECTOR dir);

	// �v���C���[�Ɠ���������
	bool GetIsBikeCol(void);

	//�X�R�A�����Z���Ă悢���擾
	bool GetIsAddScore(void);

	//��Ԏ擾
	STATE GetState(void);

	//���S��Ԃ�
	bool IsDestroy(void);

	//���S��Ԃ�
	void Destroy(void);

protected:

	// �A�j���[�V����
	AnimationController* animationController_;

	//�o�C�N���
	std::shared_ptr<Bike> bike_;

	// ��ԊǗ�
	STATE state_;

	// �ړ��X�s�[�h
	float speed_;

	// �ړ�����
	VECTOR moveDir_;

	// �ړ���
	VECTOR movePow_;

	// �ړ���̍��W
	VECTOR movedPos_;

	//�G�̔������W���W
	VECTOR makePos_;

	//�擪�̓G���玟�̓G�܂ł̑��΍��W
	VECTOR localPos_;

	// ��]
	Quaternion enemyRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �W�����v����
	bool isJump_;

	// �W�����v�̓��͎�t����
	float stepJump_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	Capsule* capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	// �ۉe
	int imgShadow_;

	//�v���C���[(�o�C�N)�Ƃ������Ă��邩�ǂ���
	bool isBikeCol_;

	//���̓G�Ɠ������Ă��邩
	bool isEnemyCol_;

	//�U�����Ă��邩
	bool isAtk_;

	//�U���p
	VECTOR fowardPos_;
	VECTOR backPos_;

	//�U����Ԃ܂ł̎���
	float toAtkStep_;

	//�X�R�A�����Z���Ă��悢��
	bool isAddScore_;

	float flipSpeed_;
	VECTOR flipDir_;

	void InitAnimation(void);

	// ��ԑJ��
	void ChangeState(STATE state);
	void ChangeStateNone(void);
	void ChangeStatePlay(void);
	void ChangeStateFliped(void);
	void ChangeStateDead(void);

	// �X�V�X�e�b�v
	void UpdateNone(void);
	void UpdateFliped(void);
	void UpdateDead(void);

	// �`��n
	void DrawShadow(void);
	void DrawHpBar(void);

	// ����
	void ProcessJump(void);

	// ��]
	void SetGoalRotate(double rotRad);
	void Rotate(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

	// ���n���[�V�����I��
	bool IsEndLanding(void);

	//�U�����[�V���������Ă���ǂ̂��炢�ōU������ɂȂ邩�Ǘ�
	bool IsAtkStart(void);


};





