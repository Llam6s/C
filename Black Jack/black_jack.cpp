//{LIBRERIAS
//---------------------------------------------------------------------
#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Datos/cartas.h"
// Librerias de allegro asi como librerias del sistema utilizadas -----
//}
//---------------------------------------------------------------------
//{ESTRUCTURA
typedef struct _tdato
{
    int carta;
    int valor;
    struct _tdato *sig;
} Tdato;
//------ Este es el tipo de estructura o dato utilizado,  en si consiste en la
//------ definicion de un tipo de dato abstracto en el cual se definen, primero
//------ la carta, en el cual se define el sprite, y el valor correspondiente de
//------ la misma, asi como un puntero al siguiente dato abstracto por ser una
//------ estructura.
//}
//---------------------------------------------------------------------
//{VARIABLES
typedef Tdato *Tnodo;
volatile int close_button_pressed = FALSE;
bool salir = false;
bool aceptar = false;
bool regresar = false;
bool Juego = true;
int ApuInd[5],PunJug[5], apuesta, banco, ganador, tam = 52, Bar[52];
float x,y;
float Cx[9],Cy[9];
Tnodo Baraja;
Tnodo pila=NULL;            //
Tnodo nuevo=NULL;           // Seccion en la cual se crean las partes principales de
Tnodo cabeza=NULL;          // tanto las pilas y colas utilizadas en el programa.
Tnodo ultimo=NULL;          //
Tnodo temp=NULL;            //
Tnodo Jugador=NULL;         //
Tnodo croupier=NULL;        // De igual manera se crean las variables que contendran
Tnodo jugador1=NULL;        // las cartas de cada jugador asi como del croupier.
Tnodo jugador2=NULL;        //
Tnodo jugador3=NULL;        //
Tnodo jugador4=NULL;        //
//------ Estas son las variables utilizadas, en este caso, fueron definidas el boton
//------ de "close_button_pressed", el cual fue declarado como "volatile" para ser modificada directamente
//------ desde la memoria principal permitiendo al compilador definir valores boleanos para
//------ para este boton.
//------ 3 variables boleanas "salir", "aceptar" y "regresar" las cuales controlan los botones de la interfaz
//------ principal, cambian dependiendo del boton seleccionado a true.
//------ Las correspondientes a los valores de apuestas, puntuacion de jugador y el banco
//------ se definen con valores enteros y vectores "ApInd, PunJug, apuesta y banco"
//------ Asi mismo, los valores de coordenadas "Cx, Cy, x,y"
//------ Y Por ultimo un vector correspondiente a la cantidad de cartas totales.
//---------------------------------------------------------------------
//}
//---------------------------------------------------------------------
//{DECLARACION DE FUNCIONES

