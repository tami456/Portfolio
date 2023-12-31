#pragma once
class Vector2D
{
public:
	Vector2D();
	Vector2D(int x,int y);
	~Vector2D();
	int x;
	int y;

	//γόZq
	//«·¦½­Θ’κconstπ­
	Vector2D& operator=(const Vector2D& vec);

	//Y¦Zq
	int& operator[](int j);

	//PZq
	Vector2D& operator+=(const Vector2D& vec);
	Vector2D& operator-=(const Vector2D& vec);
	Vector2D& operator*=(const Vector2D& vec);
	Vector2D& operator/=(const Vector2D& vec);
	Vector2D& operator%=(const Vector2D& vec);
	Vector2D operator+()const;
	Vector2D operator-()const;

	//ρZq
	Vector2D operator+(const Vector2D& vec);
	Vector2D operator-(const Vector2D vec);
	Vector2D operator*(const Vector2D vec);
	Vector2D operator/(const Vector2D vec);
	Vector2D operator%(const Vector2D vec);
	//δrZq
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





