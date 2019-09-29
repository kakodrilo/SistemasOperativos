TAREA 1 SISTEMAS OPERATIVOS: Juego UNO

* Integrantes:
- Joaquín Castillo Tapia            rol 201773520-1
- María Paz Morales Llopis          rol 201773505-8

* Ejecución:
- Abrir terminar en la carpeta donde esté el makeile y el archivo UNO.c
- Para compilar y ejecutar el programa se debe ejecutar el siguiente comando en terminal:
                                    $ make
  -> Con ese comando se generan 6 carpetas y dos archivos (.o y ejecutable).
  -> En la carpeta mazo se crean 108 cartas, y se reparten 28 de ellas a los 4 jugadores (7 a cada uno), por lo que la carpeta queda con 80 cartas. 
  -> En las carpetas de cada jugador hay 7 cartas aleatorias. 
  -> En la carpeta ultimaCarta no hay ningún arhivo, ya que no se utiliza por ahora. 
- Para borrar las carpetas y archivos creados, junto con el ejecutable y archivo .o, se debe ejcutar en la terminal:
                                    $make clear

* Consideraciones:
- La función Mover_Carta_especifica asume que la carta a mover está en la carpeta de orgien. 

 