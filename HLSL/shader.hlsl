cbuffer WVPBuffer : register(b0)
{
    matrix WorldViewProjection;
}

//   �����_���p
cbuffer RandomBuffer : register(b1)
{
    float minValue;
    float maxValue;
    float time;
    float dummy2;

}

//�O���b�`�p
cbuffer GlitchBuffer : register(b2)
{
    //================
	// RGB�J���[�V�t�g
	//================
	// �U���̋���
    float shake_power;
	// �U���m��
    float shake_rate;
	// �U�����x
    float shake_speed;
	// �U���u���b�N�T�C�Y (������)
    float shake_block_size;
	// �F�̕�����
    float shake_color_rate;
	
    // UV�V�t�g���邩
    float shake_enable;
    
    // RGBV�V�t�g���邩
    float rgb_shift;
	
    //================
	// �m�C�Y����
	//================
    float grain_enable;

    float grain_blockSize;
    
    float grain_alpha;
    //================
	// ������
	//================
    float scanline_enbale;
	
    //�s�����x
    float scanline_opacity;

    //�m�C�Y
    float scanline_noise;
    
    //================
	// �_�Ō���
	//================
    float flick_enable;
	
    //�������x
    float flick_speed;

    //�������x
    float flick_str;
    
}




//��������
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

void VertexShaderPolygon(in float4 inPosition : POSITION0, // �����̈Ӗ����w��@���W
						  in float2 inTexCoord : TEXCOORD0, // �e�N�X�`���[���WUV

						  out float4 outPosition : SV_POSITION, // �o�͍��W
						  out float2 outTexCoord : TEXCOORD0 // �o�̓e�N�X�`���[���W
						  )
{
	// ���_�̍��W�ϊ�
    outPosition = mul(inPosition, WorldViewProjection);

	// �@����UV�͂��̂܂܃R�s�[����
    outTexCoord = inTexCoord;


}

//*******************************************************
// �O���[�o���ϐ�
//******************************************************
Texture2D g_Texture : register(t0);
SamplerState g_SamplerState : register(s0);

void PixelShaderPolygon(in float4 inPosition : POSITION0,
						 in float2 inTexCoord : TEXCOORD0,


						 out float4 outDiffuse : SV_Target)// �ŏI�I�ȃs�N�Z���̐F
{
	//================
	// UV+RGB�V�t�g
	//================
    const float uShiftPara = 0.15; //UV�V�t�g�p�����@U���W�e���̓p�����[�^�[
    const float uvShiftPeriodPara1 = 0.2; //UV�V�t�g�p�����@���������p�����[�^�[1
    const float uvShiftPeriodPara2 = 0.5; //UV�V�t�g�p�����@���������p�����[�^�[2
    const float RshiftMag = 0.8; //�ԃV�t�g���x�@�����p�����[�^�[
    const float BshiftMag = 0.6; //�V�t�g���x�@�����p�����[�^�[
    
    float enable_shift;
    float4 color;
    
    enable_shift = (random(trunc(time * shake_speed)) < shake_rate) * shake_enable;
    float2 gv = inTexCoord;
    float noise = blockNoise(inTexCoord.y * shake_block_size);
    noise += random(inTexCoord.x) * uShiftPara;
	
    float2 randomvalue = noiserandom(float2(inTexCoord.y, time * shake_speed));
	//uv�V�t�g
    gv.x += enable_shift * randomvalue * sin(sin(shake_power) * uvShiftPeriodPara2) * sin(-sin(noise) * uvShiftPeriodPara1) * frac(time);
	//color�V�t�g
    color.r = g_Texture.Sample(g_SamplerState, gv + rgb_shift * enable_shift * float2(shake_color_rate * RshiftMag, 0)).r;
    color.g = g_Texture.Sample(g_SamplerState, gv).g;
    color.b = g_Texture.Sample(g_SamplerState, gv - rgb_shift * enable_shift * float2(shake_color_rate * BshiftMag, 0)).b;
    color.a = 1.0;
  
	//================
	// �m�C�Y����
	//================
    const float RanPara = 666; //���������p�p�����[�^�[
    
    float3 gs = random(blockNoise(inTexCoord * grain_blockSize) * time + RanPara) * grain_enable;
    float4 color2 = float4(1.0, 1.0, 1.0, 1.0);
    color2 = float4(gs * grain_alpha, 1.0);
    color = lerp(color, color2, grain_alpha);
    
	//================
	// ������
	//================
    
    float3 col = float3(0, 0, 0);
    float c = (cos((inTexCoord.y) * scanline_noise - time)) * scanline_enbale;
    c *= (cos(((inTexCoord.y) * scanline_noise - time) * scanline_noise));

    col += float3(c, c, c);

    color = lerp(color, float4(col, 1.0), (1 - scanline_opacity) * scanline_enbale);
    
    //================
	// �_�Ō���
	//================
    const float periodPara1 = 0.004; //���������p�p�����[�^�[1
    const float periodPara2 = 0.008; //���������p�p�����[�^�[2
    const float periodPara3 = 0.04; //���������p�p�����[�^�[3
    const float RandPara = 0.2; //�����p�p�����[�^�[
    
    float fac;
    float flash = -(sin(flick_speed * time * periodPara1) + flick_str) * cos(flick_speed * time * periodPara2) * sin(flick_speed * time * periodPara3) - RandPara;
    fac = 1 - flash * flick_enable;
    fac = min(1.0, fac);

    color *= float4(fac, fac, fac, 1.0);

    outDiffuse = color;
}
