; Inno Setup script for Voodoo Shader Framework
; Copyright (c) 2010 by Sean Sube

[Setup]
AppID={{F954DF67-68BD-4633-88EC-CDE9AF0C1253}
AppName=Voodoo Shader Framework
AppVersion=0.1
AppPublisher=Sean Sube (peachykeen)
AppPublisherURL=http://www.voodooshader.com/
AppSupportURL=http://www.voodooshader.com/
AppUpdatesURL=http://www.voodooshader.com/
DefaultDirName=C:\VoodooShader
DefaultGroupName=Voodoo Shader Framework
OutputBaseFilename=Voodoo_Core
Compression=lzma/Ultra
SolidCompression=true
MinVersion=0,5.1
AppCopyright=Copyright © 2010 by Sean Sube
AppVerName=Voodoo Shader Framework version 0.1
PrivilegesRequired=none
AllowCancelDuringInstall=true
ChangesEnvironment=false
AllowUNCPath=false
ShowLanguageDialog=no
LicenseFile=LICENSE
VersionInfoProductName=Voodoo Shader Framework
VersionInfoProductVersion=0.1.7.68
VersionInfoTextVersion=0.1.1.68
VersionInfoVersion=0.1.1.68

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: M:\VoodooShader\bin\cgD3D9.dll; DestDir: {app}\bin; Components: core; 
Source: M:\VoodooShader\bin\cg.dll; DestDir: {app}\bin; Components: core;
Source: M:\VoodooShader\bin\Voodoo_Core.dll; DestDir: {app}\bin;  Components: core; 
Source: M:\VoodooShader\bin\Voodoo_DX9.dll; DestDir: {app}\bin;  Components: core; 
Source: M:\VoodooShader\bin\Voodoo_DX89.dll; DestDir: {app}\bin;  Components: core; 
Source: M:\VoodooShader\bin\Voodoo_Core_d.dll; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\bin\Voodoo_DX9_d.dll; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\bin\Voodoo_DX89_d.dll; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\bin\Voodoo_Core_d.pdb; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\bin\Voodoo_DX9_d.pdb; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\bin\Voodoo_DX89_d.pdb; DestDir: {app}\bin; Components: core/debug; 
Source: M:\VoodooShader\CREDITS; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\LICENSE; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\voodoo.png; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\readme.html; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\readme.css; DestDir: {app}\docs; Components: core; 

[Icons]
Name: {group}\Website; Filename: http://www.voodooshader.com; Components: startmenu; 
Name: {group}\Voodoo Folder; Filename: {app}; WorkingDir: {app}; Components: startmenu; 
Name: {group}\Uninstall; Filename: {uninstallexe}; WorkingDir: {app}; Components: startmenu; 
Name: {group}\Readme; Filename: {app}\docs\readme.html; Components: startmenu; 

[Run]
Filename: http://www.voodooshader.com; WorkingDir: {app}; Description: View Voodoo Shader website; Flags: nowait postinstall skipifsilent shellexec Unchecked; Components: core; 
Filename: {app}\Docs\readme.html; Flags: PostInstall NoWait ShellExec SkipIfSilent; Components: core; WorkingDir: {app}\docs; 

[Registry]
Root: HKCU; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Path; ValueData: "{app}"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/user; 
Root: HKLM; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Path; ValueData: "{app}"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/machine; 

[Components]
Name: core; Description: Voodoo Core; Flags: fixed; Types: Full Debug Custom; 
Name: core/debug; Description: Debug Data; MinVersion: ,5.1; Types: Debug; 
Name: registry; Description: Registry Integration; Types: Full Debug Custom; Flags: fixed; 
Name: registry/user; Description: For this user only; Flags: exclusive; Types: Full Debug Custom; 
Name: registry/machine; Description: For all users; Flags: exclusive; Types: Full Debug Custom; Check: IsAdminLoggedOn;
Name: startmenu; Description: Start Menu Shortcuts; Types: Full Debug Custom; 

[Dirs]
Name: {app}\bin; Components: core; 
Name: {app}\docs; Components: core;

[Types]
Name: Full; Description: Full (all core files);
Name: Debug; Description: Debug (debug files);
Name: Custom; Description: Custom; Flags: IsCustom;
