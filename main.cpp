#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <chrono>
#include <cstdlib>
#include <ctime>

using namespace std;

// Clase para representar una persona
struct Person {
    string nombre;
    string apellido;
    string direccion;
    string adn;
    string ciudad;
    string telefono;

    Person(string n, string a, string d, string adn, string c, string tel)
        : nombre(n), apellido(a),direccion(d), adn(adn), ciudad(c) ,telefono(tel) {}
};

// Clase para representar un grafo
class Graph {
private:
    vector<Person> nodes;                     // Lista de nodos (personas)
    vector<vector<int>> adjacencyMatrix;      // Matriz de adyacencia

public:
    // Cargar personas desde un archivo y crear nodos
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (!file) {
            cerr << "Error al abrir el archivo: " << filename << endl;
            return;
        }

        string nombre, apellido, direccion, adn, ciudad, telefono;
        while (file >> nombre >> apellido >> direccion >> adn >> ciudad >> telefono) {
            nodes.emplace_back(nombre, apellido, direccion, adn, ciudad, telefono);
        }
        file.close();

        // Inicializar matriz de adyacencia
        int size = nodes.size();
        adjacencyMatrix.resize(size, vector<int>(size, 0));
    }

    // Crear conexiones aleatorias entre nodos
    void createRandomConnections(int connectionsPerNode) {
        srand(time(0));
        int size = nodes.size();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < connectionsPerNode; j++) {
                int neighbor = rand() % size;
                if (neighbor != i) {
                    adjacencyMatrix[i][neighbor] = 1;
                    adjacencyMatrix[neighbor][i] = 1; // Grafo no dirigido
                }
            }
        }
    }

    // Búsqueda en anchura (BFS)
    int bfs(const string& targetADN) {
        queue<int> q;
        vector<bool> visited(nodes.size(), false);

        q.push(0);
        visited[0] = true;

        while (!q.empty()) {
            int current = q.front();
            q.pop();

            if (nodes[current].adn == targetADN) {
                return current;
            }

            for (int i = 0; i < nodes.size(); i++) {
                if (adjacencyMatrix[current][i] == 1 && !visited[i]) {
                    q.push(i);
                    visited[i] = true;
                }
            }
        }
        return -1;
    }

    // Búsqueda en profundidad (DFS)
    int dfs(const string& targetADN) {
        stack<int> s;
        vector<bool> visited(nodes.size(), false);

        s.push(0);

        while (!s.empty()) {
            int current = s.top();
            s.pop();

            if (visited[current]) continue;
            visited[current] = true;

            if (nodes[current].adn == targetADN) {
                return current;
            }

            for (int i = 0; i < nodes.size(); i++) {
                if (adjacencyMatrix[current][i] == 1 && !visited[i]) {
                    s.push(i);
                }
            }
        }
        return -1;
    }

    // Mostrar información del nodo encontrado
    void displayPerson(int index) {
        if (index == -1) {
            cout << "Persona no encontrada.\n";
        } else {
            cout << "Persona encontrada:\n";
            cout << "Nombre: " << nodes[index].nombre << "\n";
            cout << "Apellido: " << nodes[index].apellido << "\n";
            cout << "Direccion: " << nodes[index].direccion << "\n";
            cout << "ADN: " << nodes[index].adn << "\n";
            cout << "Ciudad: " << nodes[index].ciudad << "\n";
            cout << "Teléfono: " << nodes[index].telefono << "\n";
        }
    }
};

int main() {
    Graph graph;

    // Cargar datos desde archivo
    string filename = "people.txt";
    graph.loadFromFile(filename);

    // Configurar conexiones aleatorias
    int connectionsPerNode = 3; // Número de conexiones por nodo
    graph.createRandomConnections(connectionsPerNode);

    // Solicitar ADN objetivo
    string targetDNA;
    cout << "Ingrese el ADN a buscar: ";
    cin >> targetDNA;

    // BFS
    auto startBFS = chrono::high_resolution_clock::now();
    int bfsResult = graph.bfs(targetDNA);
    auto endBFS = chrono::high_resolution_clock::now();

    // DFS
    auto startDFS = chrono::high_resolution_clock::now();
    int dfsResult = graph.dfs(targetDNA);
    auto endDFS = chrono::high_resolution_clock::now();

    // Mostrar resultados
    cout << "\n[BFS Resultados]\n";
    graph.displayPerson(bfsResult);
    auto bfsTime = chrono::duration_cast<chrono::microseconds>(endBFS - startBFS).count();
    cout << "Tiempo de ejecución (BFS): " << bfsTime << " microsegundos.\n";

    cout << "\n[DFS Resultados]\n";
    graph.displayPerson(dfsResult);
    auto dfsTime = chrono::duration_cast<chrono::microseconds>(endDFS - startDFS).count();
    cout << "Tiempo de ejecución (DFS): " << dfsTime << " microsegundos.\n";

    // Comparar tiempos
    if (bfsTime < dfsTime) {
        cout << "\nBFS fue más rápido.\n";
    } else {
        cout << "\nDFS fue más rápido.\n";
    }

    return 0;
}
