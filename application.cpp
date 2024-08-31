#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <algorithm>
#include "tinyxml2.h"
#include "dist.h"
#include "graph.h"
#include "osm.h"

using namespace std;
using namespace tinyxml2;

struct NodeInfo {
    long long prevID;
    double dist;
};

// Dijkstra's algorithm implementation
vector<long long> Dijkstras(graph<long long, double>& G, long long start, long long end) {
    map<long long, NodeInfo> nodes;
    priority_queue<pair<double, long long>, vector<pair<double, long long>>, greater<pair<double, long long>>> pq;

    // Initialize all nodes
    for (const auto& vertex : G.getVertices()) {
        nodes[vertex] = {0, numeric_limits<double>::max()};
    }

    nodes[start].dist = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        long long current = pq.top().second;
        double currentDist = pq.top().first;
        pq.pop();

        if (current == end) break;

        if (currentDist > nodes[current].dist) continue;

        for (const auto& neighbor : G.neighbors(current)) {
            double weight = G.getWeight(current, neighbor);
            double newDist = nodes[current].dist + weight;

            if (newDist < nodes[neighbor].dist) {
                nodes[neighbor].dist = newDist;
                nodes[neighbor].prevID = current;
                pq.push({newDist, neighbor});
            }
        }
    }

    // Reconstruct path
    vector<long long> path;
    for (long long at = end; at != 0; at = nodes[at].prevID) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    return path;
}

BuildingInfo* findBuildingInfo(vector<BuildingInfo>& Buildings, const string& buildingName) {
    auto it = find_if(Buildings.begin(), Buildings.end(), [&buildingName](const BuildingInfo& building) {
        return building.Abbrev == buildingName || building.Fullname.find(buildingName) != string::npos;
    });
    return it != Buildings.end() ? &(*it) : nullptr;
}

BuildingInfo findClosestBuilding(const vector<BuildingInfo>& Buildings, const Coordinates& coordinate) {
    return *min_element(Buildings.begin(), Buildings.end(), [&coordinate](const BuildingInfo& b1, const BuildingInfo& b2) {
        return distBetween2Points(coordinate.Lat, coordinate.Lon, b1.Coords.Lat, b1.Coords.Lon) <
               distBetween2Points(coordinate.Lat, coordinate.Lon, b2.Coords.Lat, b2.Coords.Lon);
    });
}

long long findClosestNode(const map<long long, Coordinates>& Nodes, const Coordinates& coordinate) {
    auto closestNode = min_element(Nodes.begin(), Nodes.end(), [&coordinate](const auto& a, const auto& b) {
        return distBetween2Points(coordinate.Lat, coordinate.Lon, a.second.Lat, a.second.Lon) <
               distBetween2Points(coordinate.Lat, coordinate.Lon, b.second.Lat, b.second.Lon);
    });
    return closestNode->first;
}

void printPath(const vector<long long>& path, const map<long long, Coordinates>& Nodes) {
    cout << "Path: ";
    for (size_t i = 0; i < path.size(); ++i) {
        cout << path[i];
        if (i < path.size() - 1) cout << " -> ";
    }
    cout << endl;

    double totalDistance = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        const auto& coord1 = Nodes.at(path[i]);
        const auto& coord2 = Nodes.at(path[i + 1]);
        totalDistance += distBetween2Points(coord1.Lat, coord1.Lon, coord2.Lat, coord2.Lon);
    }
    cout << "Total distance: " << fixed << setprecision(2) << totalDistance << " miles" << endl;
}

void application(
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings, graph<long long, double>& G) {

    string person1Building, person2Building;

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

    while (person1Building != "#") {
        cout << "Enter person 2's building (partial name or abbreviation)> ";
        getline(cin, person2Building);

        BuildingInfo* building1 = findBuildingInfo(Buildings, person1Building);
        BuildingInfo* building2 = findBuildingInfo(Buildings, person2Building);

        if (building1 == nullptr) {
            cout << "Person 1's building not found" << endl;
        } else if (building2 == nullptr) {
            cout << "Person 2's building not found" << endl;
        } else {
            cout << "Person 1's point: " << building1->Fullname << endl;
            cout << "Person 2's point: " << building2->Fullname << endl;

            Coordinates midPt = centerBetween2Points(building1->Coords.Lat, building1->Coords.Lon, 
                                                     building2->Coords.Lat, building2->Coords.Lon);
            BuildingInfo midBuilding = findClosestBuilding(Buildings, midPt);
            cout << "Destination: " << midBuilding.Fullname << endl;

            long long startNode1 = findClosestNode(Nodes, building1->Coords);
            long long startNode2 = findClosestNode(Nodes, building2->Coords);
            long long destNode = findClosestNode(Nodes, midBuilding.Coords);

            vector<long long> path1 = Dijkstras(G, startNode1, destNode);
            vector<long long> path2 = Dijkstras(G, startNode2, destNode);

            cout << "\nPerson 1's path:" << endl;
            printPath(path1, Nodes);

            cout << "\nPerson 2's path:" << endl;
            printPath(path2, Nodes);
        }

        cout << endl;
        cout << "Enter person 1's building (partial name or abbreviation), or #> ";
        getline(cin, person1Building);
    }
}

int main() {
    graph<long long, double> G;
    map<long long, Coordinates> Nodes;
    vector<FootwayInfo> Footways;
    vector<BuildingInfo> Buildings;
    XMLDocument xmldoc;

    cout << "** Navigating UIC open street map **" << endl << endl;
    cout << setprecision(8);

    string filename;
    cout << "Enter map filename> ";
    getline(cin, filename);
    if (filename.empty()) filename = "map.osm";

    if (!LoadOpenStreetMap(filename, xmldoc)) {
        cout << "**Error: unable to load open street map." << endl;
        return 0;
    }

    int nodeCount = ReadMapNodes(xmldoc, Nodes);
    int footwayCount = ReadFootways(xmldoc, Footways);
    int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

    cout << endl;
    cout << "# of nodes: " << nodeCount << endl;
    cout << "# of footways: " << footwayCount << endl;
    cout << "# of buildings: " << buildingCount << endl;

    for (const auto& node : Nodes) {
        G.addVertex(node.first);
    }

    for (const auto& footway : Footways) {
        for (size_t i = 0; i < footway.Nodes.size() - 1; ++i) {
            long long id1 = footway.Nodes[i], id2 = footway.Nodes[i + 1];
            const auto& coord1 = Nodes[id1], coord2 = Nodes[id2];
            double distance = distBetween2Points(coord1.Lat, coord1.Lon, coord2.Lat, coord2.Lon);
            G.addEdge(id1, id2, distance);
            G.addEdge(id2, id1, distance);
        }
    }

    cout << "# of vertices: " << G.NumVertices() << endl;
    cout << "# of edges: " << G.NumEdges() << endl;
    cout << endl;

    application(Nodes, Footways, Buildings, G);

    cout << "** Done **" << endl;
    return 0;
}