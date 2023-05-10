#include<SDL_ttf.h>
#include<string>
#include "score.hpp"

void delete_font( TTF_Font* font )
{
    TTF_CloseFont( font );
}

Score::Score( std::shared_ptr<SDL_Renderer> renderer ): font_tex( renderer ),
    font( nullptr, delete_font ) {}

void Score::init_font( std::string path_font )
{
    font = std::unique_ptr< TTF_Font, void (*)( TTF_Font * ) >( TTF_OpenFont(
            path_font.c_str(), 12 ), delete_font );
}
