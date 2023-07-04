#pragma once

//�L�����̎��
enum class CharID
{
	MAN,
	CAT,
	GIRL,
	WAR,
	MAG,
	DRAW,
	MAX
};

//�L�����摜�̕��я�
enum class Dir
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
	MAX
};

#define ANIM_IMAGE_MAX 3	//�A�j���[�V������
#define CHAR_SIZE_X 32
#define CHAR_SIZE_Y 32


class CharImage
{
public:
	CharImage();
	~CharImage();

	bool Init(CharID charID);
	bool Release(void);

	int chipImage_[static_cast<int>(Dir::MAX)][ANIM_IMAGE_MAX];
private:
	CharID charID_;


};

