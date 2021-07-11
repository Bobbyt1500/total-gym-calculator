cpu_freq = 16000000
mcu = atmega328p
port = /dev/ttyUSB0
baudrate = 115200

.PHONY: flash clean

flash: main.hex
	avrdude -c arduino -p ${mcu} -P ${port} -b 115200 -U flash:w:main.hex

main.hex: main.out
	avr-objcopy -O ihex -R .eeprom main.out main.hex

main.out: main.c sevenSeg.c
	avr-gcc -Os -DF_CPU=${cpu_freq}UL -mmcu=${mcu} -o main.out main.c sevenSeg.c

clean:
	rm main.out main.hex