#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <DxLib.h>
#include "MyUtility.h"

int MyUtility::Round(float v)
{
    return static_cast<int>(roundf(v));
}

std::vector<std::string> MyUtility::Split(std::string& line, char delimiter)
{

    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> result;
    
    while (getline(stream, field, delimiter)) {
        result.push_back(field);
    }

    return result;

}

double MyUtility::Rad2DegD(double rad)
{
    return rad * (180.0 / DX_PI);
}

float MyUtility::Rad2DegF(float rad)
{
    return rad * (180.0f / DX_PI_F);
}

int MyUtility::Rad2DegI(int rad)
{
    return rad * Round(180.0f / DX_PI_F);
}

double MyUtility::Deg2RadD(double deg)
{
    return deg * (DX_PI / 180.0);
}

float MyUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}

int MyUtility::Deg2RadI(int deg)
{
    return deg * Round(DX_PI_F / 180.0f);
}

double MyUtility::DegIn360(double deg)
{
    deg = fmod(deg, 360.0);
    if (deg < 0.0f)
    {
        deg += 360.0;
    }
    return deg;
}

double MyUtility::RadIn2PI(double rad)
{
    rad = fmod(rad, DX_TWO_PI);
    if (rad < 0.0)
    {
        rad += DX_TWO_PI;
    }
    return rad;
}

int MyUtility::DirNearAroundRad(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -DX_PI_F)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int MyUtility::DirNearAroundDeg(float from, float to)
{

    float ret = 1.0f;

    float diff = to - from;

    if (diff >= 0.0f)
    {

        // ��r���������v���Ɉʒu����

        if (diff > 180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA�����v���̕����߂�
            ret = -1.0f;
        }
        else
        {
            // ���v���
            ret = 1.0f;
        }

    }
    else
    {

        // ��r�����������v���Ɉʒu����

        if (diff < -180.0f)
        {
            // �ł��A180�x�ȏ㗣��Ă���̂ŁA���v���̕����߂�
            ret = 1.0f;
        }
        else
        {
            // �����v���
            ret = -1.0f;
        }

    }

    return static_cast<int>(ret);

}

int MyUtility::Lerp(int start, int end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    int ret = start;
    ret += Round(t * static_cast<float>(end - start));
    return ret;
}

float MyUtility::Lerp(float start, float end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    float ret = start;
    ret += t * (end - start);
    return ret;
}

double MyUtility::Lerp(double start, double end, double t)
{
    // ���`���
    if (t >= 1.0)
    {
        return end;
    }

    double ret = start;
    ret += t * (end - start);
    return ret;
}

Vector2 MyUtility::Lerp(const Vector2& start, const Vector2& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    Vector2 ret = start;
    ret.x += Round(t * static_cast<float>((end.x - start.x)));
    ret.y += Round(t * static_cast<float>((end.y - start.y)));
    return ret;
}

VECTOR MyUtility::Lerp(const VECTOR& start, const VECTOR& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    VECTOR ret = start;
    ret.x += t * (end.x - start.x);
    ret.y += t * (end.y - start.y);
    ret.z += t * (end.z - start.z);

    return ret;
}

double MyUtility::LerpDeg(double start, double end, double t)
{

    double ret;

    double diff = end - start;
    if (diff < -180.0)
    {
        end += 360.0;
        ret = Lerp(start, end, t);
        if (ret >= 360.0)
        {
            ret -= 360.0;
        }
    }
    else if (diff > 180.0)
    {
        end -= 360.0;
        ret = Lerp(start, end, t);
        if (ret < 0.0)
        {
            ret += 360.0;
        }
    }
    else
    {
        ret = Lerp(start, end, t);
    }

    return ret;

}

COLOR_F MyUtility::Lerp(const COLOR_F& start, const COLOR_F& end, float t)
{
    // ���`���
    if (t >= 1.0f)
    {
        return end;
    }

    COLOR_F ret = start;
    ret.r += t * (end.r - start.r);
    ret.g += t * (end.g - start.g);
    ret.b += t * (end.b - start.b);
    ret.a += t * (end.a - start.a);
    return ret;
}

Vector2 MyUtility::Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t)
{
    Vector2 a = Lerp(p1, p2, t);
    Vector2 b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR MyUtility::Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t)
{
    VECTOR a = Lerp(p1, p2, t);
    VECTOR b = Lerp(p2, p3, t);
    return Lerp(a, b, t);
}

VECTOR MyUtility::RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad)
{
    float x = ((radiusPos.x - centerPos.x) * cosf(rad)) - ((radiusPos.z - centerPos.z) * sinf(rad));
    float z = ((radiusPos.x - centerPos.x) * sinf(rad)) + ((radiusPos.z - centerPos.z) * cosf(rad));
    return VGet(centerPos.x + x, radiusPos.y, centerPos.z + z);
}

double MyUtility::Magnitude(const Vector2& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y));
}

double MyUtility::Magnitude(const VECTOR& v)
{
    return sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

float MyUtility::MagnitudeF(const VECTOR& v)
{
    return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
}

int MyUtility::SqrMagnitude(const Vector2& v)
{
    return v.x * v.x + v.y * v.y;
}

float MyUtility::SqrMagnitudeF(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double MyUtility::SqrMagnitude(const VECTOR& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

double MyUtility::SqrMagnitude(const VECTOR& v1, const VECTOR& v2)
{
    return pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2);
}

double MyUtility::Distance(const Vector2& v1, const Vector2& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2));
}

double MyUtility::Distance(const VECTOR& v1, const VECTOR& v2)
{
    return sqrt(pow(v2.x - v1.x, 2) + pow(v2.y - v1.y, 2) + pow(v2.z - v1.z, 2));
}

