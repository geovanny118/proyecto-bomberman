#include <cstdlib>
#include <allegro.h>
#include <iostream>
//esto es un cambio
//definicion de fila y columnas
#define MAXFILAS 17 //FILAS EJE Y
#define MAXCOL 30 //COLUMNAS EJE X


using namespace std;

//variables globales
BITMAP *buffer;
BITMAP *roca;
BITMAP *fondo1 ;//fondo blanco
BITMAP *fondo2 ;//inicio amarillo
BITMAP *fondo3 ;//nivel amarillo
BITMAP *fondo4 ;//salir amarillo
BITMAP *fondo5 ;//inicio blanco
BITMAP *fondo_s ;//fondo blanco seleccion
BITMAP *fondo_s1 ;//nivel 1 amarillo
BITMAP *fondo_s2 ;//nivel 2 amarillo
BITMAP *fondo_s3 ;//nivel 3 amarillo
BITMAP *fondo_sr ;//regresar amarillo
BITMAP *cursor ;//cursor
BITMAP *bomberbmp ;//personaje
BITMAP *bomberman ;
BITMAP *bomba;
BITMAP *bomba2;
BITMAP *muro;
BITMAP *win;
BITMAP *lose;
BITMAP *fuego;
BITMAP *muertebmp;
BITMAP *muerte_buffer;
BITMAP *llave;
BITMAP *puerta;
MIDI *musica1;//musica del mundo
MIDI *musica2;//musica del menu

//coordenada del personaje
int vida=3; //vidas personaje
int dir=5;
//posicion inicial:30*90
int px=30, py=90;
int anterior_px, anterior_py;//guarda la ultima posicion de bomberman
int bx, by;//ubicacion de la bomba
bool p=false;// p=true, poner bomba
int t=0; //tiempo de espera para explotar la bomba
int s=0;//tiempo de espera de paso para los fantasmas
bool bom = false;
bool e= false; //orden para mostrar explosion
int ex, ey;//ubicacion de la explosion
int puerta_x=630, puerta_y=150;
bool mun = false;//municion de bomberman
bool clave=false;
int itemx=750, itemy=0;


int vidas_globos=5;
int anterior_enemigo_x;
int anterior_enemigo_y;

