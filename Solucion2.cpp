#include <iostream>
#include <vector>
#include <string>

using namespace std;
typedef unsigned char uchar;

struct Nodo { //estructura de un nodo
    uchar* key; //contine la palabra almacenada
    Nodo* next; //puntero al siguiente nodo en el mismo nivel
    Nodo* abajo; //puntero al mismo nodo del nivel inferior

    //Constructor de un nodo
    Nodo(uchar* palabra = nullptr) //si no se da palabra es nullptr
        : key(nullptr), next(nullptr), abajo(nullptr) {  //punteros inician en nullptr
        if (palabra) {
            size_t len = 0;
            while (palabra[len]) { //calcula la longitud de la palabra
                len++;
            }
            key = new uchar[len + 1]; //reserva memoria para la palabra
            for (size_t i = 0; i <= len; ++i) {
                key[i] = palabra[i]; //copia la palabra al nodo
            }
        }
    }

    //Destructor de un nodo
    ~Nodo() {
        delete[] key;
    }
};

class Grilla {
    int k; //int k para saltos en niveles superiores
    vector<Nodo*> heads; //vector de punteros a los nodos cabeza de cada nivel

    static int Comparar(uchar* a, uchar* b) { //recibe dos palanbras a comparar, debe ser statico para evitar problemas con punteros a miembros
        while (*a && *b && *a == *b) { //mientras ambos caracteres sean iguales y no sean el fin de la cadena
            a++;
            b++; //avanza a la siguiente posición en ambas palabras
        }
        return *a < *b ? -1 : (*a > *b ? 1 : 0); //retorna -1 si a < b, 1 si a > b, o 0 si son iguales
    }

    uchar* Duplicar(uchar* palabra) { //crea una copia dinámica de la palabra dada (mismo proceso que el constructor de Nodo)
        size_t len = 0;
        while (palabra[len]) {
            len++;
        }
        uchar* copia = new uchar[len + 1];
        for (size_t i = 0; i <= len; ++i) {
            copia[i] = palabra[i]; 
        }
        return copia;
    }

    Nodo* CrearNodo(uchar* palabra) { //crea un nodo con la palabra dada
        Nodo* nodo = new Nodo(); //crea un nuevo nodo
        nodo->key = Duplicar(palabra); //recive copia de la palabra
        nodo->next = nullptr; //inicia ambos punteros como nullptr
        nodo->abajo = nullptr;
        return nodo; //retorna el nodo
    }

    int Niveles(Nodo* head) const { //cuenta el número de nodos en el nivel dado por head
        int count = 0;
        while (head) {
            count++;
            head = head->next;
        }
        return count;
    }

    void LimpiarNivel(int nivel) { //elimina todos los nodos en el nivel dado
        Nodo* actual = heads[nivel]; //obtiene el nodo cabeza del nivel
        while (actual) { //elimina cada nodo del nivel
            Nodo* next = actual->next;
            delete actual;
            actual = next;
        }
        heads[nivel] = nullptr; //borra la referencia al nivel
    }

public:
    Grilla(int k_variable){ //constructor de grilla, recive k
        k = k_variable; //define k y crea nivel base vacío
        heads.push_back(nullptr);
    }

    ~Grilla() { //destructor de grilla
        for (size_t i = 0; i < heads.size(); ++i) {
            LimpiarNivel((int)i); //limpia cada nivel
        }
    }

    void InsertarBase(uchar* palabra) { //inserta la palabra en el nivel base lexográficamente
        if (heads.empty()) {
            heads.push_back(nullptr); //si no hay niveles, crea el nivel base
        }

        Nodo* newNode = CrearNodo(palabra); //crea un nuevo nodo con la palabra dada
        Nodo* actual = heads[0]; //puntero para recorrer el nivel base
        Nodo* anterior = nullptr; //puntero para mantener el nodo anterior

        while (actual && Comparar(actual->key, palabra) < 0) { //compara la palabra con la siguiente y para al colocarla en orden
            anterior = actual; 
            actual = actual->next;
        }

        if (actual && Comparar(actual->key, palabra) == 0) { //evita insertar duplicados
            cout << "Palabra duplicada encontrada en el nivel base" << endl;
            delete newNode;
            return;
        }

        if (!anterior) { //si no hay nodo anterior, la nueva palabra es la nueva cabeza del nivel base
            newNode->next = heads[0];
            heads[0] = newNode;
        } else { //si hay anterior, inserta el nodo normalmente
            newNode->next = actual; 
            anterior->next = newNode;
        }
    }

