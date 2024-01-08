test: test.cpp
	g++ test.cpp second.cpp -Iinclude -Llib -lraylib -o test




clean: 
	rm *.o test