//       [20-1]    [31-1]
char mapa[MAXFILAS][MAXCOL] =
{

    "NNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                         N",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X   DDDDDDDDDDDDDDDDDDDDDDX",
    "X XDXDXDX XDXDXDX X XDX X X",
    "X DDDDDDD    DDD     D    X",
    "X XDXDXDX X XDXDXDX XDXDX X",
    "X DDDDDD                  X",
    "XDX XDXDX X XDXDXDX X XDX X",
    "X   D D D   DDDDDD        X",
    "XDXDX X XDX X X XDX XDX X X",
    "X       DD      DDDDZDDDDDX",
    "XDX XDXDXDX X X XDXDXDXDX X",
    "XDDDDDDDDDD    DDDDDDD    X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

//       [20-1]    [31-1]
char mapa1[MAXFILAS][MAXCOL] =
{

    "NNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                         N",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X   DDDDDDDDDDDDDDDDDDDDDDX",
    "X XDXDXDX XDXDXDX X XDX X X",
    "X DDDDDDD    DDD     D    X",
    "X XDXDXDX X XDXDXDX XDXDX X",
    "X DDDDDD                  X",
    "XDX XDXDX X XDXDXDX X XDX X",
    "X   D D D   DDDDDD        X",
    "XDXDX X XDX X X XDX XDX X X",
    "X       DD      DDDDZDDDDDX",
    "XDX XDXDXDX X X XDXDXDXDX X",
    "XDDDDDDDDDD    DDDDDDD    X",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

char mapa2[MAXFILAS][MAXCOL] =
{

    "NNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                         N",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X      D     DDDDD        X",
    "XDXDXDXDX X XDXDXDX X X X X",
    "XDDDDDDD  DDDDDDDDDDDDDDDDX",
    "XDXDX X XDXDXDX X X X X X X",
    "XDDDD    DDDDDD           X",
    "X XDX X XDXDXDX X X X X X X",
    "X  D    DDDDDD    DDDDDDD X",
    "X XDX XDXDXDX X X XDXDXDX X",
    "X  DDDZDD     DDDDDDDDDDD X",
    "X XDXDXDX X X XDXDXDXDXDX X",
    "X DDDDDDDDDDDDDDDDDDDDDDDDX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

char mapa3[MAXFILAS][MAXCOL] =
{

    "NNNNNNNNNNNNNNNNNNNNNNNNNNN",
    "N                         N",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
    "X         DDDDDDDDDDDDD   X",
    "X XDXDXDX XDXDXDXDXDXDX X X",
    "X DDDDDDD   DDDDDDDDDDDD  X",
    "X XDXDXDXDXDXDXDXDXDXDX X X",
    "X DDDDDDDDDDDDDDDDDD      X",
    "X XDXDXDXDXDXDXDXDXDX XDXDX",
    "X DDDDDDDDD   DDDDD   DDDDX",
    "X XDX XDXDX X XDXDX XDXDXDX",
    "X     DDDDD   DDDDD DDD   X",
    "XDXDXDXDXDXDXDXDXDX XDX X X",
    "XDDDDDDDDDDDDDDDDDD DDD  ZX",
    "XXXXXXXXXXXXXXXXXXXXXXXXXXX",
};


//mapa auxiliar
char mapa_auxiliar[MAXFILAS][MAXCOL];

//funciones prototipo
void backupMapa();
void restaurarMapa();
void cargarMapa1();
void cargarMapa2();
void cargarMapa3();
void crear_mundo();
void dibujar();
void dibujar_bomberman();
void dibujar_globos();
void globos();
void dibujar_globos();
void globos1();
void poner_bomba();
void explosion();
void destrucion();
void muerte1();

//clase enemigo
class enemigo
{
    //parametros privados
    BITMAP *enebmp;
    BITMAP *ene;
    //coordenadas del globos
    int edir = 0;//globos
    //int _x=30*9, _y=30*5; posicion inicial globo
    int _x, _y;
    bool life;

public:
    enemigo() {}; //consstructor vacio
    enemigo(int x, int y); //constructor por parametros
    bool setVidaenemigo(bool l);//metodo set vida enemigo,
    void dibujarEnemigo() const;
    void vidaEnemigo();
    void moverEnemigo();
    void muerte2();
    int posicionEnemigo_X();
    int posicionEnemigo_Y();

};

bool enemigo::setVidaenemigo(bool l)
{
    life=l;//si vida es true, llama a caminar
}

enemigo::enemigo(int x, int y)
{
    _x=x;
    _y=y;
    edir=rand()%3;
    //edir=5;//globos quietos
    ene = create_bitmap(30,30);
    enebmp = load_bitmap("img\\MUNDO\\ENE.bmp", NULL);
    life=true;//si vida es true, llama a caminar
}


void enemigo::dibujarEnemigo() const
{
    blit(enebmp,ene,0,0,0,0,30,30);
    draw_sprite(buffer,ene,_x,_y);
    s++;
}

void enemigo::vidaEnemigo()
{
    if(life==true)
    {
        moverEnemigo();

    }
    if(life==false)
    {
    }
}

void enemigo::moverEnemigo()
{
    dibujarEnemigo();
    muerte2();

 //   if (mapa [_y/30][_x/30] == ' ' )
 //   {
        //rest(60);
        edir = rand()%4;
        //edir=5;//quieto

 //   }


    if (edir == 0 && s==3)
    {

        if ((mapa[_y/30][(_x-30)/30] != 'X')  && (mapa[_y/30][(_x-30)/30] != 'D') && (mapa[_y/30][(_x-30)/30] != 'b') && (mapa[_y/30][(_x-30)/30] != 'Y') && (mapa[_y/30][(_x-30)/30] != 'Z'))
            // pasos+=1;
            {
            //rest(100);
            _x-=30;
            s=0;
            }
        else {
                edir = rand()%3;
                s=0;
        }

    }
    if (edir == 1 && s==3)
    {

        if ((mapa[_y/30][(_x+30)/30] != 'X')  && (mapa[_y/30][(_x+30)/30] != 'D') && (mapa[_y/30][(_x+30)/30] != 'b') && (mapa[_y/30][(_x+30)/30] != 'Y') && (mapa[_y/30][(_x+30)/30] != 'Z'))
            // pasos+=1;
            {
            //rest(100);
            _x+=30;
            s=0;
            }
        else {
                edir = rand()%3;
                s=0;
        }

    }
    if (edir == 2 && s==3)
    {

        if ((mapa[(_y-30)/30][(_x)/30] != 'X') && (mapa[(_y-30)/30][_x/30] != 'D') && (mapa[(_y-30)/30][_x/30] != 'b') && (mapa[(_y-30)/30][_x/30] != 'Y') && (mapa[(_y-30)/30][_x/30] != 'Z'))
            //pasos+=1;
            {
            //rest(100);
            _y-=30;
            s=0;
            }
        else {
                edir = rand()%3;
                s=0;
        }

    }
    if (edir == 3 && s==3)
    {

        if ((mapa[(_y+30)/30][(_x)/30] != 'X') &&(mapa[(_y+30)/30][_x/30] != 'D') && (mapa[(_y+30)/30][_x/30] != 'b') && (mapa[(_y+30)/30][_x/30] != 'Y') && (mapa[(_y+30)/30][_x/30] != 'Z'))
            //pasos+=1;
            {
            //rest(100);
            _y+=30;
            s=0;
            }
        //tiempo para explotar la bomba
        //t+=1;
        else {
                edir = rand()%3;
                s=0;
        }

    }
}

//metodo para matar a bomberman
void enemigo::muerte2()
{
    if(px==_x && py==_y)
    {


        //inicializa las coordenadas en 0
        dir=5;

        while(!key[KEY_C])
        {

            //para imprimir el fondo game over en el buffer, se usa la funcion blit
            blit(muertebmp,muerte_buffer,0,0,0,0,800,530);
            //imprime el buffer en pantalla para que se vea todo el menu:
            draw_sprite(buffer,muertebmp,30*8,30*6);
            dibujar();
            midi_pause();//para la musica
        }
        play_midi(musica1,1); //reproduce la musica del menu
        //resta la vida
        e=false;
        mapa[by/30][bx/30] = ' ';
        vida--;
        px=30, py=90;
        bx=0;
        by=0;
        //SI MUERE PRESIONE C PARA CONTINUAR
        // while(!key[KEY_C]);

        mapa[ey/30][ex/30] = ' ';
        if((mapa[(ey+30)/30][ex/30] != 'X'))mapa[(ey+30)/30][ex/30] = ' ';
        if((mapa[(ey-30)/30][ex/30] != 'X'))mapa[(ey-30)/30][ex/30] = ' ';
        if((mapa[ey/30][(ex+30)/30] != 'X'))mapa[ey/30][(ex+30)/30] = ' ';
        if((mapa[ey/30][(ex-30)/30] != 'X'))mapa[ey/30][(ex-30)/30] = ' ';
    }

    if(_y==anterior_py && _x==anterior_px)
    {

        e=false;
        mapa[by/30][bx/30] = ' ';
        anterior_enemigo_x=_x;
        anterior_enemigo_y=_y;
        _x=px;
        _y=py;
        //resta la vida
        vida--;
        //inicializa las coordenadas en 0
        dir=5;

        while(!key[KEY_C])
        {

            //para imprimir el fondo game over en el buffer, se usa la funcion blit
            blit(muertebmp,muerte_buffer,0,0,0,0,800,530);
            //imprime el buffer en pantalla para que se vea todo el menu:
            draw_sprite(buffer,muertebmp,30*8,30*6);
            dibujar();
            midi_pause();//para la musica
        }
        play_midi(musica1,1); //reproduce la musica del menu
        px=30, py=90;
        bx=0;
        by=0;
        _x=anterior_enemigo_x;
        _y=anterior_enemigo_y;
        //SI MUERE PRESIONE C PARA CONTINUAR
        // while(!key[KEY_C]);

        mapa[ey/30][ex/30] = ' ';
        if((mapa[(ey+30)/30][ex/30] != 'X'))mapa[(ey+30)/30][ex/30] = ' ';
        if((mapa[(ey-30)/30][ex/30] != 'X'))mapa[(ey-30)/30][ex/30] = ' ';
        if((mapa[ey/30][(ex+30)/30] != 'X'))mapa[ey/30][(ex+30)/30] = ' ';
        if((mapa[ey/30][(ex-30)/30] != 'X'))mapa[ey/30][(ex-30)/30] = ' ';
    }

}


//entrega las coordenadas del enemigo
int enemigo::posicionEnemigo_X()
{
    return _x;
}

int enemigo::posicionEnemigo_Y()
{
    return _y;
}
//                              FUNCION MAIN
int main()
{
    //inicia los graficos en alegro
    allegro_init();
    //instalamos el raton
    install_mouse();
    //instalamos el TECLADO
    install_keyboard();

    //bits de los colores, 16, 32...
    set_color_depth(16);
    //detectar el controlador grafico del pc, Tamaño de la pantalla (800x530)
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 800,449,0,0);
    // incializa el audio en allegro
    if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) != 0)
    {
        allegro_message("Error: inicializando sistema de sonido\n%s\n", allegro_error);
        return 1;
    }

    // ajustamos el volumen
    set_volume(200, 200);

    // definimos el audio del programa
    musica1  = load_midi("sonido\\mundo.mid");
    musica2 = load_midi("sonido\\menu.mid");

    //para crear la pantalla (300,200), se crea una variable tipo BITMAP
    buffer = create_bitmap(800,449);

    //cargar imagen del menu, como variables tipo BITMAP

    fondo1 = load_bitmap("img\\MENU\\MENU1.bmp",NULL);//fondo blanco
    fondo2 = load_bitmap("img\\MENU\\MENU2.bmp",NULL);//inicio amarillo
    fondo3 = load_bitmap("img\\MENU\\MENU3.bmp",NULL);//nivel amarillo
    fondo4 = load_bitmap("img\\MENU\\MENU4.bmp",NULL);//salir amarillo
    fondo5 = load_bitmap("img\\MUNDO\\MUNDO.bmp",NULL);//inicio blanco

    fondo_s  = load_bitmap("img\\NIVELES\\SELECCIONAR.bmp",NULL);//fondo blanco seleccion
    fondo_s1 = load_bitmap("img\\NIVELES\\NIVEL1.bmp",NULL);//nivel 1 amarillo
    fondo_s2 = load_bitmap("img\\NIVELES\\NIVEL2.bmp",NULL);//nivel 2 amarillo
    fondo_s3 = load_bitmap("img\\NIVELES\\NIVEL3.bmp",NULL);//nivel 3 amarillo
    fondo_sr = load_bitmap("img\\NIVELES\\REGRESAR.bmp",NULL);//regresar amarillo

    cursor = load_bitmap("img\\MENU\\CURSOR.bmp",NULL);//cursor
    roca = load_bitmap("img\\MUNDO\\MURO.bmp",NULL);//roca, muro, no destructible
    muro = load_bitmap("img\\MUNDO\\MURO_D.bmp",NULL);//roca, muro, destructible
    bomberman = create_bitmap(30,30);
    bomberbmp = load_bitmap("img\\MUNDO\\PER.bmp",NULL); //personaje bomberman
    bomba = load_bitmap("img\\MUNDO\\BOMBA1.bmp",NULL); //bomba apagada
    bomba2 = load_bitmap("img\\MUNDO\\BOMBA2.bmp",NULL); //bomba apagada
    win = load_bitmap("img\\MENU\\WIN.bmp",NULL);// pantalla de victoria
    lose = load_bitmap("img\\MENU\\LOSE.bmp",NULL);//pantalla de derrota
    fuego = load_bitmap("img\\mundo\\EXPLOSION.bmp",NULL);//dibujo explosion
    muertebmp = load_bitmap("img\\mundo\\mensajemuerte.bmp",NULL);//mensaje muerte
    muerte_buffer = create_bitmap(372,101);
    llave = load_bitmap("img\\mundo\\LLAVE.bmp",NULL);
    puerta = load_bitmap("img\\mundo\\PUERTA.bmp",NULL);
    bool salida = false;
    int c=0;

    play_midi(musica2,1); //reproduce la musica del menu

    while(!salida)
    {

        //***********************************************************************INICIO JUEGO**************************************************************
        if(mouse_x > 327 && mouse_x < 469 && mouse_y > 148 && mouse_y < 193 && c==0)
        {

            //para imprimir el fondo2, se usa la funcion blit
            blit(fondo2,buffer,0,0,0,0,800,530);
            //si presiona click, 1:click izquierdo, 2:click derecho
            if(mouse_b && 1)
            {


                c=1;
                dir=5;
                px=30;
                py=90;
                bx=0;
                by=0;
                ex=0;
                ey=0;
                vida=3;
                anterior_px=0;
                anterior_py=0;
                vidas_globos=5;
                clave=false;
                puerta_x=630;
                puerta_y=150;


                //creacion de objectos enemigos
                enemigo A(30*9, 30*5);
                enemigo B(30*25, 30*4);
                enemigo C(30*25, 30*12);
                enemigo D(30*5, 30*10);
                enemigo E(30*13, 30*10);

                //fondo1=fondo5;
                //primero guarda el mapa limpio en una variable auxiliar
                backupMapa();
                //salida = true

                play_midi(musica1,1);//siempre suena musica en el mundo.
                while(!key[KEY_ESC] && !key[KEY_ENTER])
                {

                    anterior_px=px;
                    anterior_py=py;
                    //teclado: lee que tecla presiono y asigna un numero en dir
                    /*
                    if(key[KEY_RIGHT])dir=1;
                    else if(key[KEY_LEFT])dir=2;
                    else if(key[KEY_UP])dir=3;
                    else if(key[KEY_DOWN])dir=4;
                    else if (key[KEY_SPACE])p=true;
                        */
                    //muestra el tiempo de la bomba en pantalla cmd
                    if(bom)
                    {
                        t++;
                        cout << t << endl;
                    }


                    //depende del valor de dir, incrementa en el eje x o y
                    //derecha
                    if(key[KEY_RIGHT])
                    {
                        //pasa el personaje siempre que no halla muro
                        if((mapa[py/30][(px+30)/30] != 'X') && (mapa[py/30][(px+30)/30] != 'D') && (mapa[py/30][(px+30)/30] != 'b') && (mapa[py/30][(px+30)/30] != 'Z'))
                        {
                            //rest(20);
                            px +=30;
                        }

                    }
                    //izquierda
                    if(key[KEY_LEFT])
                    {
                        if((mapa[py/30][(px-30)/30] != 'X') && (mapa[py/30][(px-30)/30] != 'D') && (mapa[py/30][(px-30)/30] != 'b') && (mapa[py/30][(px-30)/30] != 'Z'))
                        {
                            rest(30);
                            px -=30;
                        }

                    }
                    //arriba
                    if(key[KEY_UP])
                    {
                        if ((mapa[(py-30)/30][px/30] != 'X') && (mapa[(py-30)/30][px/30] != 'D') && (mapa[(py-30)/30][px/30] != 'b') && (mapa[(py-30)/30][px/30] != 'Z'))
                        {
                            //rest(20);
                            py -=30;
                        }

                    }
                    //abajo
                    if(key[KEY_DOWN])
                    {
                        if((mapa[(py+30)/30][px/30] != 'X') && (mapa[(py+30)/30][px/30] != 'D') && (mapa[(py+30)/30][px/30] != 'b') && (mapa[(py+30)/30][px/30] != 'Z'))
                        {
                            //rest(20);
                            py +=30;

                        }
                        else dir = 0;//no se mueve
                    }
                    //poner bomba
                    if(key[KEY_SPACE] && mun == false)
                    {

                        poner_bomba();

                    }

                    //tiempo de espera de la explosion, son aproximadamente 2 segundos desde que se pone la bomba hasta que
                    //explote
                    if(t==5 && e==true)
                    {
                        explosion();
                        //k=0;
                        //t=0;
                        e=false;
                    }

                    //
                    if(t==7)
                    {
                        destrucion();
                        //k=0;
                        //t=0;
                    }

                    //espera para poner bomba tras explosion
                    if(t==9)
                    {
                        mun=false;
                        t=0;
                    }

                    //-------------------------------------CONDICIONES QUE DETECTAN SI EL ENEMIGO ES ALCANZADO POR LA EXPLOSION--
                    //detecta si la posicion del enemigo [A] esta en el rango de la explosion
                    if((mapa[(A.posicionEnemigo_Y())/30][(A.posicionEnemigo_X())/30]== 'e') && t==6)
                    {

                        vidas_globos--;
                        A.setVidaenemigo(false);//quita el globo del mapa
                        A.vidaEnemigo();

                    }

                    //detecta si la posicion del enemigo [B] esta en el rango de la explosion
                    if((mapa[(B.posicionEnemigo_Y())/30][(B.posicionEnemigo_X())/30]== 'e') && t==6)
                    {

                        vidas_globos--;
                        B.setVidaenemigo(false);//quita el globo del mapa
                        B.vidaEnemigo();

                    }

                    //detecta si la posicion del enemigo [C] esta en el rango de la explosion
                    if((mapa[(C.posicionEnemigo_Y())/30][(C.posicionEnemigo_X())/30]== 'e') && t==6)
                    {

                        vidas_globos--;
                        C.setVidaenemigo(false);//quita el globo del mapa
                        C.vidaEnemigo();

                    }

                    //detecta si la posicion del enemigo [D] esta en el rango de la explosion
                    if((mapa[(D.posicionEnemigo_Y())/30][(D.posicionEnemigo_X())/30]== 'e') && t==6)
                    {

                        vidas_globos--;
                        D.setVidaenemigo(false);//quita el globo del mapa
                        D.vidaEnemigo();

                    }

                    //detecta si la posicion del enemigo [E] esta en el rango de la explosion
                    if((mapa[(E.posicionEnemigo_Y())/30][(E.posicionEnemigo_X())/30]== 'e') && t==6)
                    {

                        vidas_globos--;
                        E.setVidaenemigo(false);//quita el globo del mapa
                        E.vidaEnemigo();

                    }

                    clear(buffer);
                    blit(fondo5,buffer,0,0,0,0,800,530);
                    //imprimir en pantalla
                    crear_mundo();
                    dibujar_bomberman();
                    //globos();
                    A.vidaEnemigo();
                    B.vidaEnemigo();
                    C.vidaEnemigo();
                    D.vidaEnemigo();
                    E.vidaEnemigo();
                    dibujar();
                    muerte1();

                    if(vidas_globos==0)
                    {
                        while(!key[KEY_ENTER])
                        {
                            //para imprimir el fondo game over en el buffer, se usa la funcion blit
                            blit(win,buffer,0,0,0,0,800,530);
                            //imprime el buffer en pantalla para que se vea todo el menu:
                            blit(buffer,screen,0,0,0,0,800,530);
                            //rest(5000);
                        }
                        return 0;
                    }
                    //pone una image de la llave en el menu.
                    if(clave==true)
                    {
                        //mapa[itemy][itemx]
                        mapa[1][25] = 'Y';//coloca la llave en el inventario
                    }

                    if(px==puerta_x && py==puerta_y && clave==true)
                    {
                        //while(!key[KEY_ENTER]){
                        blit(win,buffer,0,0,0,0,800,530);
                        blit(buffer,screen,0,0,0,0,800,530);
                        //}
                        c=0;
                        //return 0;

                    }


                    //sale del juego
                    if(vida<=0)
                    {
                        while(!key[KEY_ESC])
                        {

                            //para imprimir el fondo game over en el buffer, se usa la funcion blit

                            blit(lose,buffer,0,0,0,0,800,530);
                            //imprime el buffer en pantalla para que se vea todo el menu:
                            blit(buffer,screen,0,0,0,0,800,530);
                        }
                        //return 0;
                        restaurarMapa();
                        c=0;
                        t=0;
                    }
                    //rest(180);
                    rest(180);
                    //texto en patalla
                    textprintf(fondo5, font, 10, 10, pallete_color[15], "controles:");
                    textprintf(fondo5, font, 10, 20, pallete_color[15], "[flechas de direcciones]: mover personaje");
                    //textprintf(fondo5, font, 420, 20, pallete_color[15], "[tecla C]: continuar");
                    textprintf(fondo5, font, 10, 30, pallete_color[15], "[barra espaciadora]: poner bomba");
                    textprintf(fondo5, font, 10, 40, pallete_color[15], "[esc]: salir");
                    textprintf(fondo5, font, 720, 10, pallete_color[15], "vidas: %d", vida);
                    textprintf(fondo5, font, 664, 20, pallete_color[15], "vidas globos: %d", vidas_globos);

                }
                //cuando sale del juego carga el mapa limpio.
                midi_pause();//para la musica
                play_midi(musica2,1); //reproduce la musica del menu
                restaurarMapa();
                c=0;
                t=0;

            }

        }

//*********************SELECCIONAR NIVEL*********************************************************************************************************
        else if (mouse_x > 187 && mouse_x < 612 && mouse_y > 218 && mouse_y < 269 && c==0)
        {
            //para imprimir el fondo3, se usa la funcion blit
            blit(fondo3,buffer,0,0,0,0,800,530);
            if(mouse_b & 1)
            {
                c=2;
                while(!key[KEY_ESC] && c==2)
                {
                    blit(fondo_s,buffer,0,0,0,0,800,530);

                    if(mouse_x > 332 && mouse_x < 466 && mouse_y > 127 && mouse_y < 170 && c==2)
                    {
                        blit(fondo_s1,buffer,0,0,0,0,800,530);
                        //seleccionar nivel 1
                        if(mouse_b & 1)
                        {
                            cargarMapa1();
                            c=0;
                            rest(200);
                        }

                    }

                    if(mouse_x > 324 && mouse_x < 470 && mouse_y > 189 && mouse_y < 232 && c==2)
                    {
                        blit(fondo_s2,buffer,0,0,0,0,800,530);
                        //seleccionar nivel 2
                        if(mouse_b & 1)
                        {
                            cargarMapa2();
                            c=0;
                            rest(200);
                        }
                    }

                    if(mouse_x > 320 && mouse_x < 480 && mouse_y > 252 && mouse_y < 293 && c==2)
                    {
                        blit(fondo_s3,buffer,0,0,0,0,800,530);
                        //seleccionar nivel 3
                        if(mouse_b & 1)
                        {
                            cargarMapa3();
                            c=0;
                            rest(200);
                        }
                    }


                    if(mouse_x > 302 && mouse_x < 497 && mouse_y > 313 && mouse_y < 355 && c==2)
                    {
                        blit(fondo_sr,buffer,0,0,0,0,800,530);
                        //regresar al menu
                        if(mouse_b & 1)
                        {
                            c=0;
                            rest(200);//anti-rebote
                        }
                    }
                    //else blit(fondo_s,buffer,0,0,0,0,800,530);

                    //imprimir cursor pero con transparencia, masked_blit
                    masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);
                    //imprime el buffer en pantalla para que se vea todo el menu:
                    blit(buffer,screen,0,0,0,0,800,530);
                }
                c=0;
            }
        }
//*********************SALIR*********************************************************************************************************************
        else if (mouse_x > 334 && mouse_x < 464 && mouse_y > 292 && mouse_y < 342 && c==0)
        {
            //para imprimir el fondo4, se usa la funcion blit
            blit(fondo4,buffer,0,0,0,0,800,530);
            if(mouse_b & 1)
            {
                salida = true;
            }
        }
        else blit(fondo1,buffer,0,0,0,0,800,530);

        //imprimir cursor pero con transparencia, masked_blit
        masked_blit(cursor,buffer,0,0,mouse_x,mouse_y,13,22);

        //imprime el buffer en pantalla para que se vea todo el menu:
        blit(buffer,screen,0,0,0,0,800,530);


    }
    return 0;
}
END_OF_MAIN();
//-------------------------------------------------------------------------------------------------------------------------------
// funciones auxiliares


