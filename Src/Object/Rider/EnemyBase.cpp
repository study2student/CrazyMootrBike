#include <string>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../Common/AnimationController.h"
#include "../Common/Capsule.h"
#include "../Common/Collider.h"
#include "../../Object/Planet.h"
#include "../../Object/Rider/Bike.h"
#include "../../Object/Score.h"
#include "EnemyBase.h"

EnemyBase::EnemyBase(const std::vector<std::shared_ptr<Bike>>& bikes, VECTOR loopStagePos, VECTOR localPos)
{
	bikes_ = bikes;

	animationController_ = nullptr;
	state_ = STATE::NONE;

	speed_ = 0.0f;
	moveDir_ = AsoUtility::VECTOR_ZERO;
	movePow_ = AsoUtility::VECTOR_ZERO;
	movedPos_ = AsoUtility::VECTOR_ZERO;

	makePos_ = loopStagePos;
	localPos_ = localPos;

	enemyRotY_ = Quaternion();
	goalQuaRot_ = Quaternion();
	stepRotTime_ = 0.0f;

	jumpPow_ = AsoUtility::VECTOR_ZERO;
	isJump_ = false;
	stepJump_ = 0.0f;

	// �Փ˃`�F�b�N
	gravHitPosDown_ = AsoUtility::VECTOR_ZERO;
	gravHitPosUp_ = AsoUtility::VECTOR_ZERO;

	isBikeCol_ = false;

	isEnemyCol_ = false;

	isAtk_ = false;
	isAddScore_=false;
	toAtkStep_ = 0.0f;

	imgShadow_ = -1;

	capsule_ = nullptr;

	isCollGround_ = false;

	stepMade_ = 0.0f;
}

EnemyBase::~EnemyBase(void)
{
	delete animationController_;
}

void EnemyBase::Init(void)
{
	
	// ���f���̊�{�ݒ�
	transform_.SetModel(resMng_.LoadModelDuplicate(
		ResourceManager::SRC::COPPER_MEDAL));
	transform_.scl = AsoUtility::VECTOR_ONE;
	//transform_.pos = { 700.0f, -800.0f, -2500.0f };
	transform_.pos = { makePos_.x + ADJUST_POS_X + localPos_.x, -800.0f, makePos_.z };
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal =
		Quaternion::Euler({ 0.0f, AsoUtility::Deg2RadF(180.0f), 0.0f });
	transform_.Update();


	// �A�j���[�V�����̐ݒ�
	InitAnimation();

	// �J�v�Z���R���C�_
	capsule_ = std::make_shared<Capsule>(transform_);
	capsule_->SetLocalPosTop({ 0.0f, 110.0f, 0.0f });
	capsule_->SetLocalPosDown({ 0.0f, 30.0f, 0.0f });
	capsule_->SetRadius(20.0f);

	// �ۉe�摜
	imgShadow_ = resMng_.Load(ResourceManager::SRC::PLAYER_SHADOW).handleId_;

	// �������
	ChangeState(STATE::PLAY);

	//�G�L�����ʂ̃p�����[�^�ݒ�
	SetParam();
}

void EnemyBase::SetParam(void)
{

}

void EnemyBase::Update(void)
{
	// �X�V�X�e�b�v
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		UpdateNone();
		break;
	case EnemyBase::STATE::PLAY:
		UpdatePlay();
		break;
	case EnemyBase::STATE::FLIPED:
		UpdateFliped();
		break;
	case EnemyBase::STATE::DEAD:
		UpdateDead();
		break;
	}


	// ���f������X�V
	transform_.Update();

	// �A�j���[�V�����Đ�
	animationController_->Update();
}

void EnemyBase::Draw(void)
{
	//���S��Ԃ͕`�悵�Ȃ�
	if (state_ == STATE::DEAD)
	{
		return;
	}

	// ���f���̕`��
	MV1DrawModel(transform_.modelId);

	// �ۉe�`��
	DrawShadow();

	////Hp�\��
	//DrawHpBar();

	DrawLine3D(fowardPos_, backPos_, 0x0000ff);

	//capsule_->Draw();
}

