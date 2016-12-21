#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio2.h>
#include <vector>
#include <time.h>

#define DER 77
#define IZQ 75

using namespace std;

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}

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
	int vida;
	bala *disparo = NULL;
	int color;
	int img[2][5];
public:
	bala b;
	nave();
	int getX(){return x;};
	int getY(){return y;};
	void setX(int _x){x=_x;};
	void setY(int _y){y=_y;};
	void setVida(int _vida){vida=_vida;};
	void pintar();
	void virtual mover(char t);
	void borrar();
	void pintar_vida();
	void disparar();
	void mover_disparo();
};


nave::nave(){
	img[0][0]=0;img[0][1]=194;img[0][2]=207;img[0][3]=194;img[0][4]=0;
	img[1][0]=207;img[1][1]=178;img[1][2]=184;img[1][3]=178;img[1][4]=207;
	color=6;
}
void nave::pintar(){
	textcolor(color);
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
	int posicion;
public:
	void setDir(int _dir){dir=_dir;};
	void setPosicion(int _posicion){posicion=_posicion;};
	void setPuntaje(int _puntaje){puntaje=_puntaje;};
	void mover();	
};

void enemigo::mover(){
	switch(dir)
	{
	case IZQ:
		if(x-(posicion*5)>4)x--;
		if(x-(posicion*5)==4)dir=DER;
		break;
	case DER:
		if(x+(5*(10-posicion))<76)x++;
		if(x+(5*(10-posicion))==76)dir=IZQ;
		break;
	}
	pintar();
}

//ENEMIGO 1
class enemigo_infanteria: virtual public enemigo{
public:
	enemigo_infanteria();
};

enemigo_infanteria::enemigo_infanteria(){
	img[0][0]=0;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=0;
	img[1][0]=0;img[1][1]=0;img[1][2]=0;img[1][3]=0;img[1][4]=0;
	color=4;
}

//ENEMIGO 2
class enemigo_sargento: virtual public enemigo{
public:
	enemigo_sargento();
};

enemigo_sargento::enemigo_sargento(){
	img[0][0]=0;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=0;
	img[1][0]=0;img[1][1]=0;img[1][2]=193;img[1][3]=0;img[1][4]=0;
	color=3;
}

//ENEMIGO 3
class enemigo_teniente: virtual public enemigo{
public:
	enemigo_teniente();
};

enemigo_teniente::enemigo_teniente(){
	img[0][0]=196;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=196;
	img[1][0]=0;img[1][1]=193;img[1][2]=0;img[1][3]=193;img[1][4]=0;
	color=14;
}

//ENEMIGO 4
class enemigo_general: virtual public enemigo{
public:
	enemigo_general();
};

enemigo_general::enemigo_general(){
	img[0][0]=196;img[0][1]=178;img[0][2]=207;img[0][3]=178;img[0][4]=196;
	img[1][0]=207;img[1][1]=193;img[1][2]=207;img[1][3]=193;img[1][4]=207;
	color=2;
}

// Tiempo Global
int periodoRefresco = 120; // a menor valor, mas rapidez de refresco
int ultimoRefresco = clock(); // guardamos el tiempo actual

//FORMACION

class oleada: public enemigo_sargento,public enemigo_infanteria, public enemigo_teniente, public enemigo_general{
	vector <enemigo*> formacion;
	int map[10][6];
	void cargar_enemigo(int tipo);
public:
	oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]);
	void posicionar_oleada();
	void dibujar_oleada();
};

oleada::oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]){
	for(int i=config_f1[0];i!=0;i--){
		map[i][1]=config_f1[1];
		cargar_enemigo(config_f1[1]);
	}
	
	for(int i=config_f2[0];i!=0;i--){
		map[i][2]=config_f2[1];
		cargar_enemigo(config_f2[1]);
	}
	
	for(int i=config_f3[0];i!=0;i--){
		map[i][3]=config_f3[1];
		cargar_enemigo(config_f3[1]);
	}
	
	for(int i=config_f4[0];i!=0;i--){
		map[i][4]=config_f4[1];
		cargar_enemigo(config_f4[1]);
	}
	
	
	for(int i=config_f5[0];i!=0;i--){
		map[i][5]=config_f5[1];
		cargar_enemigo(config_f5[1]);
	}

	for(int i=config_f6[0];i!=0;i--){
		map[i][6]=config_f6[1];
		cargar_enemigo(config_f6[1]);
	}
	
}

void oleada::cargar_enemigo(int tipo){
	switch(tipo)  
	{  
	case 2:  
		formacion.push_back(new enemigo_sargento());
		break;  
	case 3:  
		formacion.push_back(new enemigo_teniente());
		break;  
	case 4:  
		formacion.push_back(new enemigo_general());
		break;  
	default:  
		formacion.push_back(new enemigo_infanteria());
	}  
}

void oleada::posicionar_oleada(){

	for(int i = 0; i < 44; i++){
		if(i<30){
			if(i<10){			
				formacion[i]->setX(15+(5*i));formacion[i]->setY(20);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i);
			}else if(i<20){
				formacion[i]->setX(15+(5*i)-50);formacion[i]->setY(18);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i-10);
			}else{
				formacion[i]->setX(15+(5*i)-100);formacion[i]->setY(16);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i-20);
			}
		}else if(i<38){
			formacion[i]->setX(15+(5*i)-150);formacion[i]->setY(14);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i-30);
			//cout<<formacion[i]->getX()<<endl;
		}else if(i<42){
			formacion[i]->setX(15+(5*i)-199);formacion[i]->setY(12);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i+1-40);	
		}else{
			formacion[i]->setX(15+(5*i)-220);formacion[i]->setY(10);formacion[i]->setVida(1);formacion[i]->setDir(IZQ);formacion[i]->setPosicion(i+2-46);	
		}
	}
}

void oleada::dibujar_oleada(){
	for(int i = 0; i < 44; i++){
		formacion[i]->borrar();
		formacion[i]->mover();
	}
}
int main(int argc, char *argv[]) {
	pintar_limites();
	nave *n1 = new nave();
	//Difinición de NIVEL
	int fila_1[2] = {10,1}; 
	int fila_2[2] = {10,1};
	int fila_3[2] = {10,1};
	int fila_4[2] = {8,2};
	int fila_5[2] = {6,3};
	int fila_6[2] = {2,4};
	
	//Difinición de NIVEL
	oleada *nivel1 = new oleada(fila_1,fila_2,fila_3,fila_4,fila_5,fila_6);
	n1->setX(38);n1->setY(30);n1->setVida(3);
	n1->pintar();
	n1->pintar_vida();
	
	nivel1->posicionar_oleada();
	
	while(true){
		ShowConsoleCursor(false);
		if(kbhit())
		{ 	
			char t = getch();
			n1->borrar();
			n1->mover(t);
			if(t == 'a')
			   n1->disparar();
			
		}
		
		if(clock() > ultimoRefresco + periodoRefresco)
		{
			n1->mover_disparo();
			nivel1->dibujar_oleada();
			ultimoRefresco = clock();
		}
		
	}
	return 0;
}

