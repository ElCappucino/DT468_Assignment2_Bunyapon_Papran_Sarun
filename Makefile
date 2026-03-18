
sdl = -Isdl2.3\include\SDL2 -Lsdl2.3\lib -lSDL2main -lSDL2
sdlttf = -Isdl_ttf\include\SDL2 -Lsdl_ttf\lib -lSDL2_ttf
sdlimage = -Isdl_image\include -Lsdl_image\lib -lSDL2_image
sdlmixer = -Isdl_mixer\include -Lsdl_mixer\lib -lSDL2_mixer


mine: 
	g++ minesweeper2.cpp grid.cpp minesweepertable.cpp -lmingw32 $(sdl) $(sdlttf) -omine.exe



clean:
	del *.o *.exe
