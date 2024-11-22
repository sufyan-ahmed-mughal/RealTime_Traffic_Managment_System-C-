#include <iostream>
#include <vector>
#include <string>
#include <thread>

using namespace std;

// R O A D - C L A S S
class Road {
public:
    int id;
    int start;
    int end;
    int length;
    int capacity;
    int currentCars;
    string signalState; // To track signal state on individual roads

    Road(int id = -1, int start = -1, int end = -1, int length = 0, int capacity = 0) {
        this->id = id;
        this->start = start;
        this->end = end;
        this->length = length;
        this->capacity = capacity;
        this->currentCars = 0;
        this->signalState = "None"; // Default state
    }
};

// I N T E R S E C T I O N - C L A S S
class Signal_Intersection {
public:
    int id;
    bool hasSignal;
    string signalState; // Red or Green

    Signal_Intersection(int id = 0) {
        this->id = id;
        this->hasSignal = false;
        this->signalState = "Red"; // Default state
    }

    void toggleSignal() {
        signalState = (signalState == "Red") ? "Green" : "Red";
    }
};

// M A I N - C L A S S
class RoadNetwork {
public:
    int numIntersections;
    vector<Signal_Intersection> intersections;
    vector<vector<Road>> adjList;

    // Constructor
    RoadNetwork(int numIntersections) : numIntersections(numIntersections) {
        adjList.resize(numIntersections);
        intersections.resize(numIntersections); // Initialize intersections
        for (int i = 0; i < numIntersections; i++) {
            intersections[i] = Signal_Intersection(i);
        }
    }

    // Function to add a road
    void addRoad(int roadId, int from, int to, int length, int capacity) {
        Road road(roadId, from, to, length, capacity);
        adjList[from].push_back(road);
        adjList[to].push_back(Road(roadId, to, from, length, capacity)); // Bidirectional
    }

    // Function to add cars to a road
    void addCarsToRoad(int from, int to, int cars) {
        for (auto &road : adjList[from]) {
            if (road.end == to && road.currentCars + cars <= road.capacity) {
                road.currentCars += cars;
                cout << cars << " cars added to road " << road.id << ".\n";
                return;
            }
        }
        cout << "Unable to add cars. Road does not exist or capacity exceeded.\n";
    }

    // Function to remove cars from a road
    void removeCarsFromRoad(int from, int to, int cars) {
        for (auto &road : adjList[from]) {
            if (road.end == to && road.currentCars >= cars) {
                road.currentCars -= cars;
                cout << cars << " cars removed from road " << road.id << ".\n";
                return;
            }
        }
        cout << "Unable to remove cars. Road does not exist or not enough cars.\n";
    }

    // Function to add a signal to a road
    void addSignal(int from, int to) {
        bool found = false;
        for (auto &road : adjList[from]) {
            if (road.end == to) {
                road.signalState = "Red"; // Initialize signal state on this road
                found = true;
                intersections[from].hasSignal = true; // Mark the intersection as having a signal
                cout << "Signal added on road from " << from << " to " << to << ".\n";
            }
        }
        if (!found) {
            cout << "No road found from " << from << " to " << to << ".\n";
        }
    }

    // Function to display the road network
    void displayNetwork() {
        cout << "\n--- Traffic Network Status ---\n";
        for (int i = 0; i < numIntersections; i++) {
            if (!adjList[i].empty()) {
                cout << "Intersection " << i << "\n";
                for (auto &road : adjList[i]) {
                    cout << "  Road " << road.id << " to intersection " << road.end
                         << " | Length: " << road.length
                         << "m | Capacity: " << road.capacity
                         << " cars | Current cars: " << road.currentCars
                         << " | Signal: " << road.signalState << "\n";
                }
            }
        }
    }
};

// M E N U
void showMenu() {
    cout << "\n--- Traffic Management Menu ---\n";
    cout << "1. Add Road\n";
    cout << "2. Display Road Network\n";
    cout << "3. Add Cars to Road\n";
    cout << "4. Remove Cars from Road\n";
    cout << "5. Add Signal\n";
    cout << "Choose an option: ";
}

int main() {
    int numIntersections;
    cout << "Enter the number of intersections in the road network: ";
    cin >> numIntersections;

    RoadNetwork network(numIntersections); // Initialize network with number of intersections
    int choice;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1: {
            int roadId, from, to, length, capacity;
            cout << "Enter road ID: ";
            cin >> roadId;
            cout << "Enter start intersection: ";
            cin >> from;
            cout << "Enter end intersection: ";
            cin >> to;
            cout << "Enter road length: ";
            cin >> length;
            cout << "Enter road capacity: ";
            cin >> capacity;
            network.addRoad(roadId, from, to, length, capacity);
            cout << "Road added successfully.\n";
            break;
        }
        case 2: {
            network.displayNetwork();
            break;
        }
        case 3: {
            int cars, from, to;
            cout << "Enter start intersection: ";
            cin >> from;
            cout << "Enter end intersection: ";
            cin >> to;
            cout << "Enter cars you want to add: ";
            cin >> cars;
            network.addCarsToRoad(from, to, cars);
            break;
        }
        case 4: {
            int from, to, cars;
            cout << "Enter start intersection: ";
            cin >> from;
            cout << "Enter end intersection: ";
            cin >> to;
            cout << "Enter cars you want to remove: ";
            cin >> cars;
            network.removeCarsFromRoad(from, to, cars);
            break;
        }
        case 5: {
            int fromID, toID;
            cout << "Enter start intersection: ";
            cin >> fromID;
            cout << "Enter end intersection: ";
            cin >> toID;
            network.addSignal(fromID, toID);
            break;
        }
        default:
            cout << "Invalid option. Please try again.\n";
            break;
        }
    }
    return 0;
}