//funcion para hacer el mapa, solo crea el mundo, no lo muestra
void crear_mundo()
{

    int row, col;

    for(row =0 ; row < MAXFILAS ; row++)
    {

        for(col = 0; col < MAXCOL ; col++)
        {
            //dibuja los muros no destruibles
            if(mapa[row][col] == 'X')
            {
                draw_sprite(buffer, roca, col*30, row*30);
            }

            //donde va a estar escondida la llave
            if(mapa[row][col] == 'Z')
            {
                draw_sprite(buffer, muro, col*30, row*30);
            }

            //dibuja puerta
            if(mapa[row][col] == 'P')
            {
                draw_sprite(buffer, puerta, col*30, row*30);
            }
            //dibuja los muros destruibles
            if(mapa[row][col] == 'D')
            {
                draw_sprite(buffer, muro, col*30, row*30);
            }

            //dibuja las bombas que ponen en el campo
            if(mapa[row][col] == 'b')
            {
                draw_sprite(buffer, bomba2, col*30, row*30);
                //mapa[row][col] = ' ';
            }
            if(mapa[row][col]== 'e')
            {
                draw_sprite(buffer, fuego, col*30, row*30);
            }
            //dibuja las bombas en el campo
            else if (mapa[row][col] == 'o')
            {
                draw_sprite(buffer, bomba, col*30, row*30);
                //si bomberman se encuentra en la misma posicion de las bombas, borrar las bombas
                if(py/30 == row && px/30 == col)
                {
                    //borra la bomba del mapa
                    mapa[row][col] = ' ';
                }
            }
            //dibuja las bombas en el campo
            else if (mapa[row][col] == 'Y')
            {
                draw_sprite(buffer, llave, col*30, row*30);
                //si bomberman se encuentra en la misma posicion de las bombas, borrar las bombas
                if(py/30 == row && px/30 == col)
                {
                    clave=true;
                    //borra la bomba del mapa
                    mapa[row][col] = ' ';
                }
            }
        }
    }

}

