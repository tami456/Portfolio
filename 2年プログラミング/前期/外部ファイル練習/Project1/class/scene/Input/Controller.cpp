#include <DxLib.h>
#include "../../../_debug/_debug/_DebugConOut.h"
#include "../../../_debug/_debug/_DebugDispOut.h"
#include "KeyInput.h"
#include "PadInput.h"
#include "Controller.h"

Controller::RingBuf* Controller::RingBuf::Create(int no)
{
    //�V�K�̗v�f�����
    //new���g��
    //���������X�g�c�����t���R���X�g���N�^���Ȃ��Ă��������ł���
    //new�̏ꍇ��=������ł��Ȃ�
    //new����Ȃ��ꍇ��RingBuf() = {};
    auto beginBuf = new RingBuf{ 0,0.0,nullptr,nullptr };
    auto beforeBuf = beginBuf;
    auto nowBuf = beginBuf;

    //�Ȃ�1�Ȃ̂��͂�����ŗv�f�������Ă��邩��
    for (int i = 1;i < no ;i++)
    {
        nowBuf = new RingBuf{ 0,0.0,beforeBuf,nullptr };
        //�O�̃o�t�̎��̃o�t�͍��̃o�t
        beforeBuf->next = nowBuf;
        beforeBuf = nowBuf;
    }
    //�ŏ��̃o�t�̑O�ɍ�������
    beginBuf->before = nowBuf;
    nowBuf->next = beginBuf;

    return beginBuf;
    
}

void Controller::RingBuf::Destroy(RingBuf* buf)
{
    
    //���������Ɉړ������Ď����̑O�������Ă���
    if (buf == nullptr || buf->before == nullptr)
    {
        return;
    }
    //�����̑O���������̎��O�̑O��null�ɂ��Ċ֌W��f��
    buf->before->next = nullptr;
    while(buf->next != nullptr)
    {
        buf = buf->next;
        delete buf->before;
    }

    delete buf;
}


//ringBuf�̍X�V���~�߂���R�}���h����o��
std::string Controller::CheckCommand(DIR_LR dir, CommandMap& commandMap)
{

    int flag = true;
    auto buf = ringBuf_;
    //�v���C���[�����������R�}���h�}�b�v�ɑ΂��Ă̏���������
    //�����O�o�b�t�@�̓��e�ƃ}�b�`�����Ĉ�v���Ă�����
    //��v���Ă��Ȃ������玟�̃R�}���h�̃}�b�`���O����

    //commandMap�L�[�ƃo�����[�̃y�A
    //first�c�L�[�@second�c�o�����[
    for (const auto& command : commandMap)
    {
        flag = true;
        buf = ringBuf_;

        //array�����Ă� 2U�c���E
        //���E�Ȃ̂�dir�Ŏ��o��
        //list�c�R�}���h���e
        //�ォ����o���Ă����̂�rbegin(�t�C�e���[�^�[)
        //�Ō�܂ŉ񂵂��ꂽ��R�}���h����
        //�C�e���[�^�[�O���Z
        for (auto itr = command.second[static_cast<int>(dir)].rbegin();
            itr != command.second[static_cast<int>(dir)].rend();
            ++itr)
        {
            //���̓�𖞂����Ă����玟�̃R�}���h�`�F�b�N
            //0�c�L�[�@1�c���Ԓl
            if ((*itr)[0] != buf->value)
            {
                //���̒��ŃR�}���h�������Ă��Ȃ������ꍇ
                flag = false;
                break;
            }

            if ((*itr)[1] < buf->time)
            {
                //��t���Ԃ𒴂��Ă���ꍇ
                flag = false;
                break;
            }
            //��납��`�F�b�N���Ă����Ă��邩��before
            buf = buf->before;
        }
        
        if (flag)
        {
            //�R�}���h����
            //���������烊�Z�b�g����

            /*ringBuf_ = ringBuf_->next;
            ringBuf_->value = static_cast<int>(InputID::NON);*/

            ringBuf_->time = 0.0;
            startBuf_ = ringBuf_;

            //first�ɃR�}���h
            TRACE("�R�}���h����:%s\n",command.first.c_str());
            return command.first;
        }
    }
    
    //�ǂ̃R�}���h���������Ă��Ȃ������ꍇ���Ԃ�
    return std::string();
}



