#pragma once
#include <memory>
#include "BaseScene.h"

//lp = longptr�@�����O�|�C���^�[ 
//�����ꏊ�ɂ������
#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
	//�V���O���g��
public:
	//static = �v���O�������N�������u�ԂɃA�h���X���Œ肷��
	//static��static�łȂ��ƃ_��
	//��ΒP��ő��݂��Ăق������́��ÓI�V���O���g��
	//���������ꂽ�肵�Ăق������́����I�V���O���g��
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;

		return s_Instance;
	}
	void Run(void);
	void Update(void);
	void Draw(void);
	//static SceneMng* Create(void)
	//{
	//	
	//	if (s_Instance == nullptr)
	//	{
	//		s_Instance = new SceneMng();
	//	}
	//	return s_Instance;
	//}
	//static void Destroy(void)
	//{

	//	delete s_Instance;

	//	//new�������Ă��A�h���X���c�邩��null������
	//	//�������Ȃ��ƃo�O��
	//	s_Instance = nullptr;
	//}

private:
	//*���Ȃ����̂����Ԃƌ���
	bool SysInit(void);
	SceneMng();
	~SceneMng();
	//<>=template
	uniquScene scene_;
	

	//ptr�̎�� unique_ptr shared_ptr weak_ptr
	//A=B=C=A�@�z�Q�Ɓ@��������Ɏc���Ă��܂��i�o�O�j
	//unique_ptr�@���L���������ȊO�����Ă��Ȃ�
	//shared_ptr ���L�������L���邱�Ƃ��ł���
	// shared_ptr�������g���Ă���Əz�Q�Ƃ��N����
	// shared_ptr���P�v�I�Ɏg���K�v���Ȃ��Ȃ�weak_ptr���g�����ق�������
	//weak_ptr �ꎞ�I�ɏ��L���������ł���
};

