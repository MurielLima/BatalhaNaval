#include<stdio.h>
#include<stdlib.h>
#define N 10
#define M 10
char **criamatriz(char n,int m)
{
    char **mt,i;
    mt=(char **) malloc(n*sizeof(char *));
    if(mt==NULL)    
        return NULL;
    for(i=0;i<n;i++)
    {
        mt[i]=(char *)malloc(m*sizeof(char ));
        if(mt[i]==NULL)
            return NULL;
    }
    return mt;
}
void inicializa(char **mat,int n,int m)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            mat[i][j]=' ';
        }
    }
}
void mostra(char **mat,int n,int m)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            printf("|%c|",mat[i][j]);
        }
        printf("\n");
    }
}
int deParaBarco(char tipo)
{
    int tam;
    switch (tipo)
    {
    case 'X':
        tam = 2;
        break;
    case 'Y':
        tam = 3;
        break;
    case 'Z':
        tam = 4;
        break;
    }
    return tam;
}
int preencheMatriz(char **mat,int posX, int posY, char elem,int orient)
{
    int i,j,barco,tamX,tamY;
    barco = deParaBarco(elem);
   printf("X :%d Y: %d\n\n",posX,posY);
	mat[posX][posY] = elem;
	
	if(orient==1){
		for(i=barco-1;i>0;i--)
	mat[posX][posY+i] = elem;}
else{
	for(i=barco-1;i>0;i--)
	mat[posX+i][posY] = elem;
}

    return 0 ;
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

int hit (char **mat,int posX, int posY){
	int pont=0;
	switch(mat[posX][posY]){
        case 'X':
	    case 'Z':
        case 'Y':mat[posX][posY]='@';pont=1;break;
        case ' ':mat[posX][posY]='#';break;
	}
	return pont;
}

int main(int argsc,char *argsv[]){
 char** mat;
 char letra,numero;
 int posY,posX, pontuacao=0;
 mat = criamatriz(N,M);
 inicializa(mat,N,M);
mostra(mat,N,M);
printf("\n\npreencha com o primeiro submarino\n");
scanf("%d %d",&posX,&posY);
preencheMatriz(mat,posX,posY,'X',1);
mostra(mat,N,M);
while (pontuacao<2){
	printf("\nPontuacao: %d",pontuacao);
	printf("\n\nPosicao para Tiro\n");
	scanf("%d %d",&posX,&posY);
	if (posX>10||posX<0||posY>10||posY<0){
	printf("\n\nPosicao Invalida\n");
	}
	else
	{
	pontuacao=pontuacao+hit(mat,posX,posY);
	mostra(mat,N,M);
	}
}
printf("\n\nGanhou!!!\n\n");
getchar();
 return 0;    
}




