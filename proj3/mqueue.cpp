// CMSC 341 - Fall 2021 - Project 3 - MQueue Class
/*
  Author: Jason Garcia Solorzano
  File: mqueue.cpp
  email: jgarcia8@umbc.edu
  Description: This file contains the implementations of mqueue.h file.
  domain_error is thrown where it will likely cause error.

*/
#include "mqueue.h"
using namespace std;
using std::domain_error;


//Precondition: None
//Postcondition: initialize all member variables
MQueue::MQueue(prifn_t priFn) {
  m_heap = nullptr;
  m_size = 0;
  m_priorFunc = priFn;
}

//Precondition: Queue exists
//Postcondition: will delete and deallocate the nodes from the queue, using 
//the clear() function. Then it will set m_heap to nullptr.
MQueue::~MQueue() {
  clear();
  //m_heap = nullptr;
}

MQueue::MQueue(const MQueue& rhs) {
  //check for when rhs = empty. 
  if(rhs.m_size == 0 || rhs.m_heap == nullptr){
    throw std::domain_error("Cannot copy empty queue");
  }
  else{
    this->m_size = rhs.m_size;
    this->m_priorFunc = rhs.m_priorFunc;
    m_heap = nullptr;
    m_heap = copyHelper(this->m_heap, rhs.m_heap);
  }

}

//helper used to make a deep copy. Used in the copy constructor and 
//assignment operator
Node* MQueue::copyHelper(Node *copyNode, Node* source){
  
  if(source != nullptr){
    copyNode = new Node(*source);
    copyNode->m_left = copyHelper(copyNode->m_left, source->m_left);
    copyNode->m_right = copyHelper(copyNode->m_right, source->m_right);
    return copyNode;
  }

  return nullptr;
}

MQueue& MQueue::operator=(const MQueue& rhs) {
  
  if(rhs.m_size == 0 || rhs.m_heap == nullptr){
    throw std::domain_error("Cannot assign empty queue");
  }
  if(this != &rhs){
    this->m_size = rhs.m_size;
    this->m_priorFunc = rhs.m_priorFunc;
    m_heap = nullptr;
    m_heap = copyHelper(this->m_heap, rhs.m_heap);
  }

  return *this;
}

//Precondition: Queue exists, and orders have been constructed
//Postcondition: will insert the orders into the queueu, using mergerHelper
//function. The size of the heap will be increased for every insertion
void MQueue::insertOrder(const Order& input) {

  Node *newNode = new Node(input);
  m_heap = mergeHelper(m_heap, newNode);
  m_size++; //increase after every insert

}

//removes order from queue. if queue is emtpy then it will throw 
//domain_error.
Order MQueue::getNextOrder() {
  if(m_size == 0 || m_heap == nullptr){
    throw std::domain_error("Nothing to remove!");
  }
  Node *temp = m_heap; //stores heap
  Node *leftSubT = temp->m_left; //left subtree
  Node *rightSubT = temp->m_right; //right subtree
  Order tempOrder = m_heap->getOrder();
  m_heap = mergeHelper(leftSubT, rightSubT);
  delete temp; //delete root
  m_size--;
  return tempOrder;
}


//Precondition: takes in a queue
//Postcondition: will merge two queues into a min-skew-heap
void MQueue::mergeWithQueue(MQueue& rhs) {

  //protects from self-merging
  if(this->m_priorFunc != rhs.m_priorFunc){
    throw std::domain_error("Cannot merge queues with different priority!");
  }
  if(this == &rhs){
    cout << "Cannot merge withh self!" << endl;
  }

  else if(this != &rhs){
    this->m_heap = mergeHelper(this->m_heap, rhs.m_heap);
    this->m_size += rhs.m_size;
    rhs.m_heap = nullptr; //makes rhs empty, will cause segfault if not implemented
    //rhs.clear();
  }
 // m_size += rhs.m_size;
  //rhs.m_heap = nullptr; //makes rhs empty, will cause segfault if not implemented
}

//helper function to mergerWithQueue
//Precondition: Queues exist, ready to be merged
//Postcondition: will merge two queues and maintain min-skew-heap
Node* MQueue::mergeHelper(Node* leftSide, Node* rightSide){

  if(leftSide == nullptr){
    return rightSide;
  }
  else if(rightSide == nullptr){
    return leftSide;
  }
  if(m_priorFunc(leftSide->m_order) >= m_priorFunc(rightSide->m_order)){
    //Node1(leftSide) has lower priority. Perform a swap
    swap(leftSide, rightSide);
  }
  //swap leftSide's left and right sub-trees
  swap(leftSide->m_left, leftSide->m_right);
  //recursively merge the node2(rightSide), replace the left subtree of 
  //leftSide 
  leftSide->m_left = mergeHelper(rightSide, leftSide->m_left);
  return leftSide;
}

//helper function to clear()
//Precondition: Queue exists with orders
//Postcondition: Will go to root's left & right subtrees and delete each node
void MQueue::clear(Node* root){

  if(root != nullptr){
    clear(root->m_left);
    //delete root;
    clear(root->m_right);
    delete root;
    root = nullptr;
  }
}

//uses the clear(Node* root) helper function to deallocate node's of queue
//clear() is called in the destructor
void MQueue::clear() {
  clear(m_heap);
}

//returns the size of the orders in queue
int MQueue::numOrders() const {
  return m_size;
}

//uses printOrderQueueHelper to print the contents of m_heap
//the printOrderHelper prints the orders in pre-order traversal
//root, left, right
void MQueue::printOrderQueue() const {
  printOrderHelper(m_heap);
}

//helper function to printOrderQueue()
//Precondition: A queue exists with data filled in
//Postcondition: prints the contents of each order using format in operator<<
void MQueue::printOrderHelper(Node *root) const{

  if(root != nullptr){
    cout << "[" << m_priorFunc(root->m_order) << "] ";
    cout << root->getOrder() << endl;
    printOrderHelper(root->m_left);
    printOrderHelper(root->m_right);
  }
}

//returns the priority function of the queue
prifn_t MQueue::getPriorityFn() const {
  return m_priorFunc;
}

void MQueue::setPriorityFn(prifn_t priFn) {
  //need to rebuild the heap after changing priority
  m_priorFunc = priFn;
  MQueue newQueue(m_priorFunc); 

  while(m_size > 0){
    //cout << "size: " << m_size;
    Order orderObj = this->getNextOrder();
    newQueue.insertOrder(orderObj);
  }
  m_size = newQueue.m_size;
  m_heap = newQueue.m_heap;
  newQueue.m_heap = nullptr;
}

// overloaded insertion operator for Order class
ostream& operator<<(ostream& sout, const Order& order) {
  sout << "Order: " << order.getCustomer() << ", PM priority: " << order.getPMPriority()
       << ", quantity: " << order.getQuantityValue()
       << ", material availability: " << order.getAvailabilityProbablity() << "%"
       << ", arrival time: " << order.getArrivalTime();
  return sout;
}

// overloaded insertion operator for Node class
ostream& operator<<(ostream& sout, const Node& node) {
  sout << node.getOrder();
  return sout;
}

// for debugging
void MQueue::dump() const {

  if (m_size == 0) {
    cout << "Empty skew heap.\n" ;
  } else {
    dump(m_heap);
    cout << endl;
  }

}

// for debugging
void MQueue::dump(Node *pos) const {
  if ( pos != nullptr ) {
    cout << "(";
    dump(pos->m_left);
    cout << m_priorFunc(pos->m_order) << ":" << pos->m_order.getCustomer();
    dump(pos->m_right);
    cout << ")";
  }
}

