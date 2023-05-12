#include<SDL_ttf.h>
#include<string>
#include "score.hpp"
#include "consts.hpp"

void delete_font( TTF_Font* font )
{
    TTF_CloseFont( font );
}

Score::Score( std::shared_ptr<SDL_Renderer> renderer ): font_tex( renderer ),
    font( nullptr, delete_font ), text_rect{ 0, 0, SCORE_WIDTH, SCORE_HEIGHT } {};

void Score::init_font( std::string path_font )
{
    font = std::unique_ptr< TTF_Font, void (*)( TTF_Font * ) >( TTF_OpenFont(
            path_font.c_str(), 50 ), delete_font );
}

void Score::render()
{
    font_tex.load_text( pre_score + std::to_string( value ),
            SDL_Color{ 0xFF,0xFF,0xFF }, font.get() );
    font_tex.render( 0, 0, NULL, &text_rect );
}
