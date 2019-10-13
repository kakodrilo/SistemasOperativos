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

void ImprimirCarta(char nombre[], int i){
    char color[20];
    char colorTerminal[20];
    if (nombre[2]=='z'){
        strcpy(color,"azul\033[0m");
        strcpy(colorTerminal,"\033[34m");
    }
    else if (nombre[2]=='r'){
        strcpy(color,"rojo\033[0m");
        strcpy(colorTerminal,"\033[31m");
    }
    else if (nombre[2]=='a'){
        strcpy(color,"amarillo\033[0m");
        strcpy(colorTerminal,"\033[33m");
    }
    else if (nombre[2]=='v'){
        strcpy(color,"verde\033[0m");
        strcpy(colorTerminal,"\033[32m");
    }
    else{
        strcpy(color,"");
        strcpy(colorTerminal,"");

    }
    char tipo[20];
    if (nombre[0]=='+'){
        strcpy(tipo,"+2");
    }
    else if (nombre[0]=='r'){
        strcpy(tipo,"reversa");
    }
    else if (nombre[0]=='s'){
        strcpy(tipo,"salto");
    }
    else if (nombre[0]=='c'){
        strcpy(tipo,"colores");
    }
    else if (nombre[0]=='4' && nombre[2]=='n'){
        strcpy(tipo,"+4");
    }
    else{
        sprintf(tipo,"%c",nombre[0]);
    }
    printf(" (%d)%s %s %s\n",(i+1),colorTerminal,tipo,color);
}

int MostrarCartas(char carpeta[], int opciones){
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir (carpeta, &resultados, (*filtro), NULL); 

    int i;
    printf(" Elige una Opcion:\n\nCartas en mano:\n\n");
    for (i = 0; i < numeroResultados; i++){
        ImprimirCarta(resultados[i]->d_name,i);
    }
    if (opciones == 1){
        printf("\n(%d) Sacar carta\n",(i+1));
    }
    else{
        printf("\n(%d) Terminar turno\n",(i+1));
    }
    int posicion = i;


    printf("Ingrese Opcion: ");
    int opcion;
    scanf("%d",&opcion);

    while (opcion > (posicion+1) || opcion < 1){ 
        printf("Ingrese Opcion válida: ");
        scanf("%d",&opcion);
    }


    while (opcion <= posicion && carta_valida(resultados[opcion-1]->d_name)==0){
        printf("Ingrese carta válida: ");
        scanf("%d",&opcion);
    }

    if (opcion == (posicion+1) && opciones == 1){
        for (i=0; i<numeroResultados; i++){  
            free (resultados[i]);
            resultados[i] = NULL;
        }
        free(resultados); 
        resultados = NULL;

        Mover_Carta_random("mazo",carpeta);
        return MostrarCartas(carpeta,2);
    }

    for (i=0; i<numeroResultados; i++){  
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados); 
    resultados = NULL;

    if (opcion <= posicion){
        return opcion-1;
    }

    return -1;
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
char Tipo_Carta(char carta[]){
    if (carta[2] == 'n' && carta[0]=='4') return '6'; // retorno según el tipo
    if (carta[0] == 'c') return '5';
    if (carta[0]=='+') return '4';
    if (carta[0] == 's') return '3';
    if (carta[0] == 'r') return '2';
    return '1';
}

int main(){

}
