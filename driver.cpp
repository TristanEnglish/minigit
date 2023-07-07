#include "miniGit.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

int main()
{

    miniGit git;
    string numInput;
    string stringInput;
    string checkoutInput;
    bool init = false;

    cout << "Welcome to minigit. Would you like to initialize a new repository? (y/n): ";
    cin >> stringInput;
    while (stringInput.length() > 1)
    {
        cout << "Invalid input. Please try again.";
        cin >> stringInput;
    }

    if (stringInput == "Y" || stringInput == "y")
    {
        init = true;
        git.newRepository();
    }

    if (init)
    {
        cout << "+=================Main Menu=========================+" << endl;
        cout << "1) add" << endl;
        cout << "2) rm" << endl;
        cout << "3) commit" << endl;
        cout << "4) checkout" << endl;
        cout << "5) quit" << endl;
        cout << "#> ";
        cin >> numInput;

        while (stoi(numInput) != 5)
        {
            switch(stoi(numInput))
            {
                case 1:
                    // ADD A NODE
                    if (git.currCommit != git.tail)
                    {
                        cout << "Error. You cannot make changes when your commit version is different from the most recent commit." << endl;
                        cout << "Most recent commit is: " << git.tail->commitNumber << endl;
                    }
                    else
                    {
                        git.addFile();
                    }
                    break;
                case 2:
                    // REMOVE A NODE
                    if (git.currCommit != git.tail)
                    {
                        cout << "Error. You cannot make changes when your commit version is different from the most recent commit." << endl;
                        cout << "Most recent commit is: " << git.tail->commitNumber << endl;
                    }
                    else
                    {
                        git.removeFile();
                    }
                    break;
                case 3:
                    // COMMIT
                    if (git.currCommit != git.tail)
                    {
                        cout << "Error. You cannot make changes when your commit version is different from the most recent commit." << endl;
                        cout << "Most recent commit is: " << git.tail->commitNumber << endl;
                    }
                    else
                    {
                        git.newCommit();
                    }
                    break;
                case 4:
                    // CHECKOUT
                    cout << "Are you sure you would like to checkout? You may lose local changes if you have not committed them. (y/n): ";
                    cin >> checkoutInput;
                    if (checkoutInput == "y" || checkoutInput == "Y")
                    {
                        cout << "What version would you like to checkout? Please enter a commit number: ";
                        cin >> checkoutInput;
                        git.checkout(stoi(checkoutInput));
                        // CHECKOUT
                    }
                    break;
                default:
                    cout << "Invalid input, please try again" << endl;
                    break;
            }

            cout << "+=================Main Menu=========================+" << endl;
            cout << "1) add" << endl;
            cout << "2) rm" << endl;
            cout << "3) commit" << endl;
            cout << "4) checkout" << endl;
            cout << "5) quit" << endl;
            cout << "#> ";
            cin >> numInput;
        }  
    }
    

    return 0;
}
