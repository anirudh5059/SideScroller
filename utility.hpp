#include<SDL.h>
#include<memory>
#include"entity.hpp"
#include"obstacle.hpp"

#ifndef __UTILITY__
#define __UTILITY__

void draw_circle( std::shared_ptr<SDL_Renderer> renderer, MDEntity& ent );
void draw_circle( std::shared_ptr<SDL_Renderer> renderer, Circle circ );
#endif
