#ifndef MAP_FUNCTIONS_H
#define MAP_FUNCTIONS_H

    #include <essentials.h>
    #include "model_data_functions.h"
    void read_level_from_file(int layout[dim_y][dim_x], char filepath[]);
    void place_walls(int layout[dim_y][dim_x], struct model_data walls[dim_x*dim_y]);

#endif
