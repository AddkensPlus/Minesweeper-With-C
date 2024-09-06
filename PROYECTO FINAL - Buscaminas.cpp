/* Proyecto: BUSCAMINAS
   Estudiante: ANDRES FABIAN MARRUGO RUGELES
   Codigo: 0222111002
   Carrera: INGENIERIA DE SISTEMAS*/

#include <iostream>
#include <stdio.h>
#include <assert.h>  // assert
#include <stdlib.h>  // rand
#include <unistd.h>  // getpid

// Datos permanentes del juego

#define espacio_oculto 'ª' // Casillas que se muestra cuando no se ha seleccionado
#define espacio_descubierto ' ' // Para generar un espacio vacio
#define mina 'X' // Mina
#define filas_def 10 // Define la cantidad de filas de la tabla
#define columnas_def 10 // Define la cantidad de columnas de la tabla
#define mostrar 0 // 1= mostrar minas, 0= ocultar minas
#define n_minas 10 // Cantidad de minas en la tabla
#define mina_encontrada 1 // Cuando un usuario dijita unas coordenadas donde hay una mina que ya fue encontrada
#define espacio_ya_descubierto 2 // Cuando un usuario digita unas coordenadas donde ya habia un espacio descubierto
#define null 3 // Sin errores despues de comprobacion

using namespace std;

int fila, columna;

// Menu
void menu();
void instrucciones();
void juego();

// Minas cercanas
int calcular_minas_cercanas();

// Numero aleatorio
int aleatorio_rango();
void colocar_mina();
void minas_aleatorias();

// Creando tabla
void iniciar_tablero();
void separar_encabezado();
void separar_filas();
void imprimir_encabezado();
char int_char();
void imprimir_tablero();

// Comprobaciones
int abrirCasilla();
int todos_cuadros_abiertos();
	
/* ------------------------------------------------------------[ MINAS CERCANAS ]------------------------------------------------------------------------------------- */

// Calculo de cuantas minas hay cerca
int calcular_minas_cercanas(int fila, int columna, int tablero[filas_def][columnas_def]){
	
  int conteo= 0, fila_inicio, fila_fin, columna_inicio, columna_fin;
  
  // Delimitar el espacio donde se mostraran las minas
  
  if(fila<= 0){
    fila_inicio= 0;
  } else{
    fila_inicio= fila - 1;
  }

  if(fila+1>= filas_def){
    fila_fin= filas_def - 1;
  } else{
    fila_fin= fila + 1;
  }

  if(columna<= 0){
    columna_inicio= 0;
  } else{
    columna_inicio= columna- 1;
  }
  
  if(columna+ 1 >= columnas_def){
    columna_fin= columnas_def - 1;
  } else{
    columna_fin= columna+ 1;
  }
  
  // Conteo de cuantas minas hay cerca 
  int y;
  
  for(y= fila_inicio; y<= fila_fin; y++){
  	
    int x;
    
    for(x= columna_inicio; x<= columna_fin; x++){
      if (tablero[y][x] == mina){
        conteo++;
      }
    }
  }
  
  return conteo;
}

/*-------------------------------------------------------------[ NUMERO ALEATORIO ]----------------------------------------------------------------------------------- */

