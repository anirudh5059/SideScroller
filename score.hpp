#include<SDL.h>
#include<SDL_ttf.h>
#include<string>
#include "texture.hpp"

#ifndef __SCORE_H__
#define __SCORE_H__
class Score {
  public:
    std::unique_ptr< TTF_Font, void (*)( TTF_Font * ) > font;
    long long value;
    Score( std::shared_ptr<SDL_Renderer> renderer );
    FontTexture font_tex; 
    void init_font( std::string );
};
#endif
