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
                                    $ make clear

* Actualizaciones Entrega 2:
- Se generaron los 4 procesos, uno por cada jugador (proceso padre es el jugador 1, y los otros tres jugadores son sus hijos).
- Se generaron 6 pipes, dos por cada proceso hijo para comunicarse con el padre (uno enviar mensajes y otro para recibir).
  Para mayor entendimiento de las pipes, cada pipe se llama de la siguiente manera: pipeij, donde i es el numero del jugador que envia
  el mensaje, y j es el numero del jugador que recibe el mensaje.
- Se creo una pequeña interfaz que permite probar la comunicación entre los procesos a través de los distintos tipos de jugadas.
  Las jugadas disponibles son:
   (1) Jugar carta normal: el jugador juega una carta que no es ni salto ni reverse.
   (2) Jugar reverse: se juega un reverse.
   (3) Jugar salto: se juega una carta de salto.
   (4) Gané el juego!: simulación de que el jugador gana el juego.
  Para jugar se debe ingresar explicitamente el número de la opción.

* Consideraciones:
- En esta entrega no se muestran las cartas disponibles de cada jugador.
- En este micro juego sólo se simula una partida, y el propio jugador decide si gana.