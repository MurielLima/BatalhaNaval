#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <time.h>
#define BUF 300
#define TAB 14
#define POS 10
char tabuleiro[TAB][TAB];
int ultimapos = 0;
int buf[BUF];
char alfabeto[15] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','\0'};

void insereBuffer(char let, int kbd)
{
  if (ultimapos == BUF)
  {
    write(kbd, "Buffer cheio", 12);
    return;
  }
  buf[ultimapos] = let;
  ultimapos++;
}
void removeBuffer(int kbd)
{
  if (ultimapos == -1)
  {
    write(kbd, "Buffer vazio", 12);
    return;
  }
  buf[ultimapos] = '\0';
  ultimapos--;
}
void limpaBuffer()
{
  int i;
  for (i = ultimapos; i >= 0; i--)
    buf[i] = '\0';
  ultimapos = 0;
}
void lerTeclado(int fd, int kbd, char let)
{
  switch (let)
  {
  case '\n':
    write(fd, &buf, BUF);
    limpaBuffer();
    break; /* Send it to the console "kbd"*/
  case 8:
    removeBuffer(kbd);
    break;
  default:
    insereBuffer(let, kbd);
    break;
  }
}
int deParaLetra(char letra){
    int pos;
    switch(letra){
        case 'A':
        case 'a':pos = 0;break;
        case 'B':
        case 'b':pos = 1;break;
        case 'C':
        case 'c':pos = 2;break;
        case 'D':
        case 'd':pos = 3;break;
        case 'E':
        case 'e':pos = 4;break;
        case 'F':
        case 'f':pos = 5;break;
        case 'G':
        case 'g':pos = 6;break;
        case 'H':
        case 'h':pos = 7;break;
        case 'I':
        case 'i':pos = 8;break;
        case 'J':
        case 'j':pos = 9;break;
        case 13:pos = 11;break;
        default: pos = 99;break;
    }
return pos;
}
void mostra(int saida)
{
    int i,j;
    write(saida, "\n", 1);
    for(i=0;i<TAB;i++){
      write(saida, &alfabeto[i], 1);
      write(saida, "  ", 1);
    }
    for(i=0;i<TAB;i++)
    {
      write(saida, &i, 1);
        for(j=0;j<TAB;j++)
        {
            write(saida, "  ", 1);
            write(saida, &tabuleiro[i][j], 1);
        }
        write(saida, "\n", 1);
    }
}
int verificarTab(int tam,int* a,int* b){
	int i,j;
	*a=(rand() % TAB) - 1;
	*b=(rand() % TAB-tam) - 1;
	for(i=*a;i<=*a;i++)
		for(j=*b;j<*b+tam;j++)
			if(tabuleiro[i][j]!=' ') return verificarTab(tam,a,b);
	return 1;
}
void insere(int tam,char let){
	int a,b,i,j;
	verificarTab(tam,&a,&b);
	for(i=a;i<=a;i++)
		for(j=b;j<b+tam;j++)
			tabuleiro[i][j]=let;
} 
void inserirTabuleiro(){
	int a,b,i,j,k;
	//inserir submarino 2
	insere(2,'X');
	//inserir barco 3
	insere(3,'Y');
	//inserir navio 4
	insere(4,'Z');
}
void receberTiro(char pos[10]){
	int a,b;
	a = deParaLetra(pos[0]);
	b = pos[1];
	if(tabuleiro[a][b]!=' ')
		return 1;
	return -1;
}
void jogo(int entrada,int saida){
	inserirTabuleiro();
	mostra(saida);
}
int main(int argc, char **argv)
{
  srand(time(NULL));
  int fd, kbd;
  char letra, *portname;
  char vet[BUF];
  char msg[80];
  struct termios mytty, kbdios;
  fd_set descritores; /* Set of i/o handles */
  struct timeval timeout;
  /* Open serial port */
  portname = "/dev/ttyS0"; /* Use /dev/ttyS0 by default */
  /* or get the name of the serial port device from the command line */
  if (argc > 1 && *argv[1] == '/')
    portname = argv[1];
  /* Open the serial port */
  fd = open(portname, O_RDWR | O_NDELAY);
  if (fd == -1)
  {
    sprintf(msg, "\nUnable to open \"%s\"", portname);
    perror(msg);
    return -1;
  }
  /* Set attributes of the serial interface to raw mode */
  if (tcgetattr(fd, &mytty) == -1)
  {
    perror(NULL);
    close(fd);
    return -1;
  }
  mytty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
  mytty.c_oflag &= ~OPOST;
  mytty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  mytty.c_cflag &= ~(CSIZE | PARENB);
  mytty.c_cflag |= CS8; /* 8 bit data bytes */
  /* Set Baud Rate to 9600 baud */
  cfsetispeed(&mytty, B9600);
  cfsetospeed(&mytty, B9600);

  if (tcsetattr(fd, TCSANOW, &mytty) == -1)
  {
    perror(NULL);
    close(fd);
    return -1;
  }
  /* Get keyboard handle (assuming that stdin is the keyboard and video) */
  kbd = fileno(stdin);
  /* Set keyboard to raw mode */
  tcgetattr(kbd, &kbdios);
  memcpy(&mytty, &kbdios, sizeof(struct termios));
  mytty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
  tcsetattr(kbd, TCSANOW, &mytty);
  /* These calls make read to return imedately even when there is
   no data in the input stream */
  fcntl(fd, F_SETFL, FNDELAY);
  fcntl(kbd, F_SETFL, FNDELAY);
  /* Now we may use the serial interface and keyboard for sending
  just one character at a time */
  printf("\nSerial tty terminal on port \"%s\"\n", portname);
  printf("Type ctrl-X (meta-X) to exit.\n");

  jogo(fd,kbd);

//  do
//  {
//    /* Wait for any activitiy in the serial port (fd) or keyboard (kbd) */
//    FD_ZERO(&descritores); /* Inicializa a lista de handles */
//    FD_SET(fd, &descritores);
//    FD_SET(kbd, &descritores);
//    timeout.tv_sec = 5;
//    timeout.tv_usec = 0;
//    select(FD_SETSIZE, &descritores, NULL, NULL, &timeout);
//
//    while (read(fd, &vet, BUF) >= 0)
//    {                        /* Get a char from serial port "fd"*/
//      write(kbd, &vet, BUF); /* Send it to the console "kbd"*/
//    }
//    while (read(kbd, &letra, 1) >= 0)
//    { /* Get a char from "kbd" (keyboard)read(kbd,&letra,1)*/
//      write(kbd, &letra, 1); /* Send it to the console "kbd"*/
//      lerTeclado(fd, kbd, letra);
//    }
//  } while (letra != 'X' - 64); /* if the key was Ctrl-X exit*/
  close(fd);
  /* Set console back to normal */
  fcntl(kbd, 0, FNDELAY);
  tcsetattr(kbd, TCSANOW, &kbdios);
  putchar('\n');
  return 0;
}
