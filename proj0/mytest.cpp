/*
File: mytest.cpp
Author: Jason Garcia Solorzano
Course/Section: CMSC 341/Section: 07
email: jgarcia8@umbc.edu
Date: 9/13/21
Description: Holds class Tester that implements test functions. 
The test functions measure the efficiency of copy contructor,
efficiency of simulation time. Tests contests of assignment operator,
copy constructor, and makes sure that a deep copy has been done.
*/
#include "hist.h"

class Tester{ // Tester class to implement test functions
    public:

    /*
    Name: testCopyContructor
    Precondition: Tester object exists to run the tests.
    Postcondition: Makes sure that a deep copy is done with 
    copy constructor. 
    */
    bool testCopyConstructor(const Hist& histogram);

    /*
    Name: measureSimulationTime
    Precondition: Test object exists.
    Postcondition: Will measure the time it took for the function to complete 
    measurement will be in system clock clicks.
    */
    void measureSimulationTime(int numTrials, int items, int students);

    /*
    Name: measureCopyConstructor
    Postcondition: None
    Postcondition: Function will measure the efficiency of the copy constructor
    measurements will be in system clock clicks
    */
    void measureCopyConstructor(int numTrials, int items, int students);

    /*
    Name: testingTryCatch
    Precondition: None
    Postcondition: Try-catch statements to check for -1, 0 cases. 
    A out of range exception will be displayed 
    */
    void testingTryCatch(); 

    /*
    Name: testingCopyCons
    Precondition: None
    Postcondition: Will create a Hist object and copy using copy constructor
    contents of both Hist objects will display along with separate memory address
    */
    void testingCopyCons();

    /*
    Name: testingAssignmentOp
    Precondition: None
    Postcondition: Will test the assignment operator to make sure a deep copy 
    was made and contents were copied. Will display different 
    memory address
    */
    void testingAssignmentOp();
};

int main(){
    Tester tester;
    {
        cout << "\nDump of 2 grading items for 35 students:\n" << endl;
        Hist h1(2);
        h1.simulateData(35);
        h1.dump();
    }

    {
        //edge case when creating Hist object with 1 item
        cout << "\nDump of 1 grading item for 40 students: \n" << endl;
        Hist h2(1);
        h2.simulateData(40);
        h2.dump();
    }

    {
        //normal case test 
        cout << "\nDump of 100 grading items for 40 students: \n" << endl;
        Hist h3(100); // 100 items 
        h3.simulateData(40); 
        h3.dump();
    }

    {
        
        tester.testingTryCatch();
    }

   {
        //test deep copy, object with many members
        Hist h1(20);// 20 items, e.g. exams, quizzes, homeworks
        h1.simulateData(400);// process grades for 400 students
        cout << "\nTest case, Copy Constructor: testing for deep copy with 20 items and 400 students:\n" << endl;
        if (tester.testCopyConstructor(h1)) cout << "Copy constructor passed!" << endl;
        else cout << "Copy constructor failed!" << endl;
    }

    {
        tester.testingCopyCons();
    }

    {  
        tester.testingAssignmentOp();
    }

    
    {
        //Measuring the efficiency of insertion functionality
        cout << "\nMeasuring the efficiency of insertion functionality:\n" << endl;
        int M = 4;//number of trials
        int items = 2;//number of items, e.g. exams
        int students = 100000;//original input size, number of students taking tests
        tester.measureSimulationTime(M, items, students);
    }

    {
        //Measure the efficiency of copy constructor
        cout << "\nMeasuring the efficiency of copy constructor: Pending..." << endl;
        int N = 4; // number of trials
        int items = 1000; // number of items
        int students = 10000; //num students 
        tester.measureCopyConstructor(N, items, students); 
    }
    cout << endl;
    cout << "Goodbye!" << endl;
    return 0;
}

/*
Name: testCopyContructor
Precondition: Tester object exists to run the tests.
Postcondition: Makes sure that a deep copy is done with 
copy constructor. 
*/
bool Tester::testCopyConstructor(const Hist& histogram){
    Hist aCopy(histogram);
    //the case of empty objects
    if (histogram.m_items == 0 && aCopy.m_items == 0) return true;
    //the case that number of items is the same and the table pointers are not the same
    else if (histogram.m_items == aCopy.m_items && histogram.m_table != aCopy.m_table){
        for (int i=0;i<histogram.m_items;i++){
            // check whether every Data object is a deep copy or not
            if (histogram.m_table[i] == aCopy.m_table[i])
                //if they are the same objects, then is not a deep copy
                return false;
        }
        return true;
    }
    //everthing else
    else return false;
}

