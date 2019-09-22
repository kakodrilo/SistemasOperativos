#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>



/*
Funcion de filtro, recibe un struct puntero a carpeta, contiene todos los datos
de ella, como nombre, propietario, permisos, etc.
*/
int filtro(const struct dirent * carpeta){
    if (strcmp(carpeta->d_name,"mazo") == 0) return 1; // retornamos 1 para que se agregue a los resultados.
    return 0;  // retornamos 0 para que no se agregue
}



int main(){

    /* Comienzo con crear la carpeta 'mazo', pero primero quiero saber si existe.
    -* uso la función 'scandir' de la libreria 'dirent.h' que revisa todos los 
    -* archivos o carpetas, los guarda en un arreglo de punteros en base a una función
    -* de filtro (la funcion retorna 0 si no se desea agregar el elemento a los resultados) y una función de orden:
    -* scandir(DondeBuscar,ArregloResultados,FuncionFiltro,FuncionOrdenamiento)
    -* retorna un int con la cantidad de resultados guardados, -1 si hay error.
    -* además la funcion pide memoria por si misma para guardar los punteros a los resultados guardados*/
    
    
    struct dirent **resultados = NULL; // se crea el arreglo de punteros donde guardar los resultados
    int numeroResultados; 
    numeroResultados = scandir(".",&resultados,(*filtro),NULL); // se llama a la funcion con el filtro y sin ordenamiento

    /* si el numeroResultados es 1 quiere decir que la carpeta ya existe*/
    if (numeroResultados == 1){ 
        free((void *)resultados[0]); // se libera la memoria pedida donde se guarda el puntero a la carpeta
        system("rm -r mazo")  // se borra la carpeta ya existente
    }
    system("mkdir mazo") // creo la carpeta, ahora está vacía.

    free((void *)resultados); // se libera la memoria donde estaban los resultados
    resultados = NULL;

    
    return 0;
}
