# shade-x
Windows 10/11 system tray application to shade / rollup a window with a double click on the titlebar

## Building

Install:
- [Visual Studio Community 2019](https://visualstudio.microsoft.com/vs/older-downloads/)
- [Git for Windows](https://gitforwindows.org/)
- [TortoiseGit](https://tortoisegit.org/)
- [PuTTY](https://www.putty.org/)
- [Qt](https://www.qt.io/download-thank-you?os=windows&hsLang=en) (5.15.2)
- [NSIS](https://nsis.sourceforge.io/Download)

Clone the repository.  
Start Qt Creator and open 'Shade-X.pro'.  
Select the kit for MSC2019 64bit.  
Build the application.  
Compile the NSI script in the dist directory using the nullsoft scriptable installer.  
Use the resulting installer package to install the application on a system.  
