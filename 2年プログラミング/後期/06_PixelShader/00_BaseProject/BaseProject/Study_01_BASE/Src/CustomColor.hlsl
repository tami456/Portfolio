
//�萔�o�b�t�@�F�X���b�g0�Ԗ�(�X���b�g��0�Ԗ�)
cbuffer cbParam : register(b3)
{
	//�O���[�o���ϐ�������ǂ��ł��g����
	float4 g_color;
}

float4 main() : SV_TARGET
{
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
	//�󂯎�����J���[��Ԃ�
	return g_color;
}