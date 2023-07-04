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
cbuffer cbParam : register(b5)
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


    return srcCol;
}