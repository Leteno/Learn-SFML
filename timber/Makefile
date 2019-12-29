
GCC=g++

timber:
	$(GCC) -c main.cpp
	$(GCC) main.o -o timber -L~/Software/SFML-2.5.1-linux-gcc-64-bit/lib -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm main.o timber 2>&1 1>/dev/null || true