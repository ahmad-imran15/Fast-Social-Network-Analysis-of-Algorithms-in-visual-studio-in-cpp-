#include <iostream>  // Include necessary libraries
#include <fstream>
#include <queue>
#include <climits>
#include <stack>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Class representing a person in the university
class Person {
public:
    string name;
    string role;
    string officeNumber;
    string description;

    // Default constructor
    Person() : name(""), role(""), officeNumber(""), description("") {}

    // Parameterized constructor for students and staff
    Person(string n, string r) : name(n), role(r), officeNumber(""), description("") {}

    // Parameterized constructor for faculty with additional information
    Person(string n, string r, string office, string desc) : name(n), role(r), officeNumber(office), description(desc) {}
};

// Class representing a University Graph
class UniversityGraph {
public:
    // Structure for representing an edge in the graph
    struct Edge {
        int to;     // Destination node
        int weight; // Edge weight

        // Default constructor
        Edge() : to(-1), weight(0) {}

        // Parameterized constructor
        Edge(int t, int w) : to(t), weight(w) {}
    };

    Edge** adjacencyList; 
    //Here craeting 2D array representing the adjacency list
    int numNodes;        
    // Number of nodes in the graph
    Person* people;     
    // this is the Array of people in the university

    // Constructor
    UniversityGraph(int numOfNodes) : numNodes(numOfNodes) {
        // Allocate memory for the adjacency list
        adjacencyList = new Edge * [numNodes];
        for (int i = 0; i < numNodes; ++i) {
            adjacencyList[i] = new Edge[numNodes];
        }

        // Allocate memory for the array of people
        people = new Person[numNodes];
    }

    // Method to add an edge to the graph
    void addEdge(int from, int to, int weight) {
        adjacencyList[from][to] = Edge(to, weight);
    }

    // Destructor to free allocated memory
    ~UniversityGraph() {
        for (int i = 0; i < numNodes; ++i) {
            delete[] adjacencyList[i];
        }
        delete[] adjacencyList;
        delete[] people;
    }

    // Method to generate a DOT file for visualization
    void generateDotFile(string filename) {
        ofstream dotFile(filename);
        if (!dotFile.is_open()) {
            cout << "Error opening file for writing DOT data." << endl;
            return;
        }

        // Write DOT file header
        dotFile << "graph University {" << endl;

        // Write node information
        for (int i = 0; i < numNodes; ++i) {
            dotFile << "  " << i << " [label=\"" << people[i].name;

            // Include additional information for faculty members
            if (people[i].role == "Faculty") {
                dotFile << "\\n" << "Office: " << people[i].officeNumber << "\\n" << "Description: " << people[i].description;
            }

            dotFile << "\"];" << endl;
        }

        // Write edge information
        for (int i = 0; i < numNodes; ++i) {
            for (int j = 0; j < numNodes; ++j) {
                if (adjacencyList[i][j].to != -1 && adjacencyList[i][j].weight != 0) {
                    dotFile << "  " << i << " -- " << j << " [label=\"" << adjacencyList[i][j].weight << "\"];" << endl;
                }
            }
        }

        // Write DOT file footer
        dotFile << "}" << endl;
        dotFile.close();
    }
};

//The function takes pointers to pre-allocated arrays dist and path as parameters.
// // This means that the memory for these arrays should be allocated before calling the function, 
// //and the function will use the provided memory for storing distances and paths. This approach is useful when you want to manage memory outside the function, and the calling code is responsible for memory allocation and deallocation.


// Method to perform Dijkstra's algorithm on the graph
void Dijkstra(UniversityGraph& graph, int startNode, int*& dist, int*& path) {
    // Record the start time for performance measurement
    auto start_time = high_resolution_clock::now();

    // Get the number of nodes in the graph
    int n = graph.numNodes;

    // Priority queue to store nodes with their distances
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // Check if the provided startNode is valid
    if (startNode < 0 || startNode >= graph.numNodes) {
        cout << "Invalid starting node." << endl;
        return;
    }

    // Initialize distance and path arrays
    fill(dist, dist + n, INT_MAX);
    fill(path, path + n, -1);

    //Here Setting the distance of the starting node to 0
    dist[startNode] = 0;

    // Push the starting node and its distance into the priority queue
    pq.push({ 0, startNode });

    // Dijkstra's algorithm main loop
    while (!pq.empty()) {
        // Extract the node with the smallest distance from the priority queue
        int u = pq.top().second;
        pq.pop();

        // Iterate over all adjacent nodes (neighbors) of the current node
        for (int v = 0; v < n; ++v) {
            // Check if there is a valid edge from u to v
            if (graph.adjacencyList[u][v].to != -1 && graph.adjacencyList[u][v].weight != 0) {
                // Calculate the total weight from the starting node to v through u
                int weight = graph.adjacencyList[u][v].weight;

                // Update the distance and path if a shorter path is found
                if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    pq.push({ dist[v], v });
                    path[v] = u;
                }
            }
        }
    }

    // Record the end time for performance measurement
    auto end_time = high_resolution_clock::now();

    // Calculate and display the time taken for the algorithm
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Time taken: " << duration.count() << " milliseconds" << endl;
}

