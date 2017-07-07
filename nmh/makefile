nmh.app : nm_host.o thread_safe_queue.o
	g++ -g -o nmh.app nm_host.o thread_safe_queue.o -std=c++11

nm_host.o : nm_host.cpp
	g++ -c -g -std=c++11 nm_host.cpp

thread_safe_queue.o : thread_safe_queue.cpp
	g++ -c -g -std=c++11 thread_safe_queue.cpp

clear : 
	rm *.o