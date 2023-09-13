build:
	rm -f app.out
	g++ -std=c++20 -Wall application.cpp dist.cpp osm.cpp tinyxml2.cpp -o app.out

run:
	./app.out

buildtest:
	rm -f testing.exe
	g++ -std=c++11 -Wall testing.cpp -o testing.exe

runtest:
	./testing.exe

clean:
	rm -f application.exe	

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./application.exe
