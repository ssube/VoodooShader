
# Voodoo Shader Framework
## version 0.4.0.183 - alpha

#### Contents:
1. Summary
2. Installation
3. Adapters
4. Running Voodoo
5. Reporting Bugs
6. Contact & Legal
7. Resources


### 1. Summary

The Voodoo Shader Framework is a set of support libraries designed to provide extensive shader 
integration into graphics applications, both before and after development.

Voodoo uses a system of core modules and adapters to provide the widest compatibility and best
performance possible across a wide range of applications and APIs. Voodoo supports both OpenGL and
DirectX, using nVidia's Cg language. In addition, Voodoo provides a second processing step to
provide shaders with easy access to other resources, especially textures.

The Voodoo core and supported shaders remain constant between target applications and adapters,
providing a unified set of features and single target for shaders.


### 2. Installation

The Voodoo system function using a core-adapter model. The Voodoo core handles most functions, and
game-specific adapters interface it with each game in detail. 

The Voodoo core comes in an install wizard to simplify installation. This wizard creates the
registry keys Voodoo needs to function properly. To install the core, simply run this wizard and
Voodoo will be installed to your system.


### 3. Adapters

After the Voodoo core is installed, you will need to install adapters for any game you want to use
Voodoo in. Various adapters are available for a number of Direct3D 8, 9 and OpenGL games.

Adapters can be found included with Voodoo or on the Voodoo Shader Framework site at:
    http://www.voodooshader.com

Adapters may or may not use install wizards. Each adapter will have documentation on how to install
and configure it. 


### 4. Running Voodoo

The precise behavior of Voodoo depends on the adapter and game it is being used with, but the most
common behavior is described here.

Voodoo will automatically hook into the game on launch, if it is properly installed. The Voodoo 
runtime will create a comprehensive log file in the game directory. Voodoo uses an XML configuration
file, contained in the game directory, and may use other resources in the game directory or Voodoo
root directory.

Debug builds will perform significant logging, potentially slowing the game down but providing much
information. Voodoo debug modules may be used in addition to traditional video API debuggers; they
are capable of intercepting both DirectX and OpenGL calls and logging the times, parameters and
results in some detail.


### 5. Reporting Bugs

Voodoo is in an alpha testing stage. It undoubtedly has bugs. If you run into a bug, please go 
to http://www.voodooshader.com and report the bug on the forum.

A bug tracker for all Voodoo elements, including shaders and adapters, will be deployed soon.


### 6. Contact & Legal

To contact the developer of the Voodoo project, please post on the Voodoo forum at:
    http://www.voodooshader.com
or direct emails to:
    peachykeen@voodooshader.com
    
Questions, comments, suggestions, reports, bugs, and almost anything else are welcome. If you have
any interest in testing or contributing to Voodoo, please get in touch as soon as possible.

You may also try to get in touch via the Bethesda Softworks forums or Bioware social boards, or on
IRC (nick peachykeen on chatspike, freenode and nwn2source).



Voodoo Shader Framework (c) 2010-2011 by Sean "peachykeen" Sube, All Rights Reserved.

The Voodoo Shader Framework, all associated binary or object files and source code is released 
under the terms and conditions of the GNU General Public License. See the included LICENSE file for
more details.

The Voodoo Shader Framework has absolutely no warranty, as described in the license document. Some
support may be provided, at the developer's discretion; please use the above contact methods to get
in touch.


### 7. Resources

This section has a few useful links for various Voodoo resources. Some of these may not have
perfect uptime, if they are being moved or tweaked, but most should be relatively reliable.

Main site & forums:
    http://www.voodooshader.com
    
API documentation:
    http://docs.voodooshader.com/core (replace the /core with a given module to find docs for that)
    
Bug tracker:
    http://tracker.voodooshader.com (may be tied into the forum at some point)
