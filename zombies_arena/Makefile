
GCC=g++ -g
## ~/Software/SFML-2.5.1-linux-gcc-64-bit/lib

zombie: sfml_dep player
	$(GCC) -c ZombieArena.cpp
	$(GCC) ZombieArena.o Player.o -o zombie -L${SFML_LIB_PATH} -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

player: Player.cpp Player.h
	$(GCC) -c Player.cpp

sfml_dep:
	if [ "${SFML_LIB_PATH}" = "" ]; then echo "need to export SFML_LIB_PATH"; exit 1; fi

clean:
	rm *.o zombie 2>&1 1>/dev/null || true