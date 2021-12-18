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
    static const int RAD_VEL = 3;
    MDEntity( int _min_radius, int _max_radius );
    void populate( int x, int y, int max_width, int max_height );
    void render( SDL_Renderer* renderer);
    // Event handler for key presses
    void handle_event( SDL_Event& e );
    // TODO: Add collision detection logic to move function
    int move( int max_width, int max_height, Obstacle obstacles[],
               int num_obstcls, Pixie pixies[], int num_pixies );
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
