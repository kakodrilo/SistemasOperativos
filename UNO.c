#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>

/* Función que revisa si existe una carpeta con el nombre pasado por parámetro
    retorna 0 si no existe
    retorna 1 si existe
    se utiliza la función 'opendir' de la libreria 'dirent', la que rettorna un puntero a la carpeta
    si esta existe.
*/
int D_EXISTE(char n_carpeta[]){
    DIR * carpeta;
    carpeta = opendir(n_carpeta);
    if (carpeta == NULL){
        free((void *)carpeta);
        return 0;
    }
    else{
        free((void *)carpeta);
        return 1;
    }
}

void Crear_Carpeta(char nombre[]){
    int i = D_EXISTE(nombre);
    char comando[40] = "rm -r ";  // defino en string el comando para borrar la carpeta
    if (i == 1){  // la carpeta ya existe
        strcat(comando,nombre);   // le agrego al comando el nombre de la carpeta
        system(comando);  // ejecuto el comando para borrar
    }
    mkdir(nombre,777); // se crea la carpeta totalmente vacía
    
}


/* Funcion que crea un archivo .txt según el nombre entregado en el nombre de la carpeta entregada
    se asume que la carpeta existe.
*/
void Crear_Archivo(char nombre_archivo[], char carpeta[]){
    char direccion[40];  // string donde se guarda el string: carpeta/nombre_archivo.txt
    sprintf(direccion,"%s/%s",carpeta,nombre_archivo);
    FILE * archivo = fopen(direccion,"w"); // se crea el archivo vacío
    if (archivo == NULL){
        printf("Error al generar archivo %s\n",nombre_archivo);
    }
    fclose(archivo); // se cierra el archivo creado para liberar la memoria
}


/* Función que genera todas las cartas de un color especificado
*/
void Generar_Cartas(char color[]){
    char carta[40];
    sprintf(carta,"0_%s_1.txt",color);
    Crear_Archivo(carta,"mazo");  // se crea la carta 0 (1 de cada color)
    int i;
    for (i = 1; i <= 9; i++){
        sprintf(carta,"%d_%s_1.txt",i,color);    // se generan los numeros (2 de cada color)
        Crear_Archivo(carta,"mazo");
        sprintf(carta,"%d_%s_2.txt",i,color);
        Crear_Archivo(carta,"mazo");

        if (i < 3){      // se generan las cartas de reversa, +2 y salto (2 de cada color)
            sprintf(carta,"+_%s_%d.txt",color,i);
            Crear_Archivo(carta,"mazo");

            sprintf(carta,"r_%s_%d.txt",color,i);
            Crear_Archivo(carta,"mazo");

            sprintf(carta,"s_%s_%d.txt",color,i);
            Crear_Archivo(carta,"mazo");
        }   
    }
}


/* Fución que genera las cartas Especiales  
*/
void Generar_CartasEspeciales(){
    int i;
    char carta_especial[40];
    for (i = 1; i <= 4; i++){          // se generan las cartas especiales de cambio de color y +4 (4 de cada una)
        sprintf(carta_especial,"x_n_%d.txt",i);
        Crear_Archivo(carta_especial,"mazo");

        sprintf(carta_especial,"c_n_%d.txt",i);
        Crear_Archivo(carta_especial,"mazo");
    }
}

/* Filtro para no agregar los archivos "." y ".." en el resultado
*/
int filtro(const struct dirent * carpeta){
    if ((strcmp(carpeta->d_name,".") == 0 ) || (strcmp(carpeta->d_name,"..") == 0 ) ) return 0; 
    // retornamos 0 para que no se agregue a los resultados.
    return 1;  // retornamos 1 para que se agregue
}

