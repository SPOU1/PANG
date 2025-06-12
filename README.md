# ADICIONES Y CAMBIOS

### *Ball*
1. Se ha editado la generación de posición inicial del constructor para evitar que las bolas se atasquen en los bordes de la pantalla.
2. Se ha añadido el método `updateColor()` que actualiza el color de las bolas en función de su tamaño.
3. Se ha editado el método `split()`. Se edita la velocidad tras la colisión, para que no caigan encima del jugador. También se actualiza el color de la pelota original, pues antes no cambiaba.
4. Se ha implementado el operador `+` que devuelve la distancia entre el objeto `Ball` y el otro objeto de clase `Shape`.
 
### *Bullet*
1. Se ha cambiado el color de la bala a azul, para mejor contraste con las bolas y animales.

### *Objects List*
1. Se ha creado la clase `ObjectsList` que gestiona todos los objetos presentes en el juego.
2. Se ha creado el struct `nodo` que almacena el puntero de los objetos y el siguiente nodo de la lista.
3. Se han añadido los siguientes atributos privados:
     * `n`: número de elementos en la lista.
     * `*head`: puntero al primer objeto de la lista.
     * `*theMan`: puntero al objeto `Man`.

4. Se han añadido los siguientes métodos públicos:
     * `ObjectList()`: Constructor. Inizializa los atributos. Crea el jugador y lo añade a la lista. Randomiza la semilla.  
     * `~ObjectList()`: Elimina cada objeto de la lista
     * `move()`: Llama al método `move()` de cada objeto de la lista.
     * `draw()`: Llama al método `draw()` de cada objeto de la lista.
     * `*getMan()`: Devuelve el objeto `Man`.
     * `add()`: Añade un nuevo objeto al comienzo de la lista.
     * `remove()`: Elimina un objeto de la lista.
     * `reposition()`: Cuando se resetea la posición del jugador, todas las bolas colindantes se reposicionan para no colisionar con el jugador.
     * `collisions()`: Gestiona las distintas colisiones en el juego. Devuelve un valor en funcíon del tipo de colisión.
     * `removeAllExceptMan()`: Elimina todos los objetos excepto el jugador. Se emplea para resetear el juego.
     * `createBalls()`: Crea las bolas al comienzo de cada juego. Al principio estaba dentro del constructor de la lista, sin embargo, se debe crear nuevas bolas cada vez que se resetea el juego.

### *Animal*
1. Se ha implementado la clase `Animal`.
2. Se han implementado los siguientes métodos:
     * `Animal()`: Constructor. Se genera en la parte superior de la pantalla, en una abcisa aleatoria, y con velocidad negativa en el eje Y. 
     * `draw()`: Se dibuja el animal, mediante la composición de varias figuras geométricas.
     * `getSize()`: Devuelve el tamaño del animal.
     * `operator+()`: Igual que con la bola. Devuelve la distancia entre el animal y otro objeto.

### *Audio Player*
1. Se ha implementado la clase `AudioPlayer`, que gestiona la reproducción de música y efectos de sonido.
2. Se ha implementado el atributo `musicPID`, que se inicializa a `0`.
3. Se han implementado los siguientes métodos:
     * `playSound()`: Se crea un proceso hijo que reporduce el efecto de sonido hasta que termine. 
     * `playMusic()`: Se crea un proceso hijo que reproduce la música de fondo en bucle. 
     * `stopMusic()`: Detiene el proceso que reporduce la música de fondo del juego.

### *Man*
1. Se ha añadido el efecto de sonido al disparo (método `fire()`).

### *Shape*
1. Se ha modificado el método `move()`, para eliminar la aleatoriedad del rebote, ya que provocaba que las bolas se quedasen pegadas al suelo.

### *Commom Stuff*
1. Se han actualizado los siguientes valores:
   ```
   BALL_SPEED    0.05
   BULLETSPEED   0.5
   BOUNCE_SPEED  0.05
   ```
2. Se han añadido los siguientes valores:
   ```
   MENU_STATE     0
   GAME_STATE     1
   GAMEOVER_STATE 2
   ```

### *GL Stuff*
>[!NOTE]
>Se puede mover algúna función de `main` aquí.

### *Main PANG*
1. Se han añadido los headers de las nuevas clases.
2. Se ha cambiado la implementación de las funciones de teclado. En lugar de mover el personaje solo cuando la tecla está presionada, se comienza a mover cuando se pulsa la tecla, y se para de mover cuando se levanta. Esto evita que al presionar la tecla, el sistema operativo tarde en detectar que la tecla se mantiene presionada.
3. Se ha implementado un gestor de High Score. Para ello, se han añadido las funciones `setScore()`, que guarda la puntuación de cada partida en un archivo .txt, y `getHighScore()`, que obtiene la mayor puntuación del archivo.
4. Se han añadido un menú de inicio y un menú de Game Over. Para ello se gestiona el estado del juego mediante la variable `gameState`.
5. Se ha creado la función `exit_game()` para gestionar la salida del juego, especialmente para denener la música antes de cerrar el juego.
6. En `main()`, se añaden las inicializaciones de la función de salida y las nuevas funciones de teclado; se obtiene el `highScore` y se inicializa el reproductor de música.
7. Se ha editado la función `printdata()` para incluir el high score.
8. Se han añadido las funciones `drawBox()`, `printText()`, `printMenu()`, y `printGameOver()`, que renderizan los menús de inicio y Game Over.
9. Cambios en la función `myLogic()`:
    * Solo se ejecuta si el estado es `GAME_STATE` (si no está en el menú de inicio o de de Game Over).
    * Se ha modificado la gestión de la bala. Ahora, la bala se destruye cuando sale de la pantalla, en lugar de depender de si el tiempo `shotTime` es mayor a `MAXSHOTTIME`.
    * Se ha añadido la generación de aninmales. Se genera con un 1% de probabilidad en la parte superior de la pantalla, y se elimina cuando llega a la parte inferior.
    * Se ha completado la gestión de colisiones:
      * Colisión Bola-Jugador: si `nMan == 0`, se guarda el score, acualiza `highScore` si es necesario y se actualiza `gameState`. Si no, se resetea la posición del jugador, y de las bolas colindantes.
      * Colisión Bola-Proyectil: se elimina el proyectil, y se actualiza `score`, en función del tamaño de la bola.
      * Colisión Animal-Proyectil: se eliminan el proyectil y el jugador, y se suman 1000 puntos a `score`.
    * Se han añadido los efectos de sonido a cada colisión. Si se `nMan == 0`, también se detiene la música.
  
10. Se ha añadido en la función `OnDibuja()` para gestionar los elementos a imprimir en función del valor de `gameState`.
11. Se ha implementado la función `resetGame()`, que resetea las variables al comienzo de cada juego, e elimina los objetos presentes en ese momento, resetea al jugador, y crea las bolas iniciales. También reinicia la música.

### *Makefile*
### *Makefile*
1. Se han añadido los nuevos archivos a la instrucción de compilación en Linux.
