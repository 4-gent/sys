/*File Name: sysScript.cpp
*
*This file edits system stuff depending on user input
*
*Author: Mj Burog
*/

#include <stdio.h> //standard library input/output etc
#include <stdlib.h> //same thing as cstdlib
#include <fcntl.h> //file control library
#include <sys/stat.h> //library for getting file attributes
#include <iostream> //stream and input/output library
#include <unistd.h> //for misc symbols and constant types like null
#include <string.h> //basic strings
#include <thread> //class to represent individual threads of execution
#include <chrono> //date and time library
#include <cstdlib> //random numbers, general stuff
#include <ctime> //c-style time utilities
#include <experimental/filesystem> //filesystem library
using namespace std; //Getline namespace
namespace fs = std::experimental::filesystem;

//-------------------------------------
//Function Vars
char appName[256]; //Name of app the user enters
char input[256]; //user input choice in initialization
char realName[256]; //real name of newly added user
char options[256]; //user input choice in user account alteration
char username[256]; //username entry for user passwor alteration
char newUser[256]; //username for newly added user
char delUser[256]; //username for user being delete
char usrDelFunc[256]; //variable for conditional user deleting
char retDis[256]; //return variable for display function
int uniqueID = 1000; //establishes unique ID of new users
int numID = rand() % 9000 + 1000; //random num generator for ID generation
char endLineChar; //getline char
//-------------------------------------
//Functions
//-------------------------------------
//Initialization Declaration
void initialization();
void usrDelete();
//-------------------------------------
//Progress Bar (looks cool)
void progressbar(){
    std::srand(time(NULL));
    for(int progress=0;progress!=100;progress+=std::rand()%35){
        if(progress>100) progress=100;
        std::cout<<"[";
        for(int i=0;i<100;i++)
            if(i<progress)
                std::cout<<'=';
            else if(i==progress)
                std::cout<<'>';
            else
                std::cout<<' ';
                std::cout<<"]"<<progress<<" %"<<'\r';
                std::cout.flush();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            if(progress==100) break;

    }
    std::cout<<std::endl;
}
//-------------------------------------
//Application Open Function
void appOpen(){
    printf("All Available Applications...\n");
    std::string path = "/Users/mj/Applications";
    for(const auto & entry : fs::directory_iterator(path))
        std::cout<<entry.path()<<std::endl;
    printf("Type Application Name (Case Sensitive)\n");
    printf(":");
    scanf("%c", &endLineChar);
    cin.getline(appName, 256);
    std::string open("-a");
    system(("open "+open+appName).c_str());
    sleep(1);
    printf("Returning to Menu...\n");
    sleep(1);
    initialization();
}
//-------------------------------------
//Change User Accounts
void userAccts(){
    printf("-------------------------\n");
    printf("Add User (a)\n");
    printf("Delete User (b)\n");
    printf("Change User Password (c)\n");
    printf("Display Current Users (d)\n");
    printf("Return to Menu (e)\n");
    printf("-------------------------\n");
    printf(":");
    scanf("%s", options);

//Add User
if(!strcmp(options, "a") || !strcmp(options, "A")){
    //Adding the User
    printf("Type in New User's User Name\n");
    printf(":");
    scanf("%c", &endLineChar);
    cin.getline(newUser, 256);

    //Creating User
    std::string userNewUser("-create /Users/ ");
    system(("sudo dscl . " +userNewUser +newUser).c_str());
    
    //Mounting User to Bin
    std::string userBinpt1("-create /Users/");
    std::string userBinpt2(" UserShell /bin/bash");
    system(("sudo dscl . " +userBinpt1 +newUser +userBinpt2).c_str());
    
    //Adding real name to user
    printf("Type in User's Real Name\n");
    printf(":");
    cin.getline(realName, 256);
    std::string realInputpt1("-create /Users/");
    std::string realInputpt2(" RealName ");
    system(("sudo dscl . " +realInputpt1 +newUser +realInputpt2 +realName).c_str());
    
    //Adding unique ID to User
    printf("Creating Unique ID...\n");
    int numID = rand() % 9000 + 1000;
    uniqueID = numID;
    printf("Unique ID = %d\n", uniqueID);
    std::string sudoIDpt1("-create /Users/"); 
    std::string sudoIDpt2("UniqueID");
    std::string sudoIDpt3("%d", uniqueID);
    system(("sudo dscl . " +sudoIDpt1 +newUser +sudoIDpt2 +sudoIDpt3).c_str());
    
    //Adding user to primary group ID 1000
    std::string primeGrouppt1("-create /Users/");
    std::string primeGrouppt2(" PrimaryGroupID 1000");
    system(("sudo dscl . " +primeGrouppt1 +newUser +primeGrouppt2).c_str());
    
    //Creating home folder for the new user
    std::string homeDir("-create /Users/");
    std::string homeDirpt2(" NFSHomeDirectory /Local/Users/username");
    system(("sudo dscl . " +homeDir +newUser +homeDirpt2).c_str());
    
    //Making User Password
    std::string passSet("-passwd /Users/");
    system(("sudo dscl . " +passSet +newUser).c_str());
    
    //Returning to main menu
    printf("User Created...\n");
    sleep(1);
    printf("Returning to Menu...\n");
    initialization();
}

//Delete User
if(!strcmp(options, "b") || !strcmp(options, "B")){
    //Enable root and delete user
    printf("Enabling Root...\n");
    printf("Enter Name of Root Admin...\n");
    system("dsenableroot");

    //Delete User Function
    usrDelete();
    
    //Conditional Loop to continue or cancel delete
    if(!strcmp(usrDelFunc, "y") || !strcmp(usrDelFunc, "Y")){
        while (true){
            usrDelete();
            if(!strcmp(usrDelFunc, "n") || !strcmp(usrDelFunc, "N")){
                break;
            }
        }
    }

    //Disable Root
    printf("Disabliing root user (Re-enter root admin name)\n");
    system("dsenableroot -d");
    printf("Returning to menu...\n");
    sleep(1);
    initialization();
}

//Change User Password
if(!strcmp(options, "c") || !strcmp(options, "C")){
    printf("Type in Username\n");
    printf(":");
    scanf("%c", &endLineChar);
    cin.getline(username, 256);
    std::string password("passwd ");
    system(("sudo " +password +username).c_str());
    printf("%s\'s password has been changed\n", username);
    sleep(1);
    printf("Returning to Menu...\n");
    initialization();
}

//Display Users
if(!strcmp(options, "d") || !strcmp(options, "D")){
    system("dscl . list /Users | grep -v \"^_\"");
    printf("-------------------------\n");
    printf("Return to user alter menu (a)\n");
    printf("End Program (b)\n");
    printf("-------------------------\n");
    printf(":");
    scanf("%s", retDis);

    if(!strcmp(retDis, "a") || !strcmp(retDis, "A")){
        printf("Returning to Menu...\n");
        sleep(1);
        userAccts();
    }
    if(!strcmp(retDis, "b") || !strcmp(retDis, "B")){
        exit(0);
    }
    else {
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Menu...\n");
        initialization();
    }
}

//Return to Menu
if(!strcmp(options, "e") || !strcmp(options, "e")){
    initialization();
}
else {
    printf("Input Error...\n");
    sleep(1);
    printf("Returning to Menu...\n");
    initialization();
}
}
//-------------------------------------
//User Delete Function
void usrDelete(){
    //Delete User
    printf("Type in Username to Delete\n");
    printf(":");
    scanf("%c", &endLineChar);
    cin.getline(delUser, 256);
    std::string userDelFunc("delete /Users/");
    system(("sudo dscl . " +userDelFunc +delUser).c_str());

    //Moving user files to backup
    std::string backuppt1("/users/");
    std::string backuppt2(" /Users/mj/Desktop");
    system(("sudo mv" +backuppt1 +delUser +backuppt2).c_str());

    //Delete another user
    printf("-------------------------\n");
    printf("Delete Another User? (y/n)\n");
    printf("-------------------------\n");
    printf(":");
    scanf("%s", usrDelFunc);
}
//-------------------------------------
//Initialization
//Input Segment
void initialization(){
    printf("-------------------------");
    printf("File Name: sysScript.cpp");
    printf("-------------------------\n");
    progressbar();
    printf("-------------------------\n");
    printf("Open Application (a)\n");
    printf("Change User Accounts (b)\n");
    printf("End Program (c)\n");
    printf("-------------------------\n");
    printf(":");
    scanf("%s", input); 

    //Conditional Statement to Choose Functions
    if(!strcmp(input, "a") || !strcmp(input, "A")){
        appOpen();
    }
    if(!strcmp(input, "b") || !strcmp(input, "B")){
        userAccts();
    }
    if(!strcmp(input, "c") || !strcmp(input, "C")){
        exit(0);
    }
    else {
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Menu...\n");
        initialization();
    }
}
//-------------------------------------
//Program Initialization
int main(){
    initialization();
    return 0;
}
//-------------------------------------