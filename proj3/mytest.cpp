// CMSC 341 - Fall 2021 - Project 3

/*
  Author: Jason Garcia Solorzano
  File: mytest.cpp
  email: jgarcia8@umbc.edu
  Description: mytest.cpp tests all the efficency of insert & remove functions. 
  It will also attempt to merge a queue with itself, merge queue with 
  different priority functions. The copy contructor, assignment operator
  were also tested. The random class was copied from
  project 2 to insert random orders to test the efficiency of insert & remove
*/
#include "mqueue.h"
#include <random>
using namespace std;
// Priority functions compute an integer priority for an order.  Internal
// computations may be floating point, but must return an integer.


//random class copied from project 2
enum RANDOM {UNIFORM, NORMAL};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORM) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 and standard deviation of 20 
            m_normdist = std::normal_distribution<>(50,20);
        }
        else{
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
    }

    int getRandNum(){
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else{
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//uniform distribution

};

class Tester{
  public:
  //tests to merge queues with different priority functions
  //attempt to merge with queue with self
    void differentQtest();

    //tests the copy constructor
    void testCopyConstructor();
    //tests the assignment operator
    void testAssignmentOp();
    //makes a queue and inserts orders, mergers different queues together
    //then removes the orders from the queue.
    void normalConditions();
    //tests the effiency of the insert function
    void testInsert(int numTrials);
    //tests the getNextOrder(remove) function for effiency
    void testRemove(int numTrials);
};

int priorityFn1(const Order &order);
int priorityFn2(const Order &order);


int main() {

  Tester test;

  {
    test.differentQtest();
  }
  cout << endl;
  {
    test.testCopyConstructor();
  }
  cout << endl;
  {

    test.testAssignmentOp();
  }
  cout << endl;
  {
    //Normal conditions that will insert orders into two queues, 
    //and merge them. Changes priority functions and rebuilds heap.
    //Will dequeue orders
    test.normalConditions();
  }
  cout << endl;
  {
    cout << "Testing insert efficiency: " << endl;
    int N = 5;
    test.testInsert(N);
  }
  cout << endl;
  {
    cout << "Testing Dequeue efficiency: " << endl;
    int N = 5;
    test.testRemove(N);
  }


  return 0;
}

int priorityFn1(const Order & order) {
  //priority value is determined based on some criteria
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  int priority = order.getMaterialAvailability() + order.getQuantity() + order.getArrivalTime();
  return priority;
}

int priorityFn2(const Order & order) {
  //priority is determined by a production manager
  //priority value falls in the range [1-58]
  //the smaller value means the higher priority
  return order.getPMPriority();
}


void Tester::differentQtest(){

  try{
    cout << "Attempting to merge with different priority functions: " << endl;
    MQueue Q1(priorityFn1);
    Order order1("Ford",1,1,1,20);
    Order order2("Honda",5,1,2,21);
    Order order3("Nissan",2,0,0,22);
    Order order4("Chevy",4,3,2,23);

    Q1.insertOrder(order1);
    Q1.insertOrder(order2);
    Q1.insertOrder(order3);
    Q1.insertOrder(order4);

    MQueue Q2(priorityFn2);
    Order order5("Volkswagen",3,1,3,19);
    Q1.mergeWithQueue(Q2);

  }
  catch(std::domain_error &e){
    cout << e.what() << endl;
  }
  //attempting to self-merge
  try{
    cout << "Attempting to Merge with self: " << endl;
    MQueue firstQ(priorityFn2);
    Order order1("McLaren", 2,3,1,21);
    Order order2("Ferrari", 1,3,4,24);

    firstQ.mergeWithQueue(firstQ);
  }
  catch(std::domain_error &e){
    cout << e.what() << endl;
  }

  //attempt to remove from empty queue.
    try{
    cout << "Attempting to remove from empty queue: " << endl;
    MQueue Q1(priorityFn1);

    Q1.getNextOrder(); 

  }
  catch(std::domain_error &e){
    cout << e.what() << endl;
  }
}

//tests copy constructor to make sure that it is a deep copy
void Tester::testCopyConstructor(){

  MQueue queue1(priorityFn1);

  Order order1("Ford",1,1,1,20);
  Order order2("Honda",5,1,2,21);
  Order order3("Toyota",2,0,0,22);
  Order order4("GM",4,3,2,23);

  queue1.insertOrder(order1);
  queue1.insertOrder(order2);
  queue1.insertOrder(order3);
  queue1.insertOrder(order4);

  std::cout << "\nqueue1 after inserting 4 orders: " << std::endl;
  queue1.dump();
  cout << "Contents of queue1: " << endl;
  queue1.printOrderQueue();

  cout << "\nCopy constructor test: " << endl;

  MQueue copy(queue1); //calls the copy constructor
  copy.dump(); //dumping copy
  cout << "Contents of copy: " << endl;
  copy.printOrderQueue();

  cout << "\nAddress of copy: " << &copy << endl;
  cout << "\nAddress of queue1: " << &queue1 << endl;

  cout << "\nTesting Copy Constructor on Empty queue: " << endl;
  try{
    MQueue emtpy(priorityFn2);
    MQueue emtpyCopy(emtpy);

    emtpyCopy.dump();
  }
  catch(std::domain_error &e){
    cout << e.what() << endl;
  }

}

//tests the assignment operator to make sure it performs a deep copy
void Tester::testAssignmentOp(){
  cout << "Assignment Operator test: " << endl;
  MQueue q1(priorityFn2);
  //Order(string customer, int priority, int material, int quantity, int arrival)
  Order order1("BMW",1,1,1,20);
  Order order2("Mercedes",5,1,2,21);
  Order order3("Volkswagen",2,0,0,22);
  Order order4("Fiat",4,3,2,23);

  q1.insertOrder(order1);
  q1.insertOrder(order2);
  q1.insertOrder(order3);
  q1.insertOrder(order4);
  cout << "\nDumping contents of q1: " << endl;
  q1.dump(); 
  cout << "\nContents of q1: " << endl;
  q1.printOrderQueue();

  MQueue copyQ(priorityFn2);
  copyQ = q1; //calls overloaded assingment operator
  cout << "\nDumping contents of copyQ: " << endl;
  copyQ.dump();
  cout << "\nContents of copyQ: " << endl;
  copyQ.printOrderQueue();

  cout << "\nAddress of q1: " << &q1 << endl;
  cout << "\nAddress of copyQ: " << &copyQ << endl;
  cout << endl;

  try{
    cout << "Testing empty queue: " << endl;

    MQueue empty(priorityFn2);
    MQueue empty2(priorityFn2);

    empty = empty2;
    empty.dump();
  }
  catch(std::domain_error &e){
    cout << e.what() << endl;
  }

}

//Normal conditions that will insert orders into two queues, 
//and merge them. Changes priority functions and rebuilds heap.
//Will also remove orders from queue.
void Tester::normalConditions(){

  try{
    MQueue queue1(priorityFn1);
    MQueue queue2(priorityFn1);

    // Create some test orders and insert them into the queue
    //Order(string customer, int priority, int material, int quantity, int arrival)
    Order order1("Ford",1,1,1,20);
    Order order2("Honda",5,1,2,21);
    Order order3("Toyota",2,0,0,22);
    Order order4("GM",4,3,2,23);

    queue1.insertOrder(order3);
    queue1.insertOrder(order1);
    queue1.insertOrder(order2);
    queue1.insertOrder(order4);

    
    std::cout << "\nqueue1 dump after inserting 4 orders (Toyota, Honda, Ford, GM):" << std::endl;
    queue1.dump();
    Order order5("Ferrari",6,3,2,1);
    Order order6("Hyundai",3,0,0,10);

    queue2.insertOrder(order5);
    queue2.insertOrder(order6);

    std::cout << "\nqueue2 dump after inserting 2 students (Ferrari, Hyundai):" << std::endl;
    queue2.dump();

    queue1.mergeWithQueue(queue2);

    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.

    std::cout << std::endl << "Contents of queue1 after merging with queue2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after merging with queue2:\n";
    queue1.dump();

    queue1.setPriorityFn(priorityFn2);

    std::cout << std::endl << "Contents of queue1 after changing the priority function to priorityFn2:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function to priorityFn2:\n";
    queue1.dump();
    
    queue1.setPriorityFn(priorityFn1);

    std::cout << std::endl << "Contents of queue1 after changing the priority function back to priorityFn1:\n";
    queue1.printOrderQueue();
    std::cout << "\nDump queue1 after changing the priority function back to priorityFn1:\n";
    queue1.dump();

    // Remove and print orders in priority order

    std::cout << "\nFinal queue1 in priority order:\n";
      while (queue1.numOrders() > 0) {
      Order order = queue1.getNextOrder();
      // this will be printed in the ascending order (sorted)
      std::cout << "[" << priorityFn1(order) << "] " << order << std::endl; 
    }
  }
    catch(std::domain_error &e){
      cout << e.what() << endl;
  }
}

//will test the amortized efficiency of insert
//amortized = cost of operation(T) / total # of operations(orders * a)
void Tester::testInsert(int numTrials){

  const int a = 2.0;
  double T = 0.0;
  clock_t start, stop;

  int priority, material, quantity, arrival;
  MQueue q(priorityFn2);
  Random priorityGen(HIGHESTPRIO, LOWESTPRIO);
  Random materialGen(0,4);
  Random quantityGen(0,4);
  Random arrivalGen(1, 50);
  int orders = 100000;
  for(int i = 0; i < numTrials - 1; i ++){
    start = clock();
    for(int k = 0; k < orders; k++){
      priority = priorityGen.getRandNum();
      material = materialGen.getRandNum();
      quantity = quantityGen.getRandNum();
      arrival = arrivalGen.getRandNum();
      Order order(DEFAULTCUSTOMER, priority, material, quantity, arrival);
      q.insertOrder(order);
    }
    stop = clock();
    T = stop - start;
    cout << "Orders inserted: " << orders * a << " took "
         << T << " clock ticks (" << T/orders * a << " amortized)!\n";
    orders = orders * a;
  }
}

//will test the amortized efficiency of remove
//amortized = cost of operation(T) / total # of operations(orders * a)
void Tester::testRemove(int numTrials){
  
  const int a = 2.0;
  double T = 0.0;
  clock_t start, stop;

  int priority, material, quantity, arrival;
  MQueue q(priorityFn2);
  Random priorityGen(HIGHESTPRIO, LOWESTPRIO);
  Random materialGen(0,4);
  Random quantityGen(0,4);
  Random arrivalGen(1, 50);
  int orders = 100000;
  for(int i = 0; i < numTrials - 1; i ++){
    for(int k = 0; k < orders; k++){
      priority = priorityGen.getRandNum();
      material = materialGen.getRandNum();
      quantity = quantityGen.getRandNum();
      arrival = arrivalGen.getRandNum();
      Order order(DEFAULTCUSTOMER, priority, material, quantity, arrival);
      q.insertOrder(order);
    }
    start = clock();
    for(int j = 0; j < orders; j++){
      q.getNextOrder();
    }
    stop = clock();

    T = stop - start;
    cout << "Orders removed: " << orders * a << " took "
        //amortized = total cost of operatations / total num of operations
         << T << " clock ticks (" << T/orders * a << " amortized)!\n";
    orders = orders * a;
  }

}