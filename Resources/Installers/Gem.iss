; Inno Setup script for Voodoo Shader Framework
; Copyright (c) 2010 by Sean Sube

[Setup]
AppID={{B657ACEC-FAEF-4eef-B7ED-6A25ED68FB96}
AppName=Voodoo/Gem
AppVersion=0.1
AppPublisher=Sean Sube (peachykeen)
AppPublisherURL=http://www.voodooshader.com/
AppSupportURL=http://www.voodooshader.com/
AppUpdatesURL=http://www.voodooshader.com/
DefaultDirName=C:\VoodooShader
DefaultGroupName=Voodoo Shader Framework/Gem
OutputBaseFilename=Voodoo_Gem
Compression=lzma/Max
SolidCompression=true
MinVersion=0,5.1
AppCopyright=Copyright © 2010 by Sean Sube
AppVerName=Voodoo Shader Framework version 0.1
PrivilegesRequired=none
AllowCancelDuringInstall=true
ChangesEnvironment=false
AllowUNCPath=false
ShowLanguageDialog=no
DisableDirPage=yes
AlwaysShowDirOnReadyPage=false
VersionInfoProductName=Voodoo/Gem
VersionInfoProductVersion=0.1.5.69
VersionInfoTextVersion=0.1.1.69
VersionInfoVersion=0.1.1.69

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Files]
Source: M:\VoodooShader\bin\Gem_Redirect.dll; DestDir: {reg:HKLM\SOFTWARE\Bethesda Softworks\Morrowind,Installed Path|C:\Morrowind}; DestName: d3d8.dll; Components: adapter; 
Source: M:\VoodooShader\bin\Gem_Redirect_d.dll; DestDir: {reg:HKLM\SOFTWARE\Bethesda Softworks\Morrowind,Installed Path|C:\Morrowind}; DestName: d3d8.dll; Components: adapter/debug; 
Source: M:\VoodooShader\Resources\Shaders\test.cgfx; DestDir: {reg:HKLM\SOFTWARE\Bethesda Softworks\Morrowind,Installed Path|C:\Morrowind}; Components: adapter; 
Source: M:\VoodooShader\Resources\Readmes\README_GEM; DestDir: {reg:HKCU\SOFTWARE\VoodooShader,Path|{reg:HKLM\SOFTWARE\VoodooShader,Path|C:\VoodooShader}}\docs; Components: adapter;
Source: M:\VoodooShader\bin\Voodoo_Gem.dll; DestDir: {reg:HKCU\SOFTWARE\VoodooShader,Path|{reg:HKLM\SOFTWARE\VoodooShader,Path|C:\VoodooShader}}\bin; Components: adapter; 
Source: M:\VoodooShader\bin\Voodoo_Gem_d.dll; DestDir: {reg:HKCU\SOFTWARE\VoodooShader,Path|{reg:HKLM\SOFTWARE\VoodooShader,Path|C:\VoodooShader}}\bin; Components: adapter/debug; 

[Types]
Name: Full; Description: Full; 
Name: Debug; Description: Debug; 
Name: Custom; Description: Custom; Flags: IsCustom; 

[Components]
Name: adapter; Description: Voodoo/Gem Adapter; Flags: fixed; Types: Full Debug Custom;
Name: adapter/debug; Description: Debug Files; Types: Debug; 
Name: registry; Description: Registry Integration; Flags: fixed; Types: Full Debug Custom;
Name: registry/user; Description: Current User Only; Flags: exclusive;
Name: registry/machine; Description: All Users; Check: IsAdminLoggedOn; Flags: exclusive; Types: Full Debug Custom;

[Registry]
Components: registry/user; Root: HKCU; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Gem; ValueData: "0.1.5.69"; Flags: UninsDeleteKeyIfEmpty UninsDeleteValue; 
Components: registry/machine; Root: HKLM; SubKey: SOFTWARE\VoodooShader; ValueType: string; ValueName: Gem; ValueData: "0.1.5.69"; Flags: UninsDeleteKeyIfEmpty UninsDeleteValue; 

[Run]
Components: adapter; Filename: notepad.exe; Parameters: {app}\docs\README_GEM; WorkingDir: {app}; Description: Open the Gem Readme; Flags: PostInstall NoWait ShellExec;

[Icons]
Name: {group}\Uninstall; Filename: {uninstallexe};
Name: {group}\Readme; Filename: notepad.exe; Parameters: {app}\docs\readme_gem;

[Code]
var
  Voodoo: Boolean;
  MW: Boolean;

function InitializeSetup(): Boolean;
begin
  Voodoo := RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\VoodooShader') or RegKeyExists(HKEY_CURRENT_USER, 'SOFTWARE\VoodooShader');
  MW := RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE\Bethesda Softworks\Morrowind');
  if ( MW and Voodoo  ) then
  begin
    Result := True;
  end else
  begin
    MsgBox('Your system reports that the Voodoo Shader Framework and/or Morrowind are not installed.%OD%OAThis program requires both.%OD%OAPlease install them before continuing.', mbError, MB_OK);
    Result := False;
  end;

  Result := True;
end;

