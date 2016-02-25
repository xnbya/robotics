
PARALLAX_FOLDER=/opt/parallax
SIMPLE_LIBRARY=${HOME}/dev/robotics/lib
PORT=/dev/ttyUSB0

PROPGCC=${PARALLAX_FOLDER}/bin/propeller-elf-gcc
PROPLOAD=${PARALLAX_FOLDER}/bin/propeller-load
PROPDUMP=${PARALLAX_FOLDER}/bin/propeller-elf-objdump
LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/Utility/libsimpletools -L ${SIMPLE_LIBRARY}/Utility/libsimpletools/cmm/ -I ${SIMPLE_LIBRARY}/TextDevices/libsimpletext -L ${SIMPLE_LIBRARY}/TextDevices/libsimpletext/cmm/ -I ${SIMPLE_LIBRARY}/Protocol/libsimplei2c -L ${SIMPLE_LIBRARY}/Protocol/libsimplei2c/cmm/ -I ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabdrive -L ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabdrive/cmm -I ${SIMPLE_LIBRARY}/TextDevices/libfdserial -L ${SIMPLE_LIBRARY}/TextDevices/libfdserial/cmm -I ${SIMPLE_LIBRARY}/Sensor/libping -L ${SIMPLE_LIBRARY}/Sensor/libping/cmm -I ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabcalibrate -L ${SIMPLE_LIBRARY}/Robotics/ActivityBot/libabcalibrate/cmm -I ${SIMPLE_LIBRARY}/Motor/libservo -L ${SIMPLE_LIBRARY}/Motor/libservo/cmm
# LIBS=-I . -L . -I ${SIMPLE_LIBRARY}/**/* -L ${SIMPLE_LIBRARY}/**/*/cmm

CFLAGS=-mcmm -m32bit-doubles -std=c99

compileCalcPos: librobo.c
	$(PROPGCC) $(LIBS) -Os -mcmm -m32bit-doubles -std=c99 -c calcPos.c -lm -o cmm/calcPos.o
	
compileLibrobo: librobo.c
	$(PROPGCC) $(LIBS) -Os -mcmm -m32bit-doubles -std=c99 -c librobo.c -o cmm/librobo.o

loadProg: ${FILENAME}.c
	rm -r cmm
	mkdir cmm
	make compileLibrobo
	make compileCalcPos
	$(PROPGCC) $(LIBS) -o cmm/${FILENAME}.elf $(CFLAGS) cmm/librobo.o ${FILENAME}.c -lm -lsimpletools -lsimpletext -lsimplei2c -labdrive -lfdserial -lm -lsimpletools -lsimpletext -lsimplei2c -labdrive -lping -labcalibrate -lservo -lm -lsimpletools -lsimpletext -lsimplei2c -lm -lsimpletools -lsimpletext -lm -lsimpletools -lm
	$(PROPLOAD) -e -r -t cmm/${FILENAME}.elf
	#$(PROPDUMP) -h cmm/${FILENAME}.elf
	#$(PROPLOAD) -Dreset=dtr -I ${PARALLAX_FOLDER}/propeller-load -b RCFAST cmm/${FILENAME}.elf -r -p ${PORT} -t115200