    void InsertarBaseConNiveles(uchar* palabra) { //inserta en base usando los niveles superiores para facilitar la búsqueda
        if (heads.empty()) { //si no hay niveles, crea el nivel base
            heads.push_back(nullptr);
        }

        if (heads.size() == 1 || !heads.back()) { //si no hay niveles superiores válidos, usa inserción directa
            InsertarBase(palabra);
            return;
        }

        int nivel = (int)heads.size() - 1; //empieza en el nivel mas alto
        Nodo* actual = heads[nivel];
        Nodo* anteriorBase = nullptr;

        while (nivel >= 0) { //mientras haya niveles para revisar
            while (actual->next && Comparar(actual->next->key, palabra) <= 0) {
                actual = actual->next; //compara y avanza si la siguiente palabra es menor o igual a la actual
            }

            if (nivel == 0) { //si llegamos al nivel base, guardamos el nodo anterior para insertar la nueva palabra
                if (Comparar(actual->key, palabra) == 0) { //evita insertar duplicados en el nivel base
                    cout << "Palabra duplicada encontrada en el nivel base" << endl;
                    return;
                }
                if (Comparar(actual->key, palabra) < 0) {
                    anteriorBase = actual;
                } else {
                    anteriorBase = nullptr;
                }
                break;
            }

            if (Comparar(actual->key, palabra) == 0) { //evita insertar duplicados en niveles superiores
                cout << "Palabra duplicada encontrada en un nivel superior" << endl;
                return;
            }

            actual = actual->abajo; //desciende al siguiente nivel
            nivel--; //decrementa el nivel
        }

        Nodo* newNode = CrearNodo(palabra); //crea un nuevo nodo con la palabra dada
        if (!anteriorBase) {
            newNode->next = heads[0]; //si no hay nodo anterior, se inserta al inicio del nivel base
            heads[0] = newNode;
        } else {
            newNode->next = anteriorBase->next; //si hay nodo anterior, se inserta después de este
            anteriorBase->next = newNode;
        }
    }

    void ConstruirNiveles() {//construye los niveles superiores a partir del nivel base
        while (heads.size() > 1) { //si hay más de un nivel, limpia el nivel superior y lo elimina
            LimpiarNivel((int)heads.size() - 1);
            heads.pop_back();
        }

        int nivel = 0;
        while (true) { //construye niveles mientras el nivel actual tenga más nodos que k
            Nodo* debajo = heads[nivel];
            if (!debajo) { //si el nivel debajo está vacío, no se pueden construir más niveles
                break;
            }

            int size = Niveles(debajo); // Si el tamaño es menor o igual a k, no necesitamos más niveles.
            if (size <= k) {
                break;
            }

            Nodo* actual = debajo; //punteros para construir el nuevo nivel
            Nodo* newHead = nullptr;
            Nodo* anterior = nullptr;
            int index = 1;

            while (actual) { //recorre el nivel debajo y selecciona nodos para el nuevo nivel según k
                if (index == 1 || ((index - 1) % k) == 0) { //si el indice es 1 o multiplo de k, toma el nodo para el nivel
                    Nodo* copia = CrearNodo(actual->key); //crea una copia del nodo actual para el nuevo nivel
                    copia->abajo = actual;
                    if (anterior) {
                        anterior->next = copia;
                    } else {
                        newHead = copia;
                    }
                    anterior = copia;
                }
                actual = actual->next; //avanza al siguiente nodo en el nivel debajo
                index++; //incrementa el indice
            }

            heads.push_back(newHead); //agrega el nuevo nivel a la grilla
            nivel++;
        }
    }

    bool Buscar(uchar* palabra) const { //busca la palabra en la grilla
        if (heads.empty() || !heads.back()) { //si no hay niveles o el nivel superior está vacío, la palabra no se encuentra
            cout << "No encontrado" << endl;
            return false;
        }

        int nivel = (int)heads.size() - 1; //comienza la búsqueda desde el nivel mas alto
        Nodo* actual = heads[nivel];

        while (nivel >= 0 && actual) { //mientras haya niveles y nodos para revisar
            while (actual->next && Comparar(actual->next->key, palabra) <= 0) {
                actual = actual->next; //anaza al siguiente nodo hasta que la clave sea mayor que la actual o llegue a fin
            }

            if (Comparar(actual->key, palabra) == 0) { //revisas si encuentra la palabra en el nodo actual
                cout << "Encontrado" << endl;
                return true;
            }

            if (nivel == 0) { //si llegamos a la base y no encontramos la palabra, se detiene la búsqueda en este while
                break;
            }

            actual = actual->abajo ? actual->abajo : heads[nivel - 1]; //deciende al siguiente nivel
            nivel--;
        }

        while (actual && Comparar(actual->key, palabra) < 0) { //sigue buscando en el nivel base hasta encontrar o no a la palabra
            actual = actual->next;
        }

        if (actual && Comparar(actual->key, palabra) == 0) { //si se encontro la palabra, avisa
            cout << "Encontrado" << endl;
            return true;
        }

        cout << "No encontrado" << endl; //no se encuentra la palabra y lo avisa
        return false;
    }

