#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define PAUSA system("pause")
#define NADA printf("Ausencia de Datos\n")
#define CLS system("cls")
typedef struct _tdato{
	int dato;
	int valor;
	struct _tdato *sig;	
} Tdato;

typedef Tdato *Tnodo;

void menu(void);
Tnodo GenDatos();
void Push(Tnodo *,Tnodo *,Tnodo *);
Tnodo POP(Tnodo *,Tnodo *);
Tnodo Rep(Tnodo *nuevo,Tnodo *cabeza);
void Servicio(Tnodo *cabeza);


int main(){
	srand(time(NULL));
	menu();
}
void menu(){
	int op, contc;
	Tnodo nuevo=NULL;
	Tnodo cabeza=NULL;
	Tnodo ultimo=NULL;
	Tnodo temp=NULL;
	do{
	printf("\n\tM	E	N	U\n\n");
	printf("1.- AGREGA DATO\n");
	printf("2.- IMPRIME DATO\n");
	printf("3.- FINALIZAR PROGRAMA \n");
	printf("Selecciona una opcion: ");
	scanf("%d",&op);
		switch(op){
			case 1:
			do{
				nuevo=GenDatos();
				nuevo=Rep(&nuevo,&cabeza);
					if(nuevo){
						Push(&cabeza,&ultimo,&nuevo);
						contc++;
					}
		}while(contc<52);
			break;
			case 2:
				Servicio(&cabeza);
			break;
			case 3:
				printf("Finalizando Programa...\n");
			break;
			default:
			printf("OPCION INVALIDA\n");
			break;
		}
		PAUSA;
		CLS;
	}while(op!=3);
}
Tnodo GenDatos(){
	Tnodo temp=NULL;
	int valor=0;
	temp=(Tnodo)malloc(sizeof(Tdato));
	temp->sig=NULL;
	temp->dato=(1+rand()%52);
	valor=temp->dato%13;
	if(valor>10 || valor==0){
		temp->valor=10;
	}else{
		temp->valor=valor;
	}
	printf("carta= [%d] valor=[%d]\n",temp->dato, temp->valor);
	return temp;
}
void Push(Tnodo *cabeza,Tnodo *ultimo,Tnodo *nuevo){
	if(*cabeza){
		(*ultimo)->sig=(*nuevo);
		*ultimo=*nuevo;
		}else{
			*cabeza=*nuevo;
			*ultimo=*nuevo;
		}
	*nuevo=NULL;
}
Tnodo POP(Tnodo *cabeza,Tnodo *ultimo){
	Tnodo temp=NULL;
	if(*cabeza){
		if(*cabeza==*ultimo){
			temp=*ultimo;
			*cabeza=NULL;
			*ultimo=NULL;
		}else{
			temp=*cabeza;
			*cabeza=(temp)->sig;
			temp->sig=NULL;
		}
	}
	return temp;
}

void Servicio(Tnodo *cabeza){
	Tnodo temp=NULL;
	int i=0;
	temp=*cabeza;
	while(temp!=NULL){
		i++;
			printf("[%d] carta= [%d] valor=[%d]\n",i,temp->dato, temp->valor);
		temp=temp->sig;
		
	}
}

Tnodo Rep(Tnodo *nuevo,Tnodo *cabeza){
	Tnodo temp=NULL;
		temp=*cabeza;
			while(temp!=NULL){
				if((*nuevo)->dato==temp->dato){
						return NULL;
				}
			temp=temp->sig;				
				}
				return *nuevo;
}
