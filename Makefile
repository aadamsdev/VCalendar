caltool: caltool.c caltool.h calutil.c calutil.h
	gcc -g -Wall -std=c11 -DNDEBUG -o caltool caltool.c calutil.c
	gcc -c -g -Wall -std=c11 -DNDEBUG `pkg-config --cflags python3` -fPIC wrapper.c caltool.c calutil.c
	gcc -shared -fPIC -o Cal.so *.o
	

clean:
	rm -f *.o caltool Cal.so
