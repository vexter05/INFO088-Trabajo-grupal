#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

//estructura de los nodos
struct Nodo {
    string key;

    Nodo* next;
    Nodo* bajo;

    Nodo(string k) {
        key = k;
        next = nullptr;
        bajo = nullptr;
    }
};

class ListaGrid {
private:
    vector<Nodo*> niveles; // da niveles de tope a fondo
    int k;

public: //el valor k para dar saltos entre niveles (generalmente en potencias de 2)
    ListaGrid(int valorK) {
        k = valorK;
    }

    //funcion buscar

    bool Buscar(string palabraBuscar) { //nos dan la palabra a buscar
        if (niveles.empty()) {
            cout << "los niveles estan vacios!\n"; //si los niveles estan vacios nos da falso y avisa
            return false; 
        }

        Nodo* nivActual = niveles.back(); // nos colocamos en el nivel del tope

        while (nivActual) { //mientras el nivel actual sea valido, corre el programa

            while (nivActual->next && nivActual->next->key <= palabraBuscar) {

                nivActual = nivActual->next; //si es que la palabra siguiente es menor a la palabra que se
            }                                //busca, pasa al siguiente nodo del nivel

            if (nivActual->key == palabraBuscar){ //si la palabra se encontro, lo anuncia
                cout << "palabra encontrada!\n";
                return true;
            }
            nivActual = nivActual->bajo; //baja de nivel si es que no se encontro la palabra en el nivel actual
        }

        cout << "no se encontro la palabra\n";
        return false;
    }

    //funcion de insertar
    void insertarFondo(string palabra) {

        if (niveles.empty()) { //si todo esta vacio, añade la palabra y crea los niveles
            Nodo* nuevoNodo = new Nodo(palabra);
            niveles.push_back(nuevoNodo);
            return;
        }

        Nodo* frente = niveles[0]; //empezamos en el nivel del fondo porque tiene todas las llaves
        
        if (palabra < frente->key) {
            Nodo* nuevoNodo = new Nodo(palabra);
            nuevoNodo->next = frente;
            niveles[0] = nuevoNodo; //inserta la palabra al inicio y lo anuncia

            cout << "palabra añadida!\n"

            ReconstruirNiveles(); //reconstruye los niveles
            return;
        }

        Nodo* nivActual = frente; //ocupamos el nuevo nodo para buscar el lugar para la palabra

        while (nivActual->next && nivActual->next->key < palabra) {

            nivActual = nivActual->next; //mueve el nodo si aun no encuentra el lugar correcto
        }

        if (nivActual->key == palabra) return;
        if (nivActual->next && nivActual->next->key == palabra) return;

        Nodo* nuevoNodo = new Nodo(palabra);

        nuevoNodo->next = nivActual->next;
        nivActual->next = nuevoNodo;

        ReconstruirNiveles();
    }

    // =========================
    // DELETE
    // =========================
    void borrar(string palabra) {

        if (niveles.empty()) return;

        Nodo* frente = niveles[0];

        // delete first node
        if (frente->key == palabra) {
            niveles[0] = frente->next;
            delete frente;

            ReconstruirNiveles();
            return;
        }

        Nodo* nivActual = frente;

        while (nivActual->next && nivActual->next->key != palabra) {

            nivActual = nivActual->next;
        }

        if (nivActual->next) {
            Nodo* temp = nivActual->next;

            nivActual->next = temp->next;

            delete temp;
        }

        ReconstruirNiveles();
    }

    // =========================
    // REBUILD UPPER LEVELS
    // =========================
    void ReconstruirNiveles() {

        Nodo* fondo = niveles[0];

        niveles.clear();
        niveles.push_back(fondo);

        vector<Nodo*> nivAnterior;
        Nodo* actual = fondo;

        while (actual) {
            nivAnterior.push_back(actual);
            actual = actual->next;
        }

        while ((int)nivAnterior.size() > k) {

            vector<Nodo*> NodosNivArriba;

            Nodo* frenteArriba = nullptr;
            Nodo* colaArriba = nullptr;

            for (int i = 0;
                 i < nivAnterior.size();
                 i += k) {

                Nodo* muestra = nivAnterior[i];

                Nodo* nodoArriba =
                    new Nodo(muestra->key);

                nodoArriba->bajo = muestra;

                if (!frenteArriba) {
                    frenteArriba = nodoArriba;
                    colaArriba = nodoArriba;
                } else {
                    colaArriba->next = nodoArriba;
                    colaArriba = nodoArriba;
                }

                NodosNivArriba.push_back(nodoArriba);
            }

            niveles.push_back(frenteArriba);

            nivAnterior = NodosNivArriba;
        }
    }

    // =========================
    // PRINT
    // =========================
    void mostrarNiveles() {

        for (int i = niveles.size() - 1;
             i >= 0;
             i--) {

            cout << "nivel " << i + 1 << ": ";

            Nodo* actual = niveles[i];

            while (actual) {
                cout << actual->key << " ";
                actual = actual->next;
            }

            cout << endl;
        }
    }
};

int main() {

    ListaGrid grid(4);

    grid.insertarFondo("manzana");
    grid.insertarFondo("pato");
    grid.insertarFondo("tiburon");
    grid.insertarFondo("zorro");
    grid.insertarFondo("perro");
    grid.insertarFondo("banana");
    grid.insertarFondo("mouraton");
    grid.insertarFondo("tigre");
    grid.insertarFondo("lobo");

    grid.mostrarNiveles();

    cout << endl;

    cout << "Buscar perro: "
         << grid.Buscar("perro")
         << endl;

    cout << "Buscar zebra: "
         << grid.Buscar("zebra")
         << endl;

    cout << endl;

    grid.borrar("manzana");

    grid.mostrarNiveles();

    return 0;
}