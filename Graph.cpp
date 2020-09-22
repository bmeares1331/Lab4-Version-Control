#include "Graph.h"

#include <iostream>
#include <sstream>

using namespace std;

// fwd declarations
string make_dot(Graph* g);
string what(int& v);

Graph::Graph() {
  // DONE FOR YOU
  directed = false; // graphs are undirected by default
}

Graph::~Graph() {
  // "DONE" FOR YOU
  // no implementation needed
}

vector<shared_ptr<Node>> Graph::getNodes() {
  // DONE FOR YOU
  return nodes;
}

vector<shared_ptr<Edge>> Graph::getEdges() {
  // DONE FOR YOU
  return edges;
}

int Graph::getClock() {
  // DONE FOR YOU
  return clock;
}

void Graph::addNode(shared_ptr<Node> n) {
  // DONE FOR YOU
  nodes.push_back(n);
}

void Graph::addEdge(shared_ptr<Edge> e) {
  // DONE FOR YOU
  edges.push_back(e);
}
 
void Graph::removeNode(shared_ptr<Node> n) {
  // DONE FOR YOU
  for (vector<shared_ptr<Node>>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    if (n == *it) {
      nodes.erase(it);
      break;
    }
  }
}
 
void Graph::removeEdge(shared_ptr<Edge> e) {
  // DONE FOR YOU
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin();
       it != edges.end(); 
       it++) {
    if (e == *it) {
      edges.erase(it);
      break;
    }
  }
}

void Graph::setDirected(bool val) {
  // DONE FOR YOU
  directed = val;
}

bool Graph::isDirected() {
  // DONE FOR YOU
  return directed;
}

set<shared_ptr<Edge>> Graph::getAdjacentEdges(shared_ptr<Node> n) {
  // DONE FOR YOU
  set<shared_ptr<Edge>> ret;
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {
    shared_ptr<Edge> edge = *it;
    if (edge->getStart() == n) {
      ret.insert(edge);
    }
    if (!directed && (edge->getEnd() == n)) {
      ret.insert(edge);
    }
  }
  return ret;
}

void Graph::clear() {
  for (vector<shared_ptr<Node>>::iterator it = nodes.begin(); it != nodes.end(); it++) {
    shared_ptr<Node> node = *it; 
    node->clear();
  }
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {
    shared_ptr<Edge> edge = *it; 
    edge->setType(UNDISCOVERED_EDGE);
  }
  clock = 0; 
}

void Graph::tick(string message) {
  // DONE FOR YOU
  //
  // optional debugging function, disabled with if(false) to prevent
  // unused var warning. Change this however you want.
  //
  // recommended use: call it just after incrementing the clock.
  if (false) {
    cout << "// " << message << endl << make_dot(this) << endl;
  }
}

void Graph::dfs(shared_ptr<Node> start) {
  vector<shared_ptr<Edge>> search;
  start->setColor(2, clock); 
  clock++;
  for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {  
    shared_ptr<Edge> edge;
    edge = *it;
    if (edge->getStart()->getData() == start->getData()) {
      search.push_back(edge);
    }
  }
  for (vector<shared_ptr<Edge>>::iterator it = search.begin(); it != search.end(); it++) { 
    shared_ptr<Edge> edge = *it;
    shared_ptr<Node> a = edge->getEnd();
    a->setPredecessor(edge->getStart());
    if (a->getColor() == 1) {
      edge->setType(10);
      dfs(a);
    }
    else if (a->getColor() == 2) {
      edge->setType(11);
    } 
    else if (a->getColor() == 3 && a->isAncestor(start)) {
      edge->setType(12);
    }
    else if (a->getColor() == 3 && !a->isAncestor(start)) { 
      edge->setType(13);
    }
  }
  start->setColor(3, clock);
  clock++;  
}

void Graph::bfs(shared_ptr<Node> start) {
  vector<shared_ptr<Edge>> search;
  clear(); // clear graph
  start->setColor(2, clock);  //mark start gray
  clock++; //increment clock
  queue<shared_ptr<Node>> queue; //create empty queue
  start->setRank(0);  
  queue.push(start); //add start to queue 
  while (!queue.empty()) {
    shared_ptr<Node> pop = queue.front();
    queue.pop();
    pop->setColor(3, clock); //set node to black
    clock++;
    //create list of edges related to popped node
    for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {  
      shared_ptr<Edge> edge = *it;
      if (edge->getStart()->getData() == pop->getData()) {
        search.push_back(edge);
      }
    }
    for (vector<shared_ptr<Edge>>::iterator it = search.begin(); it != search.end(); it++) { 
      shared_ptr<Edge> edge = *it;
      shared_ptr<Node> a = edge->getEnd();
      a->setPredecessor(edge->getStart());
      if (a->getColor() == 1) { //if node is white
        a->setColor(2, clock); //color it grey
        a->setRank((edge->getStart()->getRank())+1);   
        clock++;
        queue.push(a);
      }
    }
  }
}

