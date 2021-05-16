#include "map_functions.h"

void read_level_from_file(int layout[MAP_DIM_Y][MAP_DIM_X], char filepath[])
{
    int rows = MAP_DIM_Y;
    int cols = MAP_DIM_X;
    int row=0;
    int col=0;
    char file_path[PATH_MAX];
    char *r_file = realpath("Data/Levels/Level_1/layout.txt", file_path);
    int c;
    FILE *file;
    file = fopen(file_path, "r");
    if (file)
    if (file)
    {
        _Bool commented_line = 0;
        while ((c = getc(file)) != EOF)
        {
            if (c == '/') { commented_line = 1; }
            if (c == '\n') { commented_line = 0; }
            if (!commented_line)
            {
                if (c == '.') { layout[row][col] = 0; row++; }
                if (c == '<') { layout[row][col] = 1; row++; }
                if (c == '^') { layout[row][col] = 2; row++; }
                if (c == '>') { layout[row][col] = 3; row++; }
                if (c == 'v') { layout[row][col] = 4; row++; }
                if (c == '\n' && row > 0) { row=0; col++; }
            }
        }
        fclose(file);
    }
}

void place_walls(int layout[MAP_DIM_Y][MAP_DIM_X], struct model_data walls[MAP_DIM_X*MAP_DIM_Y])
{
    mat4 import_rot, import_trans, import_scale, importMatrix;
    char model_path[PATH_MAX];
    char tex_path[PATH_MAX];

    // init wall models
    strcpy(model_path, "Data/Models/Wall/wall.obj");
    strcpy(tex_path, "Data/Textures/Wall/Plaster004.tga");
    int texScale = 5;
    int isShaded = 1;
    int specExp = 1;

    int rows = MAP_DIM_Y;
    int cols = MAP_DIM_X;
    int wall_index = 0;
    import_scale = S(MAP_SCALE*1.0f, MAP_SCALE*1.0f, MAP_SCALE*1.0f);

    int isLight = 0;
    struct light_data no_light;

    // placing all walls marked in the layout file
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int val = layout[col][row];
            if (val > 0)
            {
                vec3 corner_a,corner_b;
                vec3 position;
                if (val == 1)
                {
                    position = SetVector(MAP_SCALE*2*(col+0)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+0)-MAP_SCALE*(rows));
                    import_trans = T(position.x,position.y,position.z);
                }
                if (val == 2)
                {
                    position = SetVector(MAP_SCALE*2*(col+1)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+0)-MAP_SCALE*(rows));
                    import_trans = T(position.x,position.y,position.z);
                }
                if (val == 3)
                {
                    position = SetVector(MAP_SCALE*2*(col+1)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+1)-MAP_SCALE*(rows));
                    import_trans = T(position.x,position.y,position.z);
                }
                if (val == 4)
                {
                    position = SetVector(MAP_SCALE*2*(col+0)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+1)-MAP_SCALE*(rows));
                    import_trans = T(position.x,position.y,position.z);
                }

                GLfloat rotation = (val*0.75f)*M_PI*2;
                import_rot = Ry(rotation);
                importMatrix = Mult(import_trans,Mult(import_rot, import_scale));

                corner_a = SetVector(position.x,position.y,position.z);
                if (val == 4) { corner_b = SetVector(position.x+2*MAP_SCALE,position.y+2*MAP_SCALE,position.z-0.5*MAP_SCALE); }
                if (val == 1) { corner_b = SetVector(position.x+0.5*MAP_SCALE,position.y+2*MAP_SCALE,position.z+2*MAP_SCALE); }
                if (val == 2) { corner_b = SetVector(position.x-2*MAP_SCALE,position.y+2*MAP_SCALE,position.z+0.5*MAP_SCALE); }
                if (val == 3) { corner_b = SetVector(position.x-0.5*MAP_SCALE,position.y+2*MAP_SCALE,position.z-2*MAP_SCALE); }

                walls[wall_index] = init_model_data(walls[wall_index],
                                    model_path,tex_path,importMatrix,texScale,
                                    isShaded,specExp,isLight,no_light,corner_a,corner_b);

                wall_index++;
            }
        }
    }
    // checking if additional walls may need to be placed to create nice corners
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int val = layout[col][row];
            if (val > 0)
            {
                int neighbour_behind_val = -1;
                int row_diff, col_diff;
                if (val == 1) { row_diff = 0; col_diff = 1; }
                if (val == 2) { row_diff = 1; col_diff = 0; }
                if (val == 3) { row_diff = 0; col_diff = -1; }
                if (val == 4) { row_diff = -1; col_diff = 0; }
                if (col+col_diff < cols && col+col_diff>=0 && row+row_diff < rows && row+row_diff>=0)
                {
                    neighbour_behind_val = layout[col+col_diff][row+row_diff];
                }

                _Bool make_corner = neighbour_behind_val > 0 && abs(val - neighbour_behind_val) != 2;

                if (make_corner)
                {
                    vec3 corner_a,corner_b;
                    vec3 position;

                    if (neighbour_behind_val == 1)
                    {
                        position = SetVector(MAP_SCALE*2*(col+0)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+0)-MAP_SCALE*(rows));
                        import_trans = T(position.x,position.y,position.z);
                    }
                    if (neighbour_behind_val == 2)
                    {
                        position = SetVector(MAP_SCALE*2*(col+1)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+0)-MAP_SCALE*(rows));
                        import_trans = T(position.x,position.y,position.z);
                    }
                    if (neighbour_behind_val == 3) {
                        position = SetVector(MAP_SCALE*2*(col+1)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+1)-MAP_SCALE*(rows));
                        import_trans = T(position.x,position.y,position.z);
                    }
                    if (neighbour_behind_val == 4) {
                        position = SetVector(MAP_SCALE*2*(col+0)-MAP_SCALE*(cols), 0, MAP_SCALE*2*(row+1)-MAP_SCALE*(rows));
                        import_trans = T(position.x,position.y,position.z);
                    }
                    GLfloat rotation = (neighbour_behind_val*0.75f)*M_PI*2;
                    import_rot = Ry(rotation);
                    importMatrix = Mult(import_trans,Mult(import_rot, import_scale));

                    corner_a = SetVector(position.x,position.y,position.z);
                    if (neighbour_behind_val == 4) { corner_b = SetVector(position.x+2*MAP_SCALE,position.y+2*MAP_SCALE,position.z-0.5*MAP_SCALE); }
                    if (neighbour_behind_val == 1) { corner_b = SetVector(position.x+0.5*MAP_SCALE,position.y+2*MAP_SCALE,position.z+2*MAP_SCALE); }
                    if (neighbour_behind_val == 2) { corner_b = SetVector(position.x-2*MAP_SCALE,position.y+2*MAP_SCALE,position.z+0.5*MAP_SCALE); }
                    if (neighbour_behind_val == 3) { corner_b = SetVector(position.x-0.5*MAP_SCALE,position.y+2*MAP_SCALE,position.z-2*MAP_SCALE); }

                    walls[wall_index] = init_model_data(walls[wall_index],
                                        model_path,tex_path,importMatrix,
                                        texScale,isShaded,specExp,isLight,no_light,corner_a,corner_b);

                    wall_index++;
                }
            }
        }
    }
}
