== Libraries ==

	Following is based off of Ubuntu 11.10 (oneirc).
	
	Ubuntu:
		Since the lua5.1-socket.pc file doesn't do this for you:
			make ADD_LDFLAGS=-llua5.1-mime 

	sudo apt-get install \
			libsdl-dev libopenal-dev \
			liblua5.1-0-dev liblua5.1-socket-dev

	Still on enet 1.2.5:
			wget http://enet.bespin.org/download/enet-1.2.5.tar.gz
			tar -xvzf enet-1.2.5.tar.gz
			cd enet-1.2.5
			./configure && sudo make install

	MinGW Libraries:
		sudo apt-get install mingw32
  	wget -O - http://fly.thruhere.net/download/extras/mingw-deps-linux.tgz |
		tar -xvz


== Building OpenAL (optional) ==

	Some people may chose to install latest OpenAL.

		http://kcat.strangesoft.net/openal.html

	To build:

		cd build
		cmake .. -DCMAKE_INSTALL_PREFIX=/usr
		make
		sudo make install

== Profiling ==

	To generate profiling output run, "gprof ./projectx-binary ./gmon.out > gmon.txt"


== Debugging ==

	Use gdb as you normally would:

		gdb --args ./projectx-debug ...
		r # to start
		bt # to backtrace

