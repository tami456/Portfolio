#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x, int y);
	~Vector2D();
	int x;
	int y;

	//������Z�q
	Vector2D& operator=(const Vector2D& vec);

	//�Y�������Z�q
	//int�ŕԂ�	[]�̒��g��j
	int& operator[](int j);

	//�P�����Z�q
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
	Vector2D& operator%=(const Vector2D& vec);
	Vector2D operator+()const;
	Vector2D operator-()const;

	//�񍀉��Z�q A+B�Ȃ� �Q�Ƃł͂Ȃ��R�s�[�ŕԂ�
	//+�̑O���������g
	//�������g�ɕ�����t�^���ĕԂ�
	Vector2D operator+(const Vector2D& vec);
	Vector2D operator-(const Vector2D& vec);
	Vector2D operator*(const Vector2D& vec);
	//Vector2D operator/(const Vector2D& vec);
	Vector2D operator%(const Vector2D& vec);

	//��r���Z�q
	bool operator==(const Vector2D vec) const;
	bool operator!=(const Vector2D vec) const;
	bool operator<(const Vector2D vec) const;
	bool operator<=(const Vector2D vec) const;
	bool operator>(const Vector2D vec) const;
	bool operator>=(const Vector2D vec) const;


	/*�Q�Ƃ̏ꍇ�͖��O.�̌`
	* �|�C���^��*vec
	* �h�b�g��vec->x
	* .�F�i���̂��̂��̂ɃA�N�Z�X�j
	* ->�F�i������i�W�����v���ăW�����v������j�A�����o�[
	* �������g�̎Q�Ƃ�Ԃ����A�N���X���̂���
	*���������g�̃|�C���^(this�|�C���^)
	*this�|�C���^��l�̏�Ԃɂ��ĕԂ��ɂ́�������
	* ��������ƒ��g�ɃA�N�Z�X�ł���
	* �ł��|�C���^�[����
	*/

	/*
	�|�C���^���n���ꂽ���ɑ΂��ā������ăA�N�Z�X���ď����������Ƃ��R�s�[���̏��͏��������B
	���ɑ΂��ĎQ�Ƃ������悤�ɓ����B
	const�͏��������\�ȕϐ��ɑ΂��ĕs�\�ɕς���B
	�I�y���[�^�̃I�[�o�[���[�h�������Ƃ�operator��=�̑O�ɂ���B
	���錾�̌`�Ƃ��Ă͊֐�
	���^(���܂ł̓|�C���^(�ꏊ�̏Z����������)�E�R�s�[(�ϐ����̂��̂�Ԃ�))
	����(�Q��(�l���̂��̂�Ԃ����Ƀ|�C���^�̂悤�ȏZ��))
	�Q�Ƃ͎��̕ۏ؂�����`�Ń|�C���^�̂悤�ɃR�s�[��炸�n����
	��������ƎQ��
	*/
};

//Vector��int���g���֐�
//Vector2D�~int�̃I�[�o�[���[�h
Vector2D operator*(const Vector2D& vec1, const Vector2D& vec2);
Vector2D operator*(const Vector2D vec, const int num);
Vector2D operator*(const int num, const Vector2D vec);

Vector2D operator*(const Vector2D vec, const float num);
Vector2D operator*(const float num, const Vector2D vec);

Vector2D operator/(const Vector2D vec, const float num);
Vector2D operator/(const Vector2D num, Vector2D vec2);
Vector2D operator/(const float num, const Vector2D vec);