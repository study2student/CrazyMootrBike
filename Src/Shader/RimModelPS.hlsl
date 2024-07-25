#include "../Shader/Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Shader/Common/Pixel/PixelShader3DHeader.hlsli"

// �萔�o�b�t�@ : �X���b�g4�Ԗ�
cbuffer cbParam : register(b4)
{
    float4 g_diff_color; //�g�U��
    float4 g_ambient_color; //����
    float3 g_light_dir; //���̕���
    float3 g_camera_pos; //�J�����̈ʒu
    float4 g_specular_color; //���ˌ�
    float g_specular_pow; //���ˌ��̋���

    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 color;

// �e�N�X�`���[�̐F���擾
color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
if (color.a < 0.01f)
{
    discard;
}

// �@��
float3 norm = PSInput.normal;

// �g�U���̋���
float difDot = dot(norm, -g_light_dir);

// �t�ڐ�(���_����̃J��������)
float3 toEye = normalize(g_camera_pos - PSInput.worldAtPos);

float cDot = dot(norm, toEye);

cDot = abs(cDot);

cDot = 1.0f - cDot;

// ���˃x�N�g��
float3 reflectionDir = normalize(g_light_dir + (norm * 2.0f));

// ���ˌ��̋���
float refDot = abs(dot(reflectionDir, toEye));

// ���ˌ����i��
float refDotEx = pow(refDot, g_specular_pow);

// �g�U��
float rimDot = pow(cDot, 2.0f);
// ��������߂�
float4 rimColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
//rgb.diffuse = rimColor * rimDot;
//float3 diffuse = color.rgb * (difDot * g_diff_color).rgb;
float3 diffuse = rimColor * rimDot;

// �F�̍���
float3 rgb = diffuse + (refDotEx * g_specular_color).rgb + g_ambient_color.rgb;

return float4(rgb, color.a);

}