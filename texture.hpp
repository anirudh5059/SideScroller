#include<SDL.h>
#include<string>

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
  public:
    Texture( SDL_Renderer* lRenderer );
    ~Texture();
    bool load_path( std::string path, bool color_key = false );
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
