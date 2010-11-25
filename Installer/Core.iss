; Inno Setup script for Voodoo Shader Framework
; Copyright (c) 2010 by Sean Sube

[Setup]
AppId={{F954DF67-68BD-4633-88EC-CDE9AF0C1253}
AppName=Voodoo Shader Framework
AppVersion=0.1
AppPublisher=Sean Sube (peachykeen)
AppPublisherURL=http://www.voodooshader.com/
AppSupportURL=http://www.voodooshader.com/
AppUpdatesURL=http://www.voodooshader.com/
DefaultDirName=C:\VoodooShader
DefaultGroupName=Voodoo Shader Framework
OutputBaseFilename=setup
Compression=lzma
SolidCompression=true
MinVersion=,5.01.2600
AppCopyright=Copyright © 2010 by Sean Sube
AppVerName=Prototype
PrivilegesRequired=none
AllowCancelDuringInstall=false
ChangesEnvironment=true
AllowUNCPath=false
ShowLanguageDialog=no

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]

[Files]
Source: J:\InnoSetup\Examples\MyProg.exe; DestDir: {app}; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: {group}\My Program; Filename: {app}\MyProg.exe

[Run]
Filename: {app}\VSF_Readme.html; Description: View Readme; Flags: nowait postinstall skipifsilent; Components: ; Languages: ; WorkingDir: {app}
Filename: http://www.voodooshader.com; WorkingDir: {app}; Description: View Voodoo Shader website; Flags: nowait postinstall skipifsilent shellexec

[Registry]
Root: HKLM; Subkey: SYSTEM\CurrentControlSet\Control\Session Manager\Environment; ValueType: expandsz; ValueName: Path; ValueData: "{olddata};{app}\bin"; Tasks: ; Languages: ; Components: path/allusers
Root: HKCU; Subkey: Environment; ValueType: expandsz; ValueName: Path; ValueData: "{olddata};{app}\bin"; Tasks: ; Languages: ; Components: path/singleuser

[Components]
Name: path; Description: Path Integration; Types: full
Name: path\singleuser; Description: Current User Only; Types: full; Flags: exclusive
Name: path\allusers; Description: All Users; Flags: exclusive; Types: full; Check: IsAdmin

[Code]
function IsAdmin(): boolean;
begin
  Result := IsAdminLoggedOn();
end;
[Dirs]
Name: {app}\bin; Components: ; Languages: 
