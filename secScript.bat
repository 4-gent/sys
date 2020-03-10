@echo off
echo ------------------------
echo File Name: secScript.bat
echo ------------------------
echo This converts security settings to industry required settings

timeout 5

REM File Name: secScript.bat
REM This changes security and audit policies to industry settings
REM ------------------
REM Checklist Changes:
REM ------------------
REM Audit Policy Changes
REM Security Policy Changes (Not All Changed)
REM Automatic Updates
REM Firewall Activation
REM User Password Changes (Use userAlt.bat)
REM Remote Desktop Deactivation
REM Windows Features Deactivation
REM Services Alterations
REM Registry Required Edits
REM DNS Editing

REM --------------------------------------------------
REM Check admin privileges
echo Administrative rights status:
net sessions
if %errorlevel%==0 (
    echo -Admin enabled-
) else (
    echo disabled - Run again with Administrative rights
        pause
        Exit
)
REM --------------------------------------------------
REM Audit Policy changes
    echo Altering Audit Policies
    Auditpol /set /category:"Account Logon" /failure:enable
    Auditpol /set /category:"Logon/Logoff" /failure:enable
    Auditpol /set /category:"Account Management" /Success:enable
    REM Auditpol Directory Service stays as ND
    REM Auditpol Object Access stays as ND
    Auditpol /set /category:"policy change" /Success:enable
    Auditpol /set /category:"Privilege use" /Success:enable /failure:enable
    Auditpol /set /category:"detailed tracking" /Success:enable /failure:enable
    Auditpol /set /category:"System" /failure:enable
    echo Audit Policies Set to Industry Settings
REM --------------------------------------------------
REM Password Policy changes
    REM secedit /export /cfg c:\secpol.cfg
        REM (gc C:\secpol.cfg).replace("PasswordComplexity = 1", "PasswordComplexity = 0") | Out-File C:\secpol.cfg
    REM secedit /configure /db c:\windows\security\local.sdb /cfg c:\secpol.cfg /areas SECURITYPOLICY
        REM rm -force c:\secpol.cfg -confirm:$false
        ::Max password age
    REM Reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\SeCEdit\Reg Values\MACHINE/System/CurrentControlSet/Services/Netlogon/Parameters/MaximumPasswordAge" 
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v MaximumPasswordAge /t REG_DWORD /d 15 /f
REM --------------------------------------------------
REM Security Policy changes
    echo Deactivating Guest and Admin
    net user guest /active:no
    net user administrator /active:no
    REM start C:\Windows\System32\WindowsPowerShell\v1.0\powershell.exe "%path%resources\RenameDefAccounts.ps1"
    echo Guest and Admin Disabled
REM --------------------------------------------------
REM Registry Key activating automatic updates
    echo Activating Automatic Updates
    reg add "HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update" /v AUOptions /t REG_DWORD /d 4 /f
    reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU" /v AutoInstallMinorUpdates /t REG_DWORD /d 1 /f
    reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU" /v NoAutoUpdate /t REG_DWORD /d 0 /f
    reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate\AU" /v AUOptions /t REG_DWORD /d 4 /f
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update" /v AUOptions /t REG_DWORD /d 4 /f
    reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate" /v DisableWindowsUpdateAccess /t REG_DWORD /d 0 /f
    reg add "HKLM\SOFTWARE\Policies\Microsoft\Windows\WindowsUpdate" /v ElevateNonAdmins /t REG_DWORD /d 0 /f
    reg add "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\Explorer" /v NoWindowsUpdate /t REG_DWORD /d 0 /f
    reg add "HKLM\SYSTEM\Internet Communication Management\Internet Communication" /v DisableWindowsUpdateAccess /t REG_DWORD /d 0 /f
    reg add "HKCU\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\WindowsUpdate" /v DisableWindowsUpdateAccess /t REG_DWORD /d 0 /f
    echo Automatic Updates Enabled
REM --------------------------------------------------
REM Firewall active
    echo Firewall Activating...
    netsh advfirewall set allprofiles state on
    echo Firewall Enabled
