LIBDIR = /usr/local/lib

libmemhook.so.1.0: memhook.o
	g++ -shared -Wl,-soname,libmemhook.so.1 -o libmemhook.so.1.0 memhook.o -ldl

memhook.o : memhook.cpp memhook.h
	g++ -fPIC -c -Wall memhook.cpp

clean:
	rm memhook.o

install:
	sudo mv -u libmemhook.so.1.0 $(LIBDIR)
	sudo ln -sf $(LIBDIR)/libmemhook.so.1.0 $(LIBDIR)/libmemhook.so.1
	sudo ln -sf $(LIBDIR)/libmemhook.so.1 $(LIBDIR)/libmemhook.so