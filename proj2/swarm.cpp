//UMBC - CSEE - CMSC 341 - Fall 2021 - Proj2

/*
Author: Jason Garcia Solorzano
email: jgarcia8@umbc.edu
File: swarm.cpp
Description: All implementations for swarm.h are here
many helper functions were created to assists with remove, insert, removeDead
clear, and many more
*/
#include "swarm.h"
Swarm::Swarm(){

    m_root = nullptr;
}

Swarm::~Swarm(){
    //clear destroys the whole tree
    clear();
}

//a helper function was used to recursively insert
void Swarm::insert(const Robot& robot){

    int botID = robot.getID();
    ROBOTTYPE t = robot.getType(); 
    STATE s = robot.getState();
    m_root = insert(botID, m_root, t, s); //calls the helper function insert
}

//recursive insert
Robot* Swarm::insert(int newBotID, Robot *currBot, ROBOTTYPE t, STATE s){
    //first bot will be inserted and base case
    if(currBot == nullptr){
        currBot = new Robot(newBotID, t, s);
        return currBot;
    }

    if(currBot->getID() == newBotID){
        return currBot;
    }
    //go to left newID is less than ID already in tree  
    if(newBotID < currBot->getID()){
        currBot->m_left = insert(newBotID, currBot->getLeft(), t, s);
    }
    else if(newBotID > currBot->getID()){
        currBot->m_right = insert(newBotID, currBot->getRight(), t, s);

    }

    updateHeight(currBot);
    currBot = rebalance(currBot); //performs the necessary rotations
    return currBot;

}

void Swarm::clear(){

    clear(m_root);
    m_root = nullptr;

}

//deallocates all memory. is called in destructor
void Swarm::clear(Robot *aBot){

    if(aBot == nullptr){
        return; //return nothing
    }
    else{
        clear(aBot->getLeft()); //go to left subtree
        clear(aBot->getRight()); //go to right subtree
        delete aBot;
    }
}

//uses the helper remove to recursively call 
void Swarm::remove(int id){
       remove(m_root, id);
}


//helper to remove(id)
Robot* Swarm::remove(Robot *aBot, int id){
    if(aBot == nullptr){
        return aBot;
    }

    if(id < aBot->getID()){
        aBot->m_left = remove(aBot->getLeft(), id);
    }
    else if(id > aBot->getID()){
        aBot->m_right = remove(aBot->getRight(), id);
    }
    //value was found
    if(id == aBot->getID()){
        //deleting a leaf node; no children
       if(aBot->getLeft() == nullptr && aBot->getRight() == nullptr){
            delete aBot; 
            aBot = nullptr;
        }
        //has 1 child
        //no left child, but has a right child
        else if(aBot->getLeft() == nullptr){
            Robot *deleteBot = aBot;
            aBot = aBot->getRight(); //advance to right child
            delete deleteBot;
        }
        //no right child, but has left child
        else if(aBot->getRight() == nullptr){
            Robot *deleteBot = aBot;
            aBot = aBot->getLeft();
            delete deleteBot;
        }
        //has both a right and left child
        else if(aBot->getLeft() != nullptr && aBot->getRight() != nullptr){
            //find min value of right subtree to preverse BST
            Robot *minBot = findMinBot(aBot->getRight());
            aBot->setID(minBot->getID());
            int deleteID = aBot->getID();
            aBot->m_right = remove(aBot->getRight(), deleteID);
        }
    }
    updateHeight(aBot);
    aBot = rebalance(aBot);
    return aBot;
}

//find minimum value in a BST. used in remove()
Robot* Swarm::findMinBot(Robot *aBot){

    while(aBot->getLeft() != nullptr){
        aBot = aBot->getLeft();
    }
    return aBot;

}

//updates height of tree
void Swarm::updateHeight(Robot* aBot){

    int leftHeight = -1;
    int rightHeight = -1;

    if(aBot == nullptr){
        return; //will return nothing if null. prevents segfault
    }
    
    if(aBot->getRight() == nullptr){
        rightHeight = -1;
    }
    else{
        rightHeight = aBot->getRight()->getHeight();
    }


    if(aBot->getLeft() == nullptr){
        leftHeight = -1; 
    }else{
        leftHeight = aBot->getLeft()->getHeight();
    }
    aBot->setHeight(max(leftHeight, rightHeight) + 1);
}


int Swarm::checkImbalance(Robot* aBot){
    if(aBot == nullptr){
        return 0;
    }
    int lHeight = -1;
    int rHeight = -1; 

    if(aBot->getLeft() != nullptr){
        lHeight = aBot->getLeft()->getHeight();
    }
    else{
        lHeight = -1;
    }
    
    if(aBot->getRight() != nullptr){
        rHeight = aBot->getRight()->getHeight();
    }
   else{
        rHeight = -1;
    }

    return (lHeight - rHeight);
}

//if balance is > 1, then it is left heavy, perform a right rotation
Robot* Swarm::rightRotation(Robot *aBot){

    Robot *newRoot = aBot->getLeft(); //new root will be m_root's left
    Robot *newLeft = newRoot->getRight(); //will be aBot's new left after rotation

    newRoot->setRight(aBot); //aBot will now be new root's right
    aBot->setLeft(newLeft);
    /*                                          newRoot
            5 aBot                              4   
           /                                   /  \
          4 newRoot = aBot's left             3    5 //aBot
         / \null(newLeft)                          /
        3  <-this will carry on with newRoot     null(newLeft); 
    */
    updateHeight(aBot);
    return newRoot; 
}

