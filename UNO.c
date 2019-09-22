#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>


/* Función que revisa si existe una carpeta con el nombre pasado por parámetro
    retorna 0 si no existe
    retorna 1 si existe
    se utiliza la función 'opendir' de la libreria 'dirent', la que rettorna un puntero a la carpeta
    si esta existe.
*/
int D_EXISTE(char nombre[]){
    DIR * carpeta;
    carpeta = opendir(nombre);
    if (carpeta == NULL){
        free((void *)carpeta);
        return 0;
    }
    else{
        free((void *)carpeta);
        return 1;
    }
}

int main(){

    int i = D_EXISTE("mazo");
    if (i == 1){
        system("rm -r mazo");  // si la carpeta existe se borra
    }
    system("mkdir mazo"); // se crea la carpeta mazo totalmente vacía
    return 0;
}