//funcion dibujar mapa
void dibujar()
{
    //fondo1=fondo5;
    blit(buffer,screen,0,0,0,0,800,449);
}

void dibujar_bomberman()
{
    blit(bomberbmp, bomberman, dir*30,0,0,0,30,30);
    draw_sprite(buffer, bomberbmp, px, py);
}

void poner_bomba()
{
    //primero se guarda la posicion del personaje en by y bx
    mun = true;
    by=py;
    bx=px;
    //luego pone en esta posicion la letra b
    mapa[by/30][bx/30] = 'b';
    //play_sample(sonido1,300,150,1000,0); //volumen, velocidad, distribucion parlantes, repetir
    p=false;
    bom = true;
    t = 0;
    e=true;
}

void explosion()
{
    ey=by;
    ex=bx;
    mapa[ey/30][ex/30] = 'e';
    //cambios: campo de la explosion, en cruz y condiciones para que no destruya los muros azules
    if((mapa[(ey+30)/30][ex/30] != 'X') && (mapa[(ey+30)/30][ex/30] != 'Y'))
    {
        if((mapa[(ey+30)/30][ex/30] == 'Z'))
        {
            mapa[(ey+30)/30][ex/30] = 'e';
            mapa[(ey+30)/30][ex/30] = 'Y';
        }
        else
        {
            mapa[(ey+30)/30][ex/30] = 'e';
        }
    }
    if((mapa[(ey-30)/30][ex/30] != 'X') && (mapa[(ey-30)/30][ex/30] != 'Y'))
    {
        if((mapa[(ey-30)/30][ex/30] == 'Z'))
        {
            mapa[(ey-30)/30][ex/30] = 'e';
            mapa[(ey-30)/30][ex/30] = 'Y';
        }
        else
        {
            mapa[(ey-30)/30][ex/30] = 'e';
        }
    }
    if((mapa[ey/30][(ex+30)/30] != 'X') && (mapa[ey/30][(ex+30)/30] != 'Y'))
    {
        if((mapa[ey/30][(ex+30)/30] == 'Z'))
        {
            mapa[ey/30][(ex+30)/30] = 'e';
            mapa[ey/30][(ex+30)/30] = 'Y';
        }
        else
        {
            mapa[ey/30][(ex+30)/30] = 'e';
        }
    }
    if((mapa[ey/30][(ex-30)/30] != 'X') && (mapa[ey/30][(ex-30)/30] != 'Y'))
    {
        if((mapa[ey/30][(ex-30)/30] == 'Z'))
        {
            mapa[ey/30][(ex-30)/30] = 'e';
            mapa[ey/30][(ex-30)/30] = 'Y';
        }
        else
        {
            mapa[ey/30][(ex-30)/30] = 'e';
        }
    }

}

