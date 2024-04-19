#include"CommonFunction.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include"ImpTimer.h"
#include "ThreatsObject.h"
#include "ExplosionObject.h"
#include "ExplosionObject1.h"
#include "TextObject.h"
#include "PlayerPower.h"
#include "Geometric.h"
#include <iostream>
#include "SDL_mixer.h"

BaseObject menu;
BaseObject g_background;
TTF_Font* font_time = NULL ;
TTF_Font* font_menu = NULL;

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) {
		return false;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Game Cpp SDL 2.0 - Blog: Mygame",
									SDL_WINDOWPOS_UNDEFINED,
									SDL_WINDOWPOS_UNDEFINED,
									SCREEN_WIDTH, SCREEN_HEIGHT,
									SDL_WINDOW_SHOWN);
	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
			success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR , RENDER_DRAW_COLOR , RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG; // ho tro dinh dang PNG
			if (!(IMG_Init(imgFlags) && imgFlags))
			{
				success = false;
			}
		}

		if (TTF_Init() == -1)
		{
			success = false;
		}

		font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_time == NULL)
		{
			success = false;
			
		}

		font_menu = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_menu == NULL)
		{
			success = false;
		}

		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			return false;
		}

		// Read file wav audio
		g_sound_bullet[0] = Mix_LoadWAV("soundtirex.wav");
		g_sound_exp[0] = Mix_LoadWAV("catchcoin.wav");

		 

		if (g_sound_bullet[0] == NULL || g_sound_exp[0] == NULL)
		{
			return false;
		}

	}

	return success;

}


bool LoadMenuBackground()
{
	bool ret = menu.LoadImg("img//menu.png", g_screen);
	if(ret == false)
	{
		return false;
	}

	return true;
}


bool LoadBackground()
{
	bool ret = g_background.LoadImg("img//background.png", g_screen);
	if (ret == false)
		return false;

	return true;
}


void close() {
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}


