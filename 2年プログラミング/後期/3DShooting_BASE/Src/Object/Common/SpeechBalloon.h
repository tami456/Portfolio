#pragma once
#include <string>
#include <DxLib.h>
#include "Transform.h"
class SceneManager;

class SpeechBalloon
{

public:

	// �\�����
	enum class TYPE
	{
		TEXT,
		SPEECH
	};

	// �R���X�g���N�^
	SpeechBalloon(TYPE type, const Transform* parent);

	// �f�X�g���N�^
	~SpeechBalloon(void);

	void Draw(void);
	void DrawTextOrg(void);
	void DrawSpeech(void);
	void Release(void);

	void SetText(const std::string& text);
	void SetTime(double time);

	void SetType(TYPE type);
	void SetVisiblePermanent(void);

	void SetRelativePos(VECTOR relativePos);

private:

	TYPE mType;

	int mImage;

	// true�̏ꍇ�A�i���I�ɕ\������
	bool mIsVisiblePermanent;
	double mVisibleTime;

	// �����o����t����e�I�u�W�F�N�g
	const Transform* mTransformParent;

	// �\�����W
	VECTOR mPos;

	// �\���ʒu�̒����p
	VECTOR mRelativePos;

	// �\������
	std::string mText;

};



