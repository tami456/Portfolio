#include "FireMap.h"
#include <DxLib.h>
#include "../tmx/tmxObj.h"
#include "../_debug/_DebugDispOut.h"
#define RAD(X) (X * 3.141592/180.0)

FireMap::FireMap(const Vector2D& worldArea, const Vector2D& tileSize)
{
	worldArea_ = worldArea;
	tileSize_ = tileSize;
	Init();
}

FireMap::~FireMap()
{
}

bool FireMap::SetData(const Vector2D& pos, int power)
{
	return SetData(pos.x / tileSize_.x, pos.y / tileSize_.y, power);
}

bool FireMap::SetData(int x, int y, int power)
{
	bool flag = true;
	for (auto dir : Dir())
	{
		flag &= SetData(x, y, power,dir);
	}
	
	return flag;
}

bool FireMap::SetData(int x, int y, int power, Dir dir)
{
	if (x + static_cast<size_t>(y) * worldArea_.x >= fireData_.size() &&
		dir >= Dir::MAX)
	{
		return false;
	}
	fireData_[x + static_cast<size_t>(y) * worldArea_.x][static_cast<int>(dir)] = FireStruct{ 35,power };
	return true;
}

FireData& FireMap::GetData(const Vector2D& pos)
{
	//auto point = pos.x / tileSize_.x + (pos.y / tileSize_.y) * worldArea_.x;
	//if (point >= fireData_.size())
	//{
	//	return fireData_[0];
	//}
	//return fireData_[point];
	auto tilePos = pos / tileSize_;
	if (tilePos.x >= worldArea_.x ||
		tilePos.y >= worldArea_.y)
	{
		return fireData_[0];
	}
	auto point = tilePos.x + tilePos.y * worldArea_.x;
	if (point >= fireData_.size())
	{
		return fireData_[0];
	}
	return fireData_[point];
}

bool FireMap::CheckFire(const Vector2D& pos)
{
	for (auto fire : GetData(pos))
	{
		if (fire.count)
		{
			return true;
		}
	}
	return false;
}

void FireMap::Update(std::weak_ptr<tmxObj> tmxObj)
{
	if (tmxObj.expired())
	{
		return;
	}
	auto lockTmxObj = tmxObj.lock();
	/*fireData_[�}�X��][����].count���O���傫�������猸�Z
		�J�E���g�l��30�ɂȂ�����A����ׂɐL�΂��l��1�ȏ�Ȃ�A����ׂɐL�΂�*/

	for (int y = 0; y < worldArea_.y; y++)
	{
		for (int x = 0; x < worldArea_.x; x++)
		{
			for (auto dir : Dir())
			{
				if (fireData_[x + y * worldArea_.x][static_cast<int>(dir)].count)
				{
					//����L�΂��^�C�~���O���`�F�b�N
					if (--fireData_[x + y * worldArea_.x][static_cast<int>(dir)].count == 30)
					{
						//�L�΂��������c���Ă��邩�`�F�b�N
						if (!fireData_[x + y * worldArea_.x][static_cast<int>(dir)].length)
						{
							continue;
						}
						Vector2D point = {
							x + dirOffset_[static_cast<int>(dir)].x,
							y + dirOffset_[static_cast<int>(dir)].y
						};
					
						//wall���C���[���I�u�W�F�N�g����������L�΂��Ȃ�
						if (lockTmxObj->GetMapData("wall",point.x,point.y) != -1)
						{
							continue;
						}
						//block���C���[�ɃI�u�W�F�N�g����������A���͐L�΂���
						//���̉��΂�����E��1�ɂ���
						//�L�΂��\��̈ʒu��block�I�u�W�F�N�g������
						if (lockTmxObj->GetMapData("block", point.x, point.y) != -1)
						{
							lockTmxObj->SetMapData(-1, "block", point.x, point.y);
							fireData_[x + y * worldArea_.x][static_cast<int>(dir)].length = 1;
						}


						SetData(
							point.x,
							point.y,
							fireData_[x + y * worldArea_.x][static_cast<int>(dir)].length - 1,
							dir);
					}
				}
			}
		}
	}
	//1 = int;
	//1.5 = double;
	//1.5f = float;

}

void FireMap::Draw(std::weak_ptr<tmxObj> tmxObj, int x, int y, const Vector2D& camOffset, const Vector2D& drawOffset)
{
	//for (auto dir: Dir())
	//{
	//	if (fireData_[x + y * worldArea_.x][static_cast<int>(dir)].count)
	//	{
	//		_dbgDrawFormatString(
	//			x * tileSize_.x + camOffset.x + (static_cast<int>(dir) % 2) * 10 + drawOffset.x,
	//			y * tileSize_.y + camOffset.y + (static_cast<int>(dir) / 2) * 10 + drawOffset.y,
	//			0xffffff,
	//			"%d", fireData_[x + y * worldArea_.x][static_cast<int>(dir)].count);
	//	}
	//}
	int point = x + y * worldArea_.x;
	if (point < 0 || point >= fireData_.size())
	{
		return;
	}
	auto getAnimeID = [](int point) {
		return 3 - abs((point / 5) % 6 - 3);
	};
	if ((fireData_[point][static_cast<int>(Dir::Left)].count > 0 ||
		 fireData_[point][static_cast<int>(Dir::Right)].count > 0) &&
		(fireData_[point][static_cast<int>(Dir::Up)].count > 0 ||
		 fireData_[point][static_cast<int>(Dir::Down)].count > 0))
	{
		int count = 0;
		for (auto dir : Dir())
		{
			if (fireData_[point][static_cast<int>(dir)].count >count)
			{
				count = fireData_[point][static_cast<int>(dir)].count;
			}
		}
		//�\��
		DrawGraph(
			x * tileSize_.x + camOffset.x,
			y * tileSize_.y + camOffset.y,
		fireHdl_[getAnimeID(count)][0],true);
		return;
	}
	if (tmxObj.expired())
	{
		return;
	}
	auto lockTmxObj = tmxObj.lock();
	for (auto dir : Dir())
	{
		int partsID = 1;
		if (!fireData_[point][static_cast<int>(dir)].count)	
		{
			continue;
		}
		//�����̉��̈ʒu�ׂ̗Ƀu���b�N���Ȃ����Ƃ��`�F�b�N����
		if (lockTmxObj->GetMapData("block",x + dirOffset_[static_cast<int>(dir)].x,y + dirOffset_[static_cast<int>(dir)].y) == -1)
		{
			//�����̉����L�тĂ�r���ł͂Ȃ�
			if (!fireData_[point][static_cast<int>(dir)].length)
			{
				partsID = 2;
			}
		}
		DrawRotaGraph(
			x * tileSize_.x + camOffset.x + (tileSize_.x / 2),
			y * tileSize_.y + camOffset.y + (tileSize_.y / 2),
			1.0,
			dirRad_[static_cast<int>(dir)],
			fireHdl_[getAnimeID(fireData_[point][static_cast<int>(dir)].count)][partsID],
			true);
	}
}


bool FireMap::Init(void)
{

	LoadDivGraph("data/image/fire.png", 12, 3, 4, 20, 20, &fireHdl_[0][0]);
	fireData_.resize(static_cast<size_t>(worldArea_.x) * static_cast<size_t>(worldArea_.y));
	dirOffset_ = { Vector2D{0,1},Vector2D{-1,0},Vector2D{1,0},Vector2D{0,-1} };
	dirRad_ = {RAD(90),RAD(180),0, RAD(-90)};
	return false;
}
