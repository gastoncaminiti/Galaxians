#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio2.h>

#define DER 77
#define IZQ 75

using namespace std;

//CLASE BALA
class bala{
	int x,y;
public:
	void setX(int _x){x=_x;};
	void setY(int _y){y=_y;};
	int getX(){return x;};
	int getY(){return y;};
	void mover();
	bool limite();
	void eliminar();
};

void bala::mover(){
	gotoxy(x,y);
	printf(" ");
	if(y>4)y--;
	gotoxy(x,y);
	printf("*");
}

bool bala::limite(){
	if(y==4)
	   return true;
	return false;
}

void bala::eliminar(){
	gotoxy(x,y);
	printf(" ");
}

//CLASE NAVE BASE
class nave: public bala{
	
protected:
	int  x,y;
	int img[4][5];
	int vida;
	bala *disparo = NULL;
public:
	bala b;
	int getX(){return x;};
	int getY(){return y;};
	void setX(int _x){x=_x;};
	void setY(int _y){y=_y;};
	void setVida(int _vida){vida=_vida;};
	void virtual pintar();
	void borrar();
	void mover(char t);
	void pintar_vida();
	void disparar();
	void mover_disparo();
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


void nave::disparar(){
	if(!disparo){	
		disparo = new bala();
		disparo->setX(x+2);
		disparo->setY(y-1);
	}
}

void nave::mover_disparo(){
	if(disparo){
		if(disparo->limite()){
			disparo->eliminar();
			disparo=NULL;
		}else{
			disparo->mover();
		}
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

//CLASE NAVE ENEMIGO
class enemigo_infanteria: virtual public nave{
	
public:
	void pintar();
	void mover_enemigo();	
};

void enemigo_infanteria::pintar(){
	img[0][0]=0;img[0][1]=0;img[0][2]=219;img[0][3]=0;img[0][4]=0;
	img[1][0]=0;img[1][1]=178;img[1][2]=207;img[1][3]=178;img[1][4]=0;
	img[2][0]=0;img[2][1]=219;img[2][2]=167;img[2][3]=219;img[2][4]=0;
	img[3][0]=219;img[3][1]=177;img[3][2]=0;img[3][3]=177;img[3][4]=219;
	textcolor(4);
	for(int i=0;i<5;i++){
		for(int j=0;j<4;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

void enemigo_infanteria::mover_enemigo(){
	
}

int main(int argc, char *argv[]) {
	pintar_limites();
	nave *n1 = new nave();
	n1->setX(38);n1->setY(28);n1->setVida(3);
	n1->pintar();
	n1->pintar_vida();
	//enemigo
	nave *n2 = new enemigo_infanteria();
	n2->setX(12);n2->setY(25);n2->setVida(1);
	n2->pintar();
	while(true){
		_setcursortype( _NOCURSOR );
		if(kbhit())
		{ 	
			char t = getch();
			n1->borrar();
			n1->mover(t);
			if(t == 'a')
			   n1->disparar();
			
		}
		n1->mover_disparo();
		Sleep(30);
		
	}
	return 0;
}

