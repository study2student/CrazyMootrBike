
struct VertexToPixel
{
    float4 svPos		: SV_POSITION;
    float4 diffuse		: COLOR0;
    float4 specular		: COLOR1;
    float2 uv			: TEXCOORD;
};

struct VertexToPixelLit
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float3 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
	float3 normal       : TEXCOORD2;	// 法線( ビュー座標 )
	float4 diffuse      : COLOR0;		// ディフューズカラー
	float3 lightDir     : TEXCOORD3;	// ライト方向(ローカル)
	float3 lightAtPos   : TEXCOORD4;	// ライトから見た座標
	float3 worldAtPos   : TEXCOORD5;	// 頂点座標(ワールド座標)
};

struct VertexToPixelShadow
{
	float4 svPos		: SV_POSITION;	// 座標( プロジェクション空間 )
	float2 uv			: TEXCOORD0;	// テクスチャ座標
	float4 vwPos		: TEXCOORD1;	// 座標( ビュー座標 )
};

struct VertexToPixelLitFog
{
    float4 svPos         : SV_POSITION;  // 座標( プロジェクション空間 )
    float2 uv            : TEXCOORD0;    // テクスチャ座標
    float3 vwPos         : TEXCOORD1;    // 座標( ビュー座標 )
    float3 normal        : TEXCOORD2;    // 法線( ビュー座標 )
    float4 diffuse       : COLOR0;       // ディフューズカラー
    float3 lightDir      : TEXCOORD3;    // ライト方向(ローカル)
    float3 lightAtPos    : TEXCOORD4;    // ライトから見た座標
    float3 worldAtPos    : TEXCOORD5;    // 頂点座標(ワールド座標)
    float fogFactor      : TEXCOORD6;    // フォグの強さ(0.0:フォグが濃い, 1.0フォグが)
    float lightPower     : TEXCOORD7;    // ポイントライトの強さ
};

struct VertexToPixelLitOut
{
    float4 svPos         : SV_POSITION;     // 座標( プロジェクション空間 )
    float2 uv            : TEXCOORD0;       // テクスチャ座標
    float3 vwPos         : TEXCOORD1;       // 座標( ビュー座標 )
    float3 normal        : TEXCOORD2;       // 法線( ビュー座標 )
    float4 diffuse       : COLOR0;          // ディフューズカラー
    float3 lightDir      : TEXCOORD3;       // ライト方向(ローカル)
    float3 lightAtPos    : TEXCOORD4;       // ライトから見た座標
    float3 worldAtPos    : TEXCOORD5;       // 頂点座標(ワールド座標)
    float fogFactor      : TEXCOORD6;       // フォグの強さ(0.0:フォグが濃い, 1.0フォグが)
    float lightPower     : TEXCOORD7;       // ポイントライトの強さ
    float4 Pos           : SV_POSITION1;    // 何らかのポジション
    float4 OutlinePos    : POSITION1;       // 座標
};