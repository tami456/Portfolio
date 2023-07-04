#include <DxLib.h>
#include "ImageMng.h"
#include "../_debug/_debug/_DebugConOut.h"
#include "../_debug/_debug/_DebugConOut.h"

//Load������z�肵���d�g��

//f_name�͎擾�ς݂Ȃ̂�key�̂�
const VecInt& ImageMng::GetID(std::string key)
{
    if (ImageMap_.count(key) == 0)
    {
        GetID(key, key);
    }
    return ImageMap_[key];
    //�����ĕԂ��K�v�͂Ȃ�
    //��������ǂ���return����̂�
}

//LoadGraph�z��
//�ꖇ�̔w�i�Ȃ�
const VecInt& ImageMng::GetID(std::string f_name, std::string key)
{
    if (ImageMap_.count(key) == 0)
    {
        ImageMap_[key].resize(1);
        ImageMap_[key][0] = LoadGraph(f_name.c_str());
    }
    return ImageMap_[key];
}

//LoadDivGraph��z�肵���\����
const VecInt& ImageMng::GetID(std::string f_name, std::string key, Vector2D divSize, Vector2D divCnt)
{
   
    if (ImageMap_.count(key) == 0)
    {
        //�T�C�Y�n��int�͂ق�unsigned
         //�|���Z�̑O��cast���Ȃ��ƈ��Ă��܂�
        ImageMap_[key].resize(static_cast<UINT>(divCnt.x) * static_cast<UINT>(divCnt.y));
        if (LoadDivGraph(f_name.c_str(),
            divCnt.x * divCnt.y,
            divCnt.x, divCnt.y,
            divSize.x, divSize.y,
            &ImageMap_[key][0], true) == -1)
        {
            TRACE("�摜�̓ǂݍ��ݎ��s:%s\n", f_name.c_str());
            
       }
    }
    return ImageMap_[key];
    // TODO: return �X�e�[�g�����g�������ɑ}�����܂�
}

ImageMng::ImageMng()
{

}

ImageMng::~ImageMng()
{
}
