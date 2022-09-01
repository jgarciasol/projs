// CMSC 341 - Fall 2021 - Project 4
#include "hash.h"
#include "math.h"

HashTable::HashTable(unsigned size, hash_fn hash){

    if(isPrime(size) == false){
        size = findNextPrime(size);
    }
    if(size < MINPRIME){
        m_capacity1 = MINPRIME;
        m_capacity2 = 0;
    }
    else if(size > MAXPRIME){ 
        m_capacity1 = MAXPRIME;
        m_capacity2 = 0;
    }
    else{
        m_capacity1 = size;
        m_capacity2 = 0;
    }

    //setting variables
    m_hash = hash;
    m_size1 = 0;
    m_size2 = 0;
    m_newTable = TABLE1;
    m_numDeleted1 = 0;
    m_numDeleted2 = 0;
    m_table1 = new File[m_capacity1];
    
    //intializes all spots to empty in table 1
    for(unsigned int i = 0; i < m_capacity1; i++){
        //all indices in m_table are empty...ready to be inserted with file
        m_table1[i] = EMPTY;
    }

    m_table2 = nullptr;
    m_transfers = 0; //tracks completion of data transfer
    m_newIndex = 0;
    isRehashing = false;
}

HashTable::~HashTable(){

    if(m_newTable == TABLE1){
        delete[] m_table1;//destroys m_table1
        m_table1 = nullptr;
        m_capacity1 = 0;
        m_numDeleted1 = 0;
        m_size1 = 0;

    }
    //delete[] m_table2; 
    //resets all variables for security reasons
    
   if(m_newTable == TABLE2){
        m_size2 = 0;
        m_capacity2 = 0;
        m_numDeleted2 = 0;
        delete [] m_table2;
        m_table2 = nullptr;
   }

   m_hash = 0;
}

File HashTable::getFile(string name, unsigned int diskBlock){

    //look for two tables at one time?
    if(m_table1 != nullptr){
        for(unsigned int i = 0; i < m_capacity1; i++){
            unsigned int index = ((m_hash(m_table1[index].key()) % m_capacity1) + (i*i)) % m_capacity1;
            if(name == m_table1[index].key() && diskBlock == m_table1[index].diskBlock()){
                return m_table1[index]; //returns file 
            }
        }
    }
    
    else if(m_table2 != nullptr){
        for(unsigned int i = 0; i < m_capacity2; i++){
            unsigned int index = ((m_hash(m_table2[index].key()) % m_capacity2) + (i*i)) % m_capacity2;
            if(name == m_table2[index].key() && diskBlock == m_table2[index].diskBlock()){
                return m_table2[index];
            }
        }
    }

    //file could not be found
    return EMPTY; 
}

bool HashTable::insert(File file){
    //insert at table 1

    if(m_newTable == TABLE1){

        for(unsigned int i = 0; i < m_capacity1; i++){
          //index assigned to place file
          unsigned int index = ((m_hash(file.key()) % m_capacity1) + (i*i)) % m_capacity1;
          //if given index is empty, then insert

          if(m_table1[index] == EMPTY){
              //enters file in table at specific index
              m_table1[index] = file;
              m_size1++; //increases number of entries
              if(lambda(TABLE1) > .5){
                 rehash();
                }
              return true; //successful insertion
            }

          //no duplicates
          if(m_table1[index].key() == file.key()
            && m_table1[index].diskBlock() == file.diskBlock()){
                return false; //unsuccessful insertion
            }
        } //end FOR loop
    }
    //inserts to table 2
     else if(m_newTable == TABLE2){


        for(unsigned int i = 0; i < m_capacity2; i++){
          //index assigned to place file
          unsigned int index = ((m_hash(file.key()) % m_capacity2) + (i*i)) % m_capacity2;
          //if given index is empty, then insert

          if(m_table2[index] == EMPTY){
            //enters file in table at specific index
            m_table2[index] = file;
            m_size2++; //increases number of entries
            if(lambda(TABLE2) > .5){
                 rehash();
            }
            return true; //successful insertion
            }
          //no duplicates
          if(m_table2[index].key() == file.key()
            && m_table2[index].diskBlock() == file.diskBlock()){
                return false; //unsuccessful insertion
            }
        }
    }

    return false; //unsuccessful insertion
}

