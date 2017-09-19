#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio2.h>
#include <vector>
#include <time.h>

#define DER 77
#define IZQ 75

#define LIMITE_IZQ_OLEADA 6
#define LIMITE_DER_OLEADA 90
#define ALTURA_OLEADA 20

#define ALTURA_NAVE 2
#define ANCHO_NAVE 5

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
	textcolor(15);
	gotoxy(x,y);
	printf(" ");
	if(y>4)y--;
	gotoxy(x,y);
	printf("%c",248);
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
	bool b_activa;
public:
	bala b;
	nave();
	int getX(){return x;};
	int getY(){return y;};
	int getX_B(){return disparo->getX();};
	int getY_B(){return disparo->getY();};
	void setX(int _x){x=_x;};
	void setY(int _y){y=_y;};
	void setVida(int _vida){vida=_vida;};
	void pintar();
	void virtual mover(char t);
	void borrar();
	void pintar_vida();
	void disparar();
	void mover_disparo();
	void eliminar_disparo();
	bool bala_activa(){return b_activa;};
	bool colision_nave(int _x, int _y);
};


nave::nave(){
	img[0][0]=201;img[0][1]=203;img[0][2]=206;img[0][3]=203;img[0][4]=187;
	img[1][0]=200;img[1][1]=188;img[1][2]=248;img[1][3]=200;img[1][4]=188;
	color=6;
}
void nave::pintar(){
	textcolor(color);
	for(int i=0;i<ANCHO_NAVE;i++){
		for(int j=0;j<ALTURA_NAVE;j++){
			gotoxy(x+i,y+j);
			printf("%c",img[j][i]);
		}
		
	}
}

void nave::borrar(){
	for(int i=0;i<ANCHO_NAVE;i++){
		for(int j=0;j<ALTURA_NAVE;j++){
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
	if(!b_activa){	
		disparo = new bala();
		disparo->setX(x+2);
		disparo->setY(y-1);
		b_activa = true;
	}
}

void nave::mover_disparo(){
	if(b_activa){
		if(!disparo->limite()) disparo->mover();
		else eliminar_disparo();
	}
}

void nave::eliminar_disparo(){
	disparo->eliminar();
	//delete disparo;
	b_activa = false;
}

void pintar_limites(){
	for(int i=2;i<100;i++){
		gotoxy(i,3); printf("%c",205);
		gotoxy(i,33); printf("%c",205);
	}
	
	for(int i=4;i<33;i++){
		gotoxy(2,i); printf("%c",186);
		gotoxy(100,i); printf("%c",186);
	}
	
	gotoxy(2,3); printf("%c",201);
	gotoxy(2,33); printf("%c",200);
	gotoxy(100,3); printf("%c",187);
	gotoxy(100,33); printf("%c",188);
}


bool nave::colision_nave(int _x, int _y){
	if( _x >= x && _x <= x+ANCHO_NAVE && _y == y )return true;
	return  false;
}

//CLASE NAVE ENEMIGA BASE
class enemigo: virtual public nave{
	int dir;
	int puntaje;
	int posicion;
	int fila;
public:
	void setDir(int _dir){dir=_dir;};
	void setPosicion(int _posicion){posicion=_posicion;};
	void setPuntaje(int _puntaje){puntaje=_puntaje;};
	void setFila(int _fila){fila=_fila;};
	void mover();	
};

void enemigo::mover(){
	
	switch(dir)
	{
	case IZQ:
			if( x - (posicion * (ANCHO_NAVE + 1)) > LIMITE_IZQ_OLEADA) x--;
			else dir=DER;
		break;
	case DER:
			if(((10-posicion) * (ANCHO_NAVE + 1) ) + x < LIMITE_DER_OLEADA) x++ ;
			else dir=IZQ;
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
	img[0][0]=204;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=185;
	img[1][0]=0;img[1][1]=0;img[1][2]=0;img[1][3]=0;img[1][4]=0;
	color=4;
}

//ENEMIGO 2
class enemigo_sargento: virtual public enemigo{
public:
	enemigo_sargento();
};

enemigo_sargento::enemigo_sargento(){
	img[0][0]=204;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=185;
	img[1][0]=0;img[1][1]=0;img[1][2]=202;img[1][3]=0;img[1][4]=0;
	color=3;
}

//ENEMIGO 3
class enemigo_teniente: virtual public enemigo{
public:
	enemigo_teniente();
};

enemigo_teniente::enemigo_teniente(){
	img[0][0]=201;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=187;
	img[1][0]=202;img[1][1]=193;img[1][2]=0;img[1][3]=193;img[1][4]=202;
	color=14;
}

//ENEMIGO 4
class enemigo_general: virtual public enemigo{
public:
	enemigo_general();
};

enemigo_general::enemigo_general(){
	img[0][0]=201;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=187;
	img[1][0]=200;img[1][1]=205;img[1][2]=206;img[1][3]=205;img[1][4]=188;
	color=2;
}

// Tiempo Global
int periodoRefresco = 70; // a menor valor, mas rapidez de refresco
int ultimoRefresco = clock(); // guardamos el tiempo actual

//FORMACION

class oleada: public enemigo_sargento,public enemigo_infanteria, public enemigo_teniente, public enemigo_general{
	vector <enemigo*> formacion;
	void cargar_enemigo(int tipo,int posicion,int fila);
public:
	oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]);
	void dibujar_oleada();
	bool colision_oleada(int _x, int _y);
};

oleada::oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]){
	for(int i=1;i<=config_f1[0];i++){
		cargar_enemigo(config_f1[1],i, 0);
	}
	
	for(int i=1;i<=config_f2[0];i++){
		cargar_enemigo(config_f2[1],i,1);
	}
	
	for(int i=1;i<=config_f3[0];i++){
		cargar_enemigo(config_f3[1],i,2);
	}
	
	for(int i=1;i<=config_f4[0];i++){
		cargar_enemigo(config_f4[1],i,3);
	}
	
	for(int i=1;i<=config_f5[0];i++){
		cargar_enemigo(config_f5[1],i,4);
	}

	for(int i=1;i<=config_f6[0];i++){
		cargar_enemigo(config_f6[1],i,5);
	}
	
}