    void Eliminar(uchar* palabra) {
        if (heads.empty() || !heads[0]) { //si no hay niveles, no se puede eliminar nada
            cout << "key no encontrada" << endl;
            return;
        }

        int numNiveles = (int)heads.size();
        
        vector<Nodo*> anteriores(numNiveles, nullptr); //vector para almacenar los nodos anteriores a la palabra en cada nivel
        vector<Nodo*> encontrados(numNiveles, nullptr); 

        int level = numNiveles - 1; //comienza la búsqueda desde el nivel más alto
        Nodo* current = heads[level];

        while (level >= 0) { //mientras haya niveles para revisar
            Nodo* prev = nullptr;
            
            while (current && Comparar(current->key, palabra) < 0) { //avanza al siguiente nodo hasta que sea mayor o igual
                prev = current;
                current = current->next;
            }

            anteriores[level] = prev; // Almacena el nodo anterior al nodo actual en este nivel

            if (current && Comparar(current->key, palabra) == 0) { //si el nodo actual es la palabra, se almacena
                encontrados[level] = current;
            }

            if (level > 0) { //si no estamos en el nivel base, descendemos al siguiente nivel
                current = prev ? prev->abajo : heads[level - 1];
            }
            level--;
        }


        if (!encontrados[0]) { //si no se encontró la palabra en el nivel base, no se puede eliminar
            cout << "key no encontrada" << endl;
            return;
        }

        Nodo* target = encontrados[0]; //como sabemos que la palabra existe en el nivel base, target es el nodo a eliminar
        Nodo* successor = target->next;

        if (anteriores[0]) { //si hay un nodo anterior en el nivel base, si no el sucesor se convierte en la cabeza base
            anteriores[0]->next = successor;
        } else {
            heads[0] = successor;
        }
        delete target; //borra el nodo del nivel base

        for (int i = 1; i < numNiveles; i++) { //actualiza los niveles superiores para mostrar el siguiente nodo al eliminado
            Nodo* nodoAModificar = encontrados[i]; //si el nodo esta en un nivel superior, se crea un nodo a modificar
            
            if (nodoAModificar) { //si el nodo existe, se busca su sucesor para reemplazarlo si existe
                if (successor) {
                    delete[] nodoAModificar->key; //borra la clave del nodo a modificar para reemplazarla por la del sucesor
                    nodoAModificar->key = Duplicar(successor->key);
                    
                    if (i == 1) { //se remplaza el abajo del nodo si es el nivel 1
                        nodoAModificar->abajo = successor;
                    }
                } else { //si no hay sucesor, ve si hay nodo anterior para actualizar el next
                    if (anteriores[i]) {
                        anteriores[i]->next = nodoAModificar->next;
                    } else { //si no hay nodo anterior, el siguiente nodo se convierte en la nueva cabeza del nivel
                        heads[i] = nodoAModificar->next;
                    }
                    delete nodoAModificar; //borra el nodo a modificar al fin del proceso
                }
            }
        }

        while (heads.size() > 1 && heads.back() == nullptr) { //si el nivel superior esta vacio, lo borra y repite el proceso
            heads.pop_back();
        }

        cout << "El nodo ha sido borrado" << endl; //avisa que el nodo ha sido eliminado
    }

    void printGrid() const { //imprime la grilla desde el nivel más alto hasta el nivel base
        for (int nivel = (int)heads.size() - 1; nivel >= 0; --nivel) { //recorre cada nivel desde el más alto
            cout << "Nivel " << nivel << ": ";
            Nodo* actual = heads[nivel];
            while (actual) { //imprime cada nodo del nivel
                cout << (const char*)actual->key;
                if (actual->next) {
                    cout << " -> ";
                }
                actual = actual->next;
            }
            cout << endl;
        }
    }
};