bool HashTable::insertHelper(File file){
    if(m_newTable == TABLE1){

        for(unsigned int i = 0; i < m_capacity1; i++){
          //index assigned to place file
          unsigned int index = ((m_hash(file.key()) % m_capacity1) + (i*i)) % m_capacity1;
          //if given index is empty, then insert

          if(m_table1[index] == EMPTY){
              //enters file in table at specific index
              m_table1[index] = file;
              m_size1++; //increases number of entries
              return true; //successful insertion
            }

          //no duplicates
          if(m_table1[index].key() == file.key()
            && m_table1[index].diskBlock() == file.diskBlock()){
                return false; //unsuccessful insertion
            }
        } //end FOR loop
    }
    //inserts to table 2
     else if(m_newTable == TABLE2){


        for(unsigned int i = 0; i < m_capacity2; i++){
          //index assigned to place file
          unsigned int index = ((m_hash(file.key()) % m_capacity2) + (i*i)) % m_capacity2;
          //if given index is empty, then insert

          if(m_table2[index] == EMPTY){
            //enters file in table at specific index
            m_table2[index] = file;
            m_size2++; //increases number of entries
            return true; //successful insertion
            }
          //no duplicates
          if(m_table2[index].key() == file.key()
            && m_table2[index].diskBlock() == file.diskBlock()){
                return false; //unsuccessful insertion
            }
        }
    }

    return false; //unsuccessful insertion

}