void destrucion()
{
    mapa[ey/30][ex/30] = ' ';
    //cambios: campo de la explosion, en cruz
    if((mapa[(ey+30)/30][ex/30] != 'X') && (mapa[(ey+30)/30][ex/30] != 'Y'))
    {
        mapa[(ey+30)/30][ex/30] = ' ';
    }
    if((mapa[(ey-30)/30][ex/30] != 'X') && (mapa[(ey-30)/30][ex/30] != 'Y'))
    {
        mapa[(ey-30)/30][ex/30] = ' ';
    }
    if((mapa[ey/30][(ex+30)/30] != 'X') && (mapa[ey/30][(ex+30)/30] != 'Y'))
    {
        mapa[ey/30][(ex+30)/30] = ' ';
    }
    if((mapa[ey/30][(ex-30)/30] != 'X') && (mapa[ey/30][(ex-30)/30] != 'Y'))
    {
        mapa[ey/30][(ex-30)/30] = ' ';
    }
}

//funcion que crea un backup del mapa original, (sin ser utilizado por el jugador)
void backupMapa()
{
    for(int f=0; f<17; f++)
    {
        for(int colm=0; colm<30; colm++)
        {
            //pasando contenido la matriz mapa a la matriz auxiliar
            mapa_auxiliar[f][colm]=mapa[f][colm];
        }
    }
}

