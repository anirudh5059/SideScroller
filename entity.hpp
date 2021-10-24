#include<SDL.h>
#include"obstacle.hpp"
#include<vector>

#ifndef __MDENTITY_H__
#define __MDENTITY_H__

class MDEntity
{
  public:
    // Maximum axial velocity of entity
    static const int AX_VEL = 5;
    MDEntity( int _x, int _y, int _min_radius, int _max_radius );
    void render( SDL_Renderer* renderer);
    // Event handler for key presses
    void handle_event( SDL_Event& e );
    // TODO: Add collision detection logic to move function
    bool move( int max_width, int max_height, std::vector< Obstacle > obstacles );
    bool check_collision( SDL_Rect& rect);
    void inc_rad();
    void dec_rad();
    int get_min_rad();

  private:
    int max_rad;
    int min_rad;
    int cur_rad;
    int cen_x;
    int cen_y;
    int vel_x;
    int vel_y;
};
#endif
