#ifndef EXP_H_
#define EXP_H_

#include "CommonFunction.h"
#include "BaseObject.h"



#define NUM_FRAME_EXP 8;

class ExplosionObject1 : public BaseObject
{
public:
	ExplosionObject1();
	~ExplosionObject1();


public:

	void set_clip();
	void set_frame(const int& fr) { frame_ = fr; }
	virtual bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* screen);
	int get_frame_width() const { return frame_width_; }
	int get_frame_height() const { return frame_height_; }

private:
	int frame_width_;
	int frame_height_;

	unsigned int frame_;
	SDL_Rect frame_clip_[8];


};

#endif
