#include <iostream>
#include <fstream>
#include <queue>
#include <climits>
#include <stack>

using namespace std;

class Person {
public:
    string name;
    string role;

    Person() : name(""), role("") {}  // Default constructor

    Person(string n, string r) : name(n), role(r) {}
};

class UniversityGraph {
public:
    struct Edge {
        int to;
        int weight;

        Edge() : to(-1), weight(0) {}
        Edge(int t, int w) : to(t), weight(w) {}
    };

    Edge** adjacencyList;
    int numNodes;
    Person* people;

    UniversityGraph(int numOfNodes) : numNodes(numOfNodes) {
        adjacencyList = new Edge * [numNodes];
        for (int i = 0; i < numNodes; ++i) {
            adjacencyList[i] = new Edge[numNodes];
        }

        people = new Person[numNodes];
    }

    void addEdge(int from, int to, int weight) {
        adjacencyList[from][to] = Edge(to, weight);
    }

    ~UniversityGraph() {
        for (int i = 0; i < numNodes; ++i) {
            delete[] adjacencyList[i];
        }
        delete[] adjacencyList;
        delete[] people;
    }

    void generateDotFile(string filename) {
        ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            cout << "Error opening file for writing DOT data." << endl;
            return;
        }

        dotFile << "graph University {" << endl;
        for (int i = 0; i < numNodes; ++i) {
            dotFile << "  " << i << " [label=\"" << people[i].name << "\"];" << endl;
        }

        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                if (adjacencyList[i][j].to != -1 && adjacencyList[i][j].weight != 0) {
                    dotFile << "  " << i << " -- " << j << " [label=\"" << adjacencyList[i][j].weight << "\"];" << endl;
                }
            }
        }

        dotFile << "}" << endl;
        dotFile.close();
    }
};

void Dijkstra(UniversityGraph& graph, int startNode) {
    int n = graph.numNodes;
    int* dist = new int[n];
    int* path = new int[n];
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    if (startNode < 0 || startNode >= graph.numNodes) {
        cout << "Invalid starting node." << endl;
        return;
    }

    fill(dist, dist + n, INT_MAX);
    fill(path, path + n, -1);

    dist[startNode] = 0;
    pq.push({ 0, startNode });

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        for (int v = 0; v < n; ++v) {
            if (graph.adjacencyList[u][v].to != -1 && graph.adjacencyList[u][v].weight != 0) {
                int weight = graph.adjacencyList[u][v].weight;

                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({ dist[v], v });
                    path[v] = u;
                }
            }
        }
    }

    // Output the distances and paths
    for (int i = 0; i < n; ++i) {
        cout << "Distance from node " << startNode << " to node " << i << ": ";
        if (dist[i] == INT_MAX) {
            cout << "INFINITY";
        }
        else {
            cout << dist[i];
        }
        cout << endl;

        // Reconstruct and print the path
        if (path[i] != -1) {
            stack<int> pathStack;
            int current = i;
            while (current != -1) {
                pathStack.push(current);
                current = path[current];
            }

            cout << "Path: ";
            while (!pathStack.empty()) {
                cout << graph.people[pathStack.top()].name;
                pathStack.pop();
                if (!pathStack.empty()) {
                    cout << " <- ";
                }
            }
            cout << endl;
        }
    }

    graph.generateDotFile("university_graph.dot");

    delete[] dist;
    delete[] path;
}

int main() {
    UniversityGraph university(5);

    university.addEdge(0, 1, 5);
    university.addEdge(0, 2, 3);
    university.addEdge(1, 3, 2);
    university.addEdge(2, 4, 1);

    university.people[0] = Person("Ammar", "Student");
    university.people[1] = Person("Dr ShahNawaz", "Faculty");
    university.people[2] = Person("ALI", "Student");
    university.people[3] = Person("Arshad", "Staff");

    int startNode = 0;
    Dijkstra(university, startNode);

    return 0;
}
