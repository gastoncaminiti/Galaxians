#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <conio2.h>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>

#define DER 77
#define IZQ 75

#define LIMITE_IZQ_OLEADA 6
#define LIMITE_DER_OLEADA 90
#define ALTURA_OLEADA 20

#define ALTURA_NAVE 2
#define ANCHO_NAVE 5

#define MAX_MAP_FILA 6
#define MAX_MAP_COLUM 10 


using namespace std;

int puntaje_total = 0;
int enemigos_total = 0;

bool flag_pick = false;

void ShowConsoleCursor(bool showFlag)
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO     cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}


void pintar_gui(){
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

void text_info_gui(){
	textcolor(15);
	gotoxy(105,10);
	cout<<"PUNTAJE: "<<puntaje_total<<endl;
	gotoxy(105,12);
	cout<<"ENEMIGOS RESTANTES: "<<enemigos_total;
}

void grabar_puntaje(int p,string j){
	ifstream tabla("tabla.txt");
	string fila[6];
	char cadena[128];
	string aux;
	int n_linea = 0;
	int pos = 0;
	while(!tabla.eof()){
		tabla.getline(cadena,128);
		aux = string(cadena);
		pos = aux.find("-");
		if( pos !=-1){
			fila[n_linea] = string(cadena);
			n_linea++;
		}
		
	}
	tabla.close();
	ostringstream ss;
	ss<<j<<" - "<<p;
	//cout<<n_linea<<endl;
	fila[n_linea]= ss.str();
	int a,b;
	for(int j=0;j < n_linea + 1;j++){
		for(int j=0;j < n_linea;j++){
			a=atoi(fila[j].substr(fila[j].find("-")+2,fila[j].length()).c_str());
			b=atoi(fila[j+1].substr(fila[j+1].find("-")+2,fila[j+1].length()).c_str());
			if (a > b){
				aux = fila[j];
				fila[j]=fila[j+1];
				fila[j+1]=aux;
			}
		}
	}
	ofstream tabla2;
	tabla2.open("tabla.txt");
	for(int j=n_linea ;j > -1;j--){
		if(n_linea < 5){
			tabla2<<fila[j]<<endl;
		}
		else{
			if(j>0)tabla2<<fila[j]<<endl;
		}
	}
	tabla2.close();
}

void imprimir_tabla(){
	ifstream tabla("tabla.txt");
	char cadena[128];
	int y=25;
	while(!tabla.eof()){
		tabla.getline(cadena,128);
		gotoxy(110,y);
		cout<<cadena<<endl;
		y++;
	}
	gotoxy(112,23);
	cout<<"MEJORES JUGADORES"<<endl;
	tabla.close();
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
	int img[ALTURA_NAVE][ANCHO_NAVE];
	bool b_activa;
public:
	bala b;
	nave();
	int getX(){return x;};
	int getY(){return y;};
	int getX_B(){return disparo->getX();};
	int getY_B(){return disparo->getY();};
	int getVida(){return vida;};
	void setX(int _x){x=_x;};
	void setY(int _y){y=_y;};
	void setVida(int _vida){vida=_vida;};
	void pintar();
	virtual void mover(char t);
	void borrar();
	void pintar_vida();
	void disparar();
	void mover_disparo();
	void eliminar_disparo();
	bool bala_activa(){return b_activa;};
	bool colision_nave(int _x, int _y);
	void restar_v(){vida--;};
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
	textcolor(10);
	gotoxy(110,4); printf("VIDAS");
	for(int i=0;i< vida; i++){
		gotoxy(120+i,4);
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
	delete disparo;
	b_activa = false;
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
	bool estado_v;
public:
	void setDir(int _dir){dir=_dir;};
	void setPosicion(int _posicion){posicion=_posicion;};
	void setPuntaje(int _puntaje){puntaje=_puntaje;};
	void setEstado_v(bool _estado){estado_v=_estado;};
	int getPuntaje(){return puntaje;};
	int getFila(){return fila;};
	int getPosicion(){return posicion;};
	bool getEstado_v(){return estado_v;};
	void setFila(int _fila){fila=_fila;};
	void mover();	
	virtual void img_a(){};
	virtual void img_b(){};
	void vuelo(int limite_a, int dir);
};

void enemigo::mover(){
	
	switch(dir)
	{
	case IZQ:
			if( x - (posicion * (ANCHO_NAVE + 1)) > LIMITE_IZQ_OLEADA) x--;
			else dir=DER;
		break;
	case DER:
			if(((MAX_MAP_COLUM-posicion) * (ANCHO_NAVE + 1) ) + x < LIMITE_DER_OLEADA) x++ ;
			else dir=IZQ;
		break;
	}
	pintar();
	
}

void enemigo::vuelo(int limite,int dir){
	borrar();
	if(dir==0){
		if(y>5 && x>limite && getEstado_v()){
			y--;
		}else if(y==5 && x>limite && getEstado_v()){
			
			x--;
		}else if(y==5 && x==limite && getEstado_v()){
			img_a();
			y++;
		}else{
			if(y<31){
				y++;
				if(x<50)x++;
			}
			else{
				x=10;
				y=5;
			}
		}
	}else{
		if(y>5 && x<limite && getEstado_v()){
			y--;
		}else if(y==5 && x<limite && getEstado_v()){
			x++;
		}else if(y==5 && x==limite && getEstado_v()){
			img_a();
			y++;
		}else{
			if(y<31){
				y++;
				if(x>50)x--;
			}
			else{
				x=90;
				y=5;
			}
		}
	}
}

//ENEMIGO 1
class enemigo_infanteria: virtual public enemigo{
public:
	enemigo_infanteria();
	void img_a();  
	void img_b(); 
};

enemigo_infanteria::enemigo_infanteria(){

	img[0][0]=207;img[0][1]=205;img[0][2]=202;img[0][3]=205;img[0][4]=207;
	img[1][0]=0;img[1][1]=0;img[1][2]=0;img[1][3]=0;img[1][4]=0;
	color=4;
	setEstado_v(false);
}

 void enemigo_infanteria::img_a(){
	img[0][2]=203;
	setEstado_v(false);
}

void enemigo_infanteria::img_b(){
	img[0][2]=202;
	setEstado_v(true);
}


//ENEMIGO 2
class enemigo_sargento: virtual public enemigo{
public:
	enemigo_sargento();
	void img_a();
	void img_b();
};

enemigo_sargento::enemigo_sargento(){
	img_a();
	color=3;
}

void enemigo_sargento::img_a(){
	img[0][0]=204;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=185;
	img[1][0]=0;img[1][1]=0;img[1][2]=202;img[1][3]=0;img[1][4]=0;
	setEstado_v(false);
}

void enemigo_sargento::img_b(){
	img[0][0]=0;img[0][1]=0;img[0][2]=203;img[0][3]=0;img[0][4]=0;
	img[1][0]=204;img[1][1]=205;img[1][2]=207;img[1][3]=205;img[1][4]=185;
	setEstado_v(true);
	
}

//ENEMIGO 3
class enemigo_teniente: virtual public enemigo{
public:
	enemigo_teniente();
	void img_a();
	void img_b();
};

enemigo_teniente::enemigo_teniente(){
	img_a();
	color=14;
}

void enemigo_teniente::img_a(){
	img[0][0]=201;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=187;
	img[1][0]=202;img[1][1]=0;img[1][2]=0;img[1][3]=0;img[1][4]=202;
	setEstado_v(false);
}

void enemigo_teniente::img_b(){
	img[0][0]=203;img[0][1]=0;img[0][2]=0;img[0][3]=0;img[0][4]=203;
	img[1][0]=200;img[1][1]=205;img[1][2]=207;img[1][3]=205;img[1][4]=188;
	setEstado_v(true);
}

//ENEMIGO 4
class enemigo_general: virtual public enemigo{
public:
	enemigo_general();
	void img_a();
	void img_b();
};

enemigo_general::enemigo_general(){
	img_a();
	color=2;
}

void enemigo_general::img_a(){
	img[0][0]=201;img[0][1]=205;img[0][2]=207;img[0][3]=205;img[0][4]=187;
	img[1][0]=200;img[1][1]=205;img[1][2]=206;img[1][3]=205;img[1][4]=188;
	setEstado_v(false);
}

void enemigo_general::img_b(){
	img[1][2]=207;img[0][2]=206;
	setEstado_v(true);
}


// Tiempo Global
int periodoRefresco = 70; // a menor valor, mas rapidez de refresco
int ultimoRefresco = clock(); // guardamos el tiempo actual

//FORMACION

class oleada{
	vector <enemigo*> formacion;
	void cargar_enemigo(int tipo,int posicion,int fila);
	int n_enemigos;
	int map_control[MAX_MAP_FILA][MAX_MAP_COLUM];
	void map_clean();
public:
	oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]);
	void dibujar_oleada();
	int getNenemigos(){return n_enemigos;};
	bool colision_oleada(int _x, int _y);
	enemigo* elegir_uno();
	void borrar(int i);
	void restar_e(){n_enemigos--;};
};

