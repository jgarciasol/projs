//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj1
/*

Title: mytest.cpp
Author: Jason Garcia Solorzano
Course/Section: CMSC 341/Section: 07
email: jgarcia8@umbc.edu
Date: 10/3/21
Description: This file tests the implementations of the function in graph.cpp
It will test for normal cases, will test on an empty graph, tests the
overloaded assignment operator. It will also test for starting node and
ending nodes that do not exist.

*/
#include "graph.h"


class Tester{ // Tester class to implement test functions

    public:

    //Name: normalTestCase()
    //Precondition: none
    //Postcondition: Will create graph from alternative constructor
    //will test a path that exists, will test path from a node to itself
    void normalTestCase();

    //Name: emptyGraph()
    //Precondition: none
    //Postconditon: Will create an empty graph object. Then test a path that
    //does not exist
    void emptyGraph();

    //Name: assignmentOp()
    //Precondition: None
    //Postcondition: Will create a graph from alternative constructor, 
    //then it will copy the contents of linked list to another graph object
    //will display the contents of both linked lists, memory addresses, 
    //and will print out a path
    void assignmentOp();

    //Name: startNodeTest()
    //Precondition: None
    //Postcondition: Will test a path on a starting node that does not exist.
    void startNodeTest();

    //Name: endNodeTest()
    //Precondition: none
    //Postcondition: Will test a path on an end node that does not exists
    void endNodeTest();

    //Name: buildGraphTest()
    //Precondition: None
    //Postcondition: Will create an empty graph, but then use the buildGraph function to test
    //if graph gets populated. 
    void buildGraphTest();
};


int main(){

    Tester test;
    {
       test.normalTestCase();
    }
    cout << endl;
    {
        test.emptyGraph();
    }
    cout << endl;
    {
        test.assignmentOp();
    }
    cout << endl;
    {
        test.startNodeTest();
    }
    cout << endl;
    {
        test.endNodeTest();
    }
    cout << endl;
    {
        test.buildGraphTest();
    }
    return 0;
}


//Name: normalTestCase()
//Precondition: none
//Postcondition: Will create graph from alternative constructor
//will test a path that exists, will test path from a node to itself
void Tester::normalTestCase(){

    Graph aGraph("testdata.txt"); //alternative constructor 
    aGraph.findPath(0,5); //existing path in a graph
    cout << "Finding the path from node 0 to node 5: " << endl;
    aGraph.dump();
    aGraph.findPath(2,2); //path from node to itself
    cout << "Finding the path from node 2 to itself: " << endl;
    aGraph.dump();
    cout << "Finding the path from node 3 to node 5: " << endl;
    aGraph.findPath(3,5); //path does not exists
    aGraph.dump();

}

//Name: emptyGraph()
//Precondition: none
//Postconditon: Will create an empty graph object. Then test a path that
//does not exist
void Tester::emptyGraph(){
    
    Graph bGraph;
    cout << "Finding the path from node 1 to node 14 in an empty Graph object: " << endl;
    bGraph.findPath(1,14);
    bGraph.dump();
}

//Name: assignmentOp()
//Precondition: None
//Postcondition: Will create a graph from alternative constructor,
//then it will copy the contents of linked list to another graph object
//will display the contents of both linked lists, memory addresses,
//and will print out a path
void Tester::assignmentOp(){
    Graph aGraph("testdata.txt"); //alternative constructor
    Graph copyGraph;
    cout << "Testing assignment operator: " << endl;
    copyGraph = aGraph;
    cout << "aGraph path from node 0 to 14" << endl;
    aGraph.findPath(0,14); 
    aGraph.dump();
    cout << "copyGraph path from node 0 to 14" << endl;
    copyGraph.findPath(0,14);
    copyGraph.dump();

    cout << "Address of aGraph: " << &aGraph << endl;
    cout << "Address of copyGraph: " << &copyGraph << endl;

    cout << "Contents of aGraph: " << endl;
    aGraph.printLL();
    cout << "Contents of copyGraph: " << endl;
    copyGraph.printLL();

    Graph aGraph2; //empty graph
    Graph copyGraph2; 

    cout << "Testing = operator on two empty graphs" << endl;

    aGraph2 = copyGraph2; 

    aGraph2.printLL();

}

//Name: startNodeTest()
//Precondition: None
//Postcondition: Will test a path on a starting node that does not exist.

void Tester::startNodeTest(){

    Graph dGraph("testdata.txt");
    cout << "Testing a path for start node that does not exist" << endl;
    dGraph.findPath(-1, 14); 
    dGraph.dump();
}

//Name: endNodeTest()
//Precondition: none
//Postcondition: Will test a path on an end node that does not exists
void Tester::endNodeTest(){

    Graph eGraph("testdata.txt");
    cout << "Testing a path for an end node that does not exist" << endl;
    eGraph.findPath(0, 100);
    eGraph.dump();
}

//Name: buildGraphTest()
//Precondition: None
//Postcondition: Will create an empty graph, but then use the buildGraph function to test
//if graph gets populated.
void Tester::buildGraphTest(){

    Graph fGraph;
    cout << "Testing using buildGraph()" << endl;
    fGraph.buildGraph("testdata.txt");
    

    fGraph.findPath(1, 8);
    cout << "Testing a path from node 1 to node 8" << endl;
    fGraph.dump();
}