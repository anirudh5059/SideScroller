#include<SDL.h>
#include<string>
#include<memory>
#include<SDL_ttf.h>

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class Texture
{
  public:
    std::unique_ptr<SDL_Texture, void (*)( SDL_Texture* )> texture;
    Texture( std::shared_ptr<SDL_Renderer> renderer );
    std::shared_ptr<SDL_Renderer> renderer;
    void free();
    void render( int x, int y, SDL_Rect* clip, SDL_Rect* out );
    void texture_from_surface( std::unique_ptr<SDL_Surface,
            void (*)( SDL_Surface* )> surf );
    int get_width();
    int get_height();
    int width;
    int height;
  private:
};

class ObjectTexture : public Texture
{
  public:
    ObjectTexture( std::shared_ptr<SDL_Renderer> renderer );
    void load_path( std::string path, int color_key = 0 );
};

class FontTexture : public Texture
{
  public:
    FontTexture( std::shared_ptr<SDL_Renderer> renderer );
    void load_text( std::string texture_text,
                                  SDL_Color text_color, TTF_Font* font );
};

#endif
