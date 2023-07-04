#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x,int y);
	~Vector2D();
	int x;
	int y;

	//������Z�q
	//�������������Ȃ��ꍇconst������
	Vector2D& operator=(const Vector2D& vec);

	//�Y�������Z�q
	int& operator[](int j);

	//�P�����Z�q
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
	Vector2D& operator%=(const Vector2D& vec);
	Vector2D operator+()const;
	Vector2D operator-()const;

	//�񍀉��Z�q
	Vector2D operator+(const Vector2D& vec);
	Vector2D operator-(const Vector2D vec);
	Vector2D operator*(const Vector2D vec);
	Vector2D operator/(const Vector2D vec);
	Vector2D operator%(const Vector2D vec);
	//��r���Z�q
	bool operator==(const Vector2D& vec) const;
	bool operator!=(const Vector2D& vec) const;
	bool operator<(const Vector2D& vec) const;
	bool operator<=(const Vector2D& vec) const;
	bool operator>(const Vector2D& vec) const;
	bool operator>=(const Vector2D& vec) const;
};

Vector2D operator*(const Vector2D vec,const int num);
Vector2D operator*( const int num,const Vector2D vec);

Vector2D operator*(const Vector2D vec, const float num);
Vector2D operator*(const float num, const Vector2D vec);

Vector2D operator/(const Vector2D vec, const int num);





