#include <DxLib.h>
#include "../../_debug/_debug/_DebugConOut.h"
#include "../../_debug/_debug/_DebugDispOut.h"
#include "Raycast.h"

//���������
//Line�������CheckRay�ɓn��
bool Raycast::CheckCollision(Ray ray, Collision collision)
{
    Raycast::Line lines[4] = {
        //��̕�
        {
            collision.first,
            collision.first + Vector2DDouble{collision.second.x,0}
        },
        //�E�̕�
        {
            collision.first + Vector2DDouble{collision.second.x,0},
            collision.first + collision.second
         },
        //���̕�
         {
             collision.first + collision.second,
             collision.first + Vector2DDouble{0,collision.second.y}
         },
            //���̕�
         {
             collision.first + Vector2DDouble{0,collision.second.y},
             collision.first
         }
    };

    bool result = false;
    for (auto line:lines)
    {
        result |= CheckRay(ray, line);
    }
    return result;
}

//CheckCollision����������I�ɌĂяo��
bool Raycast::CheckRay(Ray ray, Line line)
{
    _dbgDrawLine(
        static_cast<int>(line.p.x),
        static_cast<int>(line.p.y),
        static_cast<int>(line.end.x),
        static_cast<int>(line.end.y),0xff0000);

    auto checkCross = [](Vector2DDouble& vec1, Vector2DDouble& vec2)
    {
        //�O�ς�����Ă���
        //�O�ς̌��ʂ����^�[��
        return (vec1.x * vec2.y) - (vec2.x * vec1.y);
    };
    //���s�����O���Ă���
    auto vecLine = line.end - line.p;
    auto crossRayLine = checkCross(ray.vec, vecLine);
    if (crossRayLine == 0.0)
    {
        //���s�Ȃ̂�false��Ԃ�
        //������Ȃ��̂Ōv�Z�̖���
        //�O�ς̌��ʂ�0�̏ꍇ�͕��s
        //���s���͐�΂Ɍ������Ȃ��̂Œe��
        return false;
    }
    Vector2DDouble v = line.p - ray.p;
      
    auto cross_vRay = checkCross(v,ray.vec);
    auto cross_vLine = checkCross(v, vecLine);

    auto t1 = cross_vRay / crossRayLine;
    auto t2 = cross_vLine / crossRayLine;

    if (t1 > 0.0 && t1 <= 1.0 && t2 > 0.0 && t2 <= 1.0)
    {
        //�������Ă�����true
        return true;
    }

    return false;
}