Tnodo POPC(Tnodo *,Tnodo *);
Tnodo POPP(Tnodo *pila);
Tnodo repartirCarta(void);
Tnodo GenCola(int);
void PushC(Tnodo *,Tnodo *,Tnodo *);
void PushP(Tnodo *pila, Tnodo *nuevo);
void close_button_handler(void);
void llenarBarajaNum(int [],int);
void llenarBaraja(Tnodo *,int [],int);
bool busquedaSecuencial(int [],int ,int);
//------ En esta seccion se definen todas las funciones utilizadas, comenzando por
//------ definir el tipo de dato "Tdato" como un puntero con el nombre "Tnodo".
//------ A continuacion se definen funciones basicas para el manejo de estructuras
//------ de datos como lo son PUSH y POP de tanto las PILAS y COLAS utilizadas en el
//------ programa.
//------ La funcion close_button_handler se encarga de la configuracion del boton
//------ salir ubicado en la esquina superior derecha de la pantalla.
//------ Las funciones "reparitCarta" y "llenarBaraja" corresponden a funciones que
//------ repartirnan cartas a jugadores dependiendo y una funcion que llena mediante un for la
//------ baraja desde el 0 al 51
//}
//---------------------------------------------------------------------
//{JUEGO
int main()
{
//{INICIALIZACION
    allegro_init();                 // Inicio del programa de allegro.
    srand(time(NULL));              // Se inicia la semilla aleatoria.

    set_close_button_callback(close_button_handler);    // Se habilita el boton salir (X).
    install_keyboard();         //
    install_mouse();            // Se habilitan el teclado, mouse y temporizador.
    install_timer();            //

    llenarBarajaNum(Bar,tam);                                   // Se llena el vector en la baraja con numeros aleatorios sin repetirse del 0 al 51.
    llenarBaraja(&Baraja,Bar,tam);                              // Se llena la cola baraja con los numeros ya aleatorios
    set_color_depth(32);                                        // Se define la cantidad de bits que maneja la pantalla
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1200, 800, 0, 0);     // la cual en conjunto a esta funcion se define el tamaño de la ventana a utiliar.

    BITMAP *buffer = create_bitmap(1200, 800);                                      //
    BITMAP *inicio = load_bitmap("F y C/INICIO.bmp",NULL);                          //
    BITMAP *inicio2 = load_bitmap("F y C/INICIO JUGAR.bmp",NULL);                   // En esta seccion se cargan en diferentes bitmaps de memoria
    BITMAP *inicio3 = load_bitmap("F y C/INICIO SALIR.bmp",NULL);                   // los diferentes sprites o bitmaps correspondientes a la cantidad
    BITMAP *fondo = load_bitmap("F y C/FONDO MAIN.bmp", NULL);                      // de escenarios, correspondientes a el tablero, cartas (parte posterior),
    BITMAP *fondo2 = load_bitmap("F y C/FONDO APUESTA INICIAL.bmp", NULL);          // escenario de apuestas, etc.
    BITMAP *fondo3 = load_bitmap("F y C/FONDO APUESTA INICIAL - 1.bmp", NULL);      // De igual manera el sprite del mouse.
    BITMAP *fondo4 = load_bitmap("F y C/FONDO APUESTA INICIAL - 2", NULL);          //
    BITMAP *mesa = load_bitmap("F y C/MESA.bmp", NULL);                             //
    BITMAP *cursor = load_bitmap("F y C/CURSOR.bmp", NULL);                         //
    BITMAP *atras = load_bitmap("Cartas/atras.bmp", NULL);                          //
    BITMAP *fondorepb = load_bitmap("F y C/FONDO REPARTIR.bmp", NULL);              //
    BITMAP *fondoreprr = load_bitmap("F y C/FONDO REPARTIR - REPARTIR.bmp", NULL);  //
    BITMAP *fondoreprreg = load_bitmap("F y C/FONDO REPARTIR - REGRESAR.bmp", NULL);//
    BITMAP *tempo;                                                                  //
    //BITMAP *moneda = load_bitmap("F y C/MONEDA.bmp",NULL);
    BITMAP *pedir = load_bitmap("FICHAS/FichaP.bmp",NULL);
    BITMAP *quedarse =load_bitmap("FICHAS/FichaQ.bmp",NULL);
    BITMAP *quedarseR =load_bitmap("FICHAS/FichaQR.bmp",NULL);
    BITMAP *pedirR =load_bitmap("FICHAS/FichaPR.bmp",NULL);
    DATAFILE *CartasN = load_datafile("Datos/Cartas.dat");                          // El orden de las cartas es Corazones, Diamantes, Treboels y Espadas segun los datos obtenidos del grabber 0 (Corazones), 14 (Diamantes), etc...

    set_mouse_sprite(cursor);                                                       // Se define el cursor utilizado en el juego.
    show_mouse(screen);                                                             // Se muestra el cursor en pantalla.
