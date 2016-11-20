#include <stdio.h>
#include "haptic.h";
using namespace Haptic;

int main(int argc, char* argv[])
{
	Init();

	HapticDev* dev = NULL;
	char buffer[128];
	int s;

	do
	{
		printf("%s> ", dev == NULL ? "" : dev->getDevName());
		s = scanf("%s", buffer);
		if(strcmp(buffer, "init") == 0)
		{
			int id = 0;
			scanf("%d", &id);
			if (dev)
				printf("Device initialized.\n");
			else
			{
				printf("Initializing dev %d...\n", id);
				dev = HapticDev::initDev(id);
			}
		}
		else if (strcmp(buffer, "support") == 0)
		{
			if (!dev)
				printf("Device is not initialized.\n");
			else 
				printf("%s\n", dev->getSupported());
		}
		else if (strcmp(buffer, "haptic") == 0)
		{
			short lvl = 0;
			int dur = 0;
			char mot = 'b';
			if (!dev)
				printf("Device is not initialized.\n");
			else
			{
				scanf(" %c %hd %d", &mot, &lvl, &dur);
				printf("haptic %s with %i level and duration of %dms.\n",
					mot == 'h' ? "hight motor" : mot == 'l' ? "low motor" : "both motors", lvl, dur);
				dev->haptic(mot == 'h' ? HMOTOR : mot == 'l' ? LMOTOR : HMOTOR | LMOTOR, lvl, dur);
			}
		}
		else if (strcmp(buffer, "help") == 0)
		{
			printf("Commands:\n");
			printf("count \n\t- devices count\n");
			printf("init <id> \n\t- initialize device witch 'id' index\n");
			printf("support \n\t- getting support list of device\n");
			printf("haptic <motor> <level> <duration> \n\t- make device haptic\n");
			printf("\tmotor can be 'l', 'h' or 'b' for low, high or both motors\n");
			printf("\tlevel 0-100\n");
			printf("\tduration in ms\n");
			printf("quit \n\t- exit the program\n");
		}
		else if (strcmp(buffer, "count") == 0)
		{
			int count = GetDevNum();
			printf("%d device%c\n", count, count == 1 ? ' ' : 's');
		}
		else if (strcmp(buffer, "quit") == 0)
			break;

	} while (true);
	
	if(dev)
		delete dev;

	return 0;
}

