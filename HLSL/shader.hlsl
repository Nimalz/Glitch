cbuffer WVPBuffer : register(b0)
{
    matrix WorldViewProjection;
}

//   ランダム用
cbuffer RandomBuffer : register(b1)
{
    float minValue;
    float maxValue;
    float time;
    float dummy2;

}

//グリッチ用
cbuffer GlitchBuffer : register(b2)
{
    //================
	// RGBカラーシフト
	//================
	// 振動の強さ
    float shake_power;
	// 振動確率
    float shake_rate;
	// 振動速度
    float shake_speed;
	// 振動ブロックサイズ (分割数)
    float shake_block_size;
	// 色の分離率
    float shake_color_rate;
	
    // UVシフトするか
    float shake_enable;
    
    // RGBVシフトするか
    float rgb_shift;
	
    //================
	// ノイズ効果
	//================
    float grain_enable;

    float grain_blockSize;
    
    float grain_alpha;
    //================
	// 走査線
	//================
    float scanline_enbale;
	
    //不透明度
    float scanline_opacity;

    //ノイズ
    float scanline_noise;
    
    //================
	// 点滅効果
	//================
    float flick_enable;
	
    //発生速度
    float flick_speed;

    //発生強度
    float flick_str;
    
}




//乱数生成
float random(float2 seeds)
{
    return frac(sin(dot(seeds, float2(12.9898, 78.233))) * 43758.5453);
}

float2 random2(float2 seeds)
{
    seeds = float2(dot(seeds, float2(127.1, 311.7)),
 dot(seeds, float2(269.5, 183.3)));
    return frac(sin(seeds) * 43758.5453123);
}

float3 random3(float3 seeds)
{
    seeds = float3(dot(seeds, float3(127.1, 311.7, 542.3)),
 dot(seeds, float3(269.5, 183.3, 461.7)),
 dot(seeds, float3(732.1, 845.3, 231.7)));
    return frac(sin(seeds) * 43758.5453123);
}

float blockNoise(float2 seeds)
{
    return random(floor(seeds));
}

float noiserandom(float2 seeds)
{
    return -1.0 + 2.0 * blockNoise(seeds);
}

void VertexShaderPolygon(in float4 inPosition : POSITION0, // 引数の意味を指定　座標
						  in float2 inTexCoord : TEXCOORD0, // テクスチャー座標UV

						  out float4 outPosition : SV_POSITION, // 出力座標
						  out float2 outTexCoord : TEXCOORD0 // 出力テクスチャー座標
						  )
{
	// 頂点の座標変換
    outPosition = mul(inPosition, WorldViewProjection);

	// 法線とUVはそのままコピーする
    outTexCoord = inTexCoord;


}

//*******************************************************
// グローバル変数
//******************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void PixelShaderPolygon(in float4 inPosition : POSITION0,
						 in float2 inTexCoord : TEXCOORD0,


						 out float4 outDiffuse : SV_Target)// 最終的なピクセルの色
{
	//================
	// UV+RGBシフト
	//================
    const float uShiftPara = 0.15; //UVシフト用乱数　U座標影響力パラメーター
    const float uvShiftPeriodPara1 = 0.2; //UVシフト用乱数　周期調整パラメーター1
    const float uvShiftPeriodPara2 = 0.5; //UVシフト用乱数　周期調整パラメーター2
    const float RshiftMag = 0.8; //赤シフト強度　調整パラメーター
    const float BshiftMag = 0.6; //青シフト強度　調整パラメーター
    
    float enable_shift;
    float4 color;
    
    enable_shift = (random(trunc(time * shake_speed)) < shake_rate) * shake_enable;
    float2 gv = inTexCoord;
    float noise = blockNoise(inTexCoord.y * shake_block_size);
    noise += random(inTexCoord.x) * uShiftPara;
	
    float2 randomvalue = noiserandom(float2(inTexCoord.y, time * shake_speed));
	//uvシフト
    gv.x += enable_shift * randomvalue * sin(sin(shake_power) * uvShiftPeriodPara2) * sin(-sin(noise) * uvShiftPeriodPara1) * frac(time);
	//colorシフト
    color.r = g_Texture.Sample(g_SamplerState, gv + rgb_shift * enable_shift * float2(shake_color_rate * RshiftMag, 0)).r;
    color.g = g_Texture.Sample(g_SamplerState, gv).g;
    color.b = g_Texture.Sample(g_SamplerState, gv - rgb_shift * enable_shift * float2(shake_color_rate * BshiftMag, 0)).b;
    color.a = 1.0;
  
	//================
	// ノイズ効果
	//================
    const float RanPara = 666; //乱数調整用パラメーター
    
    float3 gs = random(blockNoise(inTexCoord * grain_blockSize) * time + RanPara) * grain_enable;
    float4 color2 = float4(1.0, 1.0, 1.0, 1.0);
    color2 = float4(gs * grain_alpha, 1.0);
    color = lerp(color, color2, grain_alpha);
    
	//================
	// 走査線
	//================
    
    float3 col = float3(0, 0, 0);
    float c = (cos((inTexCoord.y) * scanline_noise - time)) * scanline_enbale;
    c *= (cos(((inTexCoord.y) * scanline_noise - time) * scanline_noise));

    col += float3(c, c, c);

    color = lerp(color, float4(col, 1.0), (1 - scanline_opacity) * scanline_enbale);
    
    //================
	// 点滅効果
	//================
    const float periodPara1 = 0.004; //周期調整用パラメーター1
    const float periodPara2 = 0.008; //周期調整用パラメーター2
    const float periodPara3 = 0.04; //周期調整用パラメーター3
    const float RandPara = 0.2; //乱数用パラメーター
    
    float fac;
    float flash = -(sin(flick_speed * time * periodPara1) + flick_str) * cos(flick_speed * time * periodPara2) * sin(flick_speed * time * periodPara3) - RandPara;
    fac = 1 - flash * flick_enable;
    fac = min(1.0, fac);

    color *= float4(fac, fac, fac, 1.0);

    outDiffuse = color;
}
