#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio2.h>



#define DER 77
#define IZQ 75

using namespace std;

class nave{
	int  x,y;
	int img[4][5];
	int vida;
public:
	nave(int _x, int _y, int _vida):x(_x),y(_y),vida(_vida){};
	void pintar();
	void borrar();
	void mover(char t);
	void pintar_vida();
};

void nave::pintar(){
	img[0][0]=0;img[0][1]=0;img[0][2]=219;img[0][3]=0;img[0][4]=0;
	img[1][0]=0;img[1][1]=178;img[1][2]=207;img[1][3]=178;img[1][4]=0;
	img[2][0]=0;img[2][1]=219;img[2][2]=167;img[2][3]=219;img[2][4]=0;
	img[3][0]=219;img[3][1]=177;img[3][2]=0;img[3][3]=177;img[3][4]=219;
	textcolor(6);
	for(int i=0;i<5;i++){
		for(int j=0;j<4;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

void nave::borrar(){
	for(int i=0;i<5;i++){
		for(int j=0;j<4;j++){
			gotoxy(x+i,y+j);
			printf(" ");
		}
		
	}
}

void nave::mover(char t){
	switch(t)
	{
	case IZQ:
		if(x>3)x--;
		break;
	case DER:
		if(x+6<77)x++;
		break;
	}
	pintar();
}

void nave::pintar_vida(){
	gotoxy(64,2); printf("Vidas");
	for(int i=0;i< vida; i++){
		gotoxy(70+i,2);
		printf("%c ",207);
	}
}

void pintar_limites(){
	for(int i=2;i<78;i++){
		gotoxy(i,3); printf("%c",205);
		gotoxy(i,33); printf("%c",205);
	}
	
	for(int i=4;i<33;i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(77,i); printf("%c",186);
	}
	
	gotoxy(2,3); printf("%c",201);
	gotoxy(2,33); printf("%c",200);
	gotoxy(77,3); printf("%c",187);
	gotoxy(77,33); printf("%c",188);
}



int main(int argc, char *argv[]) {
	pintar_limites();
	nave n1(38,28,3);
	n1.pintar();
	n1.pintar_vida();
	while(true){
		_setcursortype( _NOCURSOR );
		if(kbhit())
		{ 	
			n1.borrar();
			n1.mover(getch());
		}
		Sleep(30);
	}
	return 0;
}
