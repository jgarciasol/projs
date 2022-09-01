/*
Title: graph.cpp
Author: Jason Garcia Solorzano
Course/Section: CMSC 341/Section: 07
email: jgarcia8@umbc.edu
Date: 10/3/21
Description: Implementations of functions from graph.h are written here.
A path in a graph will be attempted, each node as a north, east, south
and west directions
One helper function -- print LL() -- is written to display the contents
of a linked list. Used for testing and comparing values of overloaded assignment operator 

*/
#include "graph.h"

//Name: Graph()
//Precondition: none
//Postcondition: Will create en empty an empty graph
//initialize member variables
Graph::Graph(){
    m_numNodes = 0;
    m_head = nullptr;
    m_dataFile = "";
}

//Name: Graph(string dataFile)
//Precondition: none
//Postcondition: Will create a graph with alternative constructor
//will load data from a file and insert the data into the linked list
Graph::Graph(string dataFile){

    m_numNodes = 0;
    m_head = nullptr;
    m_dataFile = dataFile;
    loadData();

}

//Name: ~Graph()
//Precondition: A graph object exists
//Postcondition: Will call clearGraph() which deletes nodes and deallocate memory
Graph::~Graph(){
    clearGraph();
}

//Name: loadData()
//Precondition: Graph object exists with a file to read
//Postcondition: Will insert the data into the linked list
void Graph::loadData(){
    int numNodes;
    int node, n, e, s, w;
    ifstream dataFile;
    dataFile.open(m_dataFile);
    if (dataFile.is_open()) {
        dataFile >> numNodes;
        m_numNodes = numNodes;
        for (int i = 0; i < numNodes; i++) {
            dataFile >> node >> n >> e >> s >> w;
            insert(node, n, e, s, w);
        }
    }
    else
        //the following statement provides the reason if a file doesn't open
        //please note: not all platforms are providing the same message
        cerr << "Error: " << strerror(errno) << endl;
}

//Name: insert(int node, int n, int e, int s, int w)
//Precondition: Graph object with a file that was successfully open exists
//Postcondition: Will insert the values into nodes and create a linked list
//will also set the correct directionals (n, e, s, w).
void Graph::insert(int node, int n, int e, int s, int w){
    
    //looks for node
    Node *newNode = findNode(node);
    
    //no node exists so a new one will be allocated
    if(newNode == nullptr){
        newNode = new Node(node);
        insertAtHead(newNode);
    }

    //assigns north node pointer to null if value passed in is -1
    if( n == -1){
        newNode->setNorth(nullptr);
    }
    //no node exists in north position, will allocate a new node 
    else{
        if(findNode(n) == nullptr){
            Node *northNode = new Node(n);
            insertAtHead(northNode);
        }
        newNode->setNorth(findNode(n));
    }

    //assigns east node pointer
    if( e == -1){
        newNode->setEast(nullptr);
    }
    else{
        if(findNode(e) == nullptr){
            Node *eastNode = new Node(e);
            insertAtHead(eastNode); 
        }
        newNode->setEast(findNode(e));
    }

    //assigns south node pointer
     if( s == -1){
        newNode->setSouth(nullptr);
    }
    else{
        if(findNode(s) == nullptr){
            Node *southNode = new Node(s);
            insertAtHead(southNode); 
        }
        newNode->setSouth(findNode(s));
    }

    //assigns west node pointer
     if( w == -1){
        newNode->setWest(nullptr);
    }
    else{
        if(findNode(w) == nullptr){
            Node *westNode = new Node(w);
            insertAtHead(westNode); 
        }
        newNode->setWest(findNode(w));
    }
}

//Name: insertAtHead(Node *aNode)
//Precondition: Graph object exists with file being successfully read
//Postcondition: will insert node into the linked list
void Graph::insertAtHead(Node * aNode){
    aNode->setNext(m_head);
    m_head = aNode;
}


//Name: findNode(int nodeValue)
//Precondition: None
//Postcondition: A value will be pased into the function
//will attempt to find the value. If found then the pointer of the 
//node will be returned, otherwise nullptr.
Node * Graph::findNode(int nodeValue){

    Node *currentNode = m_head;
    while (currentNode != nullptr){

        //if node matches then return the value
        if(currentNode->getValue() == nodeValue){
            return currentNode; 
        }
        currentNode = currentNode->getNext(); // points to next node
    }
    return nullptr; //if node is not found
}

//Name: findPath(int start, int end)
//Precondition: Public findPath functions, 
//will clear the results from previous path, will clear visited nodes
//of previous path, then it will attempt to find path
bool Graph::findPath(int start, int end){
    clearResult();
    clearVisited();

    //searches to see if starting value exists
    Node *startingNode = findNode(start);
    Node *endNode = findNode(end);
    if(startingNode == nullptr){
        //could not find starting node
        cout << "Starting node does not exists" << endl;
        return false;
    }
    if(endNode == nullptr){
        cout << "End node does not exist" << endl;
        return false;
    }

    //calls recursive findPath to find the path
   if(findPath(startingNode, end) == true){
       return true;
   }
   else{
       cout << "No path could be found" << endl;
   }

   return false;
}

