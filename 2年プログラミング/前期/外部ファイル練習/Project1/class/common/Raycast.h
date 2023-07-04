#pragma once
#include "../Vector2D.h"
#include "Collision.h"

struct Raycast
{
	struct Ray 
	{
		Vector2DDouble p;	//�n�_
		Vector2DDouble vec;	//�����x�N�g��
	};

	struct Line 
	{
		Vector2DDouble p;	//�n�_
		Vector2DDouble end;	//�I�_
	};
	bool CheckCollision(Ray ray, Collision collision);
	bool CheckRay(Ray ray, Line line);
};

