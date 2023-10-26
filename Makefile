# wsdl2h -o onvif.h \
#   http://www.onvif.org/onvif/ver10/device/wsdl/devicemgmt.wsdl \
#   http://www.onvif.org/onvif/ver10/events/wsdl/event.wsdl \
#   http://www.onvif.org/onvif/ver10/deviceio.wsdl \
#   http://www.onvif.org/onvif/ver20/imaging/wsdl/imaging.wsdl \
#   http://www.onvif.org/onvif/ver20/media/wsdl/media.wsdl \
#   http://www.onvif.org/onvif/ver20/ptz/wsdl/ptz.wsdl \
#   http://www.onvif.org/onvif/ver10/network/wsdl/remotediscovery.wsdl \
#   http://www.onvif.org/ver10/advancedsecurity/wsdl/advancedsecurity.wsdl


# #import "wsdd5.h"
# #import "wsse.h"

# soapcpp2 -2 -C -I ~/Downloads/gsoap-2.8/gsoap -j -x  onvif.h


# g++ -g -o onvifserver -Wall -DWITH_OPENSSL -DWITH_DOM -DWITH_ZLIB\
#   -I. -I ~/gsoap-2.8/gsoap/plugin -I ~/gsoap-2.8/gsoap/custom -I ~/gsoap-2.8/gsoap \
#   main.cpp \
#   soapC.cpp \
#   soapServer.cpp \
#   stdsoap2.cpp \
#   ~/gsoap-2.8/gsoap/dom.cpp \
#   ~/gsoap-2.8/gsoap/plugin/smdevp.c \
#   ~/gsoap-2.8/gsoap/plugin/mecevp.c \
#   ~/gsoap-2.8/gsoap/plugin/wsaapi.c \
#   ~/gsoap-2.8/gsoap/plugin/wsseapi.c \
#   ~/gsoap-2.8/gsoap/custom/struct_timeval.c \
#   -lcrypto -lssl -lz












#This is an easier to use and modify makefile, but it is slightly more difficult to read than the simple one:
#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
# CFLAGS = -Wall -g 
CFLAGS = -DWITH_OPENSSL -Wall -g -DWITH_DOM -DWITH_ZLIB
# define any directories containing header files other than /usr/include
#
# INCLUDES = -I/home/newhall/include  -I../include
INCLUDES = -I.
# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
# LFLAGS = -L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
# LIBS = -lmylib -lm
LIBS = -lcrypto -lssl -lz -ljsoncpp -lpthread -std=c++11


OBJS = main.cpp
OBJS += include/onvifApi/onvifApi.o
OBJS += libOnvif/soapC.o
OBJS += libOnvif/wsddClient.o
OBJS += libOnvif/wsddServer.o
OBJS += libOnvif/soapAdvancedSecurityServiceBindingProxy.o
# OBJS += libOnvif/soapCredentialBindingProxy.o
OBJS += libOnvif/soapDeviceBindingProxy.o
# OBJS += libOnvif/soapDeviceIOBindingProxy.o
OBJS += libOnvif/soapImagingBindingProxy.o
OBJS += libOnvif/soapMedia2BindingProxy.o
OBJS += libOnvif/soapPTZBindingProxy.o
OBJS += libOnvif/soapPullPointSubscriptionBindingProxy.o
# OBJS += libOnvif/soapReceiverBindingProxy.o
# OBJS += libOnvif/soapRecordingBindingProxy.o
OBJS += libOnvif/soapwsddProxy.o
OBJS += libOnvif/soapRemoteDiscoveryBindingProxy.o
# OBJS += libOnvif/soapReplayBindingProxy.o
# OBJS += libOnvif/soapRuleEngineBindingProxy.o
# OBJS += libOnvif/soapScheduleBindingProxy.o
OBJS += libOnvif/stdsoap2.cpp
OBJS += libOnvif/dom.o
OBJS += libOnvif/smdevp.o
OBJS += libOnvif/mecevp.o
OBJS += libOnvif/wsaapi.o
OBJS += libOnvif/wsseapi.o
OBJS += libOnvif/wsddapi.o
OBJS += libOnvif/struct_timeval.o


# define the executable file 
MAIN = onvif_client

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:	$(MAIN)
	@echo  Simple compiler named onvif client has been compiled

$(MAIN):	$(OBJS) 
	$(CC) -o $(MAIN) $(CFLAGS) $(INCLUDES) $(OBJS) $(LFLAGS) $(LIBS)

# main.o: src/main.cpp
# 	$(CC) $(CFLAGS) $(INCLUDES)  -c src/main.cpp

# soapC.o: libOnvif/soapC.cpp libOnvif/soapH.h
# 	$(CC) $(CFLAGS) -c libOnvif/soapC.cpp


# wsddClient.o: libOnvif/wsddClient.cpp libOnvif/wsddH.h
# 	$(CC) $(CFLAGS) -c libOnvif/wsddClient.cpp

# wsddServer.o: libOnvif/wsddServer.cpp libOnvif/wsddH.h
# 	$(CC) $(CFLAGS) -c libOnvif/wsddServer.cpp


# stdsoap2.o: libOnvif/stdsoap2.cpp libOnvif/stdsoap2.h
# 	$(CC) $(CFLAGS) $(LIBS) -c libOnvif/stdsoap2.cpp

# dom.o: libOnvif/dom.cpp libOnvif/stdsoap2.h
# 	$(CC) $(CFLAGS) -c libOnvif/dom.cpp

# # smdevp.o: libOnvif/smdevp.h
# smdevp.o: libOnvif/smdevp.c libOnvif/smdevp.h libOnvif/stdsoap2.h
# 	$(CC) $(CFLAGS) -c libOnvif/smdevp.c

# mecevp.o: libOnvif/mecevp.h

# wsaapi.o: libOnvif/wsaapi.h

# wsseapi.o: libOnvif/wsseapi.c libOnvif/wsseapi.h libOnvif/threads.h libOnvif/wsaapi.h libOnvif/stdsoap2.h
# 	$(CC) $(CFLAGS) -c libOnvif/wsseapi.c

# wsddapi.o: libOnvif/wsddapi.h

# struct_timeval.o: libOnvif/struct_timeval.c libOnvif/soapH.h
# 	$(CC) $(CFLAGS) -c libOnvif/struct_timeval.c



# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<  -o $@

# .cpp.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -cpp $<  -o $@

clean:
	$(RM) *.o libOnvif/*.o *~ $(MAIN)

clean_main:
	$(RM) *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

