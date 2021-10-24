#include<SDL.h>
#include<string>

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
    Texture( SDL_Renderer* lRenderer );
    ~Texture();
    bool loadPath( std::string path );
    void free();
    void render( int x, int y, SDL_Rect* clip );
    int get_width();
    int get_height();

  public:
    SDL_Texture* raw_texture;
    SDL_Renderer* renderer;
    int width;
    int height;

};

#endif
