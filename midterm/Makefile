
PARALLAX_FOLDER=/opt/parallax
SIMPLE_LIBRARY=${HOME}/dev/robo/lib
PORT=/dev/ttyUSB0

PROPGCC=${PARALLAX_FOLDER}/bin/propeller-elf-gcc
PROPLOAD=${PARALLAX_FOLDER}/bin/propeller-load
PROPDUMP=${PARALLAX_FOLDER}/bin/propeller-elf-objdump
LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/Utility/libsimpletools -L ${SIMPLE_LIBRARY}/Utility/libsimpletools/cmm/ -I ${SIMPLE_LIBRARY}/TextDevices/libsimpletext -L ${SIMPLE_LIBRARY}/TextDevices/libsimpletext/cmm/ -I ${SIMPLE_LIBRARY}/Protocol/libsimplei2c -L ${SIMPLE_LIBRARY}/Protocol/libsimplei2c/cmm/ -I ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabdrive -L ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabdrive/cmm -I ${SIMPLE_LIBRARY}/TextDevices/libfdserial -L ${SIMPLE_LIBRARY}/TextDevices/libfdserial/cmm -I ${SIMPLE_LIBRARY}/Sensor/libping -L ${SIMPLE_LIBRARY}/Sensor/libping/cmm -I ${SIMPLE_LIBRARY}/Motor/libservo -L ${SIMPLE_LIBRARY}/Motor/libservo/cmm
# LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/**/* -L ${SIMPLE_LIBRARY}/**/*/cmm

CFLAGS= -no-pie -mcmm -fno-exceptions -Wl,-gc-sections,-Map=map.txt -m32bit-doubles -std=c99 -Os -Wall

compileCalcPos: calcPos.c
	$(PROPGCC) $(LIBS) $(CFLAGS) -c calcPos.c -o cmm/calcPos.o
	
compileLibrobo: librobo.c
	$(PROPGCC) $(LIBS) $(CFLAGS) -c librobo.c -o cmm/librobo.o

readDist: read.c
	$(PROPGCC) $(LIBS) -o cmm/read.elf $(CFLAGS) read.c -lsimpletools -lsimpletext -lsimplei2c  -lfdserial -lsimpletools -lsimpletext -lsimplei2c -labdrive -lping -lm -lservo -lsimpletools -lsimpletext -lsimplei2c -lsimpletools -lsimpletext -lsimpletools 
	$(PROPLOAD) -r -t cmm/read.elf


loadProg: ${FILENAME}.c
	rm -r cmm
	mkdir cmm
	make compileLibrobo
	make compileCalcPos
	$(PROPGCC) $(LIBS) -o cmm/${FILENAME}.elf $(CFLAGS) cmm/librobo.o cmm/calcPos.o ${FILENAME}.c -lsimplei2c -labdrive -lfdserial -lping -lservo -lsimpletools -lsimpletext -lsimplei2c -lsimpletools -lsimpletext -lsimpletools -lm 
	${PARALLAX_FOLDER}/bin/propeller-elf-size cmm/${FILENAME}.elf
	$(PROPLOAD) -e -r -t cmm/${FILENAME}.elf
	#$(PROPDUMP) -h cmm/${FILENAME}.elf
	#$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load -b RCFAST cmm/${FILENAME}.elf -r -p ${PORT} -t115200