void EnemyBase::AddCollider(std::shared_ptr<Collider> collider)
{
	colliders_.push_back(collider);
}

void EnemyBase::ClearCollider(void)
{
	colliders_.clear();
}

const std::weak_ptr<Capsule> EnemyBase::GetCapsule(void) const
{
	return capsule_;
}

void EnemyBase::SetBikeTrans(Transform bikeTrans)
{
	Transform bikeTrans_;
	bikeTrans_ = bikeTrans;
}

void EnemyBase::SetSpeed(float speed)
{
	speed_ = speed;
}

void EnemyBase::SetIsEnemyCol(bool isEnemyCol)
{
	isEnemyCol_ = isEnemyCol;
}

void EnemyBase::Flip(VECTOR dir)
{
	flipDir_ = dir;
	flipSpeed_ = 10.0f;
	ChangeState(STATE::FLIPED);
}

bool EnemyBase::GetIsBikeCol(void)
{
	return isBikeCol_;
}

bool EnemyBase::GetIsAddScore(void)
{
	return isAddScore_;
}

EnemyBase::STATE EnemyBase::GetState(void)
{
	return state_;
}

bool EnemyBase::IsDestroy(void)
{
	return state_==STATE::DEAD;
}

void EnemyBase::Destroy(void)
{
	ChangeState(STATE::DEAD);
}

