 
Start by following this guide BOTH on host AND target!!
http://wiringpi.com/download-and-install/

When this is up and running, copy the folder "for_pi" somewhere on your PC (or just use the repo) and include this path in your Makefile with -I and -L for ARGH=target
For instance:
INCLUDE	= -I. -I/home/stud/for_PI/include
LDFLAGS	= -L/home/stud/for_PI/lib



For ARCH=host, you need only need:
INCLUDE	= -I. -I/usr/local/include
LDFLAGS	= -L/usr/local/lib