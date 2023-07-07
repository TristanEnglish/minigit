#include <iostream>

#ifndef MINIGIT_HPP
#define MINIGIT_HPP

using namespace std;

struct singlyNode
{
    string fileName; // Name of local file
    string fileVersion; // Name of file in .minigit folder
    int versionNumber; 
    singlyNode * next;
};

struct doublyNode
{
    int commitNumber;
    singlyNode * head;
    doublyNode * previous;
    doublyNode * next;
};

class miniGit
{
    public:
        // member functions
        miniGit();
        void newRepository(); // initializtion, makes a new DLL node with a linked list attached to it
        void addFile(); // add a file to the repository
        void removeFile(); // remove a file from the repository
        singlyNode * deepCopy(singlyNode * crawler);
        void newCommit();
        bool isFileEqual(string filename1, string filename2);
        void checkout(int commitNum);
        doublyNode * head;
        doublyNode * tail;
        doublyNode * currCommit;
    private:
        // private data members

};
#endif