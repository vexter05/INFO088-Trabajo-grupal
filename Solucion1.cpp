#include <iostream>
#include <string>
#include <vector>

using namespace std;
#define OVERHEAD 1.25 //el 25% que se ocupa al expandir el vector, se añade 1 para poder multiplicar por Capacidad
typedef unsigned char uchar; 

struct Solucion1 {
    vector<uchar*> MiVector; //vector que contendra todas las palabras
    int Capacidad; //tamaño de palabras reservadas dentro de MiVector
    int Contenido; //cantidad de palabras almacenadas dentro de MiVector
    int Indices[26]; //array que contiene el indice de la primera palabra que empieza con cada letra

    //constructor del vector
    Solucion1(){
        Capacidad = 20; //coloca la cantidad de palabras reservadas iniciales de MiVector como 20 palabras
        Contenido = 0; //indica que MiVector inicia vacio
        MiVector.reserve(Capacidad); //reserva la memoria inicial de las 20 palabras en MiVector
        for (int i = 0; i < 26; ++i) Indices[i] = -1; //define los indices como no encontrados
    }

    //destructor del vector
    ~Solucion1() {
        for (uchar* palabra: MiVector) {
            delete[] palabra; //borra cada palabra almacenada en MiVector
        } //como MiVector es parte del struct, se borrara automaticamente cuando se destruya el struct
    }

    //funcion para expandir el vector
    void Expandir(){
        int NuevaCap = int(Capacidad * OVERHEAD); //incrementa la capacidad por el overhead
        vector<uchar*> NuevoVector; //crea el nuevo vector
        NuevoVector.reserve(NuevaCap); //reserva la nueva capacidad en el nuevo vector

        for (int i = 0; i < Contenido; ++i) {
            NuevoVector.push_back(MiVector[i]); //copia todas las palabras existentes
        }

        MiVector = move(NuevoVector); //reemplaza el vector original por el nuevo vector usando move() para mas eficiencia
        Capacidad = NuevaCap; //actualiza la capacidad

        cout << "se expandio el vector!\n"; //aviso de expansión
    }

    //funcion para comparar palabras
    int Comparar(uchar* p1, uchar* p2)  { //recibe 2 palabras a comparar
        int i = 0;
        while (p1[i] == p2[i]) { //la funcion corre mientras las palabras sean iguales
            if (p1[i] == '\0') return 0; //si p1 es vacia, como el while se rompe si no son iguales ambas son iguales y retorna 0
            ++i; //si aun hay characteres, avanca i
        }
        return (p1[i] > p2[i]) ? 1 : -1; //si p1 es mayor que p2 retorna 1, si p1 es menor que p2 retorna -1
    }

    //funcion para obtener el indice de la letra inicial de una palabra
    int InicialIndice(uchar* palabra) {
        if (palabra[0] == '\0') return -1; //si la palabra es vacia, retorna -1
        return palabra[0] - 'a'; //se resta 'a' para retornar un valor ascii entre 0 y 25
    }

    //funcion para actualizar los indices de las palabras iniciales de cada letra
    void ActualizarIndices() {
        for (int i = 0; i < 26; ++i) Indices[i] = -1; //reinicia los indices
        for (int i = 0; i < Contenido; ++i) {
            int iLetra = InicialIndice(MiVector[i]); //obtiene el indice de la letra inicial de cada palabra en MiVector
            if (iLetra >= 0 && Indices[iLetra] == -1) { //si el indice es valido y no se ha asignado un indice para esa letra
                Indices[iLetra] = i; //asigna el indice de la primera palabra que empieza con esa letra
            }
        }
    }

    //funcion para buscar
    int Buscar(uchar* palabra)  { //recibe una palabra a buscar
        int letra = InicialIndice(palabra); //obtiene el valor de la letra inicial de la palabra a buscar

        if (letra < 0) { //si la palabra es vacia avisa que no se encontro y retorna -1
            cout << "no se encontro la palabra\n";
            return -1;
        }

        int inicio = Indices[letra]; //si la letra es valida, tomamos su indice como inicio
        if (inicio == -1) {
            cout << "no se encontro la palabra\n"; //si el indice es -1 sabemos que no hay palabras que empiecen con esa letra
            return -1;
        }

        int fin = Contenido - 1; //definimos el fin como el indice de la ultima palabra en MiVector de momento
        for (int j = letra + 1; j < 26; ++j) { //pasamos por cada letra despues de la inicial
            if (Indices[j] != -1) { //verificamos si hay palabras que empiecen con esa letra
                fin = Indices[j] - 1; //si se encuentra, definimos el indice anterior como fin
                break; //break para romper el ciclo for
            }
        }

        while (inicio <= fin) { //empieza una busqueda binaria
            int mitad = inicio + (fin - inicio) / 2; //saca la mitad de los elementos dentro de MiVector, se ocupa esta formula para evitar overflow
            int comp = Comparar(MiVector[mitad], palabra); //compara las dos palabras entre si

            if (comp == 0) { //si encontramos la palabra avisa que se encontro la palabra y en que indice
                cout << "la palabra se encuentra en el indice " << mitad << " del vector!\n";
                return mitad;
            }
            if (comp == -1) inicio = mitad + 1; //la palabra es mas grande que la mitad
            else fin = mitad - 1; // la palabra es mas pequeña que la mitad
        }
        cout << "no se encontro la palabra\n";  //avisa que no se encontro la palabra
        return -1; //return para la funcion que lo llame
    }

    //funcion para insertar palabras
    void insertar(uchar* nuevaPalabra) { //recibe una palabra a insertar
        int i = Contenido - 1; // indice de la ultima palabra en el vector
        while (i >= 0 && Comparar(MiVector[i], nuevaPalabra) > 0) { //ve si la nueva palabra es menor
            MiVector[i + 1] = MiVector[i]; //muebe la palabra si en menor
            --i; //se mueve a la palabra anterior
        }

        MiVector[i + 1] = nuevaPalabra; //añade la palabra nueva en la celda correspondiente
        ++Contenido; //aumenta la cantidad de palabras en el vector
        ActualizarIndices(); //actualiza los indices de las palabras iniciales de cada letra
        cout << nuevaPalabra << " añadida al vector!\n"; //avisa que fue añadida con exito
        if (Contenido == Capacidad) Expandir(); //si el vector esta lleno espues de añadir la palabra y si lo esta, se expande
    }

    // funcion de eliminar palabra
    bool eliminar(uchar* palabra) { //recibe una palabra a eliminar
        int indice = Buscar(palabra); //verifica si existe la palabra en MiVector

        if (indice == -1) { //la palabra no existe
            cout << "la palabra no esta en el vector!\n"; //avisa que no existe
            return false; //termina antes y retorna falso
        }

        delete[] MiVector[indice]; //borra la palabra en el indice, creando un agujero en MiVector
        MiVector.erase(MiVector.begin() + indice); //se ocupa erase para eliminar el agujero y mover las palabras efficientemente

        --Contenido; //disminuye la cantidad de palabras en el vector
        ActualizarIndices();//actualiza los indices de las palabras iniciales de cada letra
        cout << palabra << " eliminada del vector!\n"; // avisa que se elimino con exito
        return true;
    }

    //funcion para mostrar las palabras del vector
    void mostrar()  {
        cout << "Las palabras del vector son: [ ";
        for (int i = 0; i < Contenido; ++i) cout << MiVector[i] << " ";
        cout << "]" << " (Capacidad del Vector: " << Contenido << "/" << Capacidad << ")\n";
    }
};