//}
//---------------------------------------------------------------------
    while(!close_button_pressed)    // Mientras no se presione el boton de salir (X) se hace....
    {
        blit(inicio, buffer, 0, 0, 0, 0, 1200, 800);    // Se imprime por primera vez el sprite de la pantalla de inicio.
//{MENU PRINCIPAL
        while(!salir)   // Mientras la variable boleana correspondiente a la salida del programa se encuentre en falso.........
        {
            if(mouse_x > 573 && mouse_x < 632 && mouse_y > 447 && mouse_y < 466)    // Si el mouse se encuentra entre estos pixeles....
            {
                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);                         // Se imprimen las pantallas correspondientes a el escenario de fondo
                masked_blit(inicio2, buffer, 0, 0, 350, 213, 500, 375);             // inicial (interfaz principal)
                if(mouse_b & 1 || mouse_b & 2)
                {
                    while(!regresar)
                    {
                        if(mouse_x > 273 && mouse_x < 438 && mouse_y > 667 && mouse_y < 713)
                        {
                            blit(fondoreprr, buffer, 0, 0, 0, 0, 1200, 800);
                            masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                            if(mouse_b & 1 || mouse_b & 2)
                            {
                                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);

                                masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                rest(1000);

                                draw_sprite(buffer, atras, 572, 292);
                                draw_sprite(buffer, atras, 575, 295);
                                draw_sprite(buffer, atras, 578, 298);
                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                rest(1000);
//{ REPARTICION
//---------------------------------------------------REPARTICION DE CARTAS (ANIMACION)---------------------------------------------------//

                                //-----------------JUGADOR 1-----------------
                                x=603;
                                y=301;
                                temp = repartirCarta();
                                PushP(&jugador1,&temp);
                                while(y<=358)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, x, y, ftofix(-45.17));

                                    y=x*57/276-50103/276+358;
                                    x+=2;
                                    rest(1);

                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[0]=x;
                                Cy[0]=y;
                                rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                x=603;
                                y=301;
                                temp = repartirCarta();
                                PushP(&jugador1,&temp);
                                while(y<=357)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);
                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, x, y, ftofix(-45.17));
                                    y=x*56/286-49784/286+357;
                                    x+=2;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[1]=x;
                                Cy[1]=y;
                                rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                PunJug[0] = jugador1->valor + (jugador1->sig)->valor;
                                //-----------------FIN REPARTICION A JUGADOR 1-----------------//

                                //-----------------JUGADOR 2-----------------
                                x=603;
                                y=301;
                                temp = repartirCarta();
                                PushP(&jugador2,&temp);
                                while(y<=454)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, x, y, ftofix(-16.21));
                                    y=x*153/145-114444/145+454;
                                    x++;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[2]=x;
                                Cy[2]=y;
                                rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                x=603;
                                y=301;
                                temp = repartirCarta();
                                PushP(&jugador2,&temp);
                                while(y<=457)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, x, y, ftofix(-16.21));

                                    y=x*156/152-117780/152+457;
                                    x++;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[3]=x;
                                Cy[3]=y;
                                rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                PunJug[1] = jugador2->valor + (jugador2->sig)->valor;
                                //-----------------FIN REPARTICION A JUGADOR 2-----------------//

                                //-----------------JUGADOR-----------------
                                x=578;
                                y=298;
                                temp = repartirCarta();
                                PushP(&Jugador,&temp);
                                while(y<=488)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, x, y, ftofix(0));
                                    y = x*-190/3+109250/3+488;
                                    x = x-0.03;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[4]=x;
                                Cy[4]=y;
                                rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[4], Cy[4], ftofix(0));
                                x=578;
                                y=298;
                                temp = repartirCarta();
                                PushP(&Jugador,&temp);
                                while(y<=504)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, x, y, ftofix(0));
                                    y=x*206/14-121952/14+504;
                                    x= x+0.1;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[5]=x;
                                Cy[5]=y;
                                rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                PunJug[2] = Jugador->valor + (Jugador->sig)->valor;
                                //-----------------FIN REPARTICION A JUGADOR-----------------//

                               //-----------------JUGADOR 3-----------------
                                x=563;
                                y=315;
                                temp = repartirCarta();
                                PushP(&jugador3,&temp);
                                while(y<=454)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                    rotate_sprite(buffer, atras, x, y, ftofix(16.21));
                                    y = x*-139/158+78257/158+315;
                                    x--;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[6]=x;
                                Cy[6]=y;
                                rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                x=563;
                                y=315;
                                temp = repartirCarta();
                                PushP(&jugador3,&temp);
                                while(y<=463)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                    rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                    rotate_sprite(buffer, atras, x, y, ftofix(16.21));

                                    y = x*-148/154+83324/154+315;
                                    x--;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[7]=x;
                                Cy[7]=y;
                                rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                PunJug[3] = jugador3->valor + (jugador3->sig)->valor;
                                //-----------------FIN REPARTICION A JUGADOR 3-----------------//

                                //-----------------JUGADOR 4-----------------
                                x=563;
                                y=315;
                                temp = repartirCarta();
                                PushP(&jugador4,&temp);
                                while(y<=360)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                    rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                    rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                    rotate_sprite(buffer, atras, x, y, ftofix(45.17));
                                    y = x*45/-287+25335/287+315;
                                    x-=2;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[8]=x;
                                Cy[8]=y;
                                rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                x=563;
                                y=315;
                                temp = repartirCarta();
                                PushP(&jugador4,&temp);
                                while(y<=352)
                                {
                                    blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                    masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                    draw_sprite(buffer, atras, 572, 292);
                                    draw_sprite(buffer, atras, 575, 295);
                                    draw_sprite(buffer, atras, 578, 298);

                                    rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                    rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                    rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                    rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                    rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                    rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                    rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                    rotate_sprite(buffer, atras, x, y, ftofix(45.17));

                                    y = x*-37/302+20831/302+315;
                                    x-= 2;
                                    rest(1);
                                    blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                }
                                Cx[9]=x;
                                Cy[9]=y;
                                //rotate_sprite(buffer, atras, Cx[9], Cy[9], ftofix(45.17));
                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                PunJug[4] = jugador4->valor + (jugador4->sig)->valor;
                                //-----------------FIN REPARTICION A JUGADOR 4-----------------//
                                ganador = busquedaGanador(PunJug,5,21);
                                switch(ganador){
                                    case 0:

                                    break;
                                    case 1:

                                    break;
                                    case 2:

                                    break;
                                    case 3:

                                    break;
                                    case 4:

                                    break;
                                }
