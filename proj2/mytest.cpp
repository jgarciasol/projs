#include "swarm.h"
#include <random>
/*
Author: Jason Garcia Solorzano
email: jgarcia8@umbc.edu
File: mytest.cpp
Description: this is the tester for swarm.cpp
*/ 


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
    void insertTest(); //tests inserting and removals
    void rightRotTest(); //test right rotations
    void leftRotTest(); //test left rotation
    void leftRightRotTest();
    void rightLeftRotTest();
    void measureInsert(int numTrials, int numBots);
    void measureRemove(int numTrials);
    void RandomInsertsTest();
    void RandomRemovalTest();
};

int main(){
    Tester tester;
    {
        cout << "Measuring efficiency of insert" << endl;
        int N = 10;
        int numBot = 10000;
        tester.measureInsert(N, numBot);
    }
    cout << endl;
    {
        cout << "Measuring efficiency of remove" << endl;
        int N = 10;
       // int numBot = 1000;
        tester.measureRemove(N);
    }
    cout << endl;
    {
        cout << "Testing many inserts" << endl;
        tester.RandomInsertsTest();
    }
    cout << endl;
    {
        cout << "Testing many remoovals" << endl;
        tester.RandomRemovalTest();
    }
    cout << endl;
    {
       cout << "Testing insert & remove: " << endl;
       tester.insertTest(); 
    }

    cout << endl; 

    {
        cout << "Testing right Rotation" << endl;
        tester.rightRotTest();
    }
    cout << endl;
    {
        cout << "Testing left rotation" << endl;
        tester.leftRotTest();
    }
    cout << endl;
    {
        cout << "Testing left-Right rotation" << endl;
        tester.leftRightRotTest();
    }
    cout << endl;
    {
        cout << "Testing right-left rotation" << endl;
        tester.rightLeftRotTest();
    }


    return 0;
}