/*
Name: measureSimulationTime
Precondition: Test object exists.
Postcondition: Will measure the time it took for the function to complete 
measurement will be in system clock clicks.
*/
void Tester::measureSimulationTime(int numTrials, int items, int students)
{
    // Measuring the efficiency of data simulation algorithm with system clock ticks
    // Clock ticks are units of time of a constant but system-specific length, 
    //  as those returned by function clock().
    // Dividing a count of clock ticks by CLOCKS_PER_SEC yields the number of seconds.
    const int a = 2;//scaling factor for input size, everytime it will double
    double T = 0.0;//to store running times
    clock_t start, stop;//stores the clock ticks while running the program
    for (int k=0;k<numTrials-1;k++)
    {
        Hist h1(items);
        start = clock();
        h1.simulateData(students);//the algorithm to be analyzed for efficiency
        stop = clock();
        T = stop - start;//number of clock ticks the algorithm took
        cout << "Counting " << items * students << " grades took " << T << " clock ticks ("<< T/CLOCKS_PER_SEC << " seconds)!" << endl;
        students = students * a;//increase number of students by increase factor
    }
}

/*
Name: measureCopyConstructor
Postcondition: None
Postcondition: Function will measure the efficiency of the copy constructor
measurements will be in system clock clicks
*/
void Tester::measureCopyConstructor(int numTrials, int items, int students){
    const int a = 2; 
    double T = 0.0; 
    clock_t start, stop; //stores the clock ticks while running program
    for(int j = 0; j < numTrials - 1; j++){
        Hist h1(items); //hist object
        h1.simulateData(students);
        start = clock();
        Hist h2(h1); //passing a histogram object measuring the copy constructor
        stop = clock();
        T = stop - start; //subtract ending time from start time to count # clock ticks
        cout << "This is the measurement of efficiency of copy contructor: " 
             << items * students << " grades took " << T << " ticks (" << T/CLOCKS_PER_SEC << " seconds)!" << endl;

        items = items * a;
    }

}

/*
Name: testingTryCatch
Precondition: None
Postcondition: Try-catch statements to check for -1, 0 cases. 
A out of range exception will be displayed 
*/
void Tester::testingTryCatch(){

    try{
        cout << "\nTesting exception case for number of items less than 1:\n" << endl;
            Hist h1(0); //edge case
            h1.simulateData(40);
    }
    catch (std::out_of_range &e){
            cout << e.what() << endl;
    }
    try{
    cout <<"\nTesting error case with items -1:\n" <<endl;
            Hist his2(-1); //error case
            his2.simulateData(40);
    }
    catch (std::out_of_range &f){
            cout << f.what() << endl;
    }
}

/*
Name: testingCopyCons
Precondition: None
Postcondition: Will create a Hist object and copy using copy constructor
contents of both Hist objects will display along with separate memory address
*/
void Tester::testingCopyCons(){

    cout << "Testing the Copy Constructor contents: " << endl;
    Hist original(2); 
    original.simulateData(10);
    Hist copy(original); 
    cout << "Address of original: " << &original << endl;
    cout << "Address of copy: " << &copy << endl;
    cout << "Original: " <<endl; 
    original.dump();
    cout << "copy: "  <<endl;
    copy.dump();

    try{ 
        Hist zero(0); //edge case
        zero.simulateData(10);
        Hist anotherCopy(zero);
    }
    catch(std::out_of_range &a){
        cout << a.what() << endl;
    }
}

/*
Name: testingAssignmentOp
Precondition: None
Postcondition: Will test the assignment operator to make sure a deep copy 
was made and contents were copied. Will display different 
memory address
*/
void Tester::testingAssignmentOp(){
    cout << "Assignment operator = " << endl;
    Hist OG(2);
    OG.simulateData(10); //simulate with 10 students
    cout << "Contents of OG: " << endl;
    OG.dump();
    Hist OG2 = OG; 
    cout << "Contents of OG2: " << endl;
    OG2.dump();

    cout << "OG address: " << &OG << endl; 
    cout << "OG2 Address: " << &OG2 << endl;
    cout << endl;

    cout << "Self assignment: " << endl;
    Hist self(2); 
    self.simulateData(10);
    self = self; 
    cout << "address of self " << &self;
}