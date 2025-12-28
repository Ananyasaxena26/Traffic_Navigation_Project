#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <iomanip>
#include <algorithm>
#include <chrono>
#include <windows.h> 

using namespace std;

// UI Color Codes
const string CLS = "\033[2J\033[1;1H"; 
const string BOLD = "\033[1m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string CYAN = "\033[36m";
const string RESET = "\033[0m";

enum RoadStatus { CLEAR, MODERATE, HEAVY, BLOCKED };

struct Edge {
    int to;
    double baseWeight;
    RoadStatus status;
    double getEffectiveWeight() const {
        if (status == BLOCKED) return 1e9;
        if (status == HEAVY) return baseWeight * 3.5;
        if (status == MODERATE) return baseWeight * 1.8;
        return baseWeight;
    }
};

class NavigationSystem {
    map<int, vector<Edge>> adj;
    map<int, string> cityNodes;

public:
    void addLocation(int id, string name) { cityNodes[id] = name; }
    void addRoad(int u, int v, double dist) {
        adj[u].push_back({v, dist, CLEAR});
        adj[v].push_back({u, dist, CLEAR});
    }

    void updateTraffic(int u, int v, RoadStatus s) {
        for (auto &e : adj[u]) if (e.to == v) e.status = s;
        for (auto &e : adj[v]) if (e.to == u) e.status = s;
    }

    // --- NEW VISUAL MAP FEATURE ---
    void drawMap() {
        cout << BOLD << "\n[ SPATIAL CITY VIEW ]" << RESET << endl;
        cout << "    (1)Airport -------- (4)South_Station" << endl;
        cout << "       |                    ^" << endl;
        cout << "       |                    |" << endl;
        cout << "    (0)Central_Hub ---- (3)East_Gate" << endl;
        cout << "       |                    ^" << endl;
        cout << "       |                    |" << endl;
        cout << "    (2)West_End ------------|" << endl;
        cout << "\n* IDs: 0:Hub, 1:Airport, 2:West, 3:East, 4:South\n";
    }

    void drawHeader() {
        cout << CYAN << "==========================================================" << endl;
        cout << "       CITY TRAFFIC NAVIGATION & CONTROL SYSTEM           " << endl;
        cout << "            (Dijkstra's Algorithm Engine)                 " << endl;
        cout << "==========================================================" << RESET << endl;
    }

    void showNetworkStatus() {
        drawMap();
        cout << BOLD << "\n[ LIVE NETWORK MONITOR ]" << RESET << endl;
        cout << left << setw(15) << "From" << setw(5) << " " << setw(15) << "To" << "Status" << endl;
        cout << "----------------------------------------------------------" << endl;
        for (auto const& pair : adj) {
            for (auto const& e : pair.second) {
                if (pair.first < e.to) {
                    string s = (e.status == CLEAR) ? GREEN + "CLEAR" : 
                               (e.status == MODERATE) ? YELLOW + "MODERATE" :
                               (e.status == HEAVY) ? RED + "HEAVY" : RED + BOLD + "BLOCKED";
                    cout << left << setw(15) << cityNodes[pair.first] << " <-> " 
                         << setw(15) << cityNodes[e.to] << s << RESET << endl;
                }
            }
        }
    }

    void findOptimizedPath(int start, int end) {
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
        map<int, double> dist;
        map<int, int> parent;

        for (auto const& p : cityNodes) dist[p.first] = 1e9;
        dist[start] = 0;
        pq.push({0, start});

        while (!pq.empty()) {
            int u = pq.top().second;
            pq.pop();
            if (u == end) break;
            for (auto &edge : adj[u]) {
                double weight = edge.getEffectiveWeight();
                if (dist[u] + weight < dist[edge.to]) {
                    dist[edge.to] = dist[u] + weight;
                    parent[edge.to] = u;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }

        cout << BLUE << "\n>>> COMPUTING OPTIMAL ROUTE..." << RESET << endl;
        Sleep(600); 

        if (dist[end] >= 1e9) {
            cout << RED << "!! ALERT: NO PATH AVAILABLE !! Road closures detected." << RESET << endl;
        } else {
            vector<int> path;
            int curr = end;
            while (parent.count(curr)) {
                path.push_back(curr);
                curr = parent[curr];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());

            cout << GREEN << "SUCCESS: Route Found." << RESET << endl;
            cout << BOLD << "OPTIMIZED PATH: " << RESET;
            for (size_t i = 0; i < path.size(); ++i) {
                cout << CYAN << BOLD << cityNodes[path[i]] << RESET << (i == path.size() - 1 ? "" : " >> ");
            }
            cout << "\nESTIMATED TRAVEL TIME: " << BOLD << dist[end] << " mins" << RESET << endl;
        }
    }
};

int main() {
    NavigationSystem nav;
    nav.addLocation(0, "Central_Hub"); nav.addLocation(1, "Airport");
    nav.addLocation(2, "West_End"); nav.addLocation(3, "East_Gate");
    nav.addLocation(4, "South_Station");

    // Adding physical connections
    nav.addRoad(0, 1, 15); // Hub to Airport
    nav.addRoad(0, 2, 10); // Hub to West
    nav.addRoad(1, 4, 25); // Airport to South
    nav.addRoad(2, 3, 5);  // West to East
    nav.addRoad(3, 4, 10); // East to South
    nav.addRoad(0, 3, 20); // Hub to East (direct)

    while (true) {
        cout << CLS;
        nav.drawHeader();
        nav.drawMap(); // Show the map on the main menu
        cout << "\n1. VIEW Detailed Network Status" << endl;
        cout << "2. FIND Shortest Path (Navigation)" << endl;
        cout << "3. UPDATE Traffic Intensity (Dynamic)" << endl;
        cout << "4. EXIT" << endl;
        cout << "\nInput: ";
        int choice; 
        if(!(cin >> choice)) break;

        if (choice == 1) {
            cout << CLS;
            nav.showNetworkStatus();
            cout << "\nPress Enter to return to Menu..."; cin.ignore(); cin.get();
        } else if (choice == 2) {
            int s, e;
            cout << "Starting Location ID: "; cin >> s;
            cout << "Destination Location ID: "; cin >> e;
            nav.findOptimizedPath(s, e);
            cout << "\nPress Enter to return..."; cin.ignore(); cin.get();
        } else if (choice == 3) {
            int u, v, st;
            cout << "Enter Node A ID: "; cin >> u;
            cout << "Enter Node B ID: "; cin >> v;
            cout << "Status (0:Clear, 1:Moderate, 2:Heavy, 3:Blocked): "; cin >> st;
            nav.updateTraffic(u, v, (RoadStatus)st);
            cout << GREEN << "\n[SYSTEM] Traffic Update Broadcasted!" << RESET << endl;
            Sleep(1000); 
        } else break;
    }
    return 0;
}