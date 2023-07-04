struct PS_INPUT
{
    // ���W( �v���W�F�N�V������� )
    float4 Position        : SV_POSITION;
    // �f�B�t���[�Y�J���[
    float4 Diffuse         : COLOR0;
    // �e�N�X�`�����W
    float2 TexCoords0      : TEXCOORD0;
};

//�萔�o�b�t�@�F�X���b�g0�Ԗ�(�X���b�g��0�Ԗ�)
cbuffer cbParam : register(b6)
{
    //�O���[�o���ϐ�������ǂ��ł��g����
    float4 g_color;
}

// �`�悷��e�N�X�`��
Texture2D g_SrcTexture : register(t0);

// �T���v���[�F�K�؂ȐF�����߂鏈��
SamplerState g_SrcSampler : register(s0);

float4 main(PS_INPUT PSInput) : SV_TARGET
{

    //UV���W���擾����
    float2 uv = PSInput.TexCoords0;

    //UV���W�ƃe�N�X�`�����Q�Ƃ��āA�œK�ȐF���擾����
    float4 srcCol =
        g_SrcTexture.Sample(g_SrcSampler, PSInput.TexCoords0);

    float4 dstCol = srcCol;

    //�@3�F�̕��ϒl���Ƃ�
    //3�F���𑫂�(1.0f+1.0f+1.0f)/3.0f�ŕ��ς�����
    //float gray = (srcCol.r + srcCol.g + srcCol.b) / 3.0f;

    //�AITU-R Rec BT.601
    //float gray = dot(dstCol.rgb, float3(0.299, 0.587, 0.114));
    //dstCol.rgb = float3(gray, gray, gray);

    //�B�V���v���Z�s�A
    //float gray = dot(dstCol.rgb, float3(0.299, 0.587, 0.114));
    //dstCol.rgb = float3(gray, gray, gray);
    //dstCol.rgb *= float3(1.07f, 0.74f, 0.43f);

    //�C�����t���Z�s�A
    float3 sepia = dstCol.rgb;
    sepia.r = dot(dstCol.rgb, float3(0.393f, 0.769f, 0.189f));
    sepia.g = dot(dstCol.rgb, float3(0.349f, 0.686f, 0.168f));
    sepia.b = dot(dstCol.rgb, float3(0.272f, 0.534f, 0.131f));
    dstCol.rgb = lerp(dstCol.rbg, sepia, 1.0f);

    return dstCol;
}