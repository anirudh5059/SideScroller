#include<SDL.h>
#include"obstacle.hpp"
#include<vector>
#include "consts.hpp"

#ifndef __MDENTITY_H__
#define __MDENTITY_H__

class MDEntity
{
  public:
    // Maximum axial velocity of entity
    int AX_VEL = ENT_AX_VEL;
    int RAD_VEL = ENT_RAD_VEL;
    MDEntity( int, int, int, int, int, int );
    void populate( int x, int y, int max_width, int max_height );
    void render( std::shared_ptr<SDL_Renderer> renderer);
    // Event handler for key presses
    void handle_event( SDL_Event& e );
    // TODO: Add collision detection logic to move function
    int move( int max_width, int max_height, std::vector<Obstacle>& obstacles,
              std::vector<Pixie>& pixies );
    bool check_collision_obstcl( Obstacle& obstacle );
    bool check_collision_pixie( Pixie& pixie);
    int get_min_rad();
    int get_cen_x();
    int get_cen_y();
    int get_cur_rad();

  private:
    int rad_inc_rate;
    int max_rad;
    int min_rad;
    int cur_rad;
    int cen_x;
    int cen_y;
    int vel_x;
    int vel_y;
};
#endif
