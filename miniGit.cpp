#include "miniGit.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

miniGit::miniGit()
{
    // basic constructor
}



void miniGit::newRepository()
{
    fs::remove_all(".minigit"); 
    fs::create_directory(".minigit"); // create a new directory
    doublyNode * newNode = new doublyNode;
    head = newNode;
    tail = newNode;
    currCommit = newNode;
    newNode->commitNumber = 0;
    newNode->next = nullptr;
    newNode->previous = nullptr;
    newNode->head = nullptr;
}

void miniGit::addFile()
{
    /*
    1. Prompt user to enter a file name.
    2. Check whether the file with the given name exists in the current directory. If not, keep
    prompting the user to enter a valid file name.
    3. The SLL is checked to see whether the file has already been added. A file by the same
    name cannot be added twice.
    4. A new SLL node gets added containing the name of the input file, name of the repository file, as well as a pointer to the next node. The repository file name should be
    the combination of the original file name and the version number. For example, if user
    file help.txt is added, the new file to be saved in the .minigit repository should be
    named help00.txt, where 00 is the version number. (The initial file version should
    be 00.)
    */

   // Ask the user to enter the name of the file
   string filename;
   cout << "Please enter a file name: ";
   cin >> filename;

    // Try to open the file
   ifstream myFile;
   myFile.open(filename);

    // If we can't open the file, we couldn't find it in the directory. Ask for input until we get a valid one
   while (!myFile.is_open())
   {
        cout << "File not found in current directory. Please enter a valid file name: ";
        cin >> filename;
        myFile.open(filename);
   }
    // Get a pointer to the head of the singly linked list
    singlyNode * crawler = currCommit->head ;

    // Traverse the list until we reach the end or find the filename
    while (crawler != nullptr && crawler->fileName != filename)
    {
        crawler = crawler->next;
    }
    // If we found that filename in the linked list, the file has already been added, return.
    if (crawler != nullptr)
    {
        cout << "Error. File has already been added." << endl;
        return;
    }
    // Otherwise, if the file is not in the linked list,
    else
    {
        // Insert a new node with that file name at the tail of the linked list
        // Case 1: empty list (new head)
        if (currCommit->head == nullptr)
        {
            // Create the node
            singlyNode * newNode = new singlyNode;
            newNode->fileName = filename;
            newNode->next = nullptr;
            newNode->fileVersion = filename + "_0";
            newNode->versionNumber = 0;
            currCommit->head = newNode;
        }
        // Case 2: anywhere else
        else
        {
            // Start at the head
            crawler = currCommit->head;
            // Get a pointer to just before the end of the list
            while (crawler->next != nullptr)
            {
                crawler = crawler->next;
            }
            // Create the node at the tail
            singlyNode * newNode = new singlyNode;
            crawler->next = newNode;
            newNode->fileName = filename;
            newNode->next = nullptr;
            newNode->fileVersion = filename + "_0";
            newNode->versionNumber = 0;
        }   
    }
    myFile.close();
}


void miniGit::removeFile()
{
    /*
    1. Prompt user to enter a file name.
    2. Check the SLL for whether the file exists in the current version of the repository.
    3. If found, delete the SLL node.
    */

   // Ask the user to enter a file name
   string filename;
   cout << "Please enter the name of the file you wish to delete: ";
   cin >> filename;

    // Search for the file in the current directory (we will use a hash function for this later)
   singlyNode *crawler = currCommit->head;
   while (crawler != nullptr && crawler->fileName != filename)
   {
       crawler = crawler->next;
   }

    // If we couldn't find the file, tell the user
   if (crawler == nullptr)
   {
       cout << "Error. File not found in current directory." << endl;
       return;
   }
   // Otherwise, if we did find the file, delete it
   else
   {
       // delete the node
       // Case 1: Deleting the head
       if (crawler == currCommit->head)
       {
           singlyNode * temp = currCommit->head;
           currCommit->head = temp->next;
           delete temp;
           temp = nullptr;
       }
       // Case 2: Anywhere else
       else
       {
           // Get a pointer to the node before the node we want to delete
           singlyNode * prev = currCommit->head;

           while (prev->next != crawler)
           {
               prev = prev->next;
           }

            // Set the previous node to skip over the node we want to delete
            // Delete the node we want to delete
            prev->next = crawler->next;
            delete crawler;
            crawler = nullptr;
        }
   }
}


bool miniGit::isFileEqual(string filename1, string filename2)
{ 
    // Open the first file
    ifstream myFile;
    myFile.open(filename1);
    if(!myFile.is_open())
    {
        cout << "Error reading file" << endl;
        return false;
    }

    // Open the second file
    ifstream myFile2;
    myFile2.open(filename2);
    if(!myFile2.is_open())
    {
        cout << "Error reading file 2" << endl;
        return false;
    }


    string line;
    string file1;
    string file2;
    // Read the first file into one string
    while(getline(myFile, line))
    {
        file1 += line;
    } 

    // Read the second file into another string
    while(getline(myFile2, line))
    {
        file2 += line;
    }

    // Check for equivalence
    // If equivalent, return true
    if (file1 == file2)
    {
        myFile.close();
        myFile2.close();
        return true;
    }
    else
    {
        myFile.close();
        myFile2.close();
        return false;
    }
}


singlyNode * miniGit::deepCopy(singlyNode * crawler)
{
    if (crawler == nullptr)
    {
        return nullptr;
    }

    singlyNode * newNode = new singlyNode;
    newNode->fileName = crawler->fileName;
    newNode->fileVersion = crawler->fileVersion;
    newNode->versionNumber = crawler->versionNumber;
    newNode->next = deepCopy(crawler->next);

    return newNode;
}