void Controller::WriteCmd(void)
{
    if (ringBuf_ == nullptr)
    {
        return;
    }

    //���͏��̎擾�ƌo�ߎ��Ԃ̎擾
    //�g�p���閽�߁@�o�ߎ���

    //���ݎ��Ԃ̎擾
    auto endTime = std::chrono::system_clock::now();
   
    //���������߂邱�ƂŌo�ߎ��Ԃ����܂�
    //std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_);
    
    //�L�[�̓��͂���������ۑ�
    //�����L�[�̓��͂���������o�ߎ��Ԃ��p��
    //�Ⴄ�L�[�ɂȂ������Ɍo�ߎ��Ԃ����Z�b�g startTime_��endTime����
    //ringbuf��startbuf�@�����O�o�t���猩��next
    //�f�[�^�̏������݂̍�
    //WriteCmd
    //InputID��
    //���͂��ꂽ���ɑ΂��ăV�t�g���Z����bit���Z
    //value�ɓ����
    int writeData = 0;

   for (auto id : InputID())
   {
       if (cntData_[id][static_cast<int>(Trg::Now)])
       {
           //1���d�ˍ��킹�ăV�t�g����
           writeData |= 1 << static_cast<int>(id);
       }
       //�R�[�h�ʂ����炷�ꍇ
       //writeData |= cntData_[id][static_cast<int>(Trg::Now)] << static_cast<int>(id);
   } 

   //�ǂݍ��ݗD��
    //�L�[�f�[�^���ς���Ă�����
    //�������Ȃ��ƈ�u�Ńo�b�t�@��H���Ԃ�
   if (ringBuf_->value != writeData)
   {
       //��Ɉړ������ăL�[�Ǝ��Ԃ�����
       ringBuf_ = ringBuf_->next;
       //���͂��������ꍇ
       ringBuf_->value = writeData;
       //1�O��startBuf_��������
       if (startBuf_ != ringBuf_->before)
       {
           //�L�[�̓��͂��ς�������ɉe�����󂯂�
           ringBuf_->time = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count();
       }
       else
       {
           ringBuf_->time = 0;
       }
       
      
       
       startTime_ = endTime;
   }

   //���͂������ăv���C���[���Ӑ}�����L�[�������Ă����ꍇ
    //count�Ő��l���Ƃ��ď���
    //writeData�̒l��1�ȏ�ŉ������o�ߎ��Ԃ��{���ɏ����������牟���ĂȂ����ƂƂ���(�j���[�g����)
    //�������݂��Ȃ��A�G���h����X�^�[�g���������Ԍo�߂��Z�����Ԃ����������
    //�j���[�g�����Ƃ��Ď󂯎�鎞�Ԃ𒴂��Ă�������͊J�n
    //�j���[�g�������o�R����R�}���h�ƌo�R���Ȃ��R�}���h�����
   if (writeData == 0)
   {
       if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count() <= 300.0)
       {
       }
       else
       {
           ringBuf_->value = static_cast<int>(InputID::NON); 
       }

       //���͂��Ȃ������ꍇ
       //count�Ő��l���Ƃ��
       if (std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime_).count() >= 300.0)
       {
           //500�𒴂��Ă���
           startBuf_ = ringBuf_;
       }
   }
   /*if (writeData == 0)
   {
       
   }*/

   
   
    int no = 0;
    auto drawBuf = startBuf_;
    
    while (drawBuf != ringBuf_)
    {
        _dbgDrawFormatString(no * 128, 530, 0xffffff, "%lf", drawBuf->time);
        _dbgDrawFormatString(no * 128, 560, 0xffffff, "%d", drawBuf->value);
        drawBuf = drawBuf->next;
        no++;
    }

    if (startBuf_ != ringBuf_)
    {
        _dbgDrawFormatString(no * 128, 530, 0xffffff, "%lf", drawBuf->time);
        _dbgDrawFormatString(no * 128, 560, 0xffffff, "%d", drawBuf->value);
    }
}