//funcion para cargar el mapa original al jugo, despues de que salga del mundo.
void restaurarMapa()
{
    for(int f=0; f<17; f++)
    {
        for(int colm=0; colm<30; colm++)
        {
            //pasando contenido la matriz mapa a la matriz auxiliar
            mapa[f][colm]=mapa_auxiliar[f][colm];
        }
    }
}

void cargarMapa1()
{
    for(int f=0; f<17; f++)
    {
        for(int colm=0; colm<30; colm++)
        {
            //pasando contenido la matriz mapa a la matriz auxiliar
            mapa[f][colm]=mapa1[f][colm];
        }
    }
}

void cargarMapa2()
{
    for(int f=0; f<17; f++)
    {
        for(int colm=0; colm<30; colm++)
        {
            //pasando contenido la matriz mapa a la matriz auxiliar
            mapa[f][colm]=mapa2[f][colm];
        }
    }
}

void cargarMapa3()
{
    for(int f=0; f<17; f++)
    {
        for(int colm=0; colm<30; colm++)
        {
            //pasando contenido la matriz mapa a la matriz auxiliar
            mapa[f][colm]=mapa3[f][colm];
        }
    }
}

void muerte1()
{

    //muerte si bomberman es alcanzado por la explosion de una bomba
    if(mapa[py/30][px/30] == 'e')
    {


        vida--;
        //inicializa las coordenadas en 0
        dir=5;
        /*  for(int i =px; i>=30;i--){
              px-=10;
              py-=10;

              dibujar_bomberman();
              blit(muerte,buffer,0,0,0,0,800,530);
              blit(muerte,muerte_buffer,0,0,0,0,0,0);
              draw_sprite(buffer,muerte,0,0);
              rest(20);
          }*/
        while(!key[KEY_C])
        {

            //para imprimir el fondo game over en el buffer, se usa la funcion blit

            blit(muertebmp,muerte_buffer,0,0,0,0,800,530);
            //imprime el buffer en pantalla para que se vea todo el menu:
            draw_sprite(buffer,muertebmp,30*8,30*6);
            //rest(5000);
            dibujar();
            midi_pause();//para la musica
        }
        play_midi(musica1,1); //reproduce la musica del menu
        px=30, py=90;
        bx=0;
        by=0;
        //SI MUERE PRESIONE C PARA CONTINUAR
        //while(!key[KEY_C]);
        mapa[ey/30][ex/30] = ' ';
        if((mapa[(ey+30)/30][ex/30] != 'X'))mapa[(ey+30)/30][ex/30] = ' ';
        if((mapa[(ey-30)/30][ex/30] != 'X'))mapa[(ey-30)/30][ex/30] = ' ';
        if((mapa[ey/30][(ex+30)/30] != 'X'))mapa[ey/30][(ex+30)/30] = ' ';
        if((mapa[ey/30][(ex-30)/30] != 'X'))mapa[ey/30][(ex-30)/30] = ' ';

    }

}



