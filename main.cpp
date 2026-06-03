#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <limits>
#include <chrono>
#include "Solucion1.cpp"
#include "Solucion2.cpp"

using namespace std;
typedef unsigned char uchar;

static uchar* ucharAString(const string& s) { //convierte un string a un arreglo de uchar con terminación nula
    uchar* buffer = new uchar[s.size() + 1];
    
    for (size_t i = 0; i < s.size(); ++i) buffer[i] = (uchar)s[i]; //copia cada caracter del string al arreglo de uchar
    buffer[s.size()] = '\0'; //agrega el nulo al final del uchar
    
    return buffer;
}

// Mide el tiempo en milisegundos que tarda en construir una instancia de Solucion1,
// el tiempo total de insertar D2Insertar, el tiempo total de eliminar D2Eliminar
// y el tiempo promedio de búsqueda de D2Buscar.
void testSolucion1(vector<uchar*>& ArregloD1, vector<uchar*>& D2Insertar, vector<uchar*>& D2Eliminar, vector<uchar*>& D2Buscar, int initialCap) {
    using namespace chrono;
    auto start = high_resolution_clock::now();
    Solucion1 sol(initialCap);

    for (auto p : ArregloD1) {
        size_t len = 0;
        while (p[len] != '\0') ++len;
        uchar* dup = new uchar[len + 1];
        for (size_t k = 0; k <= len; ++k) dup[k] = p[k];
        sol.insertar(dup);
    }

    auto end = high_resolution_clock::now();
    auto buildMs = duration_cast<milliseconds>(end - start).count();

    int insertedCount = 0;
    auto insertStart = high_resolution_clock::now();
    for (auto palabra : D2Insertar) {
        int before = sol.Contenido;
        size_t len = 0;
        while (palabra[len] != '\0') ++len;
        uchar* dup = new uchar[len + 1];
        for (size_t k = 0; k <= len; ++k) dup[k] = palabra[k];
        sol.insertar(dup);
        if (sol.Contenido == before + 1) {
            ++insertedCount;
        } else {
            delete[] dup;
        }
    }
    auto insertEnd = high_resolution_clock::now();
    auto totalInsertMs = duration_cast<milliseconds>(insertEnd - insertStart).count();

    int deletedCount = 0;
    auto deleteStart = high_resolution_clock::now();
    for (auto palabra : D2Eliminar) {
        if (sol.eliminar(palabra)) ++deletedCount;
    }
    auto deleteEnd = high_resolution_clock::now();
    auto totalDeleteMs = duration_cast<milliseconds>(deleteEnd - deleteStart).count();

    int foundCount = 0;
    auto searchStart = high_resolution_clock::now();
    for (auto palabra : D2Buscar) {
        if (sol.Buscar(palabra) != -1) ++foundCount;
    }
    auto searchEnd = high_resolution_clock::now();
    auto totalSearchUs = duration_cast<microseconds>(searchEnd - searchStart).count();
    double averageSearchUs = static_cast<double>(totalSearchUs) / D2Buscar.size();

    cout << "testSolucion1: tiempo de construcción = " << buildMs << " ms\n";
    cout << "testSolucion1: tiempo total de insercion = " << totalInsertMs << " ms\n";
    cout << "testSolucion1: palabras insertadas = " << insertedCount << "\n";
    cout << "testSolucion1: tiempo total de eliminacion = " << totalDeleteMs << " ms\n";
    cout << "testSolucion1: palabras eliminadas = " << deletedCount << "\n";
    cout << "testSolucion1: tiempo promedio por búsqueda = " << averageSearchUs << " us\n";
    cout << "testSolucion1: palabras encontradas = " << foundCount << "\n";

    for (auto ptr : ArregloD1) delete[] ptr;
    ArregloD1.clear();

    for (auto ptr : D2Insertar) delete[] ptr;
    D2Insertar.clear();

    for (auto ptr : D2Eliminar) delete[] ptr;
    D2Eliminar.clear();

    for (auto ptr : D2Buscar) delete[] ptr;
    D2Buscar.clear();
}

