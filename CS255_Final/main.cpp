//
//  main.cpp
//  CS255_Final
//
//  Created by Wayne Huang on 11/4/16.
//
//

#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    
    if (argc > 1)
        cout << argv[1] << endl;
    
    ifstream infile("../../dataset/facebook_combined.txt");
    string str;
    
    
    while (std::getline(infile, str))
    {
        // Process str
        cout << str << endl;
    }
    
    
    return 0;
}