std :: vector<ThreatsObject*> MakeThreadList()
{
	std::vector<ThreatsObject*> list_threats;
	
	ThreatsObject* dynamic_threats = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(500 + i * 500);
			p_threat->set_y_pos(200);

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->SetAnimationPos(pos1, pos2);
			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);
		}
	}




	ThreatsObject* threats_objs = new ThreatsObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		MainObject p_player;
		if (p_threat != NULL)
		{

			
			p_threat->set_x_pos(700 + i*1200);
			/*if (p_player.GetXpos() > p_threat->get_x_pos())
			{
				std::cout << p_player.GetXpos() << "                " << p_threat->get_x_pos();
				p_threat->LoadImg("img//threat_level_right.png", g_screen);
			}
			else
			{
				p_threat->LoadImg("img//threat_level_left.png", g_screen);
			}*/

			p_threat->LoadImg("img//threat_level_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_y_pos(250);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
	if (InitData() == false)
		return -1;

	if (LoadMenuBackground() == false)
		return -1;
//
	if (LoadBackground() == false)
		return -1;

	Mix_Music* g_music = Mix_LoadMUS("musicthrough.mp3");
	if (g_music == NULL)
	{
		return -1;
	}

	Mix_PlayMusic(g_music, -1);


	GameMap game_map;
	game_map.LoadMap("map/map01.dat");
	game_map.LoadTiles(g_screen);


	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();


	PlayerPower player_power;
	player_power.Init(g_screen);

	PlayerMoney player_money;
	player_money.Init(g_screen);
	player_money.SetPos(SCREEN_WIDTH * 0.5 - 300, 8);

	std::vector<ThreatsObject*> threats_list = MakeThreadList();


	ExplosionObject exp_threat;
	bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
	if (!tRet) return -1;
	exp_threat.set_clip();

	/*ExplosionObject exp_main;
	bool mRet = exp_main.LoadImg("img//exp3.png", g_screen);
	if (!mRet) return -1;
	exp_main.set_clip();*/

	int num_die = 0;
	// Time text

	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	TextObject mark_game;
	mark_game.SetColor(TextObject::WHITE_TEXT);
	UINT mark_value = 0;

	TextObject money_game;
	money_game.SetColor(TextObject::WHITE_TEXT);
	//
	TextObject start_button;
	TextObject guide_button;
	TextObject quit_button;
	TextObject continue_button;

	bool is_menu = true;
	bool start = true ;
	bool quit = false;
	bool is_quit = false;
	bool time = false;
	bool tmp_time = false;
	bool pause_menu = false;
	Uint32 time_menu = 0;
	Uint32 pause_time = 0;

	while (start && !is_quit )
	{
		fps_timer.start();

		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				start = false;
				is_quit = true;
			}


			p_player.HandelInputAction(g_event, g_screen , g_sound_bullet);

		}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);
		

		if (is_menu)
		{
			SDL_Texture* gBackgroundTexture = menu.GetObject();
			SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
			//SDL_SetTextureBlendMode(gBackgroundTexture, SDL_BLENDMODE_MOD);
			SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);

			//renderStart(new_texture);


			int mouseX, mouseY;
			SDL_GetMouseState(&mouseX, &mouseY);

			start_button.SetText("START");
			
			quit_button.SetText("QUIT");

			start_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120 ) {
				start_button.SetColor(TextObject::RED_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						start = true;
						is_menu = false;
						time = true;
						if (tmp_time == false)
						{
							time_menu = SDL_GetTicks() / 1000;
							tmp_time = true;
						}
					}
				}
			}
			else {

				start_button.SetColor(TextObject::ORANGE_TEXT);
			}

			start_button.RenderText(g_screen, 1280 / 2 - 40, 480 / 2 + 120 - 15 );

			/*guide_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 50 && mouseY <= 480 / 2 + 37 - 15 + 50) {
				guide_button.SetColor(TextObject::GRAY_TEXT);

			}
			else {
				guide_button.SetColor(TextObject::RED_TEXT);
			}
			guide_button.RenderText(g_screen, 1280 / 2 - 40 + 2, 480 / 2 + 56);*/

			quit_button.LoadFromRenderText(font_menu, g_screen);
			if (mouseX >= 1280 / 2 - 40 && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 220 && mouseY <= 480 / 2 + 37 - 15 + 220) {
				quit_button.SetColor(TextObject::RED_TEXT);
				if (g_event.type == SDL_MOUSEBUTTONDOWN) {
					if (g_event.button.button == SDL_BUTTON_LEFT)
					{
						is_quit = true;
					}
				}
			}
			else {
				quit_button.SetColor(TextObject::ORANGE_TEXT);
			}
			quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 220 - 15 );
			SDL_RenderPresent(g_screen);
			SDL_Delay(100);
		}

		else if (is_menu == false)
		{
			g_background.Render(g_screen, NULL);

			Map map_data = game_map.getMap();

			p_player.HandleBullet(g_screen);
			p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
			p_player.DoPlayer(map_data , g_sound_exp);

			if (g_event.type == SDL_KEYDOWN)
			{
				if (g_event.key.keysym.sym == SDLK_ESCAPE)
				{
					pause_menu = true;
					time = false;
					pause_time = SDL_GetTicks() / 1000;
				}
			}
			if (pause_menu == true)
			{
				SDL_Texture* gBackgroundTexture = menu.GetObject();
				SDL_Rect backgroundRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
				SDL_RenderCopy(g_screen, gBackgroundTexture, NULL, &backgroundRect);
				int mouseX, mouseY;

				SDL_GetMouseState(&mouseX, &mouseY);

				continue_button.SetText("CONTINUE");

				quit_button.SetText("QUIT");

				continue_button.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 40 - 20 && mouseX <= 1280 / 2 - 40 + 90 - 20 && mouseY >= 480 / 2 - 15 + 120 && mouseY <= 480 / 2 + 37 - 15 + 120) {
					continue_button.SetColor(TextObject::RED_TEXT);
					if (g_event.type == SDL_MOUSEBUTTONDOWN) {
						if (g_event.button.button == SDL_BUTTON_LEFT)
						{
							pause_menu = false;
							time = true;
							pause_time = SDL_GetTicks() / 1000 - pause_time;
						}
					}
				}
				else {

					continue_button.SetColor(TextObject::ORANGE_TEXT);
				}

				continue_button.RenderText(g_screen, 1280 / 2 - 40 - 20, 480 / 2 + 120 - 15);
				quit_button.LoadFromRenderText(font_menu, g_screen);
				if (mouseX >= 1280 / 2 - 40  && mouseX <= 1280 / 2 - 40 + 90 && mouseY >= 480 / 2 - 15 + 220 && mouseY <= 480 / 2 + 37 - 15 + 220) {
					quit_button.SetColor(TextObject::RED_TEXT);
					if (g_event.type == SDL_MOUSEBUTTONDOWN) {
						if (g_event.button.button == SDL_BUTTON_LEFT)
						{
							is_quit = true;
						}
					}
				}
				else {
					quit_button.SetColor(TextObject::ORANGE_TEXT);
				}
				quit_button.RenderText(g_screen, 1280 / 2 - 40 + 5, 480 / 2 + 220 - 15);
				SDL_RenderPresent(g_screen);
				SDL_Delay(100);
			}

			bool is_die = p_player.Getis_die();
			if (is_die == true)
			{
				num_die++;
				SDL_Delay(1000);
				player_power.Decrease();
				player_power.Render(g_screen);
				p_player.ResertDie(); // muốn thay đổi biến cục bộ thì phải viết hàm chứ không thay đổi bằng phép gán được
			}

			p_player.Show(g_screen);


			game_map.SetMap(map_data);
			game_map.DrawMap(g_screen);


			//// Draw Geometric
			//GeometricFormat rectangle_size(0, 0, SCREEN_WIDTH, 40);
			//ColorData color_data(0, 0, 0);
			//Geometric::RenderRectangle(rectangle_size, color_data, g_screen);

			//GeometricFormat outLineSize(1, 1, SCREEN_WIDTH - 1, 38);
			//ColorData color_data2(255, 0, 0);

			//Geometric::RenderOutline(outLineSize, color_data2, g_screen);


			player_power.Show(g_screen);
			player_money.Show(g_screen);

			for (int i = 0; i < threats_list.size(); i++)
			{
				ThreatsObject* p_threat = threats_list.at(i);
				if (p_threat != NULL)
				{
					p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
					p_threat->ImpMoveType(g_screen);
					p_threat->DoPlayer(map_data);
					p_threat->Get_Rect_player(p_player.GetRect());
					p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
					p_threat->Show(g_screen);




					SDL_Rect rect_player = p_player.GetRectFrame();
					bool bCol1 = false;
					std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
					for (int jj = 0; jj < tBullet_list.size(); jj++)
					{
						BulletObject* pt_bullet = tBullet_list.at(jj);
						if (pt_bullet != NULL)
						{
							pt_bullet->ChecktoMap(map_data);
							if (pt_bullet->get_is_move() == false)
							{
								p_threat->SetBulletGet(jj);
							}
						}


						if (pt_bullet)
						{
							bCol1 = SDLCommonFunction::CheckCollision(pt_bullet->GetRect(), rect_player);
							
							if (bCol1)
							{
								p_threat->SetBulletGet(jj);
								break;
							}
						}
					}

					SDL_Rect rect_threat = p_threat->GetRectFrame();
					bool bCol2 = SDLCommonFunction::CheckCollision(rect_player, rect_threat);

					if (bCol1 || bCol2)
					{

						/*for (int ex = 0; ex < 4; ex++)
						{
							int width_exp_frame = exp_main.get_frame_width();
							int heiht_exp_height = exp_main.get_frame_height();

							int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
							int y_pos = (p_player.GetRect().y + p_player.get_frame_width() * 0.5) - heiht_exp_height * 0.5;

							exp_threat.set_frame(ex);
							exp_threat.SetRect(x_pos, y_pos);
							exp_threat.Show(g_screen);
							SDL_RendererPresent(g_screen);
						}*/

						num_die++;
						if (num_die <= 3)
						{
							p_player.SetRect(0, 0);
							p_player.set_comeback_time(60);
							SDL_Delay(1000);
							player_power.Decrease();
							player_power.Render(g_screen);
							continue;
						}
						else
						{
							if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
							{
								p_threat->Free();
								close();
								SDL_Quit();
								return 0;
							}

						}

					}



				}
			}

			int frame_exp_width = exp_threat.get_frame_width();
			int frame_exp_height = exp_threat.get_frame_height();
			std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
			for (int r = 0; r < bullet_arr.size(); r++)
			{
				BulletObject* p_bullet = bullet_arr.at(r);

				if (p_bullet != NULL)
				{
					p_bullet->ChecktoMap(map_data);
					if (p_bullet->get_is_move() == false)
					{
						p_player.RemoveBullet(r);
					}
				}

				if (p_bullet != NULL)
				{
					for (int t = 0; t < threats_list.size(); t++)
					{
						ThreatsObject* obj_threat = threats_list.at(t);
						if (obj_threat != NULL)
						{
							SDL_Rect tRect;
							tRect.x = obj_threat->GetRect().x;
							tRect.y = obj_threat->GetRect().y;
							tRect.w = obj_threat->get_width_frame();
							tRect.h = obj_threat->get_height_frame();

							/*if (p_player.GetXpos() > obj_threat->get_x_pos())
							{
								obj_threat->LoadImg("img//threat_level_right.png", g_screen);
							}
							else
							{
								obj_threat->LoadImg("img//threat_level_left.png", g_screen);
							}*/

							SDL_Rect bRect = p_bullet->GetRect();

							bool bCol = SDLCommonFunction::CheckCollision(bRect, tRect);

							if (bCol)
							{
								mark_value++;
								for (int ex = 0; ex < 8; ex++)
								{
									int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.5;
									int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

									exp_threat.set_frame(ex);
									exp_threat.SetRect(x_pos, y_pos);
									exp_threat.Show(g_screen);
								}

								p_player.RemoveBullet(r);
								obj_threat->Free();
								threats_list.erase(threats_list.begin() + t);
							}
						}
					}
				}
			}

			if (time == true && tmp_time == true)
			{
				std::string str_time = "Time: ";
				Uint32 time_val = SDL_GetTicks() / 1000;
				Uint32 val_time = 150 - time_val + time_menu + pause_time;
				if (val_time <= 0)
				{
					if (MessageBox(NULL, L"GAME OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
					{
						is_quit = true;
						break;
					}
				}
				else
				{
					std::string str_val = std::to_string(val_time);
					str_time += str_val;

					time_game.SetText(str_time);
					time_game.LoadFromRenderText(font_time, g_screen);
					time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15);
				}
			}
			

			//mark_value += p_player.GetMoneyCount();

			std::string val_str_mark = std::to_string(mark_value);
			std::string strMark("Mark: ");

			strMark += val_str_mark;

			mark_game.SetText(strMark);
			mark_game.LoadFromRenderText(font_time, g_screen);
			mark_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 50, 15);

			int money_count = p_player.GetMoneyCount();
			std::string money_str = std::to_string(money_count);

			money_game.SetText(money_str);
			money_game.LoadFromRenderText(font_time, g_screen);
			money_game.RenderText(g_screen, SCREEN_WIDTH * 0.5 - 250, 15);

			SDL_RenderPresent(g_screen);

			int real_imp_time = fps_timer.get_ticks();
			int time_one_frame = 1000 / FRAMER_PER_SECOND;

			if (real_imp_time < time_one_frame)
			{
				int delay_time = time_one_frame - real_imp_time;
				if (delay_time >= 0)
					SDL_Delay(delay_time);
			}


			if (p_player.GetXpos() >= game_map.GetMaxx(map_data) - 64)
			{
				if (MessageBox(NULL, L"YOU WIN", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
				{
					close();
					SDL_Quit();
					return 0;
				}
			}

		}
	}

	Mix_HaltMusic();
	Mix_FreeMusic(g_music);
	Mix_CloseAudio();


	for (int i = 0; i < threats_list.size(); i++)
	{
		ThreatsObject* p_threat  = threats_list.at(i);
		if (p_threat)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}

	threats_list.clear();

	close();
	return 0;

}