/*Funcion mueve una carta aleatoria de la carpeta origen a la acrpeta destino*/
void Mover_Carta_random(char carpeta_origen[],char carpeta_destino[]){
    /* ver todos los archivos de la carpeta origen*/
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir (carpeta_origen, &resultados, (*filtro), NULL); // se guardan en el arreglo resultados

    if (numeroResultados != 0){
        int aleatorio = (rand()%numeroResultados); //+2; // genero un random entre 2 y (numeroResultados -1): posiciones del arreglo que son utiles

    char nombre_archivo[40];
    sprintf(nombre_archivo,"%s",resultados[aleatorio]->d_name); // se guarda el nombre del archivo buscado aleatoriamente
    

    int i;
    for (i=0; i<numeroResultados; i++){  //se libera la memoria usada en el arreglo resultados
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;

    Crear_Archivo(nombre_archivo,carpeta_destino); // se crea el archivo en la carpeta de destino
    char borrar[40];
    sprintf(borrar,"%s/%s",carpeta_origen,nombre_archivo);    // se borra el archivo de la carpeta de origen
    remove(borrar); 
    }
}

/*Funcion mueve una carta especifica entregada como parametro desde una carpeta de origen a una de destino*/
void Mover_Carta_especifica(char carpeta_origen[],char carpeta_destino[], char carta[]){
    Crear_Archivo(carta,carpeta_destino); // se crea el archivo en la carpeta de destino
    char borrar[40];
    sprintf(borrar,"%s/%s",carpeta_origen,carta);    // se borra el archivo de la carpeta de origen
    remove(borrar); 
}

/* Función que compara una carta (string) recibida como parámetro con la última carta jugada 
(carta en carpeta ultimaCarta) y retorna un 1 si es válido poner la carta, o un 0 si no es válido
*/
int carta_valida(char carta[]){
     /* ver todos los archivos de la carpeta origen*/
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir ("ultimaCarta", &resultados, (*filtro), NULL); // se guardan en el arreglo resultados
    char ultima_carta[strlen(resultados[0]->d_name)];
    strcpy(ultima_carta,resultados[0]->d_name);

    //Se libera la memoria usada en el arreglo resultados
    int i;
    for (i=0; i<numeroResultados; i++){  
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;
    
    //Se comparan las cartas
    if (carta[2]!='n'){
        if (( carta[0] == ultima_carta[0]) || (carta[2] == ultima_carta[2])){
            return 1;
        }
        else{
            return 0;
        }
        
    }
    else{
        return 1;
    }
}

/*  Función que muestra por pantalla una carta (String) pasada por parámetro
    con la opción i pasada por parámetro*/

void ImprimirCarta(char nombre[], int i){
    char color[20];  // aquí se guardará el color de la carta
    char colorTerminal[20];  // aquí se guardará el codigo del color por terminal

    if (nombre[2]=='z'){  // la carta es azul
        strcpy(color,"azul\033[0m");
        strcpy(colorTerminal,"\033[34m");
    }
    else if (nombre[2]=='r'){ // la carta es roja
        strcpy(color,"rojo\033[0m");
        strcpy(colorTerminal,"\033[31m");
    }
    else if (nombre[2]=='a'){ // la carta es amarilla
        strcpy(color,"amarillo\033[0m");
        strcpy(colorTerminal,"\033[33m");
    }
    else if (nombre[2]=='v'){ // la carta es verde
        strcpy(color,"verde\033[0m");
        strcpy(colorTerminal,"\033[32m");
    }
    else{
        strcpy(color,""); // la carta es especial sin color
        strcpy(colorTerminal,"");

    }
    char tipo[20];   // aquí se guardara el nombre de la carta
    if (nombre[0]=='+'){  // +2
        strcpy(tipo,"+2");
    }
    else if (nombre[0]=='r'){  // reversa
        strcpy(tipo,"reversa");
    }
    else if (nombre[0]=='s'){  // salto
        strcpy(tipo,"salto");
    }
    else if (nombre[0]=='c'){  // colores 
        strcpy(tipo,"colores");
    }
    else if (nombre[0]=='x' && nombre[2]=='n'){  // +4
        strcpy(tipo,"+4");
    }
    else{
        sprintf(tipo,"%c",nombre[0]);  // cartas numericas
    }
    printf(" (%d)%s %s %s\n",i,colorTerminal,tipo,color);  // se muestra el resultado: (opcion) tipo color 
}

/* Función que borra la carta de la carpeta ultimaCarta, dejándola vacía. 
*/
void borrar_ultimaCarta(){
    /* ver todos los archivos de la carpeta origen*/
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir ("ultimaCarta", &resultados, (*filtro), NULL); // se guardan en el arreglo resultados
    char nombre_archivo[40];
    sprintf(nombre_archivo,"%s",resultados[0]->d_name); // se guarda el nombre del archivo buscado aleatoriamente
    
    int i;
    for (i=0; i<numeroResultados; i++){  //se libera la memoria usada en el arreglo resultados
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;

    char borrar[40];
    sprintf(borrar,"%s/%s","ultimaCarta",nombre_archivo);    // se borra el archivo de la carpeta de origen
    remove(borrar); 
}

/*  Función que recibe el nombre de una carta y  retorna el tipo de carta que es.
    retornos según tipo:
    [0-9] -> '1'
    reversa -> '2'
    salto -> '3'
    +2 -> '4'
    colores -> '5'
    +4 -> '6'
*/
char Jugar_carta(char carta[],char carpeta_origen[]){
    borrar_ultimaCarta();
    int eleccion;
    char comando[50];
    if (carta[0]=='x' || carta[0] == 'c'){
        printf("Elegir color:\n (1) \033[34mAzul\033[0m \n (2) \033[31mRojo\033[0m \n (3) \033[33mAmarillo\033[0m \n (4) \033[32mVerde\033[0m\n Ingrese opción: ");
        scanf("%d",&eleccion);
        sprintf(comando,"rm %s/%s",carpeta_origen,carta);
        system(comando);
        if (eleccion == 1){
            sprintf(comando,"%c_z_%c.txt",carta[0],carta[4]);
        }
        else if (eleccion == 2){
            sprintf(comando,"%c_r_%c.txt",carta[0],carta[4]);
        }
        else if (eleccion == 3){
            sprintf(comando,"%c_a_%c.txt",carta[0],carta[4]);
        }
        else{
            sprintf(comando,"%c_v_%c.txt",carta[0],carta[4]);
        }
        Crear_Archivo(comando,"ultimaCarta");
        if (carta[0] == 'c')return '1';
        return '5';
    } // retorno según el tipo
    Mover_Carta_especifica(carpeta_origen,"ultimaCarta",carta);
    if (carta[0]=='+') return '4';
    if (carta[0] == 's')return '3';
    if (carta[0] == 'r')return '2';
    return '1';
}

/*  Función que mustra las cartas en la carpeta (String) pasada por parámetro como opciones, 
    se incluye la opción de sacar una carta del mazo.
    opciones == 1 -> inicio del turno, se muestra la opción de scar una carta del mazo.
    opciones == 2 -> se ha sacado una carta del mazo, se da la opción de terminar el turno sin jugar una carta.
    se retorna la posición en el arreglo resultados de la carta elegida.
    se retorna -1 si es que el jugador pasa su turno después de sacar una carta del mazo.
*/
char MostrarCartas(char carpeta[], int opciones){
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir (carpeta, &resultados, (*filtro), NULL);  // se extraen los archivos de la carpeta 

    /* Monstrar opciones de Juego*/
    int i;
    printf("Elige una opción:\n\nCartas en mano:\n\n");  
    for (i = 0; i < numeroResultados; i++){  // se muestran por pantalla las cartas en la carpeta del jugador
        ImprimirCarta(resultados[i]->d_name,(i+1));
    }
    if (opciones == 1){  // se da la opcion de sacar una carta al azar del mazo
        printf("\n (%d) Sacar carta\n\n",(i+1));
    }
    else{  // se da la opción de terminar el turno
        printf("\n (%d) Terminar turno\n\n",(i+1));
    }
    int posicion = i;

    printf("Ingrese Opcion: ");
    int opcion;
    scanf("%d",&opcion);

    /* Manejo de Opción seleccionada*/
    while ((opcion > (posicion+1) || opcion < 1) || (opcion <= posicion && carta_valida(resultados[opcion-1]->d_name)==0)){  // se verifica que la opción sea válida
        if ((opcion > (posicion+1) || opcion < 1)){
            printf("Ingrese Opcion válida: ");
        }
        else{
            printf("Ingrese carta válida: ");
        }
        scanf("%d",&opcion);
    }

    // se elige la opción de sacar una carta  al azar del mazo
    if (opcion == (posicion+1) && opciones == 1){
        for (i=0; i<numeroResultados; i++){   // libero la memoria
            free (resultados[i]);
            resultados[i] = NULL;
        }
        free(resultados); 
        resultados = NULL;

        Mover_Carta_random("mazo",carpeta); // saco la carta
        return MostrarCartas(carpeta,2); // llamo de nuevo a la funcion pero ahora con la opcion de terminar el turno
    }
    /* la opción es una carta válida o se terminó el turno*/
    char tipo;
    if (opcion <= numeroResultados){
        tipo = Jugar_carta(resultados[opcion-1]->d_name,carpeta);
    }

    for (i=0; i<numeroResultados; i++){  // libero la memoria
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados); 
    resultados = NULL;

    if (opcion <= numeroResultados){
        return tipo;
    }

    return '1'; // si se escoge terminar el turno retorno un 1
}



/* Función que revisa si a algún jugador le queda una carta, o si ganó el juego. 
Retorna 1 si le queda 1 carta, 2 si ganó el juego o 0 en caso contrario. 
*/
int revisar_ultima_carta(char carpeta[]){
     /* ver todos los archivos de la carpeta origen*/
    struct dirent **resultados = NULL;
    int numeroResultados;
    char nombre[40], color[20], colorTerminal[20], tipo[20];
    numeroResultados = scandir (carpeta, &resultados, (*filtro), NULL); // se guardan en el arreglo resultados
    if ((strcmp(carpeta,"ultimaCarta") == 0 ) && numeroResultados>0){
        sprintf(nombre,"%s",resultados[0]->d_name);
        if (nombre[2]=='z'){  // la carta es azul
            strcpy(color,"azul\033[0m");
            strcpy(colorTerminal,"\033[34m");
        }
        else if (nombre[2]=='r'){ // la carta es roja
            strcpy(color,"rojo\033[0m");
            strcpy(colorTerminal,"\033[31m");
        }
        else if (nombre[2]=='a'){ // la carta es amarilla
            strcpy(color,"amarillo\033[0m");
            strcpy(colorTerminal,"\033[33m");
        }
        else if (nombre[2]=='v'){ // la carta es verde
            strcpy(color,"verde\033[0m");
            strcpy(colorTerminal,"\033[32m");
        }
        else{
            strcpy(color,""); // la carta es especial sin color
            strcpy(colorTerminal,"");

        }   
        if (nombre[0]=='+'){  // +2
            strcpy(tipo,"+2");
        }
        else if (nombre[0]=='r'){  // reversa
            strcpy(tipo,"reversa");
        }
        else if (nombre[0]=='s'){  // salto
            strcpy(tipo,"salto");
        }
        else if (nombre[0]=='c'){  // colores 
            strcpy(tipo,"colores");
        }
        else if (nombre[0]=='x'){  // +4
            strcpy(tipo,"+4");
        }
        else{
            sprintf(tipo,"%c",nombre[0]);  // cartas numericas
        }
        printf("\n\n\nLa última carta jugada es: %s %s %s\n",colorTerminal,tipo,color);
    }
    int i;
    for (i=0; i<numeroResultados; i++){  //se libera la memoria usada en el arreglo resultados
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;

    if (numeroResultados == 0) return 2;
    if (numeroResultados == 1) return 1;
    return 0;
}

/*Función que mueve una carta inicial random a la carpeta ultimaCarta para comenzar el juego. Verifica
que la carta movida no sea una carta espacial (+2,+4,salto)*/
void Mover_Carta_inicio(){
    /* ver todos los archivos de la carpeta origen*/
    struct dirent **resultados = NULL;
    int numeroResultados, aleatorio;
    char nombre_archivo[40] = "s_r_1.txt";
    numeroResultados = scandir ("mazo", &resultados, (*filtro), NULL); // se guardan en el arreglo resultados
    while (nombre_archivo[0]=='+' || nombre_archivo[0]=='x' || nombre_archivo[0]=='s' || nombre_archivo[0]=='c'){
        aleatorio = (rand()%numeroResultados); //+2; // genero un random entre 2 y (numeroResultados -1): posiciones del arreglo que son utiles
        sprintf(nombre_archivo,"%s",resultados[aleatorio]->d_name); // se guarda el nombre del archivo buscado aleatoriamente
    }
    int i;
    for (i=0; i<numeroResultados; i++){  //se libera la memoria usada en el arreglo resultados
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;

    Crear_Archivo(nombre_archivo,"ultimaCarta"); // se crea el archivo en la carpeta de destino
    char borrar[40];
    sprintf(borrar,"%s/%s","mazo",nombre_archivo);    // se borra el archivo de la carpeta de origen
    remove(borrar); 
    
}



int main(){
    srand(time(NULL));

    /* Crear Carpetas vacías para inicar el juego*/
    Crear_Carpeta("mazo");
    Crear_Carpeta("jugador1");
    Crear_Carpeta("jugador2");
    Crear_Carpeta("jugador3");
    Crear_Carpeta("jugador4");
    Crear_Carpeta("ultimaCarta");

    /* Crear las cartas en el mazo*/
    Generar_Cartas("z");
    Generar_Cartas("r");
    Generar_Cartas("v");
    Generar_Cartas("a");
    Generar_CartasEspeciales();

    /* Repartir las cartas iniciales*/
    int i;
    for (i = 0; i < 7; i++){
        Mover_Carta_random("mazo","jugador1");
        Mover_Carta_random("mazo","jugador2");
        Mover_Carta_random("mazo","jugador3");
        Mover_Carta_random("mazo","jugador4");
    }

    /*Carta inicial*/
    Mover_Carta_inicio();

    //----------------------------------------------------------------------------------------------------//
    /* CREACION DE JUGADORES Y COMUNICACION ENTRE ELLOS */
    
    int pipe12[2];  // pipes entre jugador 1 y jugador 2
    int pipe21[2];

    int pipe13[2];  // pipes entre jugador 1 y jugador 3
    int pipe31[2];

    int pipe14[2];  // pipes entre jugador 1 y jugador 4
    int pipe41[2];

    int jugador1 = getpid();
    int jugador2 = -1;
    int jugador3 = -1;
    int jugador4 = -1;
    int pid;

    pipe(pipe12);  // creo pipe jugador 1 a jugador 2
    pipe(pipe21);  // creo pipe jugador 2 a jugador 1
    pipe(pipe13);  // creo pipe jugador 1 a jugador 3
    pipe(pipe31);  // creo pipe jugador 3 a jugador 1
    pipe(pipe14);  // creo pipe jugador 1 a jugador 4
    pipe(pipe41);  // creo pipe jugador 4 a jugador 1

    jugador2 = fork();  // 2 procesos
    if (jugador2 >0){    //Proceso Padre
        
        jugador3 = fork();  // 3 procesos
        if (jugador3 >0){ // Proceso Padre
            jugador4=fork(); // 4 procesos
            if (jugador4 == 0){
                jugador4 = getpid();
            }
        }
        else if (jugador3 == 0){
            jugador3 = getpid();
        }
    }
    else if (jugador2 == 0){
        jugador2 = getpid();
    }
    
    pid = getpid();

    // Se separan los procesos en sus respectivos ciclos para comenzar la comunicación entre ellos:
    if (pid == jugador1){ // Proceso del jugador 1
        //Se cierran las conexiones read y write de los pipes del padre con los respectivos hijos:
        close(pipe12[0]); // cierro el modo de lectura de jugador 1 a jugador 2
        close(pipe21[1]); // cierro el modo de escritura de jugador 2 a jugador 1
        close(pipe13[0]); // cierro el modo de lectura de jugador 1 a jugador 3
        close(pipe31[1]); // cierro el modo de escritura de jugador 3 a jugador 1
        close(pipe14[0]); // cierro el modo de lectura de jugador 1 a jugador 4
        close(pipe41[1]); // cierro el modo de escritura de jugador 4 a jugador 1
        
        char mensaje = '1';
        int turno = 1;      //Variable que indica el turno que se esta jugando
        int incremento = 1; //Variable que determina si va en direccion normal (1) o reverse (3)
        int v1,v2,v3,v4;
        printf("\n\nINICIO DEL JUEGO\n");
        revisar_ultima_carta("ultimaCarta");
        while (1){ 
            if (fabs(turno%4) == 1){  // Turno jugador1
                printf("\nTURNO DE JUGADOR 1 \n");
                if (mensaje == '5'){
                    printf("Jugador 1 saca 4 cartas\n");
                    int i;
                    for ( i = 0; i < 4; i++){
                        Mover_Carta_random("mazo","jugador1");
                    }
                    mensaje = '1';
                }
                else if (mensaje == '4'){
                    printf("Jugador 1 saca 2 cartas\n");
                    int i;
                    for ( i = 0; i < 2; i++){
                        Mover_Carta_random("mazo","jugador1");
                    }
                    mensaje = '1';
                }
                else if (mensaje == '3'){
                    printf("Jugador 1 es saltado\n");
                    mensaje='1';
                }
                else{
                    mensaje = MostrarCartas("jugador1",1);
                }
            }
            else if (fabs(turno%4) == 2){  // Turno jugador2
                write(pipe12[1],&mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe21[0],&mensaje,1))<0){};   //Se espera hasta recibir mensaje de turno finalizado
                
            }
            else if (fabs(turno%4) == 3){  // Turno jugador3
              
                write(pipe13[1],&mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe31[0],&mensaje,1))<0){};   //Se espera hasta recibir mensaje de turno finalizado
            
            }
            else if (fabs(turno%4) == 0){  // turno juagdor4
               
                write(pipe14[1],&mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe41[0],&mensaje,1))<0){};   //Se espera hasta recibir mensaje de turno finalizado
              
            }

            // Manejo de opciones: // 5 es +4, 6 colores,4 +2, salto 3, reversa 2, 1
            if (mensaje == '2'){ // reversa
                if (incremento == 3) incremento = 1; //Sentido normal
                else{ incremento = 3; } //Sentido inverso
            }
            if (revisar_ultima_carta("mazo")==2){
                mensaje = '0'; //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],&mensaje,1);
                write(pipe13[1],&mensaje,1);
                write(pipe14[1],&mensaje,1);
                printf("\n\nNo quedan cartas en el mazo\n");
                printf("FIN DEL JUEGO \n");
                break;
            }
            revisar_ultima_carta("ultimaCarta");
            v1 = revisar_ultima_carta("jugador1");
            v2 = revisar_ultima_carta("jugador2");
            v3 = revisar_ultima_carta("jugador3");
            v4 = revisar_ultima_carta("jugador4");
            if (v1==2){
                mensaje='0'; //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],&mensaje,1);
                write(pipe13[1],&mensaje,1);
                write(pipe14[1],&mensaje,1);
                printf("\n\nEl ganador es el jugador 1 \n");
                printf("FIN DEL JUEGO \n");
                break;
            }
            if (v2==2){
                mensaje='0'; //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],&mensaje,1);
                write(pipe13[1],&mensaje,1);
                write(pipe14[1],&mensaje,1);
                printf("\n\nEl ganador es el jugador 2 \n");
                printf("FIN DEL JUEGO \n");
                break;
            }
            if (v3==2){
                mensaje='0'; //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],&mensaje,1);
                write(pipe13[1],&mensaje,1);
                write(pipe14[1],&mensaje,1);
                printf("\n\nEl ganador es el jugador 3 \n");
                printf("FIN DEL JUEGO \n");
                break;
            }
            if (v4==2){
                mensaje='0'; //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],&mensaje,1);
                write(pipe13[1],&mensaje,1);
                write(pipe14[1],&mensaje,1);
                printf("\n\nEl ganador es el jugador 4 \n");
                printf("FIN DEL JUEGO \n");
                break;
            }
            if (v1==1){
                printf("\nEl jugador 1 tiene sólo 1 carta \n");
            }
            if (v2==1){
                printf("\nEl jugador 2 tiene sólo 1 carta \n");
            }
            if (v3==1){
                printf("\nEl jugador 3 tiene sólo 1 carta \n");
            }
            if (v4==1){
                printf("\nEl jugador 4 tiene sólo 1 carta \n");
            }
            turno = turno + incremento; //Se mueve al siguiente turno
    
        }
        

    }
    if (pid == jugador2){ // Proceso del jugador 2
        //Se cierran todos los pipes de los otros jugadores:
        close(pipe13[1]); 
        close(pipe13[0]);
        close(pipe31[1]); 
        close(pipe31[0]);
        close(pipe14[1]); 
        close(pipe14[0]);
        close(pipe41[1]); 
        close(pipe41[0]);
        //Se cierran las conexiones read y write de los respectivos pipes con el padre (jugador 1):
        close(pipe12[1]);  // cierro el modo escritura de jugador 1 a jugador 2
        close(pipe21[0]);  // cierro el modo lectura de jugador 2 a jugador 1 

        char mensaje;
        while (1){ 
            while((read(pipe12[0],&mensaje,1))<0){};
        
            if (mensaje=='0'){ // el papá le manda un cero si el juego ha terminado
                break;
            }
            printf("\nTURNO DE JUGADOR 2 \n");
            if (mensaje == '5'){  // debe sacar 4 cartas y finalizar turno
                printf("Jugador 2 saca 4 cartas\n");
                int i;
                for ( i = 0; i < 4; i++){
                    Mover_Carta_random("mazo","jugador2");
                }
                mensaje = '1'; 
            }
            else if (mensaje == '4'){ // sacar dos cartas y finalizar
                printf("Jugador 2 saca 2 cartas\n");
                int i;
                for ( i = 0; i < 2; i++){
                    Mover_Carta_random("mazo","jugador2");
                }
                mensaje = '1';
            }
            else if (mensaje == '3'){
                printf("Jugador 2 es saltado\n");
                mensaje = '1'; 
            }
            else{
                mensaje = MostrarCartas("jugador2",1);
                
            }
            write(pipe21[1],&mensaje,1); 
        }
        
        
    }
    else if (pid == jugador3){
        //Se cierran todos los pipes de los otros jugadores:
        close(pipe12[1]); 
        close(pipe12[0]);
        close(pipe21[1]); 
        close(pipe21[0]);
        close(pipe14[1]); 
        close(pipe14[0]);
        close(pipe41[1]); 
        close(pipe41[0]);
        //Se cierran las conexiones read y write de los respectivos pipes con el padre (jugador 1):
        close(pipe13[1]);  // cierro el modo escritura de jugador 1 a jugador 3
        close(pipe31[0]);  // cierro el modo lectura de jugador 3 a jugador 1

        char mensaje;
        while (1){
            while((read(pipe13[0],&mensaje,1))<0){};
            if (mensaje == '0'){ // el papá le manda un cero si el juego ha terminado
                break;
            }
            printf("\nTURNO DE JUGADOR 3 \n");
            if (mensaje == '5'){
                printf("Jugador 3 saca 4 cartas\n");
                int i;
                for ( i = 0; i < 4; i++){
                    Mover_Carta_random("mazo","jugador3");
                }
                mensaje = '1';
            }
            else if (mensaje == '4'){
                printf("Jugador 3 saca 2 cartas\n");
                int i;
                for ( i = 0; i < 2; i++){
                    Mover_Carta_random("mazo","jugador3");
                }
                mensaje = '1';
            }
            else if (mensaje == '3'){
                printf("Jugador 3 es saltado\n");
                mensaje = '1';
            }
            else{
                mensaje = MostrarCartas("jugador3",1);
            } 
            write(pipe31[1],&mensaje,1); 
        }
        
    }
    else if (pid == jugador4){
        //Se cierran todos los pipes de los otros jugadores:
        close(pipe13[1]); 
        close(pipe13[0]);
        close(pipe31[1]); 
        close(pipe31[0]);
        close(pipe12[1]); 
        close(pipe12[0]);
        close(pipe21[1]); 
        close(pipe21[0]);
        //Se cierran las conexiones read y write de los respectivos pipes con el padre (jugador 1):
        close(pipe14[1]);  // cierro el modo escritura de jugador 1 a jugador 4
        close(pipe41[0]);  // cierro el modo lectura de jugador 4 a jugador 1 

        char mensaje;
        while (1){
            while((read(pipe14[0],&mensaje,1))<0){};
            if (mensaje == '0'){ // el papá le manda un cero si el juego ha terminado
                break;
            }
            printf("\nTURNO DE JUGADOR 4 \n");
            if (mensaje == '5'){
                printf("Jugador 4 saca 4 cartas\n");
                int i;
                for ( i = 0; i < 4; i++){
                    Mover_Carta_random("mazo","jugador4");
                }
                mensaje = '1';
            }
            else if (mensaje == '4'){
                printf("Jugador 4 saca 2 cartas\n");
                int i;
                for ( i = 0; i < 2; i++){
                    Mover_Carta_random("mazo","jugador4");
                }
                mensaje = '1';
            }
            else if (mensaje == '3'){
                printf("Jugador 4 es saltado\n");
                mensaje = '1';
            }
            else{
                mensaje = MostrarCartas("jugador4",1);
                
            } 
            write(pipe41[1],&mensaje,1); 
        }
    }
    
    return 0;
}