// Numero aleatorio
int aleatorio_rango(int min, int max){
  return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

// Colocar minas en las coordenadas que se le indique
void colocar_mina(int fila, int columna, int tablero[filas_def][columnas_def]) {
  tablero[fila][columna] = mina;
}

// Colocar minas de forma aleatoria
void minas_aleatorias(int tablero[filas_def][columnas_def]){ 
  int x;
  
  for(x = 0; x< n_minas; x++){
  	
    int fila = aleatorio_rango(0, filas_def - 1);
    int columna = aleatorio_rango(0, columnas_def - 1);
    
    colocar_mina(fila, columna, tablero);
  }
}

/* ------------------------------------------------------------[ CREANDO TABLA ]-------------------------------------------------------------------------------------- */

// Las siguientes funciones son por mera estetica y para dar un mejor aspecto al juego

// Rellena cada espacio de la cuadricula con #
void iniciar_tablero(int tablero[filas_def][columnas_def]){
	
  int x;
  for(x = 0; x < filas_def; x++){
  	
    int y;
    
    for(y= 0; y< columnas_def; y++){
      tablero[x][y] = espacio_oculto;
    }
  }
}

// Para el largo de cada espacio en el encabezado
void separar_encabezado(){
	
  int x;
  
  for(x = 0; x <= columnas_def; x++){
    cout<<"----";
    
    if(x + 2 == columnas_def){
      cout<<"-";
    }
  }
  cout<<"\n";
}

// Para el largo de cada espacio en las filas 
void separar_filas(){
	
  int x;
  
  for(x = 0; x <= columnas_def; x++){
  	
    cout<<"+---";
    if(x == columnas_def){
      cout<<"+";
    }
  }
  cout<<"\n";
}

// Para imprimir en pantalla el encabezado, ademas agrega los numeros de las columnas
void imprimir_encabezado(){
	
  separar_encabezado();
  
  cout<<"|   ";
  
  int x;
  
  for (x = 0; x < columnas_def; x++){
  	
    cout<<"|  "<<x+1;
    
    if (x+1 == columnas_def){
      cout<<"|";
    }
  }
  cout<<"\n";
}

// Convierte un int a un char
char int_char(int num){
  return num + '0';
}

void imprimir_tablero(int tablero[filas_def][columnas_def], int mostrar_minas){
	
  imprimir_encabezado();
  separar_encabezado();
  
  int x, num_fila=0;
  
  for (x = 0; x < filas_def; x++){
  	
    int y;
    
    // Imprimir el numero de la fila
    cout<<"|  "<<num_fila;
    num_fila++;
    
    for (y = 0; y < columnas_def; y++){
    	
      // Ocultar minas
      char espacio_creado = espacio_oculto;
      char espacio_actual = tablero[x][y];
      
      if (espacio_actual == mina){
        espacio_creado = espacio_oculto;
      } else if (espacio_actual == espacio_descubierto){
      	
        // Para que muestre las minas cercanas, si ya abrio el espacio
        int minas_cercanas = calcular_minas_cercanas(x, y, tablero);
        espacio_creado = int_char(minas_cercanas);
      }
      
      // Mostrar las minas
      if(espacio_actual == mina && (mostrar || mostrar_minas)){
        espacio_creado = mina;
      }
      
      cout<<"|  "<<espacio_creado;
      
      if(y+ 1 == columnas_def){
        cout<<"|";
      }
    }
    
    cout<<"\n";
    separar_filas();
  }
}

/* ------------------------------------------------------------[ COMPROBACIONES ]------------------------------------------------------------------------------------- */

// Comprobar la cantidad de filas y columnas, ademas de errores por poner las mismas coordenadas
int abrirCasilla(int fila, int columna, int tablero[filas_def][columnas_def]){
  
  columna--;
  
  // Comprobar cantidad de filas y columnas
  assert(columna< columnas_def && columna>= 0);
  assert(fila < filas_def && fila>= 0);
  
  // Error por poner coordenadas en una mina, otra vez
  if (tablero[fila][columna] == mina){
    return mina_encontrada;
  }
  
  // Error por poner coordenadas en un espacio, otra vez
  if (tablero[fila][columna] == espacio_descubierto){
    return espacio_ya_descubierto;
  }
  
  // despues de comprobar que no hubo fallos, asigna a las coordenadas un "Espacio descubierto"
  tablero[fila][columna] = espacio_descubierto;
  return null;
}

// Comprobar si no hay mas cuadros por abrir
int todos_cuadros_abiertos(int tablero[filas_def][columnas_def]){
	
  int x;
  for (x = 0; x< filas_def; x++){
  	
    int y;
    
    for(y= 0; y< columnas_def; y++){
    	
      char cuadro = tablero[x][y];
      if(cuadro == espacio_oculto){
        return 0;
      }
    }
  }
  return 1;
}

/* ------------------------------------------------------------[ MENU ]----------------------------------------------------------------------------------------------- */

// Menu principal del juego
void menu(){
	system("cls");
	
	int res= 0;

	cout<<"  ____________________________________"<<endl;
	cout<<" |                                    |"<<endl;
	cout<<" |           [  BUSCAMINAS  ]         |"<<endl;
	cout<<" |                                    |"<<endl;
	cout<<" | 1. Jugar                           |"<<endl;
	cout<<" | 2. Instrucciones                   |"<<endl;
	cout<<" | 3. Salir                           |"<<endl;
	cout<<" |                                    |"<<endl;
	cout<<" | > ";
	cin>>res;
	
	switch(res){
		
		case 1: juego(); break;
		case 2: instrucciones(); break;
		case 3: 
			system("cls");
			cout<< "Fin del programa"<<endl; break;
	}
}

// Instrucciones del juego
void instrucciones(){
	
	system("cls");
	
	int res= 0;
	
	cout<<"  ___________________________________________________________________________________________________"<<endl;
	cout<<" |                                                                                                   |"<<endl;
	cout<<" |                                 [  INSTRUCCIONES DE JUEGO  ]                                      |"<<endl;
	cout<<" |                                                                                                   |"<<endl;
	cout<<" |  > La idea del juego es despejar todas aquellas casillas de una ventana que no oculten una mina.  |"<<endl;
	cout<<" |                                                                                                   |"<<endl;
	cout<<" |  > Ciertas casillas tienen asignadas un número, este número indica las minas que se encuentran    |"<<endl;
	cout<<" |  adyacentes a esta casilla. Por ejemplo si una casilla tiene el número 2, entonces de las ocho    |"<<endl;
	cout<<" |  casillas adyacentes a esta (si no es en una esquina o borde) hay 2 con minas y 6 sin minas.      |"<<endl;
	cout<<" |  Si se descubre una casilla y esta se encuentra vacía, esto indica que las casillas vecinas no    |"<<endl;
	cout<<" |  tiene mina y estas se descubren automáticamente.                                                 |"<<endl;
	cout<<" |                                                                                                   |"<<endl;
	cout<<" |  > Al descubrir una casilla con una mina se pierde el juego.                                      |"<<endl;
	cout<<" |                                                                                                   |"<<endl;
	cout<<" |  > Se gana el juego cuando se logra destapar todo el campo sin detonar una casilla con una mina.  |"<<endl;
	cout<<" |___________________________________________________________________________________________________|"<<endl;
	
	cout<<"  ___________________________________________________________________________________________________"<<endl;
	cout<<" |"<<endl;
	cout<<" | 1. Volver al menu"<<endl;
	cout<<" | 2. Salir del juego"<<endl;
	cout<<" |"<<endl;
	cout<<" | > ";
	cin>>res;
	
	switch(res){
		
		case 1: menu(); break;
		case 2:
			system("cls");
			cout<<" Fin del programa"<<endl; break;
	}
}

// Juego
void juego(){
	
	system("cls");
	
	int tablero[filas_def][columnas_def];
	int mostrar_minas=0;
  	
  	// rand, para el numero aleatorio
  	srand(getpid());
  	iniciar_tablero(tablero);
  	minas_aleatorias(tablero);
  	
  	// Ciclo infinito que ejecuta el juego y termina cuando el usuario gana o pierde
  	while(1){
  		
    	imprimir_tablero(tablero, mostrar_minas);
    	
    	if (mostrar_minas){
     		break;
    	}
    	
    int columna;
    int fila;
    
    
    cout<<" \nDigite la fila: "<<endl;
    cout<<"   > ";
    cin>>fila;
    
    cout<<" \nDigite la columna: "<<endl;
    cout<<"   > ";
    cin>>columna;
    
    cout<<"\n";
    
    int comprobacion = abrirCasilla(fila, columna, tablero);
    
    if(todos_cuadros_abiertos(tablero)){
    	cout<<" ___________________________________________________"<<endl;
    	cout<<"|                                                   |"<<endl;
      	cout<<"|                     YOU WIN                       |"<<endl;
      	cout<<"|___________________________________________________|"<<endl;
    
      mostrar_minas = 1;
    } else if (comprobacion == espacio_ya_descubierto){
      cout<<"Ya has seleccionado esta casilla"<<endl;
    } else if (comprobacion == mina_encontrada){
    	cout<<" ___________________________________________________"<<endl;
    	cout<<"|                                                   |"<<endl;
      	cout<<"|                    GAME OVER                      |"<<endl;
      	cout<<"|___________________________________________________|"<<endl;
      	
      mostrar_minas = 1;
    }
  }
}

/* ------------------------------------------------------------[ MAIN ]----------------------------------------------------------------------------------------------- */
int main()
{
	menu();
		cout<<"\n";
		cout<<" ___________________________________________________"<<endl;
    	cout<<"|                                                   |"<<endl;
      	cout<<"|                  FIN DEL JUEGO                    |"<<endl;
      	cout<<"|___________________________________________________|"<<endl;
      	
    system("pause");
	return 0;
}

