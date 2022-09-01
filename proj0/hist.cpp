/*
Title: hist.cpp
Author: Jason Garcia Solorzano
Course/Section: CMSC 341/Section: 07
email: jgarcia8@umbc.edu
Date: 9/13/21
Description: hist.cpp holds the implementations from hist.h
Hist overloaded constructor will allocate a Data object so 
that grades can be populated. There is also implementations 
for copy constructor, destructor, and assignment operator
*/

#include "hist.h"

//Name: Default constructor;
//Precondition: None
//Post: A Data object will be created
Data::Data():m_arraySize(DATAARRAYSIZE){
    //m_arraySize = DATAARRAYSIZE
    m_data = new int[DATAARRAYSIZE]; //allocate new memory for m_data size of 10
    //initialize all elements in array to 0
    for(int i = 0; i < DATAARRAYSIZE; i++){
        m_data[i] = 0; 
    }
}

//Name: Destructor;
//Precondition: A Hist object needs to have been created
//Postcondition: Will deallocate any memory that was allocated for Data object
Data::~Data(){
    //use delete keyword to deallocate the memory of the memory allocated object
    delete [] m_data;
    m_data = nullptr;
}

//Name: simulateData(int students)
//Precondition: Hist object which dynamically allocates Data exists
//Postcondition: Simulates the data and populates the indices with the generated scores
//based on the number of students entered in parameter
void Data::simulateData(int students){

     if( students < 0){
        cout << "Cannot have negative grades amount" << endl;
    }
    //used to generate nums from 0-100
    Random gradeGenerator(MINGRADE, MAXGRADE);
    for( int i = 0; i < students; i++){
    
        //saves random grade
        int num = gradeGenerator.getNormRand(); 

        if(num >= 0 && num <= 9){
            m_data[0]++; //counts first index
        }//end of if statement 
        else if(num >= 10 && num <= 19){
                 m_data[1]++;
        }
        else if(num >= 20 && num <= 29){
                 m_data[2]++;
        }
        else if(num >= 30 && num <= 39){
                 m_data[3]++;
        }
        else if(num >= 40 && num <= 49){
                 m_data[4]++;
        }
        else if(num >= 50 && num <= 59){
                 m_data[5]++;
        }
        else if(num >= 60 && num <= 69){
                 m_data[6]++;
        }
        else if(num >= 70 && num <= 79){
                 m_data[7]++;
        }
        else if(num >= 80 && num <= 89){
                 m_data[8]++;
        }
        else if(num >= 90 && num <= 100){
                 m_data[9]++;
        }  

    }//end of for loop


}

//Name: dump(int item)
//Precondition: Data object exists with grades generated
//Postcondition: Prints out the histogram
void Data::dump(int item){
    //draws out the histogram
    cout << "Histogram for item: " << item << endl; 
    int startBucket = 0; //counts buckets
    //loops through size of array
    for(int i = 0; i < DATAARRAYSIZE; i++){
        startBucket = startBucket + 10; 
        cout << startBucket << " ";
        //loops through data items to print out *
        for(int j = 0; j < m_data[i]; j++){
            cout << "*";
        }
     cout << "\n";
    }
}

//Name: getData(int index)
//Precondition: Data object exists with existing grades
//Postcondition: Returns data based on the index that was provided 
int Data::getData(int index){
    return m_data[index];
}

//Name: getArraySize()
//Precondition: Data object exists with grades filled in
//Postcondition: will return the size of the array in Data
int Data::getArraySize(){
    return m_arraySize;
}

//Name: overloaded equality operator
//Precondition: Data object exists with grades
// Overloaded equality operator. It returns true if two objects are the same.
// The "same objects" means they are the same pointers and carry the same value  
// in every correpsonding cell.
// For testing inequality we can simply negate the return value of this operator.
bool operator==(const Data& lhs, const Data& rhs){
    if (&lhs == &rhs)
    {
        for (int i=0;i<lhs.m_arraySize;i++){
            if (lhs.m_data[i] != rhs.m_data[i])
                return false;
        }
        return true;
    }
    else return false;
}

    //Name: Overloaded Constructor
    //Precondition: None
    //Postcondition: Will dynamically allocate a Data object that will be
    //lenght of items that has been passed in.
    //if 0 items are passed in, then will not allocate data
Hist::Hist(int items){

    if(items < 1 ){
        m_items = 0; //sets to 0
        m_table = new Data[m_items];

    }
    else{
        m_items = items; //sets m_items to items
        //array of pointers to arrays
        m_table = new Data[items]; 
        
    }

}

    //Name: Destructor
    //Precondition: A Hist object exists
    //Postcondition: Will delete and deallocate Hist objects.
Hist::~Hist(){


    delete[] m_table; 
    m_table = nullptr; //sets to nullptr
    m_items = 0; //resets to zero; security reasons
}

    //Name: dump()
    //Precondition: Hist object exists with grades filled in
    //Postcondition: will call Data::dump(int items) to print out histogram
void Hist::dump(){
    for (int i=0;i<m_items;i++){
        m_table[i].dump(i);
        cout << endl;
    }
}

//Name: simulateData(int students)
//Precondition: A hist object exists with existing grades
//postcondition: Will call Data::simulateData(int students) that will
//populate every cell of m_table. student parameter indicate the number of grades
void Hist::simulateData(int students){

    if(m_items < 1 ){
    throw 
    std::out_of_range("Error at simulate: number of items is out of range!!");
    } 
    
    for(int i = 0; i < m_items; i++){
        m_table[i].simulateData(students);
    }
}

//Name: Copy Constructor
//Precondition: A Hist object exists to make a copy
//Postcondition: Will make a deep copy of Hist object.
//all values will copied over to new Hist object

Hist::Hist(const Hist & rhs){
    //allocate new memory
    m_table = nullptr;
    m_table = new Data[rhs.m_items];
    m_items = rhs.m_items;
   
    //copies the values;
   
    for(int i = 0; i < m_items; i++){ //loops through num of grades
        //loops through grades in table
        for(int j = 0; j < m_table->getArraySize(); j++){
            //assigns value from rhs to lhs
            m_table[i].m_data[j] = rhs.m_table[i].m_data[j];
        }
    }

}

   //Name: Overloaded assignment operator
    //Precondition: Hist object exists
    //Postcondition: Will copy values of the rhs side of = to lhs
    //A deep copy will be created and all values will be copied over. 
    //Object will also be protected from self-assignment. 
const Hist & Hist::operator=(const Hist & rhs){
    
    //self-assignment check
    if(this != &rhs){
        delete [] m_table; //delete old memory
        m_table = nullptr;
        m_items = rhs.m_items; //copies num of items of rhs to lhs
        //m_table = new Data[rhs.m_table->m_arraySize];
        m_table = new Data[rhs.m_items]; //works
        //m_table = new Data[DATAARRAYSIZE]; //works

        for(int i = 0; i < m_items; i++){ //loops through m_items
            for(int j = 0; j < m_table->getArraySize(); j++){
                m_table[i].m_data[j] = rhs.m_table[i].m_data[j];
            }
        }
    }

    return *this;
}