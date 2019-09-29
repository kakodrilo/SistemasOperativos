#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include<time.h>

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
    sprintf(carta,"0_%s.txt",color);
    Crear_Archivo(carta,"mazo");  // se crea la carta 0 (1 de cada color)
    int i;
    for (i = 1; i <= 9; i++){
        sprintf(carta,"%d_%s_1.txt",i,color);    // se generan los numeros (2 de cada color)
        Crear_Archivo(carta,"mazo");
        sprintf(carta,"%d_%s_2.txt",i,color);
        Crear_Archivo(carta,"mazo");

        if (i < 3){      // se generan las cartas de reversa, +2 y salto (2 de cada color)
            sprintf(carta,"+2_%s_%d.txt",color,i);
            Crear_Archivo(carta,"mazo");

            sprintf(carta,"reversa_%s_%d.txt",color,i);
            Crear_Archivo(carta,"mazo");

            sprintf(carta,"salto_%s_%d.txt",color,i);
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
        sprintf(carta_especial,"+4_%d.txt",i);
        Crear_Archivo(carta_especial,"mazo");

        sprintf(carta_especial,"colores_%d.txt",i);
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
    Generar_Cartas("azul");
    Generar_Cartas("rojo");
    Generar_Cartas("verde");
    Generar_Cartas("amarillo");
    Generar_CartasEspeciales();

    /* Repartir las cartas iniciales*/
    int i;
    for (i = 0; i < 7; i++){
        Mover_Carta_random("mazo","jugador1");
        Mover_Carta_random("mazo","jugador2");
        Mover_Carta_random("mazo","jugador3");
        Mover_Carta_random("mazo","jugador4");
    }

    /*Mover_Carta_especifica("mazo","jugador1","2_rojo_1.txt");*/
    
    return 0;
}
