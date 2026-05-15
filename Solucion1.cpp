#include <iostream>
#include <string>
#include <fstream>

using namespace std;
typedef unsigned char uchar; 

struct Solucion1 {
    uchar** Array; //Array que contendra todas las palabras
    float overhead; //porcentaje que aumenta la capacidad del array exponensialmente cuando se llene
    int capacidad; //tamaño limite del array
    int contenido; //cantidad de palabras almacenadas en el array

    //constructor del array
    Solucion1(int CapInicial, float OverInicial){
        capacidad = CapInicial; //coloca la capacidad a un monto inicial dado
        overhead = OverInicial; //coloca al overhead como un porcentage dado al inicio 
        contenido = 0; //indica que el array inicia vacio
        Array = new uchar*[capacidad]; //crea el array vacio con la capacidad inicial
    }

    //desctructor del array
    ~Solucion1() {
        delete[] Array; //borra el array
    }

    //funcion para expandir el array
    Expandir(){
        int NuevaCap = capacidad (capacidad * overhead) + 1; //añade el overhead a la capacidad mas 1 una celda-
                                                             // extra en caso de ser muy poco
        uchar** NuevoArray = new uchar*[NuevaCap]; //crea el nuevo array con la nueva capacidad

        for(int i = 0; i < capacidad; i++) {
            NuevoArray[i] = Array[i]; // Copia las celdas del array anterior
        }
        delete[] Array; // elimina el array antiguo
        Array = NuevoArray; //remplaza el array antiguo
        capacidad = NuevaCap; //remplaza la capacidad antigua
    }
    
}