void HashTable::rehash(){
    
    if(m_newTable == TABLE1){
        m_newTable = TABLE2;
        File *oldTable = m_table1;
        unsigned int oldSize = m_size1;
        m_capacity2 = findNextPrime(((m_size1 - m_numDeleted1)*4));
        m_table2 = new File[m_capacity2];
        m_size2 = 0;
        m_numDeleted2 = 0;
        for(unsigned int i = 0; i < m_capacity2; i ++){
            m_table2[i] == EMPTY;
        }

        unsigned int x = 0; //items we have hashed from one table to another
        // unsigned int quarter = m_size1 * .25;
        // while(x < quarter && m_tranfers < 4){
        //     if(!(oldTable[m_newIndex] == EMPTY || oldTable[m_newIndex] == DELETED)){
        //         insertHelper(oldTable[m_newIndex]);
        //         oldTable[m_newIndex] = DELETED;
        //         x++;
        //     }
        //     //cout << "index: " << m_newIndex << " "; 
        //     m_newIndex++; // counter
        // }
        // m_tranfers++;
      //while(m_transfers < 4){
        switch(m_transfers){
        
            case 0: {
                cout << "FIRST" << endl;
                for(int i = 0; x < oldSize * .25; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                      insertHelper(oldTable[i]);
                      x++;
                      oldTable[i] = DELETED;
                      m_newIndex = i;
                      
                    }
                }
             m_transfers++;
             //cout << "transfer: " << m_tranfers << endl;
             break; //end case 0;
            }

            case 1: {
                cout << "SECOND" << endl;
                 //+ 1 from where we left off
                for(int i = m_newIndex + 1; x < oldSize * .5; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end case 1
            }
            case 2:{
                cout << "THIRD" << endl;
                
                for(int i = m_newIndex + 1; x < oldSize * .75; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end of case 2
            }
            case 3:{
                cout << "FOURTH" << endl;
              
                for(int i = m_newIndex + 1; x < oldSize; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end of case 3
           }
        } //end of switch

        //deletes after final transfer
        if(m_transfers == 4){
             delete[] m_table1;
             m_table1 = nullptr;
        }
      //}
 
    }
    

    else if(m_newTable == TABLE2){
        m_newTable = TABLE1;
        File *oldTable = m_table2;
        unsigned int oldSize = m_size2;
        m_capacity1 = findNextPrime(((m_size2 - m_numDeleted2)*4));
        m_table1 = new File[m_capacity1];
        m_size1 = 0;
        m_numDeleted1 = 0;
        for(unsigned int i = 0; i < m_capacity1; i ++){
            m_table1[i] == EMPTY;
        }

        unsigned int x = 0; //items we have hashed from one table to another
        /*unsigned int quarter = m_size2 * .25;
        while(x < quarter){
            if(!(oldTable[m_newIndex] == EMPTY || oldTable[m_newIndex] == DELETED)){
                insertHelper(oldTable[m_newIndex]);
                oldTable[m_newIndex] = DELETED;
                x++;
            }
            //cout << "index: " << m_newIndex << " "; 
            m_newIndex++;
            // counter
        }
        m_tranfers++;*/
      //while(m_transfers < 4){
        switch(m_transfers){

            case 0:{
                cout << "FIRST" << endl;
                for(int i = 0; x < oldSize * .25; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                      insertHelper(oldTable[i]);
                      x++;
                      oldTable[i] = DELETED;
                      m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end case 0;
            }
            case 1: {
                cout << "SECOND" << endl;
                 //+ 1 from where we left off
                for(int i = m_newIndex + 1; x < oldSize * .5; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end case 1
            }
            case 2:{
                cout << "THIRD" << endl;
                
                for(int i = m_newIndex + 1; x < oldSize * .75; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             //m_tranfers++;
             break; //end of case 2
            }
            case 3:{
                cout << "FOURTH" << endl;
              
                for(int i = m_newIndex + 1; x < oldSize; i++){
                    if(!(oldTable[i] == EMPTY || oldTable[i] == DELETED)){
                        insertHelper(oldTable[i]);
                        x++;
                        oldTable[i] = DELETED;
                        m_newIndex = i;
                    }
                }
             m_transfers++;
             break; //end of case 3
            }
        } // end of switch

      //}
        if(m_transfers == 4){
        delete[] m_table2;
        m_table2 = nullptr;
      }
    }
}

bool HashTable::remove(File file){
    //removal for table 1
    if(m_newTable == TABLE1){
        for(unsigned int i = 0; i < m_capacity1; i++){
            //index that will be removed
            unsigned int removeIndex = ((m_hash(file.key()) % m_capacity1) + (i*i)) % m_capacity1;
            //setting that index to DELETED
            if(m_table1[removeIndex].key() == file.key() && 
            m_table1[removeIndex].diskBlock() == file.diskBlock()){
                //sets to deleted
                m_table1[removeIndex] = DELETED;
                m_numDeleted1++;
                //check deleted ratio 
                if(deletedRatio(TABLE1) > .80){
                    rehash();
                }
                return true;
            }
        } //end of for loop

    }
    //removal for table 2
    else if(m_newTable == TABLE2){
        for(unsigned int i = 0; i < m_capacity2; i++){
            //index that will be removed
            unsigned int removeIndex = ((m_hash(file.key()) % m_capacity2) + (i*i)) % m_capacity2;
            //setting that index to DELETED
            if(m_table2[removeIndex].key() == file.key() && 
            m_table2[removeIndex].diskBlock() == file.diskBlock()){
                //sets to deleted
                m_table2[removeIndex] = DELETED;
                m_numDeleted2++;
                //check deleted ratio 
                if(deletedRatio(TABLE2) > .80){
                    rehash();
                }
                return true;
            }
        } //end of for loop
    }

    
    //unsuccessful deletion
    return false;
}

float HashTable::lambda(TABLENAME tablename) const {
    float lambda = 0.0; //load factor of the hash table

    //calculates lambda for table 1
    if(tablename == TABLE1 && m_table1 != nullptr){
        lambda = (float)m_size1/(float)m_capacity1;
    }
    //calculates lambda for table 2
    else if(tablename == TABLE2 && m_table2 != nullptr){
        lambda = (float)m_size2/(float)m_capacity2;
    }
    return lambda;
}

float HashTable::deletedRatio(TABLENAME tableName) const {
    float deleteR = 0.0; //deleted ratio
    if(m_size1 || m_size2 == 0.0){
        return 0.0;
    }
    if(tableName == TABLE1 && m_table1 != nullptr){
        deleteR = (float)m_numDeleted1/(float)m_size1;
    }
    else if(tableName == TABLE2 && m_table2 != nullptr){
        deleteR = (float)m_numDeleted2/(float)m_size2;
    }
    return deleteR;
}

void HashTable::dump() const {
    cout << "Dump for table 1: " << endl;
    if (m_table1 != nullptr)
        for (unsigned int i = 0; i < m_capacity1; i++) {
            cout << "[" << i << "] : " << m_table1[i] << endl;
        }
    cout << "Dump for table 2: " << endl;
    if (m_table2 != nullptr)
        for (unsigned int i = 0; i < m_capacity2; i++) {
            cout << "[" << i << "] : " << m_table2[i] << endl;
        }
}



bool HashTable::isPrime(int number){
    bool result = true;
    for (int i = 2; i <= number / 2; ++i) {
        if (number % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}

int HashTable::findNextPrime(int current){
    //we always stay within the range [MINPRIME-MAXPRIME]
    //the smallest prime starts at MINPRIME
    if (current < MINPRIME) current = MINPRIME-1;
    for (int i=current; i<MAXPRIME; i++) { 
        for (int j=2; j*j<=i; j++) {
            if (i % j == 0) 
                break;
            else if (j+1 > sqrt(i) && i != current) {
                return i;
            }
        }
    }
    //if a user tries to go over MAXPRIME
    return MAXPRIME;
}
