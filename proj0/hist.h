#ifndef HIST_H
#define HIST_H
#include <iostream>
#include <iomanip>
#include <random>
using namespace std;
const int MINGRADE = 0;
const int MAXGRADE = 100;
const int DATAARRAYSIZE = 10;
class Hist;//forward declaration
class Grader;//this class is for grading purposes, no need to do anything
class Tester;//this is your tester class, you add your test functions in this class
class Random {
public:
    Random(int min, int max) : m_min(min), m_max(max), 
        m_generator(m_device()),
        //the data set will have the mean of 50 and standard deviation of 20 
        m_normdist(std::normal_distribution<>(50,20)){}

    int getNormRand(){//returns a random number in a set with normal distribution
        //we limit random numbers by the min and max values
        int result = m_min - 1;
        while(result < m_min || result > m_max)
            result = m_normdist(m_generator);
        return result;
    }
    private:
    std::random_device m_device;
    int m_min;
    int m_max;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;
};
class Data{
    public:
    friend class Hist;//Hist has access to private members of Data
    friend class Grader;
    friend class Tester;
    Data();
    ~Data();
    void simulateData(int students);
    void dump(int item);
    int getData(int index);
    int getArraySize();
    friend bool operator==(const Data& lhs, const Data& rhs);
    private:
    int * m_data;
    const int m_arraySize;

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
};
class Hist{
    public:
    friend class Grader;
    friend class Tester;//Tester will have access to private members of Puzzle
    Hist(int items);
    ~Hist();
    Hist(const Hist & rhs);
    const Hist & operator=(const Hist & rhs);
    void simulateData(int students);
    void dump();
    private:
    Data * m_table;//array of pointers to arrays
    int m_items;//number of grading items, e.g. number of quizzes and exams

    // ***************************************************
    // Any private helper functions must be delared here!
    // ***************************************************
};
#endif