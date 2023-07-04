#pragma once
#include "Vector2D.h"
class Shape
{
public:
	Shape();
	Shape(int posX,int posY, int sizeX, int sizeY);
	void SetColor(unsigned int color);
	//���z�֐��cvirtual���t�����֐��̂���
	//�Z�~�R�����̑O�� = 0�������Ǝ��̂������Ȃ��Ȃ�c�������z�֐�
	virtual void Draw(void) = 0;		//�ʏ�`��
	virtual void Draw(float exp) = 0;	//�{���w��`��

	void Draw(unsigned int color);

protected:
	Vector2D pos_;
	Vector2D size_;
	unsigned int color_;
};