void oleada::cargar_enemigo(int tipo,int posicion,int fila){
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
	formacion.back()->setFila(fila);
	formacion.back()->setY(ALTURA_OLEADA - (ALTURA_NAVE * fila));
	formacion.back()->setX(LIMITE_IZQ_OLEADA + ((ANCHO_NAVE + 1) * posicion));
	formacion.back()->setDir(DER);
	formacion.back()->setVida(1);
	formacion.back()->setPosicion(posicion);
}

void oleada::dibujar_oleada(){
	int t = formacion.size();
	for(int i = 0; i < t; i++){
		if(formacion[i]!= NULL){
			formacion[i]->borrar();
			formacion[i]->mover();
		}
	}
}


bool oleada::colision_oleada(int _x, int _y){
	int t = formacion.size();
	for(int i = 0; i < t; i++){
		if(formacion[i]!= NULL){
			if(formacion[i]->colision_nave(_x,_y)){
				formacion[i]->borrar();
				formacion[i]=NULL;
				gotoxy(105,10);
				int p=i;
				cout<<"PUNTAJE: "<<p;
				return true;
			}
		}
	}
	return false;
}

int main(int argc, char *argv[]) {
	pintar_limites();
	nave *n1 = new nave();
	//Difinición de NIVEL
	int fila_1[2] = {2,1}; 
	int fila_2[2] = {10,1};
	int fila_3[2] = {10,1};
	int fila_4[2] = {8,2};
	int fila_5[2] = {6,3};
	int fila_6[2] = {4,4};
	//Difinición de NIVEL
	oleada *nivel1 = new oleada(fila_1,fila_2,fila_3,fila_4,fila_5,fila_6);
	n1->setX(38);n1->setY(30);n1->setVida(3);
	n1->pintar();
	n1->pintar_vida();
	
	while(true){
		ShowConsoleCursor(false);
		if(kbhit())
		{ 	
			char t = getch();
			n1->borrar();
			n1->mover(t);
			if(t == 'a'){
				n1->disparar();
			}
		}

		if(clock() > ultimoRefresco + periodoRefresco)
		{
			if(n1->bala_activa()){
				n1->mover_disparo();
				if(nivel1->colision_oleada(n1->getX_B(),n1->getY_B()))n1->eliminar_disparo();
			}
			nivel1->dibujar_oleada();
			ultimoRefresco = clock();
		}
		
	}
	return 0;
}

