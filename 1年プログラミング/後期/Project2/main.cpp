#include <memory>
#include <Dxlib.h>
#include "Scene/SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance, LPSTR lpCmdLine,
	int nCmdShow)

{
	////Base�N���X�����j�[�N�|�C���^�ŊǗ�����Ƃ����^�@���g���Ȃ�
	//std::unique_ptr<BaseScene> scene;
	////�@�錾��ɒ��g���擾
	//scene.reset(new TitleScene());
	//SceneMng::Create();
	SceneMng::GetInstance().Run();
	
	//delete scene;		//���������[�N��h�����߂�delte����
	return 0;
}