#pragma once
#include "../ActorBase.h"
#include <vector>
class Collider;
class Capsule;
class Bike;
class Score;
class GameScene;

class CoinBase : public ActorBase
{

public:

	//���a
	static constexpr float RADIUS = 200.0f;

	//1���[�v�X�e�[�W������̓G�̐�����
	static constexpr int MAX_MAKE_NUM = 3;

	//�G�̐������̃Z���^�[��������̉���
	static constexpr float DIR_LEN = 500.0f;

	//�c�̓G�ԋ���
	static constexpr float X_LEN = 400.0f;

	//�G�������̉��̒����p
	static constexpr float ADJUST_POS_X = 14300.0f;

	//���S��ԂɂȂ�܂ł̍ő厞��
	static constexpr float TO_DEAD_TIME_MAX = 6.0f;

	// �R�C���̑傫��
	static constexpr float SCL = 200.0f;

	// �R�C������Y���W
	static constexpr float INIT_POS_Y = 700.0f;

	// �R�C���������[�J��Y��]
	static constexpr float INIT_LOCAL_ROT_Y = 180.0f;

	// ���
	enum class STATE
	{
		NONE,
		PLAY,
		FLIPED,
		DEAD,
	};

	//�G�̎��
	enum class TYPE
	{
		GOLD,
		SILVER,
		COPPER,
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
	CoinBase(const std::vector<std::shared_ptr<Bike>>& bikes, GameScene* gameScene, VECTOR loopStagePos, VECTOR localPos);

	// �f�X�g���N�^
	virtual ~CoinBase(void);

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
	const std::weak_ptr<Capsule> GetCapsule(void) const;

	// �G�ɐ�����΂��ꂽ
	void Flip(VECTOR dir);

	// �v���C���[�Ɠ���������
	const bool& GetIsBikeCol(void) const;

	//�X�R�A�����Z���Ă悢���擾
	const bool& GetIsAddScore(void) const;

	//��Ԏ擾
	const STATE& GetState(void) const;

	//���S��Ԃ�
	const bool& IsDestroy(void) const;

	//���S��Ԃ�
	void Destroy(void);

	// �X�R�A���Z���\�b�h
	void AddScoreToPlayer(int playerId, int score);
protected:

	//�o�C�N���
	std::vector< std::shared_ptr<Bike>> bikes_;

	//�Q�[���V�[��
	GameScene* gameScene_;

	// ��ԊǗ�
	STATE state_;

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

	//�n�ʂƏՓ˂������ǂ���
	bool isCollGround_;

	//��������Ă���̕b��
	float stepMade_;

	// ��]
	Quaternion enemyRotY_;
	Quaternion goalQuaRot_;
	float stepRotTime_;

	// �W�����v��
	VECTOR jumpPow_;

	// �Փ˔���ɗp������R���C�_
	std::vector<std::shared_ptr<Collider>> colliders_;
	std::shared_ptr<Capsule> capsule_;

	// �Փ˃`�F�b�N
	VECTOR gravHitPosDown_;
	VECTOR gravHitPosUp_;

	//�v���C���[(�o�C�N)�Ƃ������Ă��邩�ǂ���
	bool isBikeCol_;

	//�X�R�A�����Z���Ă��悢��
	bool isAddScore_;

	float flipSpeed_;
	VECTOR flipDir_;

	// �q�b�g�G�t�F�N�g
	int effectHitResId_;
	int effectHitPlayId_;

	// �G�t�F�N�g������
	void InitEffect(void);
	// Hit�G�t�F�N�g�̈ʒu
	void HitEffect();

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

	// ��]
	void SetGoalRotate(float rotRad);
	void RotY(void);

	// �Փ˔���
	void Collision(void);
	void CollisionGravity(void);
	void CollisionCapsule(void);

	// �ړ��ʂ̌v�Z
	void CalcGravityPow(void);

};





