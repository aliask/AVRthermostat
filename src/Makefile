CC = avr-gcc
STRIP = avr-strip
OBJCOPY = avr-objcopy
AVRDUDE = avrdude -c usbtiny -p attiny25

CFLAGS = -mmcu=attiny25 -DF_CPU=8000000 -O

main.o: main.c
	$(CC) $(CFLAGS) $? -o $@
	$(STRIP) $@

main.hex: main.o
	$(OBJCOPY) -O ihex $? $@

program: main.hex
	$(AVRDUDE) -U flash:w:main.hex:i

clean:
	rm *.o
