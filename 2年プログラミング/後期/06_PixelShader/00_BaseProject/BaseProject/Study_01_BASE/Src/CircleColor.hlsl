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
cbuffer cbParam : register(b3)
{
    //�O���[�o���ϐ�������ǂ��ł��g����
    float4 g_color;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{

    //UV���W���擾����
    float2 uv = PSInput.TexCoords0;
    //(a)�����E�@
    //return float4(uv.x, uv.x, uv.x, 1.0f);

    //(b)�と��
    //return float4(1.0f - uv.y, 1.0f - uv.y, 1.0f - uv.y, 1.0f);

    //(c)���と�E���@
    float4 color = float4(uv.x + uv.y, uv.x + uv.y, uv.x + uv.y, 1.0f);
    //uv.x+uv.y��2�ɂȂ��Ă��܂�����1�ɖ߂�����
    /*color.x /= 2.0f;
    color.y /= 2.0f;
    color.z /= 2.0f;*/
    //�V�F�[�_�[���L�̌v�Z���@
    color.xyz /= 2.0f;
    return color;
}