//---------------------------------------------------FIN REPARTICION CARTAS (ANIMACION)---------------------------------------------------//
//}
//{JUEGO EN PROGRESO
                                    while(Juego){
                                        if(mouse_x > 375 && mouse_x < 529 && mouse_y > 570 && mouse_y < 725){
                                            blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                            masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                            draw_sprite(buffer, atras, 572, 292);
                                            draw_sprite(buffer, atras, 575, 295);
                                            draw_sprite(buffer, atras, 578, 298);
                                            draw_sprite(buffer, quedarse, 700, 570);
                                            draw_sprite(buffer, pedirR, 375, 570);

                                            rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                            rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                            rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                            rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                            rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                            rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                            rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                            rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                            rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                            rotate_sprite(buffer, atras, Cx[9], Cy[9], ftofix(45.17));

                                            blit(buffer, screen, 0, 0, 0, 0, 1200, 800);

                                            if(mouse_b & 1 || mouse_b & 2){

                                                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                                masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                                draw_sprite(buffer, atras, 572, 292);
                                                draw_sprite(buffer, atras, 575, 295);
                                                draw_sprite(buffer, atras, 578, 298);
                                                draw_sprite(buffer, quedarse, 700, 570);
                                                draw_sprite(buffer, pedirR, 375, 570);

                                                rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                                rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                                rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                                rotate_sprite(buffer, atras, Cx[9], Cy[9], ftofix(45.17));

                                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                            }
                                        }else{
                                            if(mouse_x > 698 && mouse_x < 850 && mouse_y > 570 && mouse_y < 725){
                                                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                                masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                                draw_sprite(buffer, atras, 572, 292);
                                                draw_sprite(buffer, atras, 575, 295);
                                                draw_sprite(buffer, atras, 578, 298);
                                                draw_sprite(buffer, quedarseR, 700, 570);
                                                draw_sprite(buffer, pedir, 375, 570);

                                                rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                                rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                                rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                                rotate_sprite(buffer, atras, Cx[9], Cy[9], ftofix(45.17));

                                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                            }else{
                                                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                                                masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                                                draw_sprite(buffer, atras, 572, 292);
                                                draw_sprite(buffer, atras, 575, 295);
                                                draw_sprite(buffer, atras, 578, 298);
                                                draw_sprite(buffer, quedarse, 700, 570);
                                                draw_sprite(buffer, pedir, 375, 570);

                                                rotate_sprite(buffer, atras, Cx[0], Cy[0], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[1], Cy[1], ftofix(-45.17));
                                                rotate_sprite(buffer, atras, Cx[2], Cy[2], ftofix(-16.21));
                                                rotate_sprite(buffer, atras, Cx[3], Cy[3], ftofix(-16.21));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[(Jugador->sig)->carta].dat, Cx[4], Cy[4], ftofix(0));
                                                rotate_sprite(buffer, (BITMAP*)CartasN[Jugador->carta].dat, Cx[5], Cy[5], ftofix(0));
                                                rotate_sprite(buffer, atras, Cx[6], Cy[6], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[7], Cy[7], ftofix(16.21));
                                                rotate_sprite(buffer, atras, Cx[8], Cy[8], ftofix(45.17));
                                                rotate_sprite(buffer, atras, Cx[9], Cy[9], ftofix(45.17));

                                                blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                                            }
                                        }
                                    }