//The function allocates memory dynamically for the dist and path arrays using the new keyword:


// Method to perform Dijkstra's algorithm on the graph
void Dijkstra(UniversityGraph& graph, int startNode) {
    auto start_time = high_resolution_clock::now();

    int n = graph.numNodes;
    int* dist = new int[n];
    int* path =  new int[n];
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
                    cout << " -> ";
                }
            }
            cout << endl;
        }
    }

    // Generate DOT file for visualization
    graph.generateDotFile("university_graph.dot");

    // Free allocated memory
    delete[] dist;
    delete[] path;

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Total Time taken: " << duration.count() << " milliseconds" << endl;
}

// Main function
int main() {
    // Create a university graph with 12 nodes
    UniversityGraph university(12);

    // Add edges to the graph
    // 
    //node 0 to node 1 with a weight of 5.
    university.addEdge(0, 1, 5);
    university.addEdge(0, 2, 3);
    university.addEdge(1, 3, 2);
    university.addEdge(2, 4, 1);
    university.addEdge(1, 5, 4);
    university.addEdge(3, 6, 3);
    university.addEdge(4, 7, 2);
    university.addEdge(5, 8, 3);
    university.addEdge(6, 9, 2);
    university.addEdge(7, 10, 4);
    university.addEdge(8, 11, 1);

    // Set information for each person in the university
    university.people[0] = Person("Ahmad", "Student");
    university.people[1] = Person("Dr Behjat Zuhaira", "Faculty", "Office #205-A Block-C", "Head of Department");
    university.people[2] = Person("Umer", "Student");
    university.people[3] = Person("Amir Rehman", "Staff", "Office #203-C Block-C", "Administrative Assistant");
    university.people[4] = Person("Aiman", "Student");
    university.people[5] = Person("Prof. Usama Imran", "Faculty", "Office A-307 Block-A", "Senior Professor");
    university.people[6] = Person("Farhan", "Staff", "Office #101-a Block-B", "Manager Academics");
    university.people[7] = Person("Rabia", "Student");
    university.people[8] = Person("Dr Ifrah Qaiser", "Faculty", "Office #110-B Block-C", "Assistant Professor");
    university.people[9] = Person("Zubair", "Staff", "Office #503-B Block-C", "Manager Academics Computing");
    university.people[10] = Person("Mrs Amna", "Faculty", "Office #102-B Block-C", "Associate Professor");
    university.people[11] = Person("Malaika", "Staff", "Office #07-B Block-A", "One-stop Office");

    int choice = 0;

    while (choice != 3) {
        cout << "\t1. Display All output\n";
        cout << "\t2. Want to give Start and End point\n";
        cout << "\t3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "\n\n";
            // Specify the starting node for Dijkstra's algorithm
            int startNode = 0;

            // Perform Dijkstra's algorithm on the university graph
            Dijkstra(university, startNode);
            cout << "\n\n";
            break;
        }
        case 2: {
            // Display points for user reference
            cout << "Points:\n";
            for (int i = 0; i < university.numNodes; ++i) {
                cout << i + 1 << " " << university.people[i].name << "\n";
            }

            // Input start and end nodes
            int startnode;
            int endNode;
            cout << "Enter start node (1-" << university.numNodes << "): ";
            cin >> startnode;
            cout << "Enter end node (1-" << university.numNodes << "): ";
            cin >> endNode;

            // Allocate memory for Dijkstra's algorithm arrays
            int* dist = new int[university.numNodes];
            int* path = new int[university.numNodes];

            // Perform Dijkstra's algorithm on the university graph
            Dijkstra(university, startnode - 1, dist, path); // Adjust for 0-based indexing

            // Calculate distance from user input start to end nodes
            cout << "Distance from " << university.people[startnode - 1].name << " to " << university.people[endNode - 1].name << ": ";
            if (startnode < 1 || startnode > university.numNodes || endNode < 1 || endNode > university.numNodes) {
                cout << "Invalid nodes.";
            }
            else {
                cout << dist[endNode - 1];
            }
            cout << endl;

            // Free allocated memory
            delete[] dist; 
            delete[] path;

            // Display time taken
            cout << "\n\n";
            break;
        }
        case 3: {
            cout << "Exiting the program. Goodbye!\n";
            cout << "\n\n";
            break;
        }
        default: {
            cout << "Error.....Please select a valid option\n";
            break;
        }
        }
    }

    return 0;
}