// Versión del test que usa la estructura Grilla (Solucion2)
void testSolucion2(vector<uchar*>& ArregloD1, vector<uchar*>& D2Insertar, vector<uchar*>& D2Eliminar, vector<uchar*>& D2Buscar) {
    using namespace chrono;

    int k_usuario;
    cout << "Ingrese el valor de k para la Grilla: ";
    cin >> k_usuario;

    Grilla gr(k_usuario);

    auto start = high_resolution_clock::now();

    // Construcción inicial: insertar en base y luego construir niveles
    for (auto p : ArregloD1) {
        size_t len = 0;
        while (p[len] != '\0') ++len;
        uchar* dup = new uchar[len + 1];
        for (size_t i = 0; i <= len; ++i) dup[i] = p[i];
        gr.InsertarBase(dup);
        delete[] dup; // Grilla duplica internamente
    }
    gr.ConstruirNiveles();
    auto end = high_resolution_clock::now();
    auto buildMs = duration_cast<milliseconds>(end - start).count();

    int insertedCount = 0;
    auto insertStart = high_resolution_clock::now();
    for (auto palabra : D2Insertar) {
        // sólo insertar si no existe
        if (!gr.Buscar(palabra)) {
            size_t len = 0;
            while (palabra[len] != '\0') ++len;
            uchar* dup = new uchar[len + 1];
            for (size_t i = 0; i <= len; ++i) dup[i] = palabra[i];
            gr.InsertarBaseConNiveles(dup);
            delete[] dup;
            ++insertedCount;
        }
    }
    auto insertEnd = high_resolution_clock::now();
    auto totalInsertMs = duration_cast<milliseconds>(insertEnd - insertStart).count();

    int deletedCount = 0;
    auto deleteStart = high_resolution_clock::now();
    for (auto palabra : D2Eliminar) {
        if (gr.Buscar(palabra)) {
            gr.Eliminar(palabra);
            ++deletedCount;
        }
    }
    auto deleteEnd = high_resolution_clock::now();
    auto totalDeleteMs = duration_cast<milliseconds>(deleteEnd - deleteStart).count();

    int foundCount = 0;
    auto searchStart = high_resolution_clock::now();
    for (auto palabra : D2Buscar) {
        if (gr.Buscar(palabra)) ++foundCount;
    }
    auto searchEnd = high_resolution_clock::now();
    auto totalSearchUs = duration_cast<microseconds>(searchEnd - searchStart).count();
    double averageSearchUs = static_cast<double>(totalSearchUs) / D2Buscar.size();

    cout << "testSolucion2: tiempo de construcción = " << buildMs << " ms\n";
    cout << "testSolucion2: tiempo total de insercion = " << totalInsertMs << " ms\n";
    cout << "testSolucion2: palabras insertadas = " << insertedCount << "\n";
    cout << "testSolucion2: tiempo total de eliminacion = " << totalDeleteMs << " ms\n";
    cout << "testSolucion2: palabras eliminadas = " << deletedCount << "\n";
    cout << "testSolucion2: tiempo promedio por búsqueda = " << averageSearchUs << " us\n";
    cout << "testSolucion2: palabras encontradas = " << foundCount << "\n";

    // liberar memoria de los vectores recibidos
    for (auto ptr : ArregloD1) delete[] ptr;
    ArregloD1.clear();
    for (auto ptr : D2Insertar) delete[] ptr;
    D2Insertar.clear();
    for (auto ptr : D2Eliminar) delete[] ptr;
    D2Eliminar.clear();
    for (auto ptr : D2Buscar) delete[] ptr;
    D2Buscar.clear();
}

int main() {
    const string D2 = "D2.txt"; //constantes con los nombres de los archivos D1 y D2
    const string D1 = "D1.txt";

    vector<string> PalabrasD2; //vectores para almacenar las palabras de D1 y D2
    vector<string> PalabrasD1;
    string line;

    ifstream inputD2(D2); //abre el archivo D2.txt y verifica que se haya abierto correctamente
    if (!inputD2) {
        cerr << "Error: no se pudo abrir " << D2 << "\n";
        return 1;
    }

    while (getline(inputD2, line)) { //lee cada linea de D2.txt
        if (!line.empty() && line.back() == '\r') line.pop_back(); //elimina el enter de Windows si existe
        if (!line.empty()) PalabrasD2.push_back(line); //añade la palabra a palabrasD2 si no está vacía
    }
    inputD2.close(); //cierra el archivo D2.txt

    ifstream inputD1(D1); //mismo proceso pero para D1.txt
    if (!inputD1) {
        cerr << "Error: no se pudo abrir " << D1 << "\n";
        return 1;
    }

    while (getline(inputD1, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (!line.empty()) PalabrasD1.push_back(line);
    }
    inputD1.close();

    vector<uchar*> ArregloD1; //crea un vector uchar* con todas las palabras de D1 convertidas a uchar* para insertar
    ArregloD1.reserve(PalabrasD1.size());
    for (const auto& palabra : PalabrasD1) ArregloD1.push_back(ucharAString(palabra));

    vector<uchar*> D2Buscar; //lo mismo pero para D2 para buscarlas mas tarde
    D2Buscar.reserve(10000);
    for (int i = 0; i < 10000; ++i) D2Buscar.push_back(ucharAString(PalabrasD2[i]));

    vector<uchar*> D2Insertar; //toma las primeras 5000 palabras de D2 y las convierte a uchar* para insertar
    D2Insertar.reserve(5000);
    for (int i = 0; i < 5000; ++i) D2Insertar.push_back(ucharAString(PalabrasD2[i]));
    random_device rd1; //mezcla el vector D2Insertar para que las palabras estén en orden aleatorio
    mt19937 gen1(rd1());
    shuffle(D2Insertar.begin(), D2Insertar.end(), gen1);

    vector<uchar*> D2Eliminar; //lo mismo pero con las ultimas 5000 palabras de D2 para eliminar
    D2Eliminar.reserve(5000);
    size_t totalD2 = PalabrasD2.size();
    for (size_t i = totalD2 - 5000; i < totalD2; ++i) D2Eliminar.push_back(ucharAString(PalabrasD2[i]));
    random_device rd2;
    mt19937 gen2(rd2());
    shuffle(D2Eliminar.begin(), D2Eliminar.end(), gen2);

    cout << "ArregloD1 size: " << ArregloD1.size() << "\n"; //mensajes para verificar que los vectores se crearon correctamente
    cout << "D2Buscar size: " << D2Buscar.size() << "\n";
    cout << "D2Insertar size: " << D2Insertar.size() << "\n";
    cout << "D2Eliminar size: " << D2Eliminar.size() << "\n";
    
    return 0;
}