REM --------------------------------------------------
REM All user passwords set to provided
REM All passwords have expiration
echo Note: Reset Main User Password To Default To Prevent Complex Sign In
echo Altering All User Passwords
for /f "tokens=*" %%a in ('type %path%resources\users.txt') do (
    net user "%%a" "B1N6bong@2k19"
    C:\Windows\System32\wbem\wmic UserAccount where Name="%%a" set PasswordExpires=True
)
echo Passwords Altered/Passwords Have expiration
REM --------------------------------------------------
REM Remote desktop deactivation
    echo Disabling Remote Desktop...
	reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fDenyTSConnections /t REG_DWORD /d 1 /f
	reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v AllowTSConnections /t REG_DWORD /d 0 /f
	reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server" /v fAllowToGetHelp /t REG_DWORD /d 0 /f
	reg add "HKLM\SYSTEM\CurrentControlSet\Control\Terminal Server\WinStations\RDP-Tcp" /v UserAuthentication /t REG_DWORD /d 0 /f
	netsh advfirewall firewall set rule group="remote desktop" new enable=no
	echo Disabled Remote Desktop
REM --------------------------------------------------
REM Setting alterations for Services
    echo Adjusting Services...
    echo Note: Remote Desktop Services Are Not Deactivated With This Script
    REM Variable assignments to services
    set servicesD=RemoteAccess TapiSrv Tlntsvr tlntsvr p2pimsvc simptcp fax msftpsvc iprip ftpsvc RemoteRegistry RasMan RasAuto seclogon MSFTPSVC W3SVC SMTPSVC Dfs TrkWks MSDTC DNS ERSVC NtFrs MSFtpsvc helpsvc HTTPFilter IISADMIN IsmServ WmdmPmSN Spooler RDSessMgr RPCLocator RsoPProv ShellHWDetection ScardSvr Sacsvr TermService Uploadmgr VDS VSS WINS WinHttpAutoProxySvc SZCSVC CscService hidserv IPBusEnum PolicyAgent SCPolicySvc SharedAccess SSDPSRV Themes upnphost nfssvc nfsclnt MSSQLServerADHelper Server SNMPTrap SSDPDiscovery lmhosts LanmanServer workfolderssvc
    set servicesM=dmserver SrvcSurg
    set servicesG=Dhcp Dnscache NtLmSsp
    REM Bad Services Disabled
        for %%a in (%servicesD%) do (
	        echo Service: %%a
	        sc stop "%%a"
	        sc config "%%a" start= disabled
        )
    REM Ok services set to manual
        for %%b in (%servicesM%) do (
	        echo Service: %%b
	        sc config "%%b" start= demand
        )
    REM Ok services set to automatic
        for %%c in (%servicesG%) do (
	        echo Service: %%c
	        sc config "%%c" start= auto
        )
    echo Services Adjusted
