#pragma once
#include <chrono>
#include <string>
#include <map>
#include <vector>

class Measure
{

public:

	// ������
	static constexpr bool IS_DISABLE = true;

	// ���ς���钼�߃t���[����
	static constexpr int AVG_FRAME = 180;

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Measure& GetInstance(void);

	void Init(void);

	// �v���J�n
	void Start(void);

	// �v��
	void Watch(std::string name);

	// �`��
	void Draw(void);

	// ���Z�b�g
	void Reset(void);

private:

	// �V���O���g���p�C���X�^���X
	static Measure* mInstance;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	Measure(void);
	~Measure(void);

	// �v���J�n
	std::chrono::system_clock::time_point mStartTime;

	// �P�t���[���O�̎���
	std::chrono::system_clock::time_point mPreFrameTime;

	// �e�v���ӏ��̌v������
	std::map<std::string, double> mWatchTimes;

	// �C���X�^���X����������̌o�ߎ���
	std::chrono::system_clock::time_point mInitTime;
	double mTotalTime;

	// �t���[���J�E���g
	int mCntTotalFrame;

	// ����180�t���[��
	std::vector<double> mAverage;

};

