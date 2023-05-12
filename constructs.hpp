#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL.h>
#include<memory>
#include<string>
#include<vector>
#include "score.hpp"
#include "entity.hpp"
#include "obstacle.hpp"
#include "texture.hpp"
#include "consts.hpp"

#ifndef __CONSTRUCTS__
#define __CONSTRUCTS__

class GameConstructs {
  private:
    std::unique_ptr<SDL_Window, void (*) ( SDL_Window* )> window;
    // Obstacle obstacles[ MAX_OBSTCLS ];
    // Pixie pixies[ MAX_PIXIES ];
  public:
    std::shared_ptr<SDL_Renderer> renderer;
    MDEntity ent;
    std::vector< Obstacle > obstacles;
    std::vector< Pixie > pixies;
    int obs_idx;
    int pix_idx;
    ObjectTexture background; 
    Score score;
    SDL_Rect screen_rect;
    GameConstructs();
    ~GameConstructs();
    void init( bool, bool, std::string, std::string, std::string, std::string );
    void generate_environment( bool );
    void move_environment();
    void move_entity();
    void render();
};

#endif
