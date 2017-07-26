
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>

#include <wiringPi.h>
#include <lcd.h>

int initLCD()
{
  const int RS = 3;
  const int EN = 14;
  const int D0 = 4;
  const int D1 = 12;
  const int D2 = 13;
  const int D3 = 6;

  if (wiringPiSetup() < 0)
  {
    fprintf(stderr, "Unable to open serial device: %s\n", strerror(errno));
    exit(1);
  }

  return lcdInit(2, 16, 4, RS, EN, D0, D1, D2, D3, D0, D1, D2, D3);
}

void initTemp(char *path)
{
  char rom[20];
  DIR *dirp;
  struct dirent *direntp;

  strcpy(path, "/sys/bus/w1/devices/");

  system("sudo modprobe w1-gpio");
  system("sudo modprobe w1-therm");

  if ((dirp = opendir(path)) == NULL)
  {
    printf("opendir error\n");
    exit(1);
  }

  while ((direntp = readdir(dirp)) != NULL)
  {
    if (strstr(direntp->d_name, "28-00000"))
    {
      strcpy(rom, direntp->d_name);
      printf(" rom: %s\n", rom);
    }
  }
  closedir(dirp);

  strcat(path, rom);
  strcat(path, "/w1_slave");
}
int main()
{
  int fd = -1;
  char buf[100];
  char *temp;
  float value;
  
  int lcdFD = initLCD();
  char path[50];
  initTemp(path);
  
  while (1)
  {
    if ((fd = open(path, O_RDONLY)) < 0)
    {
      printf("open error\n");
      return 1;
    }

    if (read(fd, buf, sizeof(buf)) < 0)
    {
      printf("read error\n");
      return 1;
    }

    temp = strchr(buf, 't');
    sscanf(temp, "t=%s", temp);
    value = atof(temp) / 1000;

    lcdPosition(lcdFD, 0, 0);
    char buffer[100];
    sprintf(buffer, "temp : %3.3f C", value);
    lcdPrintf(lcdFD, buffer);
    sleep(1);
  }
  return 0;
}
