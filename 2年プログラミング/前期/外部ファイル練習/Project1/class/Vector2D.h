#pragma once
//template�@�\���g���ɂ�class�̑O��template<class T>class �N���X���@�Ə���
//T��int��float�Adouble�̖������ʂ���
//T�̒��g���m�肵�ĂȂ��ƃG���[���o�Ȃ�
template <class T>class Vector2DTmp
{
public:
	Vector2DTmp();
	Vector2DTmp(T x,T y);
	~Vector2DTmp();
	T x;
	T y;

	//������Z�q
	//�������������Ȃ��ꍇconst������
	Vector2DTmp& operator=(const Vector2DTmp& vec);

	//�Y�������Z�q
	T& operator[](int j);

	//�P�����Z�q
	Vector2DTmp& operator+=(const Vector2DTmp& vec);
	Vector2DTmp& operator-=(const Vector2DTmp& vec);
	Vector2DTmp& operator*=(const Vector2DTmp& vec);
	Vector2DTmp& operator/=(const Vector2DTmp& vec);
	Vector2DTmp& operator%=(const Vector2DTmp& vec);
	Vector2DTmp operator+()const;
	Vector2DTmp operator-()const;

	//�񍀉��Z�q
	Vector2DTmp operator+(const Vector2DTmp& vec);
	Vector2DTmp operator-(const Vector2DTmp vec);
	Vector2DTmp operator*(const Vector2DTmp vec);
	//Vector2D operator/(const Vector2D vec);
	Vector2DTmp operator%(const Vector2DTmp vec);
	//��r���Z�q
	bool operator==(const Vector2DTmp& vec) const;
	bool operator!=(const Vector2DTmp& vec) const;
	bool operator<(const Vector2DTmp& vec) const;
	bool operator<=(const Vector2DTmp& vec) const;
	bool operator>(const Vector2DTmp& vec) const;
	bool operator>=(const Vector2DTmp& vec) const;
};

//{}�̊O�ɏ����Ă���ꍇ��<T>������
template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T>& vec1, const Vector2DTmp<T>& vec2);

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const int num);

template <class T>
Vector2DTmp<T> operator*( const int num,const Vector2DTmp<T> vec);

template <class T>
Vector2DTmp<T> operator*(const Vector2DTmp<T> vec, const float num);

template <class T>
Vector2DTmp<T> operator*(const float num, const Vector2DTmp<T> vec);

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec, const int num);

template <class T>
Vector2DTmp<T> operator/(const Vector2DTmp<T> vec1, Vector2DTmp<T> vec2);

//����Vector2D���o�Ă����Ƃ��Ă�template�Ƃ��đウ�Ă����
using Vector2D = Vector2DTmp<int>;
using Vector2DFlot = Vector2DTmp<float>;
using Vector2DDouble = Vector2DTmp<double>;


#include "detail/Vector2D.h"



