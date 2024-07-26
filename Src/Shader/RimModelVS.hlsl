// VS/PS����
#include "../Shader/Common/VertexToPixelHeader.hlsli"

// IN
#include "../Shader/Common/Vertex/VertexInputType.hlsli"
#define VERTEX_INPUT DX_MV1_VERTEX_TYPE_NMAP_1FRAME

// OUT
#define VS_OUTPUT VertexToPixelLit
#include "../Shader/Common/Vertex/VertexShader3DHeader.hlsli"

VS_OUTPUT main(VS_INPUT VSInput)
{
    VS_OUTPUT ret;
    // ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �J�n )
    float4 lLocalPosition;
    float4 lWorldPosition;
    float4 lViewPosition;

    // float3 �� float4
    lLocalPosition.xyz = VSInput.pos;
    lLocalPosition.w = 1.0f;

    // ���[�J�����W�����[���h���W�ɕϊ�(����)
    lWorldPosition.w = 1.0f;
    lWorldPosition.xyz = mul(lLocalPosition, g_base.localWorldMatrix);
    ret.worldAtPos = lWorldPosition.xyz;

    // ���[���h���W���r���[���W�ɕϊ�
    lViewPosition.w = 1.0f;
    lViewPosition.xyz = mul(lWorldPosition, g_base.viewMatrix);
    ret.vwPos.xyz = lViewPosition.xyz;

    // �r���[���W���ˉe���W�ɕϊ�
    ret.svPos = mul(lViewPosition, g_base.projectionMatrix);

    // ���_���W�ϊ� +++++++++++++++++++++++++++++++++++++( �I�� )

    // ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �J�n )
    // UV���W
    ret.uv.x = VSInput.uv0.x;
    ret.uv.y = VSInput.uv0.y;

    // �@��(���[�J�����W�n)
    //ret.normal = VSInput.norm;
    // �@��(���[���h���W�n)
    ret.normal = normalize(
        mul(VSInput.norm, (float3x3) g_base.localWorldMatrix));

    // �f�B�t���[�Y�J���[
    ret.diffuse = VSInput.diffuse;
    // ���C�g����(���[�J��)
    ret.lightDir = float3(0.0f, 0.0f, 0.0f);
    // ���C�g���猩�����W
    ret.lightAtPos = float3(0.0f, 0.0f, 0.0f);
    // ���̑��A�s�N�Z���V�F�[�_�ֈ��p&������ ++++++++++++( �I�� )
    // �o�̓p�����[�^��Ԃ�
    return ret;
}

