
GCC=g++
## ~/Software/SFML-2.5.1-linux-gcc-64-bit/lib

timber: sfml_dep
	$(GCC) -c main.cpp
	$(GCC) main.o -o timber -L${SFML_LIB_PATH} -lsfml-graphics -lsfml-window -lsfml-system

sfml_dep:
	if [ "${SFML_LIB_PATH}" = "" ]; then echo "need to export SFML_LIB_PATH"; exit 1; fi

clean:
	rm main.o timber 2>&1 1>/dev/null || true