#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;
typedef unsigned char uchar;

struct Solucion1 {
    uchar** Array; // Array que contendrá todas las palabras (cadenas terminadas en '\0')
    float Overhead; // factor de crecimiento (por ejemplo 0.5 = +50%)
    int Capacidad; // tamaño máximo actual del arreglo
    int Contenido; // número de palabras almacenadas

    // constructor
    Solucion1(int CapInicial = 10, float OverInicial = 0.5f) {
        Capacidad = CapInicial;
        Overhead = OverInicial;
        Contenido = 0;
        Array = new uchar*[Capacidad];
        for (int i = 0; i < Capacidad; ++i) Array[i] = nullptr;
    }

    // destructor: libera las cadenas almacenadas y el arreglo
    ~Solucion1() {
        for (int i = 0; i < Contenido; ++i) {
            delete[] Array[i];
        }
        delete[] Array;
    }

    // Expande el arreglo según el overhead
    void expandir() {
        int NuevaCap = int(Capacidad * (1.0f + Overhead)) + 1;
        uchar** NuevoArray = new uchar*[NuevaCap];
        for (int i = 0; i < Contenido; ++i) NuevoArray[i] = Array[i];
        for (int i = Contenido; i < NuevaCap; ++i) NuevoArray[i] = nullptr;
        delete[] Array;
        Array = NuevoArray;
        Capacidad = NuevaCap;
        cout << "se expandio el array!\n";
    }

    // Compara dos cadenas de uchar lexicográficamente
    int comparar(const uchar* p1, const uchar* p2) {
        int i = 0;
        while (p1 && p2 && p1[i] != '\0' && p2[i] != '\0') {
            if (p1[i] > p2[i]) return 1;
            if (p1[i] < p2[i]) return -1;
            ++i;
        }
        if (!p1 && !p2) return 0;
        if (!p1) return -1;
        if (!p2) return 1;
        if (p1[i] == '\0' && p2[i] == '\0') return 0;
        if (p1[i] == '\0') return -1;
        return 1;
    }

    // Búsqueda binaria de una palabra; retorna índice o -1
    int buscar(const uchar* palabra) {
        if (Contenido == 0) return -1;
        int inicio = 0;
        int fin = Contenido - 1;
        while (inicio <= fin) {
            int mitad = inicio + (fin - inicio) / 2;
            int comp = comparar(Array[mitad], palabra);
            if (comp == 0) return mitad;
            if (comp < 0) inicio = mitad + 1;
            else fin = mitad - 1;
        }
        return -1;
    }

    // Inserta una copia de la palabra manteniendo orden
    void insertar(const uchar* nuevaPalabra) {
        if (Contenido == Capacidad) expandir();
        size_t len = strlen((const char*)nuevaPalabra);
        uchar* copia = new uchar[len + 1];
        memcpy(copia, nuevaPalabra, len + 1);

        int i = Contenido - 1;
        while (i >= 0 && comparar(Array[i], copia) > 0) {
            Array[i + 1] = Array[i];
            --i;
        }
        Array[i + 1] = copia;
        ++Contenido;
        cout << (const char*)copia << " añadida al array!\n";
    }

    // Elimina una palabra (si existe) y libera su memoria
    bool eliminar(const uchar* palabra) {
        int indice = buscar(palabra);
        if (indice == -1) {
            cout << "la palabra no esta en el array!\n";
            return false;
        }
        delete[] Array[indice];
        for (int i = indice; i < Contenido - 1; ++i) Array[i] = Array[i + 1];
        Array[Contenido - 1] = nullptr;
        --Contenido;
        cout << (const char*)palabra << " eliminada del array!\n";
        return true;
    }

    // Muestra el contenido
    void mostrar() {
        cout << "Las palabras del array son: [ ";
        for (int i = 0; i < Contenido; ++i) cout << (const char*)Array[i] << " ";
        cout << "] (Contenido/Capacidad: " << Contenido << "/" << Capacidad << ")\n";
    }
};