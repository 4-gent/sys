//File Name: linScript.cpp
//
//This file will adjust linux security and settings
//
//Author: Mj Burog

//---------------------------
//Library Declaration
#include <stdio.h>
#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <ctime>
#include <unistd.h>
#include <string.h>
#include <thread>
//---------------------------
//Variable Declaration
using namespace std;
char endLineChar; //getline char
//---------------------------
//Function Declaration
void sudoUser();
void usrAccts();
void sysEdit();
void initialization();
void toolInstall();
void uninstall();
int main();
//---------------------------
//Initialization (sys editing)
//---------------------------
void sudoUser(){
    //sudoUser Variables
    char root[256];

    //Activate Root User
    printf("Activating Root User...\n");
    sleep(1);
    printf("Note: Moving to Root might lose your current directory position\n");
    sleep(2);
    printf("Continue? (y/n)\n");
    printf(":");
    scanf("%s", root);
    
    if(!strcmp(root, "y") || !strcmp(root,"Y")){
        system("sudo -i");
        sleep(2);
        printf("Root User Enabled...\n");
        sleep(2);
        main();
    }
    if(!strcmp(root, "n") || !strcmp(root, "N")){
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
}
//---------------------------
//System Sec Editing
void sysEdit(){
    //Variables for system editing
    char edits[256]; //User input to decide if script will do sys edits

    //System Checklist Editing
    printf("---------------------------- linScript.cpp ----------------------------\n");
    printf(".\n");
    printf("(Check if Forensic Questions are Completed)\n");
    printf(".\n");
    printf("----------------------\n");
    printf("Continue with Checklist Edits? (y/n)\n");
    printf("----------------------\n");  
    printf(":");
    scanf("%s", edits);

    if(!strcmp(edits, "y") || !strcmp(edits, "Y")){
        //Automatic Updates
        printf("Activating Automatic Updates...\n");
        system("sudo apt update -y");
        system("sudo apt full-upgrade -y");
        system("sudo apt install unattended-upgrades apt-listchanges bsd-mailx -y");
        system("sudo dpkg-reconfigure -plow unattended-upgrades");
        //system("sudo vi /etc/apt/apt.conf.d/50unattended-upgrades");
        printf("Automatic Updates Activated...\n");

        //Enabling Firewall
        printf("Activating Firewall...\n");
        system("sudo ufw enable");
        printf("Firewall Enabled...\n");

        //gufw Download for Managing Firewall with GUI
        printf("Installing GUI for in-depth firewall editing...\n");
        system("sudo apt install gufw -y");
        printf("gufw Installed...\n");

        //Editing Auddit policies
	printf("Installing auditd and enabling all audit policies\n");
        system("sudo apt-get install auditd -y");
        system("sudo auditctl -e 1");
	printf("Auditd installed and all audit policies are installed...\n");

	//Install terminator to allow multiple CLI processes
	printf("Installing terminator to allow multiple CLI Windows\n");
	system("sudo apt-get install terminator -y");
	printf("Terminator installed...\n");

        //Listing all the changes the script was able to do
        printf("-----------------\n");
        printf("Completed changes\n");
        printf("-----------------\n");
        printf("Firewall enabled\n");
        printf("Firewall gui installed\n");
        printf("Automatic updates activated\n");
        printf("Audit policy thingy installed and all audits enabled (gedit /etc/audit/auditd.conf to edit)\n");

	sleep(3);
        printf("Returning to Main Menu...\n");
        sleep(1);
        main();
    }
    if(!strcmp(edits, "n") || !strcmp(edits, "N")){
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
}
//---------------------------
//User Account Alteration
void usrAccts(){
    //User Account Variables
    char userAccount[256]; //User Input to choose which process to perform
    char password[256]; //User Password Change
    char username[256]; //Username for new users
    char userDel[256]; //User to be deleted
    char groupDel[256]; //Group to be deleted
    char delContinue[256]; //Decides whether or not to delete home directory along with user
    char homeMove[256]; //Decides whether or not 
    int numID = rand() % 9000 + 1000; //random num generator for uniqueID generation
    int uniqueID = 1000; //unique id for new user

    //User Alteration Options
    printf("---------------------------- linScript.cpp ----------------------------\n");
    printf(".\n");
    printf("----------------------\n");
    printf("Add a User (a)\n");
    printf("Delete a User (b)\n");
    printf("Change User Password (c)\n");
    printf("Delete Group (d)\n");
    printf("Display Current Users and Groups (e)\n");
    printf("Edit Admin Privileges (f)\n");
    printf("Changing User Readable Permissions (g)\n");
    printf("Locking/Unlocking Users (h)\n");
    printf("Setting Expirations (i)\n");
    printf("Return to Menu (j)\n");
    printf("----------------------\n");
    printf(":");
    scanf("%s", userAccount);

    //New User Creation
    if(!strcmp(userAccount, "a") || !strcmp(userAccount, "A")){
        //Creates New User
        printf("Type in New User's Name\n");
        printf(":");
        scanf("%c", &endLineChar);
        cin.getline(username, 256);
        std::string space(" ");
        system(("sudo adduser" +space +username).c_str());

        //Return to menu
        printf("Returning to User Alter Menu...\n");
        sleep(1);
        usrAccts();
    }

    //Delete User
    if(!strcmp(userAccount, "b") || !strcmp(userAccount, "B")){
        printf("Type in User Name to Delete\n");
        printf(":");
        scanf("%s", userDel);
        std::string space(" ");
        system(("sudo deluser" +space +userDel).c_str());

        //Home Directory Deletion
        printf("Delete Home Directory? (y/n)\n");
        printf(":");
        scanf("%s", delContinue);
        //Delete Home Directory
            if(!strcmp(delContinue, "y") || !strcmp(userAccount, "Y")){
                std::string home("/home/");
                system(("sudo rm -r " +home +userDel).c_str());
                printf("Returning to Menu...\n");
                sleep(1);
                main(); 
            }
        //Move Home Directory to Desktop Location
            if(!strcmp(delContinue, "n") || !strcmp(delContinue, "N")){
                printf("Home Directory still in /home \n");
                sleep(1);
                printf("Returning to Menu...\n");
                sleep(1);
                main();
            }
            else{
                printf("Input Error...\n");
                sleep(1);
                printf("Home Directory still in /home \n");
                sleep(1);
                printf("Returning to Menu...\n");
                sleep(1);
                main();
            }  
    }
    //Password Change
    if(!strcmp(userAccount, "c") || !strcmp(userAccount, "C")){
        printf("Type in User Name for Password Change\n");
        printf(":");
        scanf("%s", password);
        std::string pass("passwd ");
        system(("sudo " +pass +password).c_str());
        usrAccts();
    }
    //Group Delete
    if(!strcmp(userAccount, "d") || !strcmp(userAccount, "D")){
        printf("Type in Group to Delete\n");
        printf(":");
        scanf("%s", groupDel);
        std::string del("groupdel ");
        system(("sudo " +del +groupDel).c_str());
    }
    //Display Current Users and User Groups
    if(!strcmp(userAccount, "e") || !strcmp(userAccount, "E")){
        printf("----------------------------\n");
        printf("Currently Existing Users...\n");
        printf("----------------------------\n");
        system("sudo cut -d: -f 1 /etc/passwd");
        printf("\n");
        printf("----------------------------\n");
        printf("Currently Existing Groups\n");
        printf("----------------------------\n");
        system("groups");
        printf("\n");
        printf("----------------------------\n");
        usrAccts();
    }
    //Giving Users Admin Privileges 
    if(!strcmp(userAccount,"f") || !strcmp(userAccount, "F")){
        //User Admin Privilege Variables
        char adminChoice[256]; //Determines if they will take or give admin privileges
        char adminName[256]; //The username used for alteration

        //Main Admin Alteration Block
        printf("----------------------------\n");
        printf("Give a User Admin Privileges (a)\n");
        printf("Take Away a User's Admin Privileges (b)\n");
        printf("Specify User Admin Privileges (c)\n");
        printf("Return to Menu (d)\n");
        printf("----------------------------\n");
        printf(":");
        scanf("%s", adminChoice);

        //Give Admin
        if(!strcmp(adminChoice, "a") || !strcmp(adminChoice, "A")){
            printf("Type in username you want to make admin\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(adminName, 256);
            std::string mod("sudo ");
            system(("sudo usermod -aG " +mod +adminName).c_str());
            usrAccts();
        }
        //Take Away Admin
        if(!strcmp(adminChoice, "b") || !strcmp(adminChoice, "B")){
            printf("Type in username to take away admin privileges\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(adminName, 256);
            std::string deluser("deluser ");
            std::string deluser2(" sudo");
            system(("sudo " +deluser +adminName +deluser2).c_str());
            usrAccts();
        }
        //Specify Admin
        if(!strcmp(adminChoice, "c") || !strcmp(adminChoice, "C")){
            system("sudo visudo");
            usrAccts();
        }
        //Return to Menu
        if(!strcmp(adminChoice, "d") || !strcmp(adminChoice, "D")){
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
        //User Input Error
        else{
            printf("Input Error...\n");
            sleep(1);
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
    }
    //User Readable Permissions
    if(!strcmp(userAccount, "g") || !strcmp(userAccount, "G")){
        printf("Type in username to edit\n");
        printf(":");
        scanf("%c", &endLineChar);
        cin.getline(username, 256);
        std::string read("/home/username ");
        system(("sudo chmod 0750 " +read +username).c_str());
        printf("Returning to User Alter Menu...\n");
        sleep(1);
        usrAccts();
    }
    //Locking/Unlocking Users
    if(!strcmp(userAccount, "h") || !strcmp(userAccount, "H")){
        //Variables for this one
        char unLock[256];
        
        printf("-------------------\n");
        printf("Lock an Account (a)\n");
        printf("Unlock an Account (b)\n");
        printf("Return to User Alter Menu (c)\n");
        printf("-------------------\n");
        printf(":");
        scanf("%s", unLock);
        if(!strcmp(unLock, "a") || !strcmp(unLock, "A")){
            printf("Type in the username to lock\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(username, 256);
            std::string lock("passwd -l ");
            system(("sudo " +lock +username).c_str());
            printf("Returning to Menu...\n");
            sleep(1);
            main();
        }
        if(!strcmp(unLock, "b") || !strcmp(unLock, "B")){
            printf("Type in the username to unlock\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(username, 256);
            std::string unlock("passwd -u ");
            system(("sudo " +unlock +username).c_str());
            printf("Returning to Menu...\n");
            sleep(1);
            main();
        }
        if(!strcmp(unLock, "c") || !strcmp(unLock, "C")){
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
        else{
            printf("Input Error...\n");
            sleep(1);
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
    }
    //Setting Expirations
    if(!strcmp(userAccount, "i") || !strcmp(userAccount, "I")){
        //Variables for Expiration
        char expire [256];
        char add[256];
        int expireAdd = 0;

        //Expiration Options
        printf("Add Password Expiration (a)\n");
        printf("Remove Password Expiration (b)\n");
        printf("Return to User Alter Menu (c)\n");
        printf(":");
        scanf("%s", expire);

        if(!strcmp(expire, "a") || !strcmp(expire, "A")){
            //Get username
            printf("Type in username to add expiration\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(add, 256);
            
            //Get expiration days
            printf("Type in amount of days for expiration\n");
            printf(":");
            scanf("%i", &expireAdd);

            //Put em together now
            std::string chage("-E " +std::to_string(expireAdd));
            std::string space(" ");
            system(("sudo chage " +chage +space +add).c_str());

        }
        if(!strcmp(expire, "b") || !strcmp(expire, "B")){
            printf("Type in username for expiration removal\n");
            printf(":");
            scanf("%c", &endLineChar);
            cin.getline(username, 256);
            std::string chageRem("-1 ");
            system(("sudo chage -E " +chageRem +username).c_str());
        }
        if(!strcmp(expire, "c") || !strcmp(expire, "C")){
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
        //Input Error
        else{
            printf("Input Error...\n");
            sleep(1);
            printf("Returning to User Alter Menu...\n");
            sleep(1);
            usrAccts();
        }
    }
    //Return to Menu
    if(!strcmp(userAccount, "j") || !strcmp(userAccount, "J")){
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
    //Input Error
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to User Alter Menu...\n");
        sleep(1);
        usrAccts();
    }
}
//---------------------------
//Tool Installation
void toolInstall(){
    //Tool Variables
    char tool[256]; //Decides whether or not to continue with tool install

    //Actual Installation
    printf("---------------------------- linScript.cpp ----------------------------\n");
    printf(".\n");
    printf("(This will install ClamAV for antivirus AND run a full system scan)\n");
    printf(".\n");
    printf("----------------------\n");
    sleep(2);
    printf("Continue?(y/n)\n");
    printf(":");
    scanf("%s", tool);

    if(!strcmp(tool, "y") || !strcmp(tool, "Y")){
        printf("Installing clamAV...\n");
        system("sudo apt-get install clamav clamav-daemon -y");
        printf("ClamAV installled, running system scan...\n");
        system("sudo clamscan -r --remove /");
        printf("Scan complete, returning to main menu...\n");
        sleep(1);
        main();
    }
    if(!strcmp(tool, "n") || !strcmp(tool, "N")){
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Main Menu...\n");
        sleep(1);
        main();
    }
}
//---------------------------
//Uninstall Applications
void uninstall(){
    //Variables for uninstall
    char uninstApp[256];
    char contUninstall[256];

    //First display currently installed apps
    printf("-----------------------------------\n");
    printf("Currently Installed Applications...\n");
    printf("-----------------------------------\n");
    sleep(1);
    system("dpkg --list");
    printf("-----------------------------------\n");

    //Second ask which app will be deleted
    printf("Type in EXACT (case sensitive) name of app you want removed\n");
    printf(":");
    scanf("%c", &endLineChar);
    cin.getline(uninstApp, 256);

    //Third Delete the application
    std::string purge("remove ");
    system(("sudo apt-get " +purge +uninstApp).c_str());
    printf("%s has been deleted", uninstApp);
    sleep(1);

    //Fourth ask for another deletion
    printf("-----------------------------------\n");
    printf("Delete Another Application? (y/n)\n");
    printf("-----------------------------------\n");
    printf(":");
    scanf("%s", contUninstall);
    
    if(!strcmp(contUninstall, "y") || !strcmp(contUninstall, "Y")){
        uninstall();
    }
    if(!strcmp(contUninstall, "n") || !strcmp(contUninstall, "N")){
        printf("Returning to Menu...\n");
        sleep(1);
        main();
    }
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Main Menu...\n");
        sleep(1);
        main();
    }

}
//---------------------------
//Main Block
int main(){
    //Variables for main block
    char input[256]; //Main User Input to Choose Script Options

    //Beginning User Directory Block
    printf("---------------------------- linScript.cpp ----------------------------\n");
    printf(".\n");
    printf("----------------------\n");
    printf("Switch to Root User (a)\n");
    printf("Adjust System Settings (b)\n");
    printf("User Account Editing (c)\n");
    printf("Tool Installation (d)\n");
    printf("Uninstall Applications (e)\n");
    printf("End Program (f)\n");
    printf("----------------------\n");
    printf(":");
    scanf("%s", input);

    if(!strcmp(input, "a") || !strcmp(input, "A")){
        sudoUser();
    }
    if(!strcmp(input, "b") || !strcmp(input, "B")){
        sysEdit();
    }
    if(!strcmp(input, "c") || !strcmp(input, "C")){
        usrAccts();
    }
    if(!strcmp(input, "d") || !strcmp(input, "D")){
        toolInstall();
    }
    if(!strcmp(input, "e") || !strcmp(input, "E")){
        uninstall();
    }
    if(!strcmp(input, "f") || !strcmp(input, "F")){
        printf("Good Luck :)\n");
        sleep(1);
        printf("Don't Mess Up, Bye Bye BB\n");
        sleep(1);
        exit(0);
    }
    else{
        printf("Input Error...\n");
        sleep(1);
        printf("Returning to Main Menu...\n");
        sleep(1);
        main();
    }
}
//---------------------------
