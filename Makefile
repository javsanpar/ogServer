# makefile

# Nombre del proyecto
PROYECTO := ogAdmServer

# Directorio de instalación
INSTALL_DIR := /opt/opengnsys

# Opciones de compilacion
CFLAGS := -O0 -g -Wall -I../../Includes	# Depuracion
#CFLAGS := -O3 -Wall			# Optimizacion
CPPFLAGS := $(CFLAGS)

# Opciones de linkado
LBIT := $(shell getconf LONG_BIT)
ifeq ($(LBIT), 64)
    LDFLAGS := -L/usr/lib64 -L/usr/lib64/mysql -lpthread -lmysqlclient
else
    LDFLAGS := -L/usr/lib -L/usr/lib/mysql -lpthread -lmysqlclient
endif

# Ficheros objetos
OBJS := ../../Includes/Database.o sources/ogAdmServer.o 


all: $(PROYECTO)

$(PROYECTO): $(OBJS)
	g++ $(LDFLAGS) $(OBJS) -o $(PROYECTO)
#	strip $(PROYECTO)		# Optimizacion

install: $(PROYECTO)
	cp $(PROYECTO) $(INSTALL_DIR)/sbin
	cp $(PROYECTO).cfg $(INSTALL_DIR)/etc
 
clean:
	rm -f $(PROYECTO) $(OBJS)

uninstall: clean
	rm -f /usr/local/sbin/$(PROYECTO) /usr/local/etc/$(PROYECTO).cfg

sources/%.o: sources/%.cpp
	g++ $(CPPFLAGS) -c -o"$@" "$<"
	
sources/%.o: sources/%.c
	gcc $(CFLAGS) -c -o"$@" "$<"