REM --------------------------------------------------
REM Registry Key Assortments
    echo Using Registry Keys to Alter Settings
    ::Restrict CD ROM drive
    reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v AllocateCDRoms /t REG_DWORD /d 1 /f
    ::Disallow remote access to floppy disks
    reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v AllocateFloppies /t REG_DWORD /d 1 /f
    ::Disable auto Admin logon
    reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Winlogon" /v AutoAdminLogon /t REG_DWORD /d 0 /f
    ::Clear page file (Will take longer to shutdown)
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Memory Management" /v ClearPageFileAtShutdown /t REG_DWORD /d 1 /f
    ::Prevent users from installing printer drivers 
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Print\Providers\LanMan Print Services\Servers" /v AddPrinterDrivers /t REG_DWORD /d 1 /f
    ::Add auditing to Lsass.exe
    reg add "HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\LSASS.exe" /v AuditLevel /t REG_DWORD /d 00000008 /f
    ::Enable LSA protection
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v RunAsPPL /t REG_DWORD /d 00000001 /f
    ::Limit use of blank passwords
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v LimitBlankPasswordUse /t REG_DWORD /d 1 /f
    ::Auditing access of Global System Objects
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v auditbaseobjects /t REG_DWORD /d 1 /f
    ::Auditing Backup and Restore
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v fullprivilegeauditing /t REG_DWORD /d 1 /f
    ::Restrict Anonymous Enumeration #1
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v restrictanonymous /t REG_DWORD /d 1 /f
    ::Restrict Anonymous Enumeration #2
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v restrictanonymoussam /t REG_DWORD /d 1 /f
    ::Disable storage of domain passwords
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v disabledomaincreds /t REG_DWORD /d 1 /f
    ::Take away Anonymous user Everyone permissions
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v everyoneincludesanonymous /t REG_DWORD /d 0 /f
    ::Allow Machine ID for NTLM
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\Lsa" /v UseMachineId /t REG_DWORD /d 0 /f
    ::Do not display last user on logon
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v dontdisplaylastusername /t REG_DWORD /d 1 /f
    ::Enable UAC
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v EnableLUA /t REG_DWORD /d 1 /f
    ::UAC setting (Prompt on Secure Desktop)
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v PromptOnSecureDesktop /t REG_DWORD /d 1 /f
    ::Enable Installer Detection
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v EnableInstallerDetection /t REG_DWORD /d 1 /f
    ::Disable undocking without logon
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v undockwithoutlogon /t REG_DWORD /d 0 /f
    ::Enable CTRL+ALT+DEL
    reg add "HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Policies\System" /v DisableCAD /t REG_DWORD /d 0 /f
    ::Disable machine account password changes
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v DisablePasswordChange /t REG_DWORD /d 1 /f
    ::Require strong session key
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v RequireStrongKey /t REG_DWORD /d 1 /f
    ::Require Sign/Seal
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v RequireSignOrSeal /t REG_DWORD /d 1 /f
    ::Sign Channel
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v SignSecureChannel /t REG_DWORD /d 1 /f
    ::Seal Channel
    reg add "HKLM\SYSTEM\CurrentControlSet\services\Netlogon\Parameters" /v SealSecureChannel /t REG_DWORD /d 1 /f
    ::Set idle time to 45 minutes
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanServer\Parameters" /v autodisconnect /t REG_DWORD /d 45 /f
    ::Require Security Signature - Disabled pursuant to checklist:::
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanServer\Parameters" /v enablesecuritysignature /t REG_DWORD /d 0 /f
    ::Enable Security Signature - Disabled pursuant to checklist:::
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanServer\Parameters" /v requiresecuritysignature /t REG_DWORD /d 0 /f
    ::Clear null session pipes
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanServer\Parameters" /v NullSessionPipes /t REG_MULTI_SZ /d "" /f
    ::Restict Anonymous user access to named pipes and shares
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanServer\Parameters" /v NullSessionShares /t REG_MULTI_SZ /d "" /f
    ::Encrypt SMB Passwords
    reg add "HKLM\SYSTEM\CurrentControlSet\services\LanmanWorkstation\Parameters" /v EnablePlainTextPassword /t REG_DWORD /d 0 /f
    ::Clear remote registry paths
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\SecurePipeServers\winreg\AllowedExactPaths" /v Machine /t REG_MULTI_SZ /d "" /f
    ::Clear remote registry paths and sub-paths
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\SecurePipeServers\winreg\AllowedPaths" /v Machine /t REG_MULTI_SZ /d "" /f
    ::Enable smart screen for IE8
    reg add "HKCU\Software\Microsoft\Internet Explorer\PhishingFilter" /v EnabledV8 /t REG_DWORD /d 1 /f
    ::Enable smart screen for IE9 and up
    reg add "HKCU\Software\Microsoft\Internet Explorer\PhishingFilter" /v EnabledV9 /t REG_DWORD /d 1 /f
    ::Disable IE password caching
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v DisablePasswordCaching /t REG_DWORD /d 1 /f
    ::Warn users if website has a bad certificate
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v WarnonBadCertRecving /t REG_DWORD /d 1 /f
    ::Warn users if website redirects
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v WarnOnPostRedirect /t REG_DWORD /d 1 /f
    ::Enable Do Not Track
    reg add "HKCU\Software\Microsoft\Internet Explorer\Main" /v DoNotTrack /t REG_DWORD /d 1 /f
    reg add "HKCU\Software\Microsoft\Internet Explorer\Download" /v RunInvalidSignatures /t REG_DWORD /d 1 /f
    reg add "HKCU\Software\Microsoft\Internet Explorer\Main\FeatureControl\FEATURE_LOCALMACHINE_LOCKDOWN\Settings" /v LOCALMACHINE_CD_UNLOCK /t REG_DWORD /d 1 /f
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Internet Settings" /v WarnonZoneCrossing /t REG_DWORD /d 1 /f
    ::Show hidden files
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /v Hidden /t REG_DWORD /d 1 /f
    ::Disable sticky keys
    reg add "HKU\.DEFAULT\Control Panel\Accessibility\StickyKeys" /v Flags /t REG_SZ /d 506 /f
    ::Show super hidden files
    reg add "HKCU\Software\Microsoft\Windows\CurrentVersion\Explorer\Advanced" /v ShowSuperHidden /t REG_DWORD /d 1 /f
    ::Disable dump file creation
    reg add "HKLM\SYSTEM\CurrentControlSet\Control\CrashControl" /v CrashDumpEnabled /t REG_DWORD /d 0 /f
    ::Disable autoruns
    reg add "HKCU\SYSTEM\CurrentControlSet\Services\CDROM" /v AutoRun /t REG_DWORD /d 1 /f
    echo Registry Key Alterations Complete