//Name: findPath(Node* aNode, int end)
//Precondition: Graph exists
//Postcondition: will attempt to find path if starting node exists
//function is a recursive function that will call itself on all 4 directions
bool Graph::findPath(Node* aNode, int end){

    //base case
    if(aNode->getValue() == end){
        m_path.push(aNode->getValue());
        return true;
    }

    if(aNode->getVisited() == false){
        //Once visited, set to true
        aNode->setVisited(true);
    }else{
        return false;
    }


    if(aNode->getNorth() != nullptr){
        //find North's path, will call itself until it finds end node
        if(findPath(aNode->getNorth(), end) == true){
            //push value into the path
            m_path.push(aNode->getValue());
            return true;
        }
    }

    if(aNode->getEast() != nullptr){
        if (findPath(aNode->getEast(), end) == true){
            m_path.push(aNode->getValue());
            return true;
        }
    }

    if(aNode->getSouth() != nullptr){
        if(findPath(aNode->getSouth(), end) == true){
            m_path.push(aNode->getValue());
            return true;
        }
    }

    if(aNode->getWest() != nullptr){
        if(findPath(aNode->getWest(), end) == true){
            m_path.push(aNode->getValue());
            return true;
        }
    }

    return false; //no path could be found
}

//Name: dump()
//Precondition: Graph exists, along with a path
//Postcondition: will display the path found
void Graph::dump(){
    while(!m_path.empty()){
        cout << m_path.top() << " => " ; //displays top of stack
        m_path.pop(); //pops it off
    }
    cout << "END" << endl; 
} 

//Name: clearResult()
//Precondition: a path exists
//Postconditon: will clear the path in m_path
void Graph::clearResult(){
    //clears the whole path
    while(!m_path.empty()){
        m_path.pop();
    }
}

//Name: clearVisited()
//Precondition: A graph exists with a path that have been visited
//Postcondition: will clear the visited of a graph, will set m_visited to false
void Graph::clearVisited(){
    Node *temp = m_head; 
   //sets visited variable to false on all nodes
    while(temp != nullptr){
        temp->setVisited(false); //sets m_visited to false
        temp = temp->getNext();
   }
}

//Name: buildGraph(string file);
//precondition: none
//postcondition: will clear graph and will load m_dataFile with file being
//passed in. 
void Graph::buildGraph(string file){
    clearGraph();
    m_dataFile = file;
    loadData();
}

//Name: clearGraph()
//postcondition: Will clear result, visited, and will delete all nodes
//in the graph. Will reset m_head to nullptr and m_numNodes to 0
void Graph::clearGraph(){
    //clears path
    clearResult();
    //clears visited
    clearVisited();

    //deallocates memory
    Node *temp = m_head; 
    while(temp != nullptr){
        m_head = temp;
        temp = temp->getNext();
        delete m_head; 
    }
    //resets for security
    m_head = nullptr; 
    m_numNodes = 0;
}

//Name: Overloaded assignment operator
//Precondition: Graph object exists
//Postcondition: will perform a deep copy of graph object into another.
//Copying all of the nodes into another linked list
const Graph & Graph::operator=(const Graph & rhs){

    if(this != &rhs){
        delete m_head;
        m_head = nullptr;
        m_numNodes = rhs.m_numNodes;
        m_dataFile = rhs.m_dataFile;
        m_path = rhs.m_path;
        //will allocate new node if rhs.m_head exists
        if(rhs.m_head != nullptr){
            m_head = new Node(rhs.m_head->getValue());
            Node *curr = rhs.m_head; //points to rhs head
            //directionals 
            int node, n, e, s, w;
            //loops through list
            while(curr != nullptr){
                node = curr->getValue();
                if(curr->getNorth() != nullptr){
                    n = curr->getNorth()->getValue();
                }
                else{
                    n = -1;
                }

                if(curr->getEast() != nullptr){
                    e = curr->getEast()->getValue();
                }
                else{
                    e = -1;
                }

                if(curr->getSouth() != nullptr){
                    s = curr->getSouth()->getValue();
                }
                else{
                    s = -1;
                }

                if(curr->getWest() != nullptr){
                    w = curr->getWest()->getValue();
                }
                else{
                    w = -1;
                }
                //inserts the value into the copied graph
                insert(node, n, e, s, w);
                //advances to next node
                curr = curr->getNext();
            }
        }
    }
    return *this;
}

//Name: empty()
//Precondition: none
//Postcondition: none
bool Graph::empty() const		// is the list empty?
    { return m_head == nullptr; }


//prints contents of a linked list. It is a private helper function that I created
void Graph::printLL(){
    Node *temp = m_head; 

    if(temp == nullptr){
        cout << "EMTPY. " << endl;
    }

    while(temp != nullptr){
        cout << temp->getValue() << " => ";
        temp = temp->getNext();
    }
    cout << "END" << endl;
}