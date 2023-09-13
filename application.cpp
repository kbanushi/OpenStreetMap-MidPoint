// application.cpp <Starter Code>
// <Your name>
//
//
// Adam T Koehler, PhD
// University of Illinois Chicago
// CS 251, Spring 2023
//
// Project Original Variartion By:
// Joe Hummel, PhD
// University of Illinois at Chicago
//
// 
// References:
// TinyXML: https://github.com/leethomason/tinyxml2
// OpenStreetMap: https://www.openstreetmap.org
// OpenStreetMap docs:
//   https://wiki.openstreetmap.org/wiki/Main_Page
//   https://wiki.openstreetmap.org/wiki/Map_Features
//   https://wiki.openstreetmap.org/wiki/Node
//   https://wiki.openstreetmap.org/wiki/Way
//   https://wiki.openstreetmap.org/wiki/Relation
//

#include <iostream>
#include <iomanip>  /*setprecision*/
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "tinyxml2.h"
#include "dist.h"
#include "graph.h"
#include "osm.h"

using namespace std;
using namespace tinyxml2;

struct NodeInfo{
  long long prevID;
  double dist;
};

double Dijkstras(graph<long long, double>& G, long long start, long long end){
  map<long long, NodeInfo> nodes;
  priority_queue<double> pq;
  NodeInfo info;
  info.prevID = 0;
  info.dist = INT_MAX;

  // for (auto& node : G){ FIXME: CANNOT USE GRAPH TO TRAVERSE WITHOUT VALID
  //   nodes[node] = info;
  // }
  //nodes[start] = 0;
}


BuildingInfo* FindBuildingInfo(vector<BuildingInfo>& Buildings, string buildingName){
  for (int i = 0; i < Buildings.size(); i++){
    if (Buildings.at(i).Abbrev == buildingName){
      return &Buildings.at(i);
    }
  }

  for (int i = 0; i < Buildings.size(); i++){
    if (int(Buildings.at(i).Fullname.find(buildingName)) >= 0){
      return &Buildings.at(i);
    }
  }
  return nullptr;
}

BuildingInfo FindClosestBuilding(vector<BuildingInfo>& Buildings, Coordinates coordinate){
  BuildingInfo closest;
  double minDist = numeric_limits<double>::max(), dist;

  for (int i = 0; i < Buildings.size(); i++){
    dist = distBetween2Points(coordinate.Lat, coordinate.Lon, Buildings.at(i).Coords.Lat, Buildings.at(i).Coords.Lon);
    if (dist < minDist){
      closest = Buildings.at(i);
    }
  }
  return closest;
}

long long FindClosestNode(map<long long, Coordinates>& Nodes, Coordinates coordinate){
  double dist, minDist = numeric_limits<double>::max();
  long long closestID = 0;

  for (auto& node : Nodes){
    dist = distBetween2Points(coordinate.Lat, coordinate.Lon, node.second.Lat, node.second.Lon);
    if (dist < minDist){
      minDist = dist;
      closestID = node.first;
    }
  }
  return closestID;
}

void application(
    map<long long, Coordinates>& Nodes, vector<FootwayInfo>& Footways,
    vector<BuildingInfo>& Buildings, graph<long long, double>& G) {
      
    string person1Building, person2Building;
    BuildingInfo* building1, *building2, midBuilding;
    Coordinates midPt;

    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);

  while (person1Building != "#") {
    cout << "Enter person 2's building (partial name or abbreviation)> ";
    getline(cin, person2Building);

    building1 = FindBuildingInfo(Buildings, person1Building);
    building2 = FindBuildingInfo(Buildings, person2Building);

    if (building1 == nullptr){
      cout << "Person 1's building not found" << endl;
      return;
    }
    if (building2 == nullptr){
      cout << "Person 2's building not found" << endl;
      return;
    }

    //FIXME: Potentially not finding center building correctly. Middle building output is the same as "first" building
    midPt = centerBetween2Points(building1->Coords.Lat, building1->Coords.Lon, building2->Coords.Lat, building2->Coords.Lon);
    midBuilding = FindClosestBuilding(Buildings, midPt);

    
    
    cout << building1->Fullname << " " << building2->Fullname << endl; //Seg fault if a building is not found
    cout << midBuilding.Fullname << endl;

    //
    // TO DO: lookup buildings, find nearest start and dest nodes, find center
    // run Dijkstra's alg from each start, output distances and paths to destination:
    //


    
    


    //
    // another navigation?
    //
    cout << endl;
    cout << "Enter person 1's building (partial name or abbreviation), or #> ";
    getline(cin, person1Building);
  }    
}

int main() {
  graph<long long, double> G;

  // maps a Node ID to it's coordinates (lat, lon)
  map<long long, Coordinates>  Nodes;
  // info about each footway, in no particular order
  vector<FootwayInfo>          Footways;
  // info about each building, in no particular order
  vector<BuildingInfo>         Buildings;
  XMLDocument                  xmldoc;

  cout << "** Navigating UIC open street map **" << endl;
  cout << endl;
  cout << std::setprecision(8);

  string def_filename = "map.osm";
  string filename;

  cout << "Enter map filenamge> ";
  getline(cin, filename);

  if (filename == "") {
    filename = def_filename;
  }

  //
  // Load XML-based map file
  //
  if (!LoadOpenStreetMap(filename, xmldoc)) {
    cout << "**Error: unable to load open street map." << endl;
    cout << endl;
    return 0;
  }

  //
  // Read the nodes, which are the various known positions on the map:
  //
  int nodeCount = ReadMapNodes(xmldoc, Nodes);

  //
  // Read the footways, which are the walking paths:
  //
  int footwayCount = ReadFootways(xmldoc, Footways);

  //
  // Read the university buildings:
  //
  int buildingCount = ReadUniversityBuildings(xmldoc, Nodes, Buildings);

  //
  // Stats
  //
  assert(nodeCount == (int)Nodes.size());
  assert(footwayCount == (int)Footways.size());
  assert(buildingCount == (int)Buildings.size());

  cout << endl;
  cout << "# of nodes: " << Nodes.size() << endl;
  cout << "# of footways: " << Footways.size() << endl;
  cout << "# of buildings: " << Buildings.size() << endl;


  for (auto& i : Nodes){
    G.addVertex(i.first);
  }

  double distance;
  for (int i = 0; i < Footways.size(); i++){
    for (int j = 0; j < Footways.at(i).Nodes.size() - 1; j++){
      Coordinates firstCoord = Nodes[Footways.at(i).Nodes.at(j)];
      Coordinates secondCoord = Nodes[Footways.at(i).Nodes.at(j + 1)];
      distance = distBetween2Points(firstCoord.Lat, firstCoord.Lon, secondCoord.Lat, secondCoord.Lon);
      G.addEdge(firstCoord.ID, secondCoord.ID, distance);
      G.addEdge(secondCoord.ID, firstCoord.ID, distance);
    }
  }

  cout << "# of vertices: " << G.NumVertices() << endl;
  cout << "# of edges: " << G.NumEdges() << endl;
  cout << endl;

  // Execute Application
  application(Nodes, Footways, Buildings, G);

  //
  // done:
  //
  cout << "** Done **" << endl;
  return 0;
}
