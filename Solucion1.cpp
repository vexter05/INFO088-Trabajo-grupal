#include <iostream>
#include <string>
#include <fstream>

using namespace std;
typedef unsigned char uchar; 

struct Solucion1 {
    uchar** Array; //Array que contendra todas las palabras
    float Overhead; //porcentaje que aumenta la capacidad del array exponensialmente cuando se llene
    int Capacidad; //tamaño limite del array
    int Contenido; //cantidad de palabras almacenadas en el array

    //constructor del array
    Solucion1(int CapInicial, float OverInicial){
        Capacidad = CapInicial; //coloca la capacidad a un monto inicial dado
        Overhead = OverInicial; //coloca al overhead como un porcentage dado al inicio 
        Contenido = 0; //indica que el array inicia vacio
        Array = new uchar*[Capacidad]; //crea el array vacio con la capacidad inicial
    }

    //desctructor del array
    ~Solucion1() {
        for (int i = 0; i < Contenido; ++i) { //borra todos los elementos del array
            delete[] Array[i];
        }
        delete[] Array; //borra el array
    }

    //funcion para expandir el array
    Void Expandir(){
        int NuevaCap = int (Capacidad * (1.0f * Overhead) + 1); //incrementa la capacidad por el overhead y suma 1
        uchar** NuevoArray = new uchar*[NuevaCap]; //crea el nuevo array con la nueva capacidad

        for(int i = 0; i < Contenido; ++i) NuevoArray[i] = Array[i]; // Copia las celdas del array anterior
        delete[] Array; // elimina el array antiguo
        Array = NuevoArray; //remplaza el array antiguo
        Capacidad = NuevaCap; //remplaza la capacidad antigua

        cout << "se expandio el array!\n"; //da un aviso para saver que se expandio el array
    }

    //funcion para comparar palabras
    int Comparar(uchar* p1, uchar* p2)  { //resive 2 arrays con el valor ascii de las palabras
        int i = 0;
        while (p1[i] != '\0' && p2[i] != '\0') { //la funcion retornara 1 si la palabra p1 es mayor
            if (p1[i] > p2[i]) return 1;         //retorna -1 si p2 es mayor y 0 si ambas palabras son lo mismo
            if (p1[i] < p2[i]) return -1;
            ++i;
        }
        if (p1[i] == '\0' && p2[i] == '\0') return 0;
        if (p1[i] == '\0') return -1;
        return 1;
    }

    //funcion para uscar
    int Buscar(uchar* palabra)  { //la funcion recive un array 
        int inicio = 0;
        int fin = Contenido - 1; //toma el index del fin y el inicio del array

        while (inicio <= fin) { //empieza una busqueda binaria
            int mitad = inicio + (fin - inicio) / 2; //saca la mitad de los elementos del array
            int comp = Comparar(Array[mitad], palabra); //compara las dos palabras entre si

            if (comp == 0) { //si encontramos la palabra avisa que se encontro la palabra y en que indice
                cout << "la palabra se encuentra en el indice " << mitad << " del array!\n";
                return mitad;  
            }
            if (comp == -1) inicio = mitad + 1; //la palabra es mas grande que la mitad
            else fin = mitad - 1; // la palabra es mas pequeña que la mitad
        }
        cout << "no se encontro la palabra\n";  //avisa que no se encontro la palabra
        return -1; //return para la funcion que lo llame
    }

    // funcion para insertar palabras
    void insertar(uchar* nuevaPalabra) { //se le da una palabra
        if (Contenido == Capacidad) expandir(); //verifica y expande el array si no hay espacio
        
        int i = Contenido - 1; //da el indice de la ultima palabra en el array
        while (i >= 0 && Comparar(Array[i], nuevaPalabra) > 0) { //ve si la palabra nueva es mayor o menor que la ultima 
            Array[i + 1] = Array[i]; // si la nueva palabra es menor, hace espacio
            --i; // y se mueve a la palabra antes de esa
        }

        Array[i + 1] = nuevaPalabra; //añade la palabra nueva en la celda correspondiente
        ++Contenido; //aumenta la cantidad de palabras en el array
        cout << nuevaPalabra << " añadida al array!\n" //avisa que fue añadida con exito
    }

    // funcion de eliminar palabra
    bool eliminar( uchar* palabra) { //recive una palabra a eliminar
        int indice = Buscar(palabra); // verifique si existe
        
        if (indice == -1) { //la palabra no existe
            cout << "la palabra no esta en el array!\n" //avisa que no existe
            return false; //hace un return para acabar el programa antes
        }

        delete[] Array[indice]; //borra el indice del array, creando un indice vacio
        for (int i = indice; i < Contenido - 1; ++i) Array[i] = Array[i + 1];
        //mueve las palabras siguientes para llenar el espacio vacio
        //no es necesario reducir la capacidad del array, solo se reduce el contenido

        --Contenido; //disminuye la cantidad de palabras en el array
        cout << palabra << " eliminada del array!\n" //avisa que se elimino con exito
        return true;
    }

    //funcion para mostrar las palabras del array
    void mostrar()  {
        cout << "Las palabras del array son: [ ";
        for (int i = 0; i < Contenido; ++i) cout << Array[i] << " ";
        
        cout << "]" << " (Capacidad del Array: " << Contenido << "/" << Capacidad << ")\n";
    }
}