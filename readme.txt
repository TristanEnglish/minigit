Tristan English
CSCI 2270
Recitation 209

This program is a miniature version control system, which allows the user to add and remove files to be committed,
commit changes, and checkout previously committed file versions.

For compilation, please use
g++ -std=c++17 miniGit.cpp driver.cpp

Program Features:
Add - adds a file to the current list to be committed (given we are on the most recent commit)
Remove - removes a file from the current list to be committed (given we are on the most recent commit)
Commit - commits changes, saving a version of all the current files into a new commit (given we are on the most recent commit)
Checkout - allows the user to checkout a previous commit, overwriting the current files with those of a specified commit number.


