#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Solucion1.cpp" //incluimos el archivo que contiene la solucion 1

using namespace std;

int main() {
    srand(time(0)); //creamos la semilla para el rand

    ifstream archivoD1("D1.txt"); //abrimos el diccionario
    vector<string> DiccionarioD1; //creamos el vector D1

    if (!archivoD1.is_open()) {
        cout << "Error: No se pudo abrir D1.txt\n"; //error por si el archivo no se pudo abrir
        return 1;
    }

    archivoD1.close(); //cerramos el archivo

    if (DiccionarioD1.empty()) {
        cerr << "Error: El archivo D1.txt esta vacio.\n"; //error por si el archivo existe pero esta vacio
        return 1;
    }

    cout << "--- Archivo D1.txt cargado con " << DiccionarioD1.size() << " palabras. ---\n";

    Solucion1 ArrayD1(10, 0.1); //creamos un array con 10 celdas iniciales y un overhead de 10%

    // definimos la cantidad aleatoria de palabras a añadir, buscar y eliminar
    int cantInsertar = rand() % 100 + 1; 
    int cantBuscar = rand() % 50 + 1;
    int cantEliminar = rand() % 30 + 1;

    //anunciamos los valores
    cout << "== iniciando test aleatorio ==\n";
    cout << "cantidad de inserciones planificadas: " << cantInsertar << "\n";
    cout << "cantidad de busquedas planificadas: " << cantBuscar << "\n";
    cout << "cantidad de eliminaciones planificadas: " << cantEliminar << "\n";

    // test de insertacion de palabras
    cout << "--- ejecutando inserciones aleatorias ---\n";
    for (int i = 0; i < cantInsertar; i++) {
        int indiceAleatorio = rand() % DiccionarioD1.size(); //escogemos un indice aleatorio
        
        // .c_str() nos da el puntero const char* del string. 
        // Usamos reinterpret_cast para transformarlo al (unsigned char*) que pide la estructura
        unsigned char* ucharPalabra = reinterpret_cast<unsigned char*>(const_cast<char*>(DiccionarioD1[indiceAleatorio].c_str()));
        
        ArrayD1.insertar(ucharPalabra);
    }
    cout << "contenido actual de la estructura ordenada alfabeticamente:\n";
    ArrayD1.mostrar();
    cout << "\n";

    // test de busqueda
    cout << "--- ejecutando Busquedas Aleatorias ---\n";
    for (int i = 0; i < cantBuscar; i++) {
        string palabraABuscar;
        if (rand() % 10 < 8) {
            palabraABuscar = DiccionarioD1[rand() % DiccionarioD1.size()]; //80% buscamos por una palabra del diccionario
        } else {
            palabraABuscar = "palabra_inexistente_" + to_string(rand() % 100); //20% buscamos por una palabra que no esta en el diccionario
        }

        //obtenemos el puntero directamente de la variable temporal 'palabraABuscar'
        unsigned char* ucharBuscar = reinterpret_cast<unsigned char*>(const_cast<char*>(palabraABuscar.c_str()));
        
        ArrayD1.buscar(ucharBuscar); //buscamos la palabra, como la funcion avisa si se encuentra o no
    }                                // no es necesario avisar en este programa
    cout << "\n";

    //test de eliminar palabras
    cout << "--- ejecutando eliminaciones aleatorias ---\n";

    for (int i = 0; i < cantEliminar; i++) {
        string palabraAEliminar = DiccionarioD1[rand() % DiccionarioD1.size()];
        
        //obtenemos el puntero directamente de la variable temporal 'palabraAEliminar'
        unsigned char* ucharEliminar = reinterpret_cast<unsigned char*>(const_cast<char*>(palabraAEliminar.c_str()));

        ArrayD1.eliminar(ucharEliminar); //eliminamos la palabra y avisa si se completo o no
    }
    
    cout << "=== Test Finalizado ===\n";
    cout << "Estado final de tu Solucion1:\n";
    ArrayD1.mostrar(); //mostramos el array final
    //como ya tenemos el deconstructor, el array se librara cuando termine el main
    return 0;
}