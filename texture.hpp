#include<SDL.h>
#include<string>
#include<SDL_ttf.h>

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
  public:
    Texture( SDL_Renderer* lRenderer );
    ~Texture();
    bool load_path( std::string path, int color_key = 0 );
    bool load_from_rendered_text( std::string& texture_text,
                                  SDL_Color& text_color, TTF_Font* font );
    void free();
    void render( int x, int y, SDL_Rect* clip, SDL_Rect* out );
    int get_width();
    int get_height();
  private:
    SDL_Texture* raw_texture;
    SDL_Renderer* renderer;
    int width;
    int height;

};

#endif