void Graph::bfs(shared_ptr<Node> start, shared_ptr<Node> finish) {
  vector<shared_ptr<Edge>> search;
  clear(); // clear graph
  start->setColor(2, clock);  //mark start gray
  clock++; //increment clock
  queue<shared_ptr<Node>> queue; //create empty queue
  start->setRank(0);  
  queue.push(start); //add start to queue 
  while (!queue.empty()) {
    shared_ptr<Node> pop = queue.front();
    queue.pop();
    pop->setColor(3, clock); //set node to black
    clock++;
    //create list of edges related to popped node
    for (vector<shared_ptr<Edge>>::iterator it = edges.begin(); it != edges.end(); it++) {  
      shared_ptr<Edge> edge = *it;
      if (edge->getStart()->getData() == pop->getData()) {
      //if start value is the pop value, set node == end of edge
        shared_ptr<Node> a = edge->getEnd();
        a->setPredecessor(edge->getStart());
        if (a->getData() == finish->getData()) {
          a->setColor(3, clock);
          clock++; 
          a->setRank((edge->getStart()->getRank())+1);
          if (isDirected() == false) {
              a->setRank(1);
          }  
          return;
        }
        else if (a->getColor() == 1) { //if node is white
          a->setColor(2, clock); //color it grey
          if (isDirected() == false) {
            a->setRank(1);
          } 
          else { 
            a->setRank((edge->getStart()->getRank())+1); 
          }
          clock++;
          queue.push(a);
        }
       }
     }
   }
}


// overloading operator << lets you put a Graph object into an output
// stream.
ostream &operator << (ostream& out, Graph graph) {
  // DONE FOR YOU
  out << graph.nodes.size() << " Nodes:" << endl;
  out << "[";
  for (vector<shared_ptr<Node>>::iterator it = graph.nodes.begin(); it != graph.nodes.end(); it++) {
    shared_ptr<Node> n = *it;
    out << *n << ", ";
  }
  out << "]" << endl;
  out << graph.edges.size() << " Edges:" << endl;
  out << "[";
  for (vector<shared_ptr<Edge>>::iterator it = graph.edges.begin(); it != graph.edges.end(); it++) {
    shared_ptr<Edge> e = *it;
    out << *e << ", ";
  }
  out << "]";
  return out;
}

// what is a helper function that turns a constant (e.g. WHITE or
// FORWARD_EDGE) into a string for debugging goodness. 
string what(int& v) {
  // DONE FOR YOU
  string ret = "Unknown";
  if (v == WHITE) {
    ret = "white";
  } else if (v == GRAY) {
    ret = "gray";
  } else if (v == BLACK) {
    ret = "black";
  } else if (v == UNDISCOVERED_EDGE) {
    ret = "undiscovered";
  } else if (v == TREE_EDGE) {
    ret = "tree";
  } else if (v == BACK_EDGE) {
    ret = "back";
  } else if (v == FORWARD_EDGE) {
    ret = "forward";
  } else if (v == CROSS_EDGE) {
    ret = "cross";
  }
  return ret;
}

// make_dot generates a dotfile string for viewing your graph. Have
// tick() print out the results of this function, and then use
// http://www.webgraphviz.com/ (or get a copy of GraphViz) to view the
// result. If you're stuck, this can help you get un-stuck.
string make_dot(Graph* g) {
  // DONE FOR YOU
  stringstream ss;
  vector<shared_ptr<Node>> nodes = g->getNodes();
  vector<shared_ptr<Edge>> edges = g->getEdges();
  string gt = "graph";
  string con = "--";
  if (g->isDirected()) {
    con = "->";
    gt = "digraph";
  }
  ss << gt << " homework {" << endl;
  int c, dt, ft, r;
  string textColor = "black";
  for (auto it=nodes.begin(); it != nodes.end(); ++it) {
    shared_ptr<Node> n = *it;
    n->getDiscoveryInformation(c, dt, ft, r);
    if (c == BLACK) {
      textColor = "white";
    } else {
      textColor = "black";
    }
    ss << "  " << n->getData() << " [ style=\"filled\" fontcolor=\"" << textColor << "\" fillcolor=\"" << what(c) << "\"" << " ]" << endl;
  }
  
  string edgeColor = "black";
  for (auto it=edges.begin(); it != edges.end(); ++it) {
    shared_ptr<Edge> e = *it;
    if (e->getType() == TREE_EDGE) {
      edgeColor = "black";
    } else if (e->getType() == FORWARD_EDGE) {
      edgeColor = "purple";
    } else if (e->getType() == BACK_EDGE) {
      edgeColor = "blue";
    } else if (e->getType() == CROSS_EDGE) {
      edgeColor = "green";
    } else {
      edgeColor = "gray";
    }
    ss << "  " << e->getStart()->getData() << con << e->getEnd()->getData() << " [color=\"" << edgeColor << "\"]" << endl;
  }
  ss << "}" << endl;
  return ss.str();
}