//if balance is < -1, it is right heavy, perform left rotation
Robot* Swarm::leftRotation(Robot* aBot){

    Robot *newRoot = aBot->getRight(); 
    Robot *newRight = newRoot->getLeft();

    newRoot->setLeft(aBot);
    aBot->setRight(newRight);
    updateHeight(aBot);
    return newRoot;
}

Robot* Swarm::leftRightRotation(Robot *aBot){

    Robot *tempBot = aBot->getLeft();
    Robot *leftR = leftRotation(tempBot);
    //aBot->left = leftRotation(tempBot)
    aBot->setLeft(leftR);
    Robot *newRoot = rightRotation(aBot);
    //aBot = rightRotation(aBot);
    updateHeight(newRoot);
    return newRoot;
}

Robot *Swarm:: rightLeftRotation(Robot *aBot){
    Robot *tempBot = aBot->getRight();
    Robot *rightR = rightRotation(tempBot);

    //aBot->m_right = rightRotation(tempBot);
    aBot->setRight(rightR);

    Robot *newRoot = leftRotation(aBot);
    //aBot = leftRotation(aBot);
    updateHeight(newRoot);
    return newRoot;
}

Robot* Swarm::rebalance(Robot* aBot){
    int balanceFactor = checkImbalance(aBot); 

    //balance > 1; left heavy right rotation
    if(balanceFactor > 1){
        //do double rotation if second robot's height is right heavy
        int secondBF = checkImbalance(aBot->getLeft());
        if(secondBF < 0){
          //  cout << "leftRightRot" << endl;
            Robot *LR = leftRightRotation(aBot);
            return LR; 
        }
        else{
           // cout << "right rotation" << endl;
            Robot *r = rightRotation(aBot);
            return r;
        }
    }
    //right heavy, perform left rotation
    if(balanceFactor < -1){
        //second robot's right height is left heavy
        int secondBF = checkImbalance(aBot->getRight());
        if(secondBF > 0){
           // cout << "RightLeft Rot" << endl;
            Robot *RL = rightLeftRotation(aBot);
            return RL;
        }
        else{
            //cout << "leftRotation" << endl;
            Robot *l = leftRotation(aBot);
            return l; 
        }
    }

    return aBot;
}

void Swarm::listRobots() const {

    listRobotsInOrder(m_root);
}

void Swarm::listRobotsInOrder(Robot *aBot) const{
    //base case
    if(aBot == nullptr){
        return;
    }

    listRobotsInOrder(aBot->getLeft());
    cout << aBot->getID() << ":" << aBot->getStateStr() << ":" << aBot->getTypeStr() << endl;
    listRobotsInOrder(aBot->getRight());
}

bool Swarm::setState(int id, STATE state){

   Robot *foundBot = returnBot(m_root, id);

   if(foundBot != nullptr){
       foundBot->setState(state);
       return true;
   }
    return false; //could not find bot with id
}

void Swarm::removeDead(){
    removeDead(m_root, DEAD);

}

Robot* Swarm::removeDead(Robot *aBot, STATE DEAD){

    if(aBot == nullptr){
        return nullptr; 
    }

    aBot->m_left = removeDead(aBot->getLeft(), DEAD);
    aBot->m_right = removeDead(aBot->getRight(), DEAD);

    if(aBot->getState() == DEAD){
        /*int botToDel = aBot->getID();
        remove(botToDel);*/

        if(aBot->getLeft() == nullptr && aBot->getRight() == nullptr){
            delete aBot;
            aBot = nullptr;
        }
        else if(aBot->getLeft() == nullptr){
            Robot *deleteDead = aBot; 
            aBot = aBot->getRight();
            delete deleteDead;
        }
        else if (aBot->getRight() == nullptr){
            Robot *deleteDead = aBot;
            aBot = aBot->getLeft();
            delete deleteDead;
        }
        //node with two children
        else if(aBot->getLeft() != nullptr && aBot->getRight() != nullptr){
            Robot *minBot = findMinBot(aBot->getRight());
            aBot->setID(minBot->getID());
            aBot->setState(ALIVE);
            STATE deleteDead = minBot->getState();

            aBot->m_right = removeDead(aBot->getRight(), deleteDead);
        }
    }
    updateHeight(aBot);
    aBot = rebalance(aBot);
    return aBot;
}


bool Swarm::findBot(int id) const {

    //Robot *currBot = m_root;
    return findBot(m_root, id);

}

//helper function that will be used to find a bot in the tree
bool Swarm::findBot(Robot *aBot, int value) const{

    //base case
    if (aBot == nullptr) 
        return false;

    //base case
    if(aBot->getID() == value) 
        return true;

    //ID to compare is less than
    if(value < aBot->getID()){
        //traverse to left side of tree
        if (findBot(aBot->getLeft(), value) == true){
            return true;
        }
        else return false;
    }
    else{
        //traverse to right side of tree
       if (findBot(aBot->getRight(), value) == true){
           return true;
       }
       else{
           return false;
       }
    }
}

void Swarm::dumpTree() const {
    dump(m_root);
}

void Swarm::dump(Robot* aBot) const{
    if (aBot != nullptr){
        cout << "(";
        dump(aBot->m_left);//first visit the left child
        cout << aBot->m_id << ":" << aBot->m_height;//second visit the node itself
        dump(aBot->m_right);//third visit the right child
        cout << ")";
    }
}

Robot* Swarm::returnBot(Robot* root, int id){

    if(root == nullptr){
        return nullptr;
    }
   else if(root->getID() == id){
        return root;
    }
    else{
        Robot *botToReturn;
        botToReturn = returnBot(root->getLeft(), id);
        if(botToReturn == nullptr){
            botToReturn = returnBot(root->getRight(), id);
        }
        return botToReturn;
    }

    return nullptr;

}