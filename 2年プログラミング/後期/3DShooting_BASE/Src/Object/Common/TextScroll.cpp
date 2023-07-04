#include <DxLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "TextScroll.h"

TextScroll::TextScroll(void)
{
}

TextScroll::~TextScroll(void)
{
}

void TextScroll::Init(void)
{
    //�A���t�@�x�b�g�̉摜�̓ǂݍ���
    mResource = ResourceManager::GetInstance().Load(
        ResourceManager::SRC::ALPHABET);

    //�e�L�X�g���[���̎�ʂ������ݒ�
    mType = TYPE::TITLE;

    //1�s������̏��
    //�\���̂�1����
    std::vector<MsgInfo> infos;

    //���͂̐ݒ�i�P�s���j
    infos.emplace_back(MakeMsgInfo("Hajimemashite.",infos.size()));
    infos.emplace_back(MakeMsgInfo("Konnichiwa.",infos.size()));

    mTextMap.emplace(TYPE::TITLE, infos);

    //�ړ������i���K�|�C���g�@�j
    
    Quaternion tmpQ = Quaternion::Euler(
        AsoUtility::Deg2RadF(-40.0f),
        0.0f, 0.0f
    );

    mVelocity = tmpQ.GetForward();

    //�ړ����x�i�ړ���(�����~���x))
    mVelocity = VScale(mVelocity, SPEED_MOVE);

}

void TextScroll::Update(void)
{
    std::vector<MsgInfo>& infos = mTextMap[mType];
    for (auto& info : infos)
    {
        //�����̈ړ�����
        info.pos = VAdd(info.pos, mVelocity);
    }

}

void TextScroll::Draw(void)
{
    const std::vector<MsgInfo>& infos = mTextMap[mType];
    
    VECTOR pos;

    float plusX;

    //�P�s����
    for (auto& info : infos)
    {

        plusX = 0.0f;

        //�P��������
        for (auto& id : info.messages)
        {
            pos = info.pos;
            pos.x += plusX;
            //���K�A���ɍ��W�����炷

            DrawBillboard3D(pos, 0.5f, 0.5f, SIZE_3D, 0.0f,mResource.mHandleIds[id],true);

            plusX += SIZE_3D;
        }
    }
}

void TextScroll::Release(void)
{
}

TextScroll::MsgInfo TextScroll::MakeMsgInfo(const std::string& msg, size_t mapCount)
{
    MsgInfo ret;
    //�������摜�z��̓Y�����ɕϊ�
    std::vector<int> messages;

    int ascii;
    int idx;
    size_t len = msg.size();
    for (int i = 0; i < len; i++)
    {
        //���K�B�ϊ�����������������
        idx = 0;
        
        //�P������ASCII�R�[�h�ɕϊ�����
        ascii = msg.at(i);
       

        //(32��������X�y�[�X(�摜�Y����52))
        if (ascii == 32)
        {
            idx = 52;
        }

        //�h�b�g(.)
        if (ascii == 46)
        {
            idx = 53;
        }

        //�J���}(,)
        if (ascii == 44)
        {
            idx = 54;
        }

        //�啶��
        if (ascii >= 65 && ascii <= 90)
        {
            idx = ascii - 65;
        }

        //������
        if (ascii >= 97 && ascii <= 122)
        {
            idx = ascii - 71;
        }

        messages.emplace_back(idx);
    }

    //�s���Ƃ̍��W
    ret.pos =
    {
        -400.0f,
        START_Y - (mapCount * SIZE_3D),
        START_Z - (mapCount * SIZE_3D)
    };




    //���W�ƕϊ����ꂽID��������
    ret.messages = messages;

    return ret;
}
