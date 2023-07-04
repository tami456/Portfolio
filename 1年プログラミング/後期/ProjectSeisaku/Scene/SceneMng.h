#pragma once
#include "../common/Vector2D.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"

#define lpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	void Run(void);
	static SceneMng& GetInstance(void)
	{
		static SceneMng s_Instance;
		//Create();
		return s_Instance;
	}
	Vector2D GetScreenSize(void);
	//static void Create(void)			//�ÓI�Ȋ֐�
	//{
	//	if (!s_Instance)				//���̂��Ȃ�������A�C���X�^���X����
	//	{
	//	s_Instance = new SceneMng();
	//	}
	//}
	//static void Destroy(void)					//����Ȃ��Ȃ�����null����B����Ƃ���create�����
	//{
	//	if (s_Instance)
	//	{
	//		delete s_Instance;
	//		s_Instance = nullptr;
	//	}
	//}
private:
	//static SceneMng* s_Instance;
	SceneMng();
	~SceneMng();
	bool SysInit(void);		//�������֌W�̊֐�
	bool initFlag_;
};

