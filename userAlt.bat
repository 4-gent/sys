@echo off
REM File Name: userAlt.bat

REM This file lets the user adjust any user accounts

REM ------------------------------
REM User Input
:intialization
echo --------------
echo userAlt.bat
echo --------------
echo This allows the user to adjust any user accounts
echo .
echo .
echo .
echo ---------------------------------------
echo Add a user (a)
echo Delete a user (b)
echo Make Users Admin or Standard (c)
echo Alter a passsword (d)
echo Alter all passwords (e)
echo End Script (f)
echo ---------------------------------------

set /p input=:
if /i %input%==a (
goto add
)
if /i %input%==b (
goto delete
)
if /i %input%==c (
goto priv
)
if /i %input%==d (
goto alterPass
)
if /i %input%==e (
goto intialization
)
if /i %input%==f (
goto end
)

if not defined input (
echo Invalid Input...
timeout 3
echo Reinitializing...
timeout 3
goto intialization
)
REM ------------------------------
REM Add a User
:add
echo Current Users...
net user
echo Adding User...
set /p add="Enter Username [space] Enter Password: "
net user %add% /add
echo User Added
goto intialization
REM ------------------------------
REM Delete a User
:delete
echo Current Users...
net user
echo Deleting User...
set /p delete="Enter Username: "
net user %delete% /delete
echo User Deleted
goto intialization
REM ------------------------------
REM Change a Password
:alterPass
echo Changing a Password...
set /p shift="Enter Username [space] Enter New Password: "
net user %shift%
echo Password Changed
goto intialization
REM ------------------------------
REM Make Users Admin or Standard
:priv
echo Altering User Status...
set /p status=Edit Admin Users (a) / Display Current Groups (b) / Return to Menu (c)

if %status%==a goto admin
if %status%==b goto groups
if %status%==c goto intialization

REM Make User Admin
:admin
set /p switch=Make a User an Admin (a) / Take Away Admin (b) / Return to Menu (c)

if %switch%==a goto adminAdd
if %switch%==b goto adminDelete
if %switch%==c goto intialization

:adminAdd
echo Making User Admin...
echo Current Administrative Group...
net localgroup administrators
set /p uAdmin="Enter Username: "
net localgroup administrators %uAdmin% /add
echo %uAdmin% is now an Adminstrative User
goto priv

:adminDelete
echo Removing User Admin...
echo Current Administrative Group...
net localgroup administrators
set /p dAdmin="Enter Username: "
net localgroup administrators %dAdmin% /delete
echo %dAdmin% is no longer an Adminstrative User
goto priv

REM Reveal Groups
:groups
echo Displaying Local Groups...
net localgroup
echo Groups Displayed
goto priv
REM ------------------------------
REM Change All Passwords
REM Uses Conditional Loop Without For Loop to Alter All Passwords

:all
echo Changing All Passwords...
set /p all="Enter Password for All Users: "
goto user

:user
echo Enter Individual Name
set /p username=:
net user %username% %all%
echo %username%'s password altered

echo Leave to Menu (a) / Enter Next Name (b)
set /p question=:
if /i %question%==b goto user
if /i %question%==a (
echo Passwords Changed
goto intialization
)
REM ------------------------------
REM Shutdown
:end
echo Shutting Down...
timeout 3
pause
exit
