
project_dir = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
external_libs_dir = $(project_dir)External_libs/
code_dir = $(project_dir)Code/

all : external_libs main

external_libs :
	rm -rf $(external_libs_dir) && wget -qO- --user-agent="Mozilla/5.0 (X11; Linux i686; rv:10.0) Gecko/20100101 Firefox/10.0" https://www.computer-graphics.se/common.tar.xz | tar xJ -C $(project_dir) && mv $(project_dir)common/Linux/MicroGlut.c /$(project_dir)common/MicroGlut.c && mv $(project_dir)common/Linux/MicroGlut.h $(project_dir)common/MicroGlut.h && rm -r $(project_dir)common/Windows $(project_dir)common/Mac $(project_dir)common/Linux $(project_dir)common/obsolete && mv $(project_dir)common $(external_libs_dir)
	

main : $(code_dir)main.c $(external_libs_dir)GL_utilities.c $(external_libs_dir)VectorUtils3.c $(external_libs_dir)LittleOBJLoader.c $(external_libs_dir)LoadTGA.c $(external_libs_dir)MicroGlut.c
	gcc -Wall -o $(project_dir)3D_project.out -I$(external_libs_dir) -DGL_GLEXT_PROTOTYPES $(code_dir)main.c $(external_libs_dir)GL_utilities.c $(external_libs_dir)LittleOBJLoader.c $(external_libs_dir)VectorUtils3.c $(external_libs_dir)LoadTGA.c $(external_libs_dir)MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm main
	rm external_libs
