#include"BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SEPHERE_BULLET;
}

BulletObject::~BulletObject()
{

}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ == LAZER_BULLET_RIGHT)
	{
		ret = LoadImg("img//lazer_bullet_right.png", des);
	}
	else if (bullet_type_ == SEPHERE_BULLET)
	{
		ret = LoadImg("img//bullet.png", des);
	}
	else if (bullet_type_ == LAZER_BULLET_LEFT)
	{
		ret = LoadImg("img//lazer_bullet_left.png", des);
	}


	return ret;
}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
}

void BulletObject :: ChecktoMap(Map& data)
{
	int x1 = (rect_.x + data.start_x_) / TILE_SIZE;
	int x2 = (rect_.x + data.start_x_ + rect_.w - 1) / TILE_SIZE;
	int y1 = (rect_.y + data.start_y_) / TILE_SIZE;
	int y2 = (rect_.y + data.start_y_ + rect_.h - 1) / TILE_SIZE;

	if (x_val_ > 0)
	{
		int val1 = data.tile[y1][x2];
		int val2 = data.tile[y2][x2];

		if (val1 != BLANK_TILE || val2 != BLANK_TILE)
			{
			is_move_ = false;
			}
	}
}