void Tester::measureInsert(int numTrials, int numBots){

    const int a = 2.0;
    double T = 0.0;
    clock_t start, stop;
    Swarm team;
    int ID = 0;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    for(int i = 0; i < numTrials - 1; i++){
        start = clock();
        for(int k = 0; k < numBots; k++){
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        stop = clock();
        T = stop - start;
        //cout << "Bots inserted: " << numBots << " took "
        cout << "Bots inserted: " << numBots*a << " took "
             << T << " clock ticks (" << T/CLOCKS_PER_SEC << " seconds)!\n"; 
        numBots = numBots * a;
    }

}

void Tester::measureRemove(int numTrials){
    const int a = 2.0;
    double T = 0.0;
    clock_t start, stop;
    Swarm team;
    int ID = 0;
    int size = 1000;
    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    for(int i = 0; i < numTrials - 1; i++){
        for(int k = 0; k < size; k++){
            ID = idGen.getRandNum();
            Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
            team.insert(robot);
        }
        start = clock();
        for(int j = 0; j < size; j++){
            ID = idGen.getRandNum();
            team.remove(ID);
        }
        stop = clock();
        T = stop - start;
        cout << "Bots deleted: " << size*a << " took "
             << T << " clock ticks (" << T/CLOCKS_PER_SEC << " seconds)!\n"; 
        size = size * a;
    }

}

void Tester::insertTest(){

    Swarm team; 
    Robot rob1(5, BIRD, ALIVE);
    Robot rob2(4, BIRD, ALIVE);
    Robot rob3(11, BIRD, DEAD);
    Robot rob4(10, BIRD, ALIVE);
    Robot rob5(7, BIRD, DEAD);
    Robot rob6(2, BIRD, DEAD);
    Robot rob7(12, BIRD, ALIVE);
    Robot rob8(1, BIRD, ALIVE);
    Robot dup(1, DRONE, ALIVE);

    cout << "Inserting bots" << endl;
    team.insert(rob1); //5
    team.insert(rob2); //4
    team.insert(rob3); //11
    team.insert(rob4); //10
    team.insert(rob5); //7
    team.insert(rob6); //2
    team.insert(rob7);  //12
    team.insert(rob8); //1
    team.dumpTree();
    cout << endl;
    team.listRobots();
    cout << endl;
    cout << "Testing duplicate" << endl;
    Robot duplicate(12, BIRD, DEAD);
    if(team.findBot(12) == true){
        cout << "no duplicates allowed" << endl;
    }
    else{
        team.insert(duplicate);
    }

    cout << endl;
    cout << "after removing dead:" << endl;
    team.removeDead();
    team.dumpTree();
    cout << endl;
    team.listRobots();
    cout << endl;

    cout << "After removing bots 1, 10 & 11" << endl;
    team.remove(1);
    team.remove(10);
    team.remove(11);
    team.dumpTree();
    cout << endl;
    team.listRobots();
    cout << endl;

    team.setState(12, DEAD);
    team.listRobots();
    
}

void Tester::rightRotTest(){

    Swarm team;

    Robot rob1(20);
    Robot rob2(15);
    Robot rob3(10);

    team.insert(20);
    team.insert(15);
    team.insert(10);

    team.dumpTree();
    cout << endl;
    team.listRobots();
}

void Tester::leftRotTest(){
    Swarm team; 

    Robot rob1(20);
    Robot rob2(30);
    Robot rob3(40);

    team.insert(rob1);
    team.insert(rob2);
    team.insert(rob3);

    team.dumpTree();
    cout << endl;
    team.listRobots();
}

void Tester::leftRightRotTest(){
    Swarm team; 

    Robot rob1(20);
    Robot rob2(10);
    Robot rob3(15);

    team.insert(rob1);
    team.insert(rob2);
    team.insert(rob3);

    team.dumpTree();
    cout << endl;
    team.listRobots();
    
}

void Tester::rightLeftRotTest(){
    Swarm team; 

    Robot rob1(20);
    Robot rob2(30);
    Robot rob3(25);

    team.insert(rob1);
    team.insert(rob2);
    team.insert(rob3);

    team.dumpTree();
    cout << endl;
    team.listRobots();
}

//inserting many bots and checking balance
void Tester::RandomInsertsTest(){

    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm team;
    int teamSize = 100;
    //int tempID = 0;
    int ID = 0;
    for(int i=0;i<teamSize;i++){
        ID = idGen.getRandNum();
       // if (i == teamSize / 2) tempID = ID;
        Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        if(team.findBot(ID) == true){
            cout << "no duplicates allowed"<< endl;
        }
        else{
            team.insert(robot);
        }

    }
    cout << "checking if tree is balanced" << endl;
    int leftH = team.m_root->getLeft()->getHeight(); 
    int rightH = team.m_root->getRight()->getHeight();

    if(abs(leftH - rightH) <= 1){
        cout << "tree is balanced" << endl;
    }
    else{
        cout << "not balanced" << endl;
    }
    cout << endl;
    cout << "displaying tree" << endl;
    team.dumpTree();
    cout << endl;
    team.listRobots();
}

//removing many bots and checking if balanced
void Tester::RandomRemovalTest(){

    Random idGen(MINID,MAXID);
    Random typeGen(0,4);
    Swarm team;
    int teamSize = 100;
    int ID = 0;
    for(int i=0;i<teamSize;i++){
        ID = idGen.getRandNum();
        Robot robot(ID,static_cast<ROBOTTYPE>(typeGen.getRandNum()));
        if(team.findBot(ID) == true){
            cout << "no duplicates allowed"<< endl;
        }
        else{
            team.insert(robot);
        }
    }

    //removing random robots 
    int counter = 0;
    for(int i = 0; i < teamSize; i++){
        if(counter % 2 == 0){
            team.remove(i);
        }
        counter++;
    }
    cout << "checking if tree is balanced" << endl;
    int leftH = team.m_root->getLeft()->getHeight(); 
    int rightH = team.m_root->getRight()->getHeight();

    if(abs(leftH - rightH) <= 1){
        cout << "tree is balanced" << endl;
    }
    else{
        cout << "not balanced" << endl;
    }
    cout << endl;
    cout << "displaying tree" << endl;
    team.dumpTree();
    cout << endl;
    team.listRobots();

}
