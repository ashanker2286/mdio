#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/io.h>
#include <string.h>
#include <errno.h>

#define LPC_IO_ADDR 0x100
#define LPC_IO_NUMS 0xc0
#define MDIO_CMD_ADDR 0x06

#define MDIO_DELAY_TIME 10000
#define MDIO_DELAY_COUNT 100

#define MDIO_BUS_BUSY_SHIFT (1)
#define MDIO_BUS_BUSY_MASK (0x1 << MDIO_BUS_BUSY_SHIFT)

#define MDIO_BUSY_ERROR (-1)
#define MDIO_TIMEOUT_ERROR (-2)
#define MDIO_WR_RD_ERROR (-3)


int main(int argc, char **argv) {
	int reg;
	int delay = 0;
	unsigned char temp = 0;
	int color;

	if (argc < 3) {
		printf("Invalid number of argument\n");
		return 0;
	}

	reg = (int)strtoul(argv[1], NULL, 16);
	printf("reg address:%d\n", reg);
	color = (int)strtoul(argv[2], NULL, 16);
	

	if (ioperm(LPC_IO_ADDR, LPC_IO_NUMS, 1)) {
		fprintf(stderr, "Error: ioperm setting Failed!\n");
		return -1;
	}
	
        while(inb(LPC_IO_ADDR + reg) & MDIO_BUS_BUSY_MASK) {
                if(delay++ < MDIO_DELAY_COUNT)
                        usleep(MDIO_DELAY_TIME);
                else {
                        return MDIO_BUSY_ERROR;
                }
        }

	temp = inb(LPC_IO_ADDR + reg);
	printf("Temp: %x\n", temp);

	temp = color & 0xff;
	outb(temp, LPC_IO_ADDR + reg);

	temp = inb(LPC_IO_ADDR + reg);
	printf("Temp: %x\n", temp);

        if(ioperm(LPC_IO_ADDR, LPC_IO_NUMS, 0)) {
                fprintf(stderr, "Error: ioperm release Failed!\n");
                return -1; 
        }

	return 0;
}