bool MyUtility::IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2)
{
    // ���̓��m�̏Փ˔���
    bool ret = false;

    // ���݂��̔��a�̍��v
    float radius = radius1 + radius2;

    // ���W�̍����炨�݂��̋��������
    VECTOR diff = VSub(pos2, pos1);

    // �O�����̒藝�Ŕ�r(SqrMagnitude�Ɠ���)
    float dis = (diff.x * diff.x) + (diff.y * diff.y) + (diff.z * diff.z);
    if (dis < (radius * radius))
    {
        ret = true;
    }

    return ret;
}

bool MyUtility::IsHitSphereCapsule(
    const VECTOR& sphPos, float sphRadius, 
    const VECTOR& capPos1, const VECTOR& capPos2, float capRadius)
{

    bool ret = false;

    // �J�v�Z�����̂̒��S���q���x�N�g��
    VECTOR cap1to2 = VSub(capPos2, capPos1);

    // �x�N�g���𐳋K��
    VECTOR cap1to2ENor = VNorm(cap1to2);

    // �J�v�Z���q���̒P�ʃx�N�g���ƁA
    // ���̃x�N�g�������狅�̂ւ̃x�N�g���̓��ς����
    float dot = VDot(cap1to2ENor, VSub(sphPos, capPos1));

    // ���ςŋ��߂��ˉe�������g���āA�J�v�Z���q����̍��W�����
    VECTOR capRidePos = VAdd(capPos1, VScale(cap1to2ENor, dot));

    // �J�v�Z���q���̃x�N�g���̒��������
    float len = MyUtility::MagnitudeF(cap1to2);

    // ���̂��J�v�Z���q����ɂ��邩���ʂ��邽�߁A�䗦�����
    float rate = dot / len;

    VECTOR centerPos;

    // ���̂̈ʒu���R�G���A�ɕ������ꂽ�J�v�Z���`��̂ǂ��ɂ��邩����
    if (rate > 0.0f && rate <= 1.0f)
    {
        // �@���̂��J�v�Z���q����ɂ���
        centerPos = VAdd(capPos1, VScale(cap1to2ENor, dot));
    }
    else if (rate > 1.0f)
    {
        // �A���̂��J�v�Z���̏I�_���ɂ���
        centerPos = capPos2;
    }
    else if (rate < 0.0f)
    {
        // �B���̂��J�v�Z���̎n�_���ɂ���
        centerPos = capPos1;
    }
    else
    {
        // �����ɂ��Ă͂����Ȃ�
    }

    // ���̓��m�̓����蔻��
    if (MyUtility::IsHitSpheres(centerPos, capRadius, sphPos, sphRadius))
    {
        ret = true;
    }
    else
    {
        ret = false;
    }

    return ret;

}

bool MyUtility::Equals(const VECTOR& v1, const VECTOR& v2)
{
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

bool MyUtility::EqualsVZero(const VECTOR& v1)
{
    const VECTOR& v2 = VECTOR_ZERO;
    if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
    {
        return true;
    }
    return false;
}

VECTOR MyUtility::Normalize(const Vector2& v)
{
    VECTOR ret = VGet(
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        0.0f
    );
    float len = static_cast<float>(Magnitude(v));
    ret.x /= len;
    ret.y /= len;
    ret.z /= len;
    return ret;
}

VECTOR MyUtility::VNormalize(const VECTOR& v)
{
    if (MyUtility::EqualsVZero(v))
    {
        // Quaternion�v�Z�Ń[����n���āA
        // �G���[(-1, -1, -1)���Ԃ��Ă���ƍ���
        return v;
    }
    return VNorm(v);
}

double MyUtility::AngleDeg(const VECTOR& from, const VECTOR& to)
{
    // sqrt(a) * sqrt(b) = sqrt(a * b) -- valid for real numbers
    auto fLen = SqrMagnitude(from);
    auto tLen = SqrMagnitude(to);
    auto denominator = sqrt(fLen * tLen);
    if (denominator < kEpsilonNormalSqrt)
    {
        return 0.0f;
    }

    //float dot = std::clamp(Dot(from, to) / denominator, -1.0f, 1.0f);
    //auto dot = Dot(from, to) / denominator;
    auto dot = VDot(from, to) / denominator;
    if (dot < -1.0f)
    {
        dot = -1.0f;
    }
    if (dot > 1.0f)
    {
        dot = 1.0f;
    }

    return acos(dot) * (180.0 / DX_PI);
}

void MyUtility::DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len)
{
    auto nDir = MyUtility::VNormalize(dir);
    auto sPos = VAdd(pos, VScale(nDir, -len));
    auto ePos = VAdd(pos, VScale(nDir, len));
    DrawLine3D(sPos, ePos, color);
    DrawSphere3D(ePos, 5.0f, 5, color, color, true);
}

void MyUtility::DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len)
{

    VECTOR dir;

    // X
    dir = VTransform(MyUtility::DIR_R, rot);
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = VTransform(MyUtility::DIR_U, rot);
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = VTransform(MyUtility::DIR_F, rot);
    DrawLineDir(pos, dir, 0x0000ff, len);

}

void MyUtility::DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len)
{

    VECTOR dir;

    // X
    dir = rot.GetRight();
    DrawLineDir(pos, dir, 0xff0000, len);

    // Y
    dir = rot.GetUp();
    DrawLineDir(pos, dir, 0x00ff00, len);

    // Z
    dir = rot.GetForward();
    DrawLineDir(pos, dir, 0x0000ff, len);

}


