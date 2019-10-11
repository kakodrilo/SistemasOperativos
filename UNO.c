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
        sprintf(carta_especial,"4_n_%d.txt",i);
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
        
        char mensaje[2];
        int turno = 1;      //Variable que indica el turno que se esta jugando
        int incremento = 1; //Variable que determina si va en direccion normal (1) o reverse (3)
        int ganador = 0;    //Variable que guardara el jugador ganador
        while (1){
            if (fabs(turno%4) == 1){  // Turno jugador1
                printf("\nTURNO DE JUGADOR 1 \n");
                printf("OPCIONES DE JUGADA: \n   (1) Jugar carta normal \n   (2) Jugar reverse \n   (3) Jugar salto \n   (4) Gané el juego! \n INGRESE OPCION: ");
                scanf("%s",mensaje);
                if (strcmp(mensaje,"4")==0) ganador = 1; //Vemos si es el jugador ganador  
            }
            else if (fabs(turno%4) == 2){  // Turno jugador2
                strcpy(mensaje,"1");
                write(pipe12[1],mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe21[0],mensaje,1))<0);   //Se espera hasta recibir mensaje de turno finalizado
                if (strcmp(mensaje,"4")==0) ganador = 2;    //Vemos si es el jugador ganador
            }
            else if (fabs(turno%4) == 3){  // Turno jugador3
                strcpy(mensaje,"1");
                write(pipe13[1],mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe31[0],mensaje,1))<0);   //Se espera hasta recibir mensaje de turno finalizado
                if (strcmp(mensaje,"4")==0) ganador = 3;    //Vemos si es el jugador ganador
            }
            else if (fabs(turno%4) == 0){  // 
                strcpy(mensaje,"1");
                write(pipe14[1],mensaje,1); //Se le envia mensaje de iniciar turno
                while((read(pipe41[0],mensaje,1))<0);   //Se espera hasta recibir mensaje de turno finalizado
                if (strcmp(mensaje,"4")==0) ganador = 4;    //Vemos si es el jugador ganador
            }

            // Manejo de opciones:
            if (strcmp(mensaje,"2") == 0){
                if (incremento == 3) incremento = 1; //Sentido normal
                else{ incremento = 3; } //Sentido inverso
            }
            
            else if (strcmp(mensaje,"3")==0) turno = turno + incremento;
            else if (strcmp(mensaje,"4")==0){
                strcpy(mensaje,"0"); //Se le envia "0" al todos los hijos cuando algun jugador gano la partida. 
                write(pipe12[1],mensaje,1);
                write(pipe13[1],mensaje,1);
                write(pipe14[1],mensaje,1);
                printf("\n\nEl ganador es el jugador %d \n",ganador);
                printf("FIN DEL JUEGO \n");
                break;
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

        char mensaje[2];
        while (1){
            while((read(pipe12[0],mensaje,1))<0);
            if (strcmp(mensaje,"0")==0){
                break;
            }
            printf("\nTURNO DE JUGADOR 2 \n");
            printf("OPCIONES DE JUGADA: \n   (1) Jugar carta normal \n   (2) Jugar reverse \n   (3) Jugar salto \n   (4) Gané el juego! \n INGRESE OPCION: ");
            scanf("%s",mensaje);
            write(pipe21[1],mensaje,1); 
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

        char mensaje[2];
        while (1){
            while((read(pipe13[0],mensaje,1))<0);
            if (strcmp(mensaje,"0")==0){
                break;
            }
            printf("\nTURNO DE JUGADOR 3 \n");
            printf("OPCIONES DE JUGADA: \n   (1) Jugar carta normal \n   (2) Jugar reverse \n   (3) Jugar salto \n   (4) Gané el juego! \n INGRESE OPCION: ");
            scanf("%s",mensaje);
            write(pipe31[1],mensaje,1); 
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

        char mensaje[2];
        while (1){
            while((read(pipe14[0],mensaje,1))<0);
            if (strcmp(mensaje,"0")==0){
                break;
            }
            printf("\nTURNO DE JUGADOR 4 \n");
            printf("OPCIONES DE JUGADA: \n   (1) Jugar carta normal \n   (2) Jugar reverse \n   (3) Jugar salto \n   (4) Gané el juego! \n INGRESE OPCION: ");
            scanf("%s",mensaje);
            write(pipe41[1],mensaje,1); 
        }
    }
    
    return 0;
}
