
GCC=g++ -g
LIBRARY=-L${SFML_LIB_PATH} -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
## ~/Software/SFML-2.5.1-linux-gcc-64-bit/lib

zombie: sfml_dep player CreateBackground TextureHolder Zombie CreateHorde Bullet
	$(GCC) -c ZombieArena.cpp
	$(GCC) ZombieArena.o Player.o CreateBackground.o TextureHolder.o Zombie.o CreateHorde.o Bullet.o -o zombie $(LIBRARY)

player: Player.cpp Player.h
	$(GCC) -c Player.cpp

CreateBackground: CreateBackground.cpp
	$(GCC) -c CreateBackground.cpp

TextureHolder: TextureHolder.cpp TextureHolder.h
	$(GCC) -c TextureHolder.cpp

Zombie: Zombie.cpp Zombie.h
	$(GCC) -c Zombie.cpp

CreateHorde: CreateHorde.cpp
	$(GCC) -c CreateHorde.cpp

Bullet: Bullet.cpp Bullet.h
	$(GCC) -c Bullet.cpp

sfml_dep:
	if [ "${SFML_LIB_PATH}" = "" ]; then echo "need to export SFML_LIB_PATH"; exit 1; fi

clean:
	rm *.o zombie 2>&1 1>/dev/null || true