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

void ImprimirCarta(char nombre[], int i){
    char color[20];
    if (nombre[2]=='z'){
        strcpy(color,"azul");
    }
    else if (nombre[2]=='r'){
        strcpy(color,"rojo");
    }
    else if (nombre[2]=='a'){
        strcpy(color,"amarillo");
    }
    else if (nombre[2]=='v'){
        strcpy(color,"verde");
    }
    else{
        strcpy(color,"");
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
    printf(" (%d) %s %s \n",(i+1),tipo,color);
}

int MostrarCartas(char carpeta[]){
    struct dirent **resultados = NULL;
    int numeroResultados;
    numeroResultados = scandir (carpeta, &resultados, (*filtro), NULL); 

    int i;
    printf(" Elige una Opcion:\n\nCartas en mano:\n\n");
    for (i = 0; i < numeroResultados; i++){
        ImprimirCarta(resultados[i]->d_name,i);
    }
    printf("\n(%d) Sacar carta",(i+1));

    int posicion = i;

    for (i=0; i<numeroResultados; i++){  
        free (resultados[i]);
        resultados[i] = NULL;
    }
    free(resultados); 
    resultados = NULL;

    printf("\nIngrese Opcion: ");
    int opcion;
    scanf("%d",&opcion);

    if (opcion <= posicion){
        printf("%d\n",opcion-1);
        return opcion-1;
    }
    else if (opcion == (posicion+1)){
        printf("%d\n",-1);
        return -1;
    }
    return -2;
}

int main(){
    MostrarCartas("jugador1");
    return 0;
}
