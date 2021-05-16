#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H

    #include <essentials.h>
    #include "model_functions.h"
    void read_level_from_file(int layout[MAP_DIM_Y][MAP_DIM_X], char filepath[]);
    void place_walls(int layout[MAP_DIM_Y][MAP_DIM_X], struct model_data walls[MAP_DIM_X*MAP_DIM_Y]);

#endif