void EnemyBase::InitAnimation(void)
{
	std::string path = Application::PATH_MODEL + "Enemy/";
	animationController_ = new AnimationController(transform_.modelId);
	animationController_->Add((int)ANIM_TYPE::IDLE, path + "Idle.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::RUN, path + "Run.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::FAST_RUN, path + "FastRun.mv1", 20.0f);
	animationController_->Add((int)ANIM_TYPE::JUMP, path + "Jump.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::WARP_PAUSE, path + "WarpPose.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FLY, path + "Flying.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::FALLING, path + "Falling.mv1", 80.0f);
	animationController_->Add((int)ANIM_TYPE::VICTORY, path + "Victory.mv1", 60.0f);
	animationController_->Add((int)ANIM_TYPE::SHORT, path + "Boxing.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::LONG, path + "Sword Slash.mv1", 30.0f);
	animationController_->Add((int)ANIM_TYPE::BOMB, path + "Throw In.mv1", 30.0f);

	animationController_->Play((int)ANIM_TYPE::IDLE);
}

void EnemyBase::ChangeState(STATE state)
{
	// ��ԕύX
	state_ = state;

	// �e��ԑJ�ڂ̏�������
	switch (state_)
	{
	case EnemyBase::STATE::NONE:
		ChangeStateNone();
		break;
	case EnemyBase::STATE::PLAY:
		ChangeStatePlay();
		break;
	case EnemyBase::STATE::FLIPED:
		ChangeStateFliped();
		break;
	case EnemyBase::STATE::DEAD:
		ChangeStateDead();
		break;
	}
}

void EnemyBase::ChangeStateNone(void)
{
}

void EnemyBase::ChangeStatePlay(void)
{
}

void EnemyBase::ChangeStateFliped(void)
{
}

void EnemyBase::ChangeStateDead(void)
{
}

void EnemyBase::UpdateNone(void)
{
}

void EnemyBase::UpdateFliped(void)
{

	// �ړ�����
	ProcessMove();

	// ������΂����
	flipSpeed_ -= 0.16f;
	if (flipSpeed_ < 0.0f)
	{
		flipSpeed_ = 0.0f;
		ChangeState(STATE::PLAY);
	}
	movePow_ = VAdd(movePow_, VScale(flipDir_, flipSpeed_));

	// �W�����v����
	ProcessJump();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	isAtk_ = false;

	// ��]������
	transform_.quaRot = enemyRotY_;

}

void EnemyBase::UpdateDead(void)
{
	isAddScore_ = false;
}

void EnemyBase::UpdatePlay(void)
{
	// �ړ�����
	ProcessMove();

	// �W�����v����
	ProcessJump();

	//��]
	RotY();

	// �d�͂ɂ��ړ���
	CalcGravityPow();

	// �Փ˔���
	Collision();

	isAtk_ = false;

	// ��]������
	transform_.quaRot = enemyRotY_;
}

void EnemyBase::DrawShadow(void)
{
	float PLAYER_SHADOW_HEIGHT = 300.0f;
	float PLAYER_SHADOW_SIZE = 30.0f;

	int i;
	MV1_COLL_RESULT_POLY_DIM HitResDim;
	MV1_COLL_RESULT_POLY* HitRes;
	VERTEX3D Vertex[3] = { VERTEX3D(), VERTEX3D(), VERTEX3D() };
	VECTOR SlideVec;
	int ModelHandle;

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �e�𗎂Ƃ����f���̐������J��Ԃ�
	for (const auto c : colliders_)
	{

		// �`�F�b�N���郂�f���́Aj��0�̎��̓X�e�[�W���f���A1�ȏ�̏ꍇ�̓R���W�������f��
		ModelHandle = c->modelId_;

		// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
		HitResDim = MV1CollCheck_Capsule(
			ModelHandle, -1,
			transform_.pos, VAdd(transform_.pos, { 0.0f, -PLAYER_SHADOW_HEIGHT, 0.0f }), PLAYER_SHADOW_SIZE);

		// ���_�f�[�^�ŕω��������������Z�b�g
		Vertex[0].dif = GetColorU8(255, 255, 255, 255);
		Vertex[0].spc = GetColorU8(0, 0, 0, 0);
		Vertex[0].su = 0.0f;
		Vertex[0].sv = 0.0f;
		Vertex[1] = Vertex[0];
		Vertex[2] = Vertex[0];

		// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
		HitRes = HitResDim.Dim;
		for (i = 0; i < HitResDim.HitNum; i++, HitRes++) 
		{
			// �|���S���̍��W�͒n�ʃ|���S���̍��W
			Vertex[0].pos = HitRes->Position[0];
			Vertex[1].pos = HitRes->Position[1];
			Vertex[2].pos = HitRes->Position[2];

			// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
			SlideVec = VScale(HitRes->Normal, 0.5f);
			Vertex[0].pos = VAdd(Vertex[0].pos, SlideVec);
			Vertex[1].pos = VAdd(Vertex[1].pos, SlideVec);
			Vertex[2].pos = VAdd(Vertex[2].pos, SlideVec);

			// �|���S���̕s�����x��ݒ肷��
			Vertex[0].dif.a = 0;
			Vertex[1].dif.a = 0;
			Vertex[2].dif.a = 0;
			if (HitRes->Position[0].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[0].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[0].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[1].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[1].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[1].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			if (HitRes->Position[2].y > transform_.pos.y - PLAYER_SHADOW_HEIGHT)
				Vertex[2].dif.a = static_cast<int>(roundf(128.0f * (1.0f - fabs(HitRes->Position[2].y - transform_.pos.y) / PLAYER_SHADOW_HEIGHT)));

			// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
			Vertex[0].u = (HitRes->Position[0].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[0].v = (HitRes->Position[0].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].u = (HitRes->Position[1].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[1].v = (HitRes->Position[1].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].u = (HitRes->Position[2].x - transform_.pos.x) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;
			Vertex[2].v = (HitRes->Position[2].z - transform_.pos.z) / (PLAYER_SHADOW_SIZE * 2.0f) + 0.5f;

			// �e�|���S����`��
			DrawPolygon3D(Vertex, 1, imgShadow_, TRUE);
		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(HitResDim);
	}

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);

}

void EnemyBase::DrawHpBar(void)
{
	Transform hpTrans_;

	// �Ώە��̎p�����猩���A�������3D���W�����炷
	VECTOR relativePos = transform_.quaRot.PosAxis({ 0,200,0 });
	VECTOR pos = VAdd(transform_.pos, relativePos);
	hpTrans_.pos = ConvWorldPosToScreenPos(pos);
	VECTOR hpPos = hpTrans_.pos;

	//�J��������O��Ă�����\�����Ȃ�
	if (hpPos.z > 0.0f && hpPos.z < 1.0f)
	{
		//HP�̕\��
		// HP�̍��g
		DrawBoxAA(hpPos.x - 50, hpPos.y,
			hpPos.x + 30, hpPos.y + 10,
			0x000000, false, 5.0f);

		// HP�Q�[�W
		DrawBox(hpPos.x - 50, hpPos.y,
			hpPos.x + 30, hpPos.y + 10,
			0xff000, true);
	}

}

void EnemyBase::ProcessMove(void)
{
	auto& ins = InputManager::GetInstance();

	//// �ړ��ʂ��[��
	//movePow_ = AsoUtility::VECTOR_ZERO;

	// X����]���������A�d�͕����ɐ����ȃJ�����p�x(XZ����)���擾
	Quaternion cameraRot = SceneManager::GetInstance().GetCamera()->GetQuaRotOutX();

	// ��]�������p�x
	double rotRad = 0;

	//VECTOR dir = AsoUtility::DIR_F;
	VECTOR dir;// = AsoUtility::DIR_F;

	//// �J���������ɑO�i������
	//if (ins.IsNew(KEY_INPUT_W))
	//{
	//	rotRad = AsoUtility::Deg2RadD(0.0);
	//	dir = cameraRot.GetForward();
	//}

	//// �J�������������ނ�����
	//if (ins.IsNew(KEY_INPUT_S))
	//{
	//	rotRad = AsoUtility::Deg2RadD(180.0);
	//	dir = cameraRot.GetBack();
	//}

	//// �J������������E���ֈړ�������
	//if (ins.IsNew(KEY_INPUT_D))
	//{
	//	rotRad = AsoUtility::Deg2RadD(90.0);
	//	dir = cameraRot.GetRight();
	//}

	//// �J�����������獶���ֈړ�������
	//if (ins.IsNew(KEY_INPUT_A))
	//{
	//	rotRad = AsoUtility::Deg2RadD(270.0);
	//	dir = cameraRot.GetLeft();
	//}

	for (const auto& bike : bikes_) {
		Transform bikeTrans_ = bike->GetTransform();
	}

	//�v���C���[�ւɌ����������擾
	/*VECTOR len = VSub(bikeTrans_.pos, transform_.pos);
	dir = VNorm(len);*/


	/*if (!AsoUtility::EqualsVZero(dir) && (isJump_ || IsEndLanding())) {*/

	// �ړ�����
	//speed_ = SPEED_MOVE;
	//�Փ˔���(�G�ƃv���C���[)
	for (const auto& bike : bikes_) {
		VECTOR diff = VSub(bike_->GetCapsule().lock()->GetCenter(), capsule_->GetCenter());
		float  dis = AsoUtility::SqrMagnitudeF(diff);
		if (dis < RADIUS * RADIUS)
		{
			//�͈͂ɓ�����
			speed_ = 0;
			isAddScore_ = true;
			isBikeCol_ = true;
			if (isBikeCol_)
			{
				ChangeState(STATE::DEAD);
			}
		}
		else
		{
			if (!isEnemyCol_)
			{
				speed_ = SPEED_MOVE;
				isBikeCol_ = false;
				isAddScore_ = false;
			}
		}
	}
	/*if (ins.IsNew(KEY_INPUT_RSHIFT))
	{
		speed_ = SPEED_RUN;
	}*/

	//�����Ă����
	/*moveDir_ = dir;
	movePow_ = VScale(dir, speed_);*/

	// ��]����(�v���C���[�̕�������������)
	//VECTOR subVec = VSub(bikeTrans_.pos, transform_.pos);
	//double subDeg = atan2(subVec.x, subVec.z);
	//SetGoalRotate(subDeg);

	if (!isJump_ && IsEndLanding())
	{

		animationController_->Play((int)ANIM_TYPE::FAST_RUN);

	}

	//}
	/*else
	{
		if (!isJump_ && IsEndLanding())
		{
			animationController_->Play((int)ANIM_TYPE::IDLE);
		}
	}*/

	//�폜����
	stepMade_ += SceneManager::GetInstance().GetDeltaTime();
	if (stepMade_ >= TO_DEAD_TIME_MAX)
	{
		stepMade_ = TO_DEAD_TIME_MAX;
		ChangeState(STATE::DEAD);
	}

}

void EnemyBase::ProcessJump(void)
{
	bool isHit = CheckHitKey(KEY_INPUT_BACKSLASH);

	// �W�����v
	if (isHit && (isJump_ || IsEndLanding()))
	{

		if (!isJump_)
		{
			// ���䖳���W�����v
			//mAnimationController->Play((int)ANIM_TYPE::JUMP);
			// ���[�v���Ȃ��W�����v
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false);
			// �؂���A�j���[�V����
			//mAnimationController->Play((int)ANIM_TYPE::JUMP, false, 13.0f, 24.0f);
			// �������A�j���[�V����
			animationController_->Play((int)ANIM_TYPE::JUMP, true, 13.0f, 25.0f);
			animationController_->SetEndLoop(23.0f, 25.0f, 5.0f);
		}

		isJump_ = true;

		// �W�����v�̓��͎�t���Ԃ��w���X
		stepJump_ += scnMng_.GetDeltaTime();
		if (stepJump_ < TIME_JUMP_IN)
		{
			jumpPow_ = VScale(AsoUtility::DIR_U, POW_JUMP);
		}

	}

	// �{�^���𗣂�����W�����v�͂ɉ��Z���Ȃ�
	if (!isHit)
	{
		stepJump_ = TIME_JUMP_IN;
	}
}

void EnemyBase::SetGoalRotate(double rotRad)
{
	//VECTOR cameraRot = SceneManager::GetInstance().GetCamera()->GetAngles();
	Quaternion axis = Quaternion::AngleAxis(rotRad, AsoUtility::AXIS_Y);

	// ���ݐݒ肳��Ă����]�Ƃ̊p�x�������
	double angleDiff = Quaternion::Angle(axis, goalQuaRot_);

	// �������l
	if (angleDiff > 0.1)
	{
		stepRotTime_ = TIME_ROT;
	}

	goalQuaRot_ = axis;
}

void EnemyBase::RotY(void)
{
	//�n�ʂƏՓ˂������]
	if (isCollGround_)
	{
		//��]
		// �f�O���[���烉�W�A��(�ϊ�)
		float rad = AsoUtility::Deg2RadF(SPEED_ROT);

		// ���W�A������N�H�[�^�j�I��(�w�莲���w��p����]������)
		Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

		//�N�H�[�^�j�I��(��])�̍���
		enemyRotY_ = enemyRotY_.Mult(rotPow);
	}
}

void EnemyBase::Collision(void)
{
	// ���ݍ��W���N�_�Ɉړ�����W�����߂�
	movedPos_ = VAdd(transform_.pos, movePow_);

	//y���W�ł̎��S����
	if (transform_.pos.y <= -500.0f)
	{
		ChangeState(STATE::DEAD);
	}

	// �Փ�(�J�v�Z��)
	CollisionCapsule();

	// �Փ�(�d��)
	CollisionGravity();

	// �ړ�
	transform_.pos = movedPos_;
}

void EnemyBase::CollisionGravity(void)
{

	// �W�����v�ʂ����Z
	movedPos_ = VAdd(movedPos_, jumpPow_);

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͕����̔���
	VECTOR dirUpGravity = AsoUtility::DIR_U;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	float checkPow = 10.0f;
	gravHitPosUp_ = VAdd(movedPos_, VScale(dirUpGravity, gravityPow));
	gravHitPosUp_ = VAdd(gravHitPosUp_, VScale(dirUpGravity, checkPow * 2.0f));
	gravHitPosDown_ = VAdd(movedPos_, VScale(dirGravity, checkPow));
	for (const auto c : colliders_)
	{

		// �n�ʂƂ̏Փ�
		auto hit = MV1CollCheck_Line(
			c->modelId_, -1, gravHitPosUp_, gravHitPosDown_);

		// �ŏ��͏�̍s�̂悤�Ɏ������āA�؂̏�ɓo���Ă��܂����Ƃ��m�F����
		//if (hit.HitFlag > 0)
		if (hit.HitFlag > 0 && VDot(dirGravity, jumpPow_) > 0.9f)
		{

			// �Փ˒n�_����A������Ɉړ�
			movedPos_ = VAdd(hit.HitPosition, VScale(dirUpGravity, 2.0f));

			// �W�����v���Z�b�g
			jumpPow_ = AsoUtility::VECTOR_ZERO;
			stepJump_ = 0.0f;

			if (isJump_)
			{
				// ���n���[�V����
				animationController_->Play(
					(int)ANIM_TYPE::JUMP, false, 29.0f, 45.0f, false, true);
			}

			isJump_ = false;
			isCollGround_ = true;

		}

	}
}

void EnemyBase::CollisionCapsule(void)
{
	// �J�v�Z�����ړ�������
	Transform trans = Transform(transform_);
	trans.pos = movedPos_;
	trans.Update();
	Capsule cap = Capsule(*capsule_, trans);

	// �J�v�Z���Ƃ̏Փ˔���
	for (const auto c : colliders_)
	{

		auto hits = MV1CollCheck_Capsule(
			c->modelId_, -1,
			cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius());

		for (int i = 0; i < hits.HitNum; i++)
		{

			auto hit = hits.Dim[i];

			for (int tryCnt = 0; tryCnt < 10; tryCnt++)
			{

				int pHit = HitCheck_Capsule_Triangle(
					cap.GetPosTop(), cap.GetPosDown(), cap.GetRadius(),
					hit.Position[0], hit.Position[1], hit.Position[2]);

				if (pHit)
				{
					movedPos_ = VAdd(movedPos_, VScale(hit.Normal, 1.0f));
					// �J�v�Z�����ړ�������
					trans.pos = movedPos_;
					trans.Update();
					continue;
				}

				break;

			}

		}

		// ���o�����n�ʃ|���S�����̌�n��
		MV1CollResultPolyDimTerminate(hits);

	}



}

void EnemyBase::CalcGravityPow(void)
{
	//1�񓖂������璆�f
	if (isCollGround_)
	{
		transform_.pos.y = COLL_AFTER_POS_Y;
		return;
	}

	// �d�͕���
	VECTOR dirGravity = AsoUtility::DIR_D;

	// �d�͂̋���
	float gravityPow = Planet::DEFAULT_GRAVITY_POW;

	// �d��
	VECTOR gravity = VScale(dirGravity, gravityPow);
	jumpPow_ = VAdd(jumpPow_, gravity);

	// �ŏ��͎������Ȃ��B�n�ʂƓ˂������邱�Ƃ��m�F����B
	// ����
	float dot = VDot(dirGravity, jumpPow_);
	if (dot >= 0.0f)
	{
		// �d�͕����Ɣ��Ε���(�}�C�i�X)�łȂ���΁A�W�����v�͂𖳂���
		jumpPow_ = gravity;
	}
}

bool EnemyBase::IsEndLanding(void)
{
	bool ret = true;

	// �A�j���[�V�������W�����v�ł͂Ȃ�
	if (animationController_->GetPlayType() != (int)ANIM_TYPE::JUMP)
	{
		return ret;
	}

	// �A�j���[�V�������I�����Ă��邩
	if (animationController_->IsEnd())
	{
		return ret;
	}

	return false;
}

bool EnemyBase::IsAtkStart(void)
{


	return false;
}
