; Inno Setup script for Voodoo Shader Framework
; Copyright (c) 2010 by Sean Sube

[Setup]
AppID={{1d3d7fb4-6f32-11e0-8ac0-005056c00000}
AppName=Voodoo Shader Framework
AppVersion=0.10.3
AppPublisher=Sean Sube
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
AppVerName=Voodoo Shader Framework version 0.2
PrivilegesRequired=none
AllowCancelDuringInstall=true
ChangesEnvironment=false
AllowUNCPath=false
ShowLanguageDialog=no
LicenseFile=LICENSE
VersionInfoProductName=Voodoo Shader Framework
VersionInfoProductVersion=0.10.3.206
VersionInfoTextVersion=0.10.3.206
VersionInfoVersion=0.10.3.206

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]

[Components]
Name: core; Description: Voodoo Core; Flags: fixed; Types: Full Debug Custom; 
Name: core/debug; Description: Debug Data; MinVersion: ,5.1; Types: Debug; 
Name: registry; Description: Registry Integration; Types: Full Debug Custom; Flags: fixed; 
Name: registry/user; Description: For this user only; Flags: exclusive; Types: Full Debug Custom; 
Name: registry/machine; Description: For all users; Flags: exclusive; Types: Full Debug Custom; Check: IsAdminLoggedOn;
Name: startmenu; Description: Start Menu Shortcuts; Types: Full Debug Custom; 

[Types]
Name: Standard; Description: Standard (Core files);
Name: Debug; Description: Debug (Core debug files);
Name: Custom; Description: Custom; Flags: IsCustom;

[Dirs]
Name: {app}\bin; Components: core; 
Name: {app}\docs; Components: core;

[Files]
; NOTE: Don't use "Flags: ignoreversion" on any shared system files
Source: M:\VoodooShader\bin\cg.dll; DestDir: {app}\bin; Components: core;
Source: M:\VoodooShader\bin\Voodoo_Core.dll; DestDir: {app}\bin;  Components: core; Flags: regserver 32bit; 
Source: M:\VoodooShader\bin\Voodoo_Core_d.dll; DestDir: {app}\bin;  Components: core\debug; Flags: regserver 32bit; 
Source: M:\VoodooShader\docs\voodoo.png; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\voodoo.css; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\readme.html; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\credits.html; DestDir: {app}\docs; Components: core; 
Source: M:\VoodooShader\docs\license.html; DestDir: {app}\docs; Components: core; 

[Icons]
Name: {group}\Website; Filename: http://www.voodooshader.com; Components: startmenu; 
Name: {group}\Voodoo Folder; Filename: {app}; WorkingDir: {app}; Components: startmenu; 
Name: {group}\Uninstall; Filename: {uninstallexe}; Components: startmenu; 
Name: {group}\Readme; Filename: {app}\docs\readme.html; Components: startmenu; 

[Run]
Filename: http://www.voodooshader.com; WorkingDir: {app}; Description: Voodoo Shader website; Flags: nowait postinstall skipifsilent shellexec Unchecked; Components: core; 
Filename: {app}\Docs\readme.html; Flags: PostInstall NoWait ShellExec SkipIfSilent; Components: core; WorkingDir: {app}\docs; 

[Registry]
Root: HKCU; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Path; ValueData: "{app}"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/user; 
Root: HKCU; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Version; ValueData: "0.10.3.209"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/user; 
Root: HKLM; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Path; ValueData: "{app}"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/machine;
Root: HKLM; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Version; ValueData: "0.10.3.209"; Flags: CreateValueIfDoesntExist UninsDeleteKeyIfEmpty UninsDeleteValue; Components: registry/machine; 