void miniGit::newCommit()
{
    /*
    1. The current SLL should be traversed in its entirety, and for every node

        (a) Check whether the corresponding fileVersion file exists in .minigit directory.
        If the fileVersion file DOES NOT EXIST, copy the file from the current directory
        into the .minigit directory. The newly copied file should get the name from the
        node’s fileVersion member. (Note: this will only be the case when a file is
        added to the repository for the first time.)

        (b) If the fileVersion file DOES EXIST in .minigit, check whether the current directory file has been changed 
        (i.e. has it been changed by the user?) with respect to
        the fileVersion file. (To do the comparison, you can read in the file from the
        current directory into one string and read in the file from the .minigit directory
        into another string, and check for equality.)1 Based on the comparison result, do
        the following:

            • File is unchanged: do nothing.
            • File is changed: copy the file from the current directory to the .minigit
            directory, and give it a name with the incremented version number. Also,
            update the SLL node member fileVersion to the incremented name.

    2. Once all the files have been scanned, the final step of the commit will create a new
        Doubly Linked List node of the repository. An exact (deep) copy of the SLL from the
        previous node shall be copied into the new DLL node. The commit number of the new
        DLL node will be the previous nodes commit number incremented by one.
    */

    singlyNode * crawler = currCommit->head;
    string line;
    string s;
    string filename;
    ifstream myFile;
    ofstream writeFile;

    // Traverse the entire list, and for every node:
    while (crawler != nullptr)
    {
        /*(a) 
                Check whether the corresponding fileVersion file exists in .minigit directory.
                If the fileVersion file DOES NOT EXIST, copy the file from the current directory
                into the .minigit directory. The newly copied file should get the name from the
                node’s fileVersion member. (Note: this will only be the case when a file is
                added to the repository for the first time.)
        */
        
        filename = crawler->fileName;
        s = ".minigit/" + crawler->fileVersion;
        myFile.open(s);

        // FILE DOES NOT EXIST IN MINIGIT
        if (!myFile.is_open())
        {
            // FILE DOES NOT EXIST
            // Copy it into the minigit directory

            // create a file in the minigit directory
            writeFile.open(s);
            // open the original file
            myFile.open(filename);
            // if the original cannot be opened, exit w/ an error message
            if (!myFile.is_open())
            {
                cout << "error. file " << filename << " cannot be opened." << endl;
                return;
            }  
            // copy the original file into the minigit directory line by line
            while (getline(myFile, line))
            {
                writeFile << line;
                writeFile << endl;
            }
            writeFile.close();
            myFile.close();
            line = "";
        }
        else
        {
            writeFile.close();
            myFile.close();
            line = "";
            // check to see if the file has been changed
            // If the file has been changed, copy it into minigit with an incremented version number
            if (!isFileEqual(s, filename))
            {
                crawler->versionNumber = crawler->versionNumber + 1;
                crawler->fileVersion = crawler->fileName + "_" + to_string(crawler->versionNumber);
                // give the new file a name with an incremented version number
                s = ".minigit/" + crawler->fileVersion;
                // open the file we're writing to
                writeFile.open(s);
                // open the original file
                myFile.open(filename);
                // if the original cannot be opened, exit w/ an error message
                if (!myFile.is_open())
                {
                    cout << "error. file " << filename << " cannot be opened." << endl;
                    return;
                }  
                // copy the original file into the minigit directory line by line
                while (getline(myFile, line))
                {
                    writeFile << line;
                    writeFile << endl;
                }
                writeFile.close();
                myFile.close();
            }
        }
        // Traverse
        crawler = crawler->next;
    }

    // deep copy
    crawler = currCommit->head;
    doublyNode * newCommit = new doublyNode;
    newCommit->commitNumber = currCommit->commitNumber + 1;
    newCommit->previous = currCommit;
    currCommit->next = newCommit;
    newCommit->next = nullptr;

    newCommit->head = deepCopy(crawler);

    // Change the current commit and the tail
    currCommit = currCommit->next;
    tail = currCommit;



    // check to make deep copy is working

    //crawler = currCommit->head;
    //singlyNode * crawler2 = currCommit->next->head;
    /*
    while(crawler != nullptr && crawler2 != nullptr)
    {
        cout << crawler->fileName << endl;
        cout << crawler2->fileName << endl;
        crawler = crawler->next;
        crawler2 = crawler2->next;
    }
    */
}


void miniGit::checkout(int commitNum)
{
    ifstream myFile;
    ofstream writeFile;

    // Start at the head of the DLL
    doublyNode * DLLCrawler = head;

    // Search for that particular commit in the list
    while (DLLCrawler != nullptr && DLLCrawler->commitNumber != commitNum)
    {
        DLLCrawler = DLLCrawler->next;
    }

    // If we couldn't find the commit, tell the user
    if (DLLCrawler == nullptr)
    {
        cout << "Error. Could not find commit number" << endl;
    }
    // Otherwise, we found the commit!
    else
    {
        // we found the commit!
        string line;
        currCommit = DLLCrawler;
        singlyNode *crawler = DLLCrawler->head;
        // for every node, overwrite the original file with it's version in that commit
        while (crawler != nullptr)
        {
            // Open the file version in that commit
            myFile.open(".minigit/" + crawler->fileVersion);
            // Open the current file
            writeFile.open(crawler->fileName);

            // If either files didn't open, tell the user (error)
            if (!writeFile.is_open() || !myFile.is_open())
            {
                cout << "Error opening files" << endl;
            }

            // Overwrite the current file with it's previous version
            while (getline(myFile, line))
            {
                writeFile << line;
                writeFile << endl;
            }

            // Close the files
            myFile.close();
            writeFile.close();
            // Traverse through the linked list
            crawler = crawler->next;
        }

    }

}