//}
//---------------------------------------------------------------------
                                regresar = true;
                                fflush(stdin);
                                readkey();
                            }
                        }
                        else if(mouse_x > 699 && mouse_x < 899 && mouse_y > 667 && mouse_y < 713)
                        {
                            blit(fondoreprreg, buffer, 0, 0, 0, 0, 1200, 800);
                            masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                            if(mouse_b & 1 || mouse_b & 2)
                            {
                                regresar = true;
                            }
                        }
                        else
                        {
                            blit(fondorepb, buffer, 0, 0, 0, 0, 1200, 800);
                            masked_blit(mesa, buffer, 0, 0, 142, 170, 917, 461);
                        }
                        blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
                    }
                }
            }
            else if(mouse_x > 571 && mouse_x < 640 && mouse_y > 520 && mouse_y < 561)
            {
                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                masked_blit(inicio3, buffer, 0, 0, 350, 213, 500, 375);
                if(mouse_b & 1)
                {
                    close_button_handler();
                }
            }
            else
            {
                blit(fondo, buffer, 0, 0, 0, 0, 1200, 800);
                masked_blit(inicio, buffer, 0, 0, 350, 213, 500, 375);
            }
            blit(buffer, screen, 0, 0, 0, 0, 1200, 800);
            regresar = false;
        }
//}
        destroy_bitmap(cursor);
        destroy_bitmap(mesa);
        destroy_bitmap(buffer);
    }
    return 0;
}
END_OF_MAIN();
//}
//---------------------------------------------------------------------
//{FUNCIONES
void llenarBarajaNum(int Bar[], int tam){
    bool repetido = true;
    int numero,i;
    numero = (rand()%52);
    Bar[0] = numero;
    for(i=0;i<tam;i++){
        while(repetido){
            numero = (rand()%52);
            repetido = busquedaSecuencial(Bar,i,numero);
        }
        Bar[i] = numero;
        repetido=true;
    }
}

void llenarBaraja(Tnodo *Baraja,int Bar[],int tam){
    int i;
    for(i=tam-1;i>=0;i--){
        nuevo = GenCola(Bar[i]);
        PushC(Baraja,&ultimo,&nuevo);
    }
}

Tnodo GenCola(int carta){
	Tnodo temp=NULL;
	int valor;
	temp=(Tnodo)malloc(sizeof(Tdato));
	temp->sig=NULL;
	temp->carta=carta;
	valor = temp->carta%13;
    if(valor>10)
    {
        temp->valor=10;
    }else{
            temp->valor=valor;
    }
	return temp;
}

Tnodo repartirCarta(){
    Tnodo temp;
    temp = POPC(&Baraja,&ultimo);
    return temp;
}

bool busquedaSecuencial(int Bar[],int tam,int numero){
    int i;
    for(i=0;i<tam;i++){
        if(Bar[i] == numero){
            return true;
        }
    }
    return false;
}

int busquedaGanador(int Bar[],int tam,int numero){
    int i;
    for(i=0;i<tam;i++){
        if(Bar[i] == numero){
            return i;
        }
    }
    return -1;
}

void PushC(Tnodo *cabeza,Tnodo *ultimo,Tnodo *nuevo)
{
    if(*cabeza)
    {
        (*ultimo)->sig=(*nuevo);
        *ultimo=*nuevo;
    }
    else
    {
        *cabeza=*nuevo;
        *ultimo=*nuevo;
    }
    *nuevo=NULL;
}

Tnodo POPC(Tnodo *cabeza,Tnodo *ultimo)
{
    Tnodo temp=NULL;
    if(*cabeza)
    {
        if(*cabeza==*ultimo)
        {
            temp=*ultimo;
            *cabeza=NULL;
            *ultimo=NULL;
        }
        else
        {
            temp=*cabeza;
            *cabeza=(temp)->sig;
            temp->sig=NULL;
        }
    }
    return temp;
}

Tnodo RepC(Tnodo *nuevo,Tnodo *cabeza)
{
    Tnodo temp=NULL;
    temp=*cabeza;
    while(temp!=NULL)
    {
        if((*nuevo)->carta==temp->carta)
        {
            return NULL;
        }
        temp=temp->sig;
    }
    return *nuevo;
}

void PushP(Tnodo *pila, Tnodo *nuevo)
{
    (*nuevo)->sig=(*pila);
    *pila=*nuevo;
    *nuevo=NULL;
}

Tnodo POPP(Tnodo *pila)
{
    Tnodo temp=NULL;
    if(*pila)
    {
        temp=(*pila);
        *pila=(temp)->sig;
        temp->sig=NULL;
    }
    return temp;
}

void close_button_handler(void)
{
    close_button_pressed = TRUE;
    salir=true;
}
END_OF_FUNCTION(close_button_handler)
//}
//---------------------------------------------------------------------
