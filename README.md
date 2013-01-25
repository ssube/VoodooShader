## Voodoo Shader Framework
### version 0.5.6 - alpha
#### [VoodooShader.com](https://www.voodooshader.com)

-----

### 0. Build Status
<a href="http://build.voodooshader.com/teamcity/viewType.html?buildTypeId=bt2&tab=buildTypeStatusDiv&guest=1"><img src="http://build.voodooshader.com/teamcity/app/rest/builds/buildType:(id:bt2)/statusIcon"/></a>

### 1. Summary

Voodoo Shader improves and extends graphics in old and new games. It's fast, stable, open-source, and runs on ATi/AMD and 
nVidia with DirectX or OpenGL. Better yet, it works with many games, allowing you to share your favorite effects with all 
your favorites and tweak them how you want.


### 2. Installation

To install the Voodoo framework, you need to download and run the Voodoo Shader UI. This will check for and create 
installation information and allow you to download and install packages, as well as view a list of available packages. 

If you prefer not to use the UI package manager, you may download full archives from the Voodoo Shader website.

Once the UI or full archive has been downloaded, you will need to install the framework package. In the UI, this involves
opening the package manager and clicking install, then entering the following information:

* Origin: https://www.voodooshader.com/packages
* Path: framework

You should also install any adapters needed for particular games; supported games, the adapters needed, and a package list 
may be found at:

* https://github.com/peachykeen/VoodooShader/wiki/Game-List
* https://github.com/peachykeen/VoodooShader/wiki/Package-List


### 3. Modules & Adapters

After the Voodoo framework is installed, you will need to install adapters for any game you want to use Voodoo in. Various
adapters are available for a number of Direct3D 8, 9 and OpenGL games.

Adapters and other modules may be downloaded or updated from the Voodoo UI. These are listed in the download manager window.

Third-party modules may be downloaded with the UI by giving the proper path and package name. _Care should be taken when 
using modules from third parties, as they may be unsafe. No support is given for third-party modules, nor do Voodoo Shader 
license terms apply. Make sure you read all documentation carefully and don't use modules from untrusted sources._


### 4. Running Voodoo

Voodoo has two major modes of operation:

* The primary and preferred mode is for the Voodoo UI to be running. This will automatically load into any applications for
  which a hook is present and active. Hooks may be managed within the UI.

* The secondary method is to drop the Voodoo Shader intercept DLL into the game directory. This is significantly more 
  limited, but does not require the UI to be running. Hooks must still be defined for the application.

Once the game has been hooked, Voodoo will automatically load itself and all plugins given in the config file. Behavior
after this depends on the modules used.


### 5. Reporting Bugs

Voodoo is in an alpha testing stage. It undoubtedly has bugs. If you run into a bug, please add a report to the bug tracker
at:

* [http://tracker.voodooshader.com](http://tracker.voodooshader.com)

Any information that can be included, especially a log, is beneficial. Full logs should be posted at
[github:gist](https://gist.github.com/) whenever possible and linked in the bug report.


### 6. Contact

For help and support (or questions and comments), please post on the Voodoo forum or stop by the IRC channel:

* [http://forum.voodooshader.com](http://forum.voodooshader.com)
* [irc://irc.chatspike.net/voodoo](irc://irc.chatspike.net/voodoo)

To directly contact the developer, please send an email to:

* [peachykeen@voodooshader.com](mailto:peachykeen@voodooshader.com)

Questions, comments, suggestions, reports, bugs, and almost anything else are welcome.

If you would like a game or application supported by Voodoo, please file a request issue with the title and as much
information as you can.

If you have any interest in helping test or contributing code to Voodoo, please get in touch by email or IRC.


### 7. Legal

Voodoo Shader Framework &copy; 2010-2012 by Sean Sube, All Rights Reserved.

The Voodoo Shader Framework, all associated binary or object files and source code is released under the terms and
conditions of the GNU Lesser General Public License, version 3. See the included LICENSE file for more details.

The Voodoo Shader Framework has absolutely no warranty, as described in the license document. Some support may be provided
at the discretion of the developer; please use the above contact methods to get in touch.
