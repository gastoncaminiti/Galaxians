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
	int *img;
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
	void virtual mover(char t);
	void borrar();
	void pintar_vida();
	void disparar();
	void mover_disparo();
};

void nave::pintar(){
	int img[2][5];
	img[0][0]=0;img[0][1]=194;img[0][2]=207;img[0][3]=194;img[0][4]=0;
	img[1][0]=207;img[1][1]=178;img[1][2]=184;img[1][3]=178;img[1][4]=207;
	textcolor(6);
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

void nave::borrar(){
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
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

//CLASE NAVE ENEMIGA BASE
class enemigo: virtual public nave{
	int dir;
	int puntaje;
public:
	void setDir(int _dir){dir=_dir;};
	void setPuntaje(int _puntaje){puntaje=_puntaje;};
	void mover();	
};

void enemigo::mover(){
	switch(dir)
	{
	case IZQ:
		if(x>3)x--;
		if(x==3)dir=DER;
		break;
	case DER:
		if(x+6<77)x++;
		if(x+6==77)dir=IZQ;
		break;
	}
	pintar();
}

//ENEMIGO 1
class enemigo_infanteria: virtual public enemigo{
public:
	void pintar();
};

void enemigo_infanteria::pintar(){
	int img[2][5];
	img[0][0]=0;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=0;
	img[1][0]=0;img[1][1]=0;img[1][2]=0;img[1][3]=0;img[1][4]=0;
	textcolor(4);
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

//ENEMIGO 2
class enemigo_sargento: virtual public enemigo{
public:
	void pintar();
};

void enemigo_sargento::pintar(){
	int img[2][5];
	img[0][0]=0;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=0;
	img[1][0]=0;img[1][1]=0;img[1][2]=193;img[1][3]=0;img[1][4]=0;
	textcolor(3);
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

//ENEMIGO 3
class enemigo_teniente: virtual public enemigo{
public:
	void pintar();
};

void enemigo_teniente::pintar(){
	int img[2][5];
	img[0][0]=196;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=196;
	img[1][0]=0;img[1][1]=193;img[1][2]=0;img[1][3]=193;img[1][4]=0;
	textcolor(14);
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

//ENEMIGO 4
class enemigo_general: virtual public enemigo{
public:
	void pintar();
};

void enemigo_general::pintar(){
	int img[2][5];
	img[0][0]=196;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=196;
	img[1][0]=207;img[1][1]=193;img[1][2]=207;img[1][3]=193;img[1][4]=207;
	textcolor(2);
	for(int i=0;i<5;i++){
		for(int j=0;j<2;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

int main(int argc, char *argv[]) {
	pintar_limites();
	nave *n1 = new nave();
	n1->setX(38);n1->setY(28);n1->setVida(3);
	n1->pintar();
	n1->pintar_vida();
	//enemigo
	enemigo_infanteria *n2 = new enemigo_infanteria();
	n2->setX(38);n2->setY(20);n2->setVida(1);n2->setDir(IZQ);
	n2->pintar();
	enemigo_sargento *n3 = new enemigo_sargento();
	n3->setX(38);n3->setY(16);n3->setVida(1);n3->setDir(IZQ);
	n3->pintar();
	enemigo_teniente *n4 = new enemigo_teniente();
	n4->setX(38);n4->setY(12);n4->setVida(1);n4->setDir(IZQ);
	n4->pintar();
	enemigo_general *n5 = new enemigo_general();
	n5->setX(38);n5->setY(8);n5->setVida(1);n5->setDir(IZQ);
	n5->pintar();
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
		n2->borrar();
		n2->mover();
		n3->borrar();
		n3->mover();
		n4->borrar();
		n4->mover();
		n5->borrar();
		n5->mover();
		Sleep(70);
		
	}
	return 0;
}

