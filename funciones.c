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

int filtro(const struct dirent * carpeta){
    if ((strcmp(carpeta->d_name,".") == 0 ) || (strcmp(carpeta->d_name,"..") == 0 ) ) return 0; 
    // retornamos 0 para que no se agregue a los resultados.
    return 1;  // retornamos 1 para que se agregue
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
    int i;
    for (i=0; i<numeroResultados; i++){  //se libera la memoria usada en el arreglo resultados
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados);  // se libera el puntero al arreglo
    resultados = NULL;
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
    printf("%d \n",carta_valida("6_v_3.txt"));
    return 0;
}