oleada::oleada(int config_f1[2],int config_f2[2], int config_f3[2], int config_f4[2], int config_f5[2], int config_f6[2]){
	n_enemigos = 0;
	map_clean();
	for(int i=1;i<=config_f1[0];i++){
		cargar_enemigo(config_f1[1],i, 0);
		n_enemigos++;
	}
	
	for(int i=1;i<=config_f2[0];i++){
		cargar_enemigo(config_f2[1],i,1);
		n_enemigos++;
	}
	
	for(int i=1;i<=config_f3[0];i++){
		cargar_enemigo(config_f3[1],i,2);
		n_enemigos++;
	}
	
	for(int i=1;i<=config_f4[0];i++){
		cargar_enemigo(config_f4[1],i,3);
		n_enemigos++;
	}
	
	for(int i=1;i<=config_f5[0];i++){
		cargar_enemigo(config_f5[1],i,4);
		n_enemigos++;
	}

	for(int i=1;i<=config_f6[0];i++){
		cargar_enemigo(config_f6[1],i,5);
		n_enemigos++;
	}
	
}

void oleada::cargar_enemigo(int tipo,int posicion,int fila){
	int puntaje = 0;
	switch(tipo)  
	{  
	case 2:  
		formacion.push_back(new enemigo_sargento());
		puntaje = 100;
		break;  
	case 3:  
		formacion.push_back(new enemigo_teniente());
		puntaje = 200;
		break;  
	case 4:  
		formacion.push_back(new enemigo_general());
		puntaje = 300;
		break;  
	default:  
		formacion.push_back(new enemigo_infanteria());
		puntaje = 50;
	}
	formacion.back()->setFila(fila);
	formacion.back()->setY(ALTURA_OLEADA - (ALTURA_NAVE * fila));
	formacion.back()->setX(LIMITE_IZQ_OLEADA + ((ANCHO_NAVE + 1) * posicion));
	formacion.back()->setDir(DER);
	formacion.back()->setVida(1);
	formacion.back()->setPuntaje(puntaje);
	formacion.back()->setPosicion(posicion);
	map_control[fila][posicion]=1;
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


void oleada::borrar(int i){
	formacion[i]->borrar();
	map_control[formacion[i]->getFila ()][formacion[i]->getPosicion()]=0;
	formacion[i]=NULL;
}

bool oleada::colision_oleada(int _x, int _y){
	int t = formacion.size();
	for(int i = 0; i < t; i++){
		if(formacion[i]!= NULL){
			if(formacion[i]->colision_nave(_x,_y)){
				puntaje_total+=formacion[i]->getPuntaje();
				borrar(i);
				n_enemigos--;
				return true;
			}
		}
	}
	return false;
}

enemigo* oleada::elegir_uno(){
	int t = formacion.size();
	int fila_r = rand() % 6; 
	gotoxy(105,25);
	cout<<fila_r<<endl;
	enemigo* aux = NULL;
	for(int i = 0; i < t; i++){
		if(formacion[i]!= NULL){
			if(formacion[i]->getFila() == fila_r){
				if (fila_r < MAX_MAP_FILA -1){
					gotoxy(105,30);
					cout<<map_control[formacion[i]->getPosicion()][formacion[i]->getFila()+1]<<endl;
					if(formacion[i]->getPosicion() == 0 && map_control[formacion[i]->getPosicion()][formacion[i]->getFila()+1] ==0){
						aux = formacion[i];
						borrar(i);
						return aux;
					}
					if(formacion[i]->getPosicion() != 0 && map_control[formacion[i]->getPosicion()][formacion[i]->getFila()+1] ==0 && map_control[formacion[i]->getPosicion()-1][formacion[i]->getFila()] ==0){
						aux = formacion[i];
						borrar(i);
						return aux;
					}
				}else{
					aux = formacion[i];
					borrar(i);
					return aux;
				}
			}
		}
	}
	return aux;
}

void oleada::map_clean(){
	for(int i=0;i<MAX_MAP_COLUM;i++){
		for(int j=0;j<MAX_MAP_FILA;j++){
			map_control[j][i]=0;
		}
	}
}


int main(int argc, char *argv[]) {
	system("mode 140, 35");
	srand (time(NULL));
	nave *n1 = new nave();
	enemigo *e_activo = NULL;
	//Difinición de NIVEL
	int fila_1[2] = {10,1}; 
	int fila_2[2] = {10,1};
	int fila_3[2] = {10,1};
	int fila_4[2] = {8,2};
	int fila_5[2] = {6,3};
	int fila_6[2] = {4,4};
	//Difinición de NIVEL
	oleada *nivel1 = new oleada(fila_1,fila_2,fila_3,fila_4,fila_5,fila_6);
	pintar_gui();
	n1->setX(38);n1->setY(30);n1->setVida(3);
	n1->pintar();
	n1->pintar_vida();
	string jugador;
	enemigos_total = nivel1->getNenemigos();
	while(true){
		ShowConsoleCursor(false);
		text_info_gui();
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
			//Colision Bala-Enemigo
			if(n1->bala_activa()){
				n1->mover_disparo();
				if(nivel1->colision_oleada(n1->getX_B(),n1->getY_B())){
					n1->eliminar_disparo();
					enemigos_total = nivel1->getNenemigos();
				}
				if(e_activo != NULL){
					if(e_activo->colision_nave(n1->getX_B(),n1->getY_B())){
						n1->eliminar_disparo();
						e_activo->borrar();
						e_activo = NULL;
						nivel1->restar_e();
						flag_pick=false;
						enemigos_total = nivel1->getNenemigos();
					}
				}
			}
			//Colision Enemigo-Jugador
			if(e_activo != NULL){
				if(e_activo->colision_nave(n1->getX(),n1->getY())){
					n1->borrar();
					if(n1->getVida()>0){
						n1->borrar();
						n1->restar_v();
						n1->pintar_vida();
					}
				}
			}
			//-----SELECCION DE ENEMIGO A VOLAR Y VUELO-----------//
			if(!flag_pick){
				e_activo=nivel1->elegir_uno();
				if(e_activo != NULL){
				    e_activo->img_b();
					flag_pick=true;
				}
			}else{
				if(e_activo->getX()<50)e_activo->vuelo(LIMITE_IZQ_OLEADA,0);
				else e_activo->vuelo(LIMITE_DER_OLEADA,1);
				e_activo->pintar();
			}
			//-----CONDICION DE DERROTA-----------//
			if(n1->getVida()==0){
				gotoxy(110,4); printf("-----GAME OVER-----");
				gotoxy(30,10);
				cout << "Tu nombre de jugador es: "<<endl;
				gotoxy(55,10);
				getline(cin,jugador);
				grabar_puntaje(puntaje_total,jugador);
				imprimir_tabla();
			}else{
				nivel1->dibujar_oleada();
				n1->pintar();
			}
			//-----CONDICION DE REINICIO DE OLEADA-----------//
			if(enemigos_total == 0){
				delete nivel1;
				nivel1= NULL;
				nivel1 = new oleada(fila_1,fila_2,fila_3,fila_4,fila_5,fila_6);
				enemigos_total = nivel1->getNenemigos();
			}
			ultimoRefresco = clock();
		}
		
	}
	return 0;
}

