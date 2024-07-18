#pragma once
#include <DxLib.h>
#include <memory>
#include "../Common/Quaternion.h"
class Transform;
class GameScene;
class Bike;

class Camera
{

public:

	// �J�����X�s�[�h(�x)
	static constexpr float SPEED = 1.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_NEAR = 10.0f;

	// �J�����N���b�v�FNEAR
	static constexpr float CAMERA_FAR = 30000.0f;

	// �J�����̏������W
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// �Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 300.0f, -400.0f };
	//static constexpr VECTOR LOCAL_F2C_POS = { 0.0f, 300.0f, -900.0f };

	// �Ǐ]�ʒu����J�����ʒu�܂ł̑��΍��W(�u�[�X�g)
	static constexpr VECTOR BOOST_LOCAL_F2C_MAX_POS = { 0.0f, 300.0f, -1000.0f };

	// �Ǐ]�ʒu���璍���_�܂ł̑��΍��W
	static constexpr VECTOR LOCAL_F2T_POS = { 0.0f, 0.0f, 500.0f };

	// �Ǐ]�Ώۂ���J�����ʒu�܂ł̑��΍��W(�u�[�X�g�J�n��)
	static constexpr VECTOR RELATIVE_F2C_POS_BOOST_START = { 0.0f, 40.0f, 150.0f };

	// �u�[�X�g�g�p���̃J�������x����
	static constexpr float ADD_CAMERA_SPEED = 0.32f;

	// �u�[�X�g�g�p���̃J�������x����l
	static constexpr float CAMERA_SPEED_MAX = 3.0f;

	// ��]�����܂ł̎���
	static constexpr float TIME_ROT = 0.6f;

	// �J������X��]����x�p
	static constexpr float LIMIT_X_UP_RAD = 40.0f * (DX_PI_F / 180.0f);
	static constexpr float LIMIT_X_DW_RAD = 15.0f * (DX_PI_F / 180.0f);

	// �J�������[�h
	enum class MODE
	{
		NONE,
		FIXED_POINT,
		FOLLOW,
		SELF_SHOT
	};

	enum class JoypadButton {
		UP = PAD_INPUT_UP,
		DOWN = PAD_INPUT_DOWN,
		LEFT = PAD_INPUT_LEFT,
		RIGHT = PAD_INPUT_RIGHT,
		ACTION = PAD_INPUT_6
	};

	// �v���C���[���Ƃ̓��̓}�b�s���O
	struct PlayerInput {
		int padId;
		JoypadButton up;
		JoypadButton down;
		JoypadButton left;
		JoypadButton right;
		JoypadButton action;
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void Draw(void);

	// �J�����ʒu
	VECTOR GetPos(void) const;
	// �J�����̑���p�x
	VECTOR GetAngles(void) const;
	// �J�����̒����_
	VECTOR GetTargetPos(void) const;

	// �J�����p�x
	Quaternion GetQuaRot(void) const;
	// X��]�𔲂����J�����p�x
	Quaternion GetQuaRotOutX(void) const;
	// �J�����̑O������
	VECTOR GetForward(void) const;

	// �J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	// �Ǐ]�Ώۂ̐ݒ�
	void SetFollow(const Transform* follow);

	//�|�[�Y�����ǂ����ݒ�
	void SetIsPause(bool isPause);

	//�J�����𐳖ʂɖ߂����ǂ����ݒ�(�X�e�[�W�O�ɂł��ꍇ)
	void SetIsCameraReset(bool isCameraReset);

	//�u�[�X�g�����ǂ����ݒ�
	void SetIsBoost(bool isBoost);


private:
	// �o�C�N
	std::vector<std::shared_ptr<Bike>> bikes_;

	// �J�������Ǐ]�ΏۂƂ���Transform
	const Transform* followTransform_;

	//�|�[�Y�����ǂ���
	bool isPause_;

	//�J�����𐳖ʂɖ߂����ǂ���(�X�e�[�W�O�ɂł��ꍇ)
	bool isCameraReset_;

	//�u�[�X�g����
	bool isBoost_;

	//�u�[�X�g��O�ɍs�����Ƃ��̈ʒu
	VECTOR boostLocalPos_;

	//��O�ɂ���܂ł̌o�ߎ���
	float stepMoveFront_;

	// �J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x(rad)
	VECTOR angles_;

	// X����]�������p�x
	Quaternion rotOutX_;

	// �J�����p�x
	Quaternion rot_;

	//�J�������x
	float velocity_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	float stepRotTime_;

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �Ǐ]�ΏۂƂ̈ʒu���������
	void SyncFollow(void);

	// �J��������
	void ProcessRot(void);

	// ���[�h�ʍX�V�X�e�b�v
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawSelfShot(void);


};

