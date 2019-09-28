#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

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
    char comando[30] = "rm -r ";  // defino en string el comando para borrar la carpeta
    if (i == 1){  // la carpeta ya existe
        strcat(comando,nombre);   // le agrego al comando el nombre de la carpeta
        system(comando);  // ejecuto el comando para borrar
    }
    mkdir(nombre,777); // se crea la carpeta totalmente vacía
    
}


/* Funcion que crea un archivo .txt según el nombre entrgado en el nombre de la carpeta entregada
    se asume que la carpeta existe.
*/
void Crear_Archivo(char nombre_archivo[], char carpeta[]){
    char direccion[30];  // string donde se guarda el string: carpeta/nombre_archivo.txt
    sprintf(direccion,"%s/%s",carpeta,nombre_archivo);
    FILE * archivo = fopen(direccion,"w"); // se crea el archivo vacío
    if (archivo == NULL)
    {
        printf("Error al generar archivo %s",nombre_archivo);
    }
    fclose(archivo); // se cierra el archivo creado para liberar la memoria
}


/* Función que genera todas las cartas de un color especificado
*/
void Generar_Cartas(char color[]){
    char carta[30];
    sprintf(carta,"0_%s.txt",color);
    Crear_Archivo(carta,"mazo");  // se crea la carta 0 (1 de cada color)
    int i;
    for (i = 1; i <= 9; i++)
    {
        sprintf(carta,"%d_%s_1.txt",i,color);    // se generan los numeros (2 de cada color)
        Crear_Archivo(carta,"mazo");
        sprintf(carta,"%d_%s_2.txt",i,color);
        Crear_Archivo(carta,"mazo");

        if (i < 3)      // se generan las cartas de reversa, +2 y salto (2 de cada color)
        {
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
    char carta_especial[30];
    for (i = 1; i <= 4; i++)          // se generan las cartas especiales de cambio de color y +4 (4 de cada una)
    {
        sprintf(carta_especial,"+4_%d.txt",i);
        Crear_Archivo(carta_especial,"mazo");

        sprintf(carta_especial,"colores_%d.txt",i);
        Crear_Archivo(carta_especial,"mazo");
    }
}

int main(){
    Crear_Carpeta("mazo");
    Crear_Carpeta("jugador1");
    Crear_Carpeta("jugador2");
    Crear_Carpeta("jugador3");
    Crear_Carpeta("jugador4");
    Generar_Cartas("azul");
    Generar_Cartas("rojo");
    Generar_Cartas("verde");
    Generar_Cartas("amarillo");
    Generar_CartasEspeciales();

    return 0;
}
