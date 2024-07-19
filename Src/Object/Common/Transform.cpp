#include <DxLib.h>
#include "../../Utility/MyUtility.h"
#include "Transform.h"

Transform::Transform(void)
{
	modelId = -1;

	scl = MyUtility::VECTOR_ONE;
	rot = MyUtility::VECTOR_ZERO;
	pos = MyUtility::VECTOR_ZERO;
	localPos = MyUtility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::Transform(int model)
{
	modelId = model;

	scl = MyUtility::VECTOR_ONE;
	rot = MyUtility::VECTOR_ZERO;
	pos = MyUtility::VECTOR_ZERO;
	localPos = MyUtility::VECTOR_ZERO;

	matScl = MGetIdent();
	matRot = MGetIdent();
	matPos = MGetIdent();
	quaRot = Quaternion();
	quaRotLocal = Quaternion();

	collider = nullptr;
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// 大きさ
	matScl = MGetScale(scl);

	// 回転
	rot = quaRot.ToEuler();
	matRot = quaRot.ToMatrix();

	// 位置
	matPos = MGetTranslate(pos);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl);
	Quaternion q = quaRot.Mult(quaRotLocal);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos);

	// 行列をモデルに判定
	if (modelId != -1)
	{
		MV1SetMatrix(modelId, mat);
	}

	// 衝突判定の更新
	if (collider != nullptr)
	{
		MV1RefreshCollInfo(modelId);
	}

}

void Transform::Release(void)
{
}

void Transform::SetModel(int model)
{
	modelId = model;
}

void Transform::MakeCollider(Collider::TYPE type)
{

	if (modelId == -1)
	{
		return;
	}

	collider = std::make_shared<Collider>(type, modelId);
	int ret = MV1SetupCollInfo(modelId, -1, 1, 1, 1);

}

VECTOR Transform::GetForward(void) const
{
	return GetDir(MyUtility::DIR_F);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(MyUtility::DIR_B);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(MyUtility::DIR_R);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(MyUtility::DIR_L);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(MyUtility::DIR_U);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(MyUtility::DIR_D);
}

VECTOR Transform::GetDir(const VECTOR& vec) const
{
	return quaRot.PosAxis(vec);
}
