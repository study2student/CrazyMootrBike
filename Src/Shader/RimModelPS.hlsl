#include "../Shader/Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Shader/Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファ : スロット4番目
cbuffer cbParam : register(b4)
{
    float4 g_diff_color; //拡散光
    float4 g_ambient_color; //環境光
    float3 g_light_dir; //光の方向
    float3 g_camera_pos; //カメラの位置
    float4 g_specular_color; //反射光
    float g_specular_pow; //反射光の強さ

    float dummy;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 color;

// テクスチャーの色を取得
color = diffuseMapTexture.Sample(diffuseMapSampler, PSInput.uv);
if (color.a < 0.01f)
{
    discard;
}

// 法線
float3 norm = PSInput.normal;

// 拡散光の強さ
float difDot = dot(norm, -g_light_dir);

// 逆目線(頂点からのカメラ方向)
float3 toEye = normalize(g_camera_pos - PSInput.worldAtPos);

float cDot = dot(norm, toEye);

cDot = abs(cDot);

cDot = 1.0f - cDot;

// 反射ベクトル
float3 reflectionDir = normalize(g_light_dir + (norm * 2.0f));

// 反射光の強さ
float refDot = abs(dot(reflectionDir, toEye));

// 反射光を絞る
float refDotEx = pow(refDot, g_specular_pow);

// 拡散光
float rimDot = pow(cDot, 2.0f);
// 強弱を強める
float4 rimColor = float4(0.0f, 1.0f, 0.0f, 1.0f);
//rgb.diffuse = rimColor * rimDot;
//float3 diffuse = color.rgb * (difDot * g_diff_color).rgb;
float3 diffuse = rimColor * rimDot;

// 色の合成
float3 rgb = diffuse + (refDotEx * g_specular_color).rgb + g_ambient_color.rgb;

return float4(rgb, color.a);

}