REM --------------------------------------------------
REM DNS Editing
    echo Adjusting DNS...
    ipconfig /flushdns >nul
    REM Flushed DNS
    REM Clearing contents of: C:\Windows\System32\drivers\etc\hosts
    attrib -r -s C:\WINDOWS\system32\drivers\etc\hosts
    REM C:\Windows\System32\drivers\etc\hosts
    attrib +r +s C:\WINDOWS\system32\drivers\etc\hosts
    echo DNS Adjusted
REM --------------------------------------------------
REM Windows Features deactivation
    echo Altering Windows Features Settings...
    echo Installing Dism.exe
    copy %path%resources\Dism.exe C:\Windows\Logs\DISM\dism.log
    copy %path%resources\Dism C:\Windows\Logs\DISM\dism.log
    echo Disabling Windows features...
    set features=IIS-WebServerRole IIS-WebServer IIS-CommonHttpFeatures IIS-HttpErrors IIS-HttpRedirect IIS-ApplicationDevelopment IIS-NetFxExtensibility IIS-NetFxExtensibility45 IIS-HealthAndDiagnostics IIS-HttpLogging IIS-LoggingLibraries IIS-RequestMonitor IIS-HttpTracing IIS-Security IIS-URLAuthorization IIS-RequestFiltering IIS-IPSecurity IIS-Performance IIS-HttpCompressionDynamic IIS-WebServerManagementTools IIS-ManagementScriptingTools IIS-IIS6ManagementCompatibility IIS-Metabase IIS-HostableWebCore IIS-StaticContent IIS-DefaultDocument IIS-DirectoryBrowsing IIS-WebDAV IIS-WebSockets IIS-ApplicationInit IIS-ASPNET IIS-ASPNET45 IIS-ASP IIS-CGI IIS-ISAPIExtensions IIS-ISAPIFilter IIS-ServerSideIncludes IIS-CustomLogging IIS-BasicAuthentication IIS-HttpCompressionStatic IIS-ManagementConsole IIS-ManagementService IIS-WMICompatibility IIS-LegacyScripts IIS-LegacySnapIn IIS-FTPServer IIS-FTPSvc IIS-FTPExtensibility TFTP TelnetClient MediaPlayback WindowsMediaPlayer Printing-Foundation-Features FaxServicesClientPackage Printing-Foundation-InternetPrinting-Client Printing-Foundation-LPDPrintService Printing-Foundation-LPRPortMonitor WorkFolders-Client SMB1Protocol SMB1Protocol-Client SMB1Protocol-Server SMB1Protocol-Deprecation SimpleTCP
    for %%a in (%features%) do dism /online /disable-feature /featurename:%%a
    echo Disabled Windows features
REM --------------------------------------------------
    echo Script Complete...
    timeout 5
    echo ------------------
    echo Checklist Changes:
    echo ------------------
    timeout 5
    echo Audit Policy Changes
    echo Security Policy Changes (Not All Changed)
    echo Automatic Updates
    echo Firewall Activation
    echo User Password Changes (Use userAlt.bat)
    echo Remote Desktop Deactivation
    echo Windows Features Deactivation
    echo Services Alterations
    echo Registry Required Edits
    echo DNS Editing
    timeout 5
    echo -----------------
    echo Note: Password Policy Was Discovered to be Unchangeable Via Scripting
REM --------------------------------------------------
    pause
    exit
