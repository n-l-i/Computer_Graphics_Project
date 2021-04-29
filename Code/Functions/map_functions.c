#include "map_functions.h"

void read_level_from_file(int layout[dim_y][dim_x], char filepath[])
{
    int rows = dim_y;
    int cols = dim_x;
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

void place_walls(int layout[dim_y][dim_x], struct model_data walls[dim_x*dim_y])
{
    mat4 import_rot, import_trans, import_scale, importMatrix;
    mat4 rot, trans, scale, transformationMatrix;
    trans = T(0.0f, 0.0f, 0.0f);
    rot = Rx(0.0f);
    scale = S(1.0f, 1.0f, 1.0f);
    transformationMatrix = Mult(trans,Mult(rot, scale));
    char model_path[PATH_MAX];
    char tex_path[PATH_MAX];

    // init wall models
    strcpy(model_path, "Data/Models/Wall/wall.obj");
    strcpy(tex_path, "Data/Textures/No_texture/no_texture.tga");
    int texScale = 50;
    int isShaded = 1;
    int specExp = 0;

    int rows = dim_y;
    int cols = dim_x;
    int wall_index = 0;
    import_scale = S(1.0f, 1.0f, 1.0f);

    // placing all walls marked in the layout file
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            int val = layout[col][row];
            if (val > 0)
            {
                if (val == 1) { import_trans = T(2*(col+0)-(cols), 0, 2*(row+0)-(rows)); }
                if (val == 2) { import_trans = T(2*(col+1)-(cols), 0, 2*(row+0)-(rows)); }
                if (val == 3) { import_trans = T(2*(col+1)-(cols), 0, 2*(row+1)-(rows)); }
                if (val == 4) { import_trans = T(2*(col+0)-(cols), 0, 2*(row+1)-(rows)); }
                import_rot = Ry((val*0.75f)*M_PI*2);
                importMatrix = Mult(import_trans,Mult(import_rot, import_scale));

                walls[wall_index] = init_model_data(walls[wall_index],model_path,tex_path,importMatrix,
                transformationMatrix,texScale,isShaded,specExp);

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
                    if (neighbour_behind_val == 1) { import_trans = T(2*(col+0)-(cols), 0, 2*(row+0)-(rows)); }
                    if (neighbour_behind_val == 2) { import_trans = T(2*(col+1)-(cols), 0, 2*(row+0)-(rows)); }
                    if (neighbour_behind_val == 3) { import_trans = T(2*(col+1)-(cols), 0, 2*(row+1)-(rows)); }
                    if (neighbour_behind_val == 4) { import_trans = T(2*(col+0)-(cols), 0, 2*(row+1)-(rows)); }
                    import_rot = Ry((neighbour_behind_val*0.75f)*M_PI*2);
                    importMatrix = Mult(import_trans,Mult(import_rot, import_scale));

                    walls[wall_index] = init_model_data(walls[wall_index],model_path,tex_path,importMatrix,
                    transformationMatrix,texScale,isShaded,specExp);

                    wall_index++;
                }
            }
        }
    }
}
