General Notes on Version 0.2.4:

This release contains some major and minor bug-fixes. I also included several improvements:
- the slider-values are now editable (click on the value, enter a new number and press enter)
- a nonlinear fit can be done on one set as usual; if more then one source-set is selected the data of the different sets will silently be combined and the fit will be done on the combined sets (the combined set will not be visible, only the fit-result)
- there has been a bug in the filter-dialog, which made the interpolation the only usable option in the point extension (it was possible to select something else, but internally always interpolation was used); I removed the bug and included an option to extend the points with the first and last point before the first point and after the last one
- I included a new help-page especially for the extensions in QtGrace in comparison with Grace (I removed the respective entries in this readme) and I included a new example especially for data-filtering and edited some of the existing examples to better match the new window-layout and some of the new features. There is also a new example for the advanced scripting options
- I extended the command-window in order to execute all commands with different set-ids and included special command for regression and filtering and some special operations (see the new help-page concerning the QtGrace-extension for any details)
- I made the decimal separator selectable (',' or '.') throughout the whole Gui of QtGrace (internally '.' is used all the time) - please note, that this is now a strict setting: if you select ',' as separator you have to enter every number with ',' as separator as well and '.' is ignored.
- I extended the support for non-ascii-character by improving the usage of QtFonts (activate it in Preferences->Extra); you also have to set the encoding for saving in the Extra-Preferences (UTF-8 recommended) - this feature is the most complicated in the internal processes and may still contain some bugs, so be watchfull and save early and often; there is obviously a limited compatibility with the original Grace if you use this feature (details: see new help-page)
- some minor stuff I do not remember in every detail has been changed / improved (for example I included a bugfix from Grace 5.1.23 concerning the fit-results)

(Just an excuse for not updating QtGrace earlier or not having included more new features: The compiler on my Windows7-system stopped working after a regular Windows update. I tried several different things to get it working again, but there still remains an error in some basic library concerning a typedef with a pointer which I do not understand. I spent more time trying unsuccessfully to resolve this problem than I spent actually doing improvements on QtGrace. This made me very frustrated! Furthermore I tried to update QtGrace for the use with Qt5. This did also not work as expected and especially on Windows I could not get it to work properly. Therefore I dropped the support for Qt5 for the moment, although QtGrace should be prepared for it. This whole procedure also cost me valueable time and made me frustrated and therefore I was not as motivated as before. With Qt5.1 it seems to be easier to compile QtGrace - but I have not tested this any further (it works on Mac OS X 10.9 but only using XCode and not QtCreator).)

One further request (as always): If you find bugs or missing feature or if you have suggestions for improvements: Let me know immediately!
Thank you!

Warning: I changed some of the shortcut-keys in contrast to Grace: In order to use Crtl+Z for Undo I used Alt+Z for Zoom. In order to use Crtl+V for paste-operations I used Alt+V for viewport.
I hope this will be helpful to someone. Don't hesitate to report bugs/suggestions/wishes.


General Notes on QtGrace:

Dear User or Developer;

Thank you for your interest in my Qt-version of Grace!
In case you do not know already: Grace is a program to plot data and analyze it.
Grace is based on xmgrace / xmgr and uses the motif-framework for the graphical user interface.
The Grace-website is: http://plasma-gate.weizmann.ac.il/Grace/
QtGrace is based on grace-5.1.22 but uses the Qt-framework (formerly by Nokia, now Open-Source) as a graphical user interface instead (Qt and their respective logos were formerly trademarks of the Nokia Corporation but are now Open-Source as far as I know).
Although I have tested QtGrace for quite a while now there may still be some problems / bugs present. Therefore give it a try, but be sure to read the Grace documentation (and save often).

I hope this will be useful to someone else as well (I like my Qt-version of Grace :-) ).

My motivation: I had problems using Grace on different operating systems. Since I had written some applications using the Qt-framework (which can be found on http://qt-project.org/) I decided to modify the latest stable version of Grace (5.1.22) and replace all dialogs by equally looking Qt-replacements. I wanted to conserve the "look and feel" of Grace 5.1.22 because I wanted all Grace-users to be able to work with QtGrace immediately without having to adjust their workflow.

I altered most of the source-files and therefore changes in Grace have to be implemented manually. Since there seems to be no major development around Grace-5.1.22 I believe that this approach is acceptable for now. All development on Grace seems to be focused on version 6 (or 5.99) which has a fundamentally different internal architecture. QtGrace is not compatible with Grace 5.99. There are other implementations of Grace 5.99 with newer graphic-libraries (like Qt and GTK) - just do a websearch if you want these.

This version 0.2.4 of QtGrace is capable of nearly all of the operations Grace 5.1.22 is able to do. Some minor functions are missing or work only on some platforms (see below). I have inserted a few extra functions I found helpful (see help-page on QtGrace-Extensions). There are a few minor known problems (see below).

The internal procedures are the ones from Grace 5.1.22 (internal data management, calculations and so on). I have only recreated the user interface in Qt which makes it possible to compile QtGrace natively (!) on Windows, Mac OS X and Linux. You can load your old Grace-project files (usual extension: '.agr'). With Qt it is also possible to easily translate the user interface and make the language of the interface selectable (I have made an attempt for a test-translation to German. The file is included but very far (!!) from being complete. If there is someone out there willing to complete the german translation or generate a translation to any other language feel free to do so using the "Linguist" program included in the Qt-installation. If you send me the translation I will include it into the downloadable zip-file.).

Don't alter the structure or remove content from the folders of QtGrace. The executable relies on the relative positions of the files it needs. Just use symbolic links to start QtGrace from your desktop.

I am always interested in hearing your opinion on QtGrace or in your bug-reports. Suggestions for improvements are welcome as well!
To contact me please use email: andreas.f.winter@web.de

I hope you enjoy QtGrace!

Best regards
Andreas Winter (andwin)


Copyright and License-Terms:

Copyright (C) 2008-2013 by Andreas Winter
andreas.f.winter@web.de

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the
Free Software Foundation, Inc.,
59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.


How to build QtGrace:

QtGrace is based on the Qt-Framework which has to be obtained via http://qt-project.org/.
Please use at least version 4.6 (or newer; I am not sure about Qt5 or higher, it should work but I have not tested it extensively - sorry) of Qt to compile QtGrace!
Please install the Qt-SDK which includes the Qt-Creator IDE. If you are an experienced developer, you may also be able to compile QtGrace without the Qt-Creator, but I will not describe how to do so here. (On Mac OS X 10.9 with Qt5.1 you may have to use XCode 5 or you will have no debugger. In order to do so you have to generate a XCode-project file using qmake - see help-pages of Qt.)
Since you have extracted this folder you may notice, that there are some sub-folders. It is important for QtGrace to keep the structure of this folder in order to find some files at runtime. I have also included the original source-code-zip-file from grace-5.1.22 as a tar-archive. You will not have to extract it. This is included just for completeness and for developers use in case someone needs it. 
I assume in the following part, that you have installed Qt and you have got a fully functional Qt-Creator. 
To compile QtGrace: 
First step: Open Qt-Creator and do File->"Open File or Project..." (you may have to specify the file format as "Qt Project file (*.pro)"). Select the file "src.pro" in the 'src'-sub-folder. On the left hand side of the IDE-window you should now be able to see all source-files that are part of QtGrace.
Second step: At the left hand side of the Qt-Creator-window you find some icons. Click on the "Projects"-icon and make sure a suitable path for the Qt-Installation is set at the general section. (You may have to consult the Qt-Manual on some systems on how to do so correctly. Especially in newer versions of QtCreator you have to prepare the compiler settings in a special way, so please consult the Qt-documentation.) You have two options now: either set the build-path in the general build settings to the 'bin'-subfolder or remember the build path that has been set automatically and copy the executable after the compilation to the 'bin'-subfolder. You will also have to set the destination of the executable to be executed from within Qt Creator in the settings in order to run QtGrace after the compilation from within Qt Creator.
Third step: Click the 'Edit'-icon on the left side. Click on the 'Build All'-Icon (the one with the hammer in the left hand lower corner). This will initiate the process of compilation and may take a while. You may also see a few thousand warnings which can be ignored (hopefully) but there should be no errors (if you get errors, report them to me and I will provide information on how to solve this problem). (Special platform specific notes: see below)
Fourth step: Everything has been compiled now and you should be able to start QtGrace. Either you double-click on the qtgrace icon (or qtgrace.exe) in the bin-folder or you set the execution path in the project settings to this file and run it from within the Qt Creator.
Done! 

On Windows:
You have to set the path to the Qt-dlls in your PATH-variable or copy the dlls to the bin-folder (look for 'QtCore4.dll' and use the path thereof). Otherwise you can only run qtgrace from within the QtCreator. (You need the following dlls: QtCore4.dll, QtGui4.dll, QtNetwork4.dll, mingwm10.dll, libgcc_s_dw2-1.dll; The path where they can be found is usually something like 'C:\Qt\2010.05\qt\bin\' or 'C:\Qt\4.8.3\bin\'.)

On Linux:
If you continue getting messages like: invalid viewport coordinates --> please contact me.

On Mac OS X:
Executables are organized as disguised folders on such systems. Therefore after compilation in order to work in a MacOS-way you have to open the package-contents of qtgrace (right-click on qtgrace in the 'bin'-folder and select 'show package contents'). Then go to 'Contents'. Copy the contents of the original unzipped folder except the 'bin'-subfolder and the grace-5.1.22.tar.gz into this 'Contents-folder'. Copy the contents (except 'qtgrace') of the 'bin'-subfolder into the 'MacOS'-folder. Now you should be able to execute qtgrace by clicking on qtgrace in the 'bin'-subfolder. It should also be possible to move/copy qtGrace in every folder you like just like any other MacOS-application (put it for example in your Applications-folder). 


Known Problems:

- Printing directly to a printer via the native printer-dialog is possible but I have not tested it extensively (but it seem to work in an acceptable way). Most settings are not mirrored in the native dialog. Direct printing without the native printer-dialog is only possible on systems that support the 'lpr' printing command (or any other printer-command like 'kprinter' on some linux-systems) and accept postscript files (i.e. a system command like 'lpr file.ps' will actually print the contents of 'file.ps' on the standard-(line)-printer). Printing to files is always possible. You have to physically print the file-contents via a different/external program (like OpenOffice). You may also copy the whole page in the clipboard via View->PageSetup and paste it into your external program. Printing on Mac-system without a physical printer present can be tricky if you use Qt5 - sorry (previously the pdf-option was always accessable)
- On some Linux-systems the scrollbars are not displayed correctly but they still stay usable (I don't know why, yet. Seems to be a Qt-problem...; I also don't know whether this bug is still present, since I have not seen it in a while).
- On Windows systems the usage of the internal html-viewer for help-files has been disabled (the option is present, but internally discarded). I had to do this, because the webkit- and phonon-packages of Qt don't seem to work together on Windows. (For developers who want to give it a try: remove the preprocess directives in 'replacement_main.cpp' at lines 1736, 1739, 1741 and 1750.) This may also be true for some Linux-systems (In this case remove everything between the directives mentioned).
- Pipes not working on Windows.
- The switching between Grace-fonts and QtFonts is not adviceable. It is possible in most cases, but especially for the Symbol-font problems are to be expected - sorry. This has several reasons especially with the ordering of characters in the T1-Symbol-font in Grace and UTF-8-fonts. At the moment I do not know how to solve this problem.
- Loading files (especially old ones) relies heavily on the encoding setting in the Extra-Preferences-dialog. For loading example-files it is often adviceable to use the Latin1-encoding (or something similar). Newer file should contain the encoding used during saving and should be loadable without problems.


Things that work in Grace, but not in qtGrace:

- I have rewritten the spreadsheet-editor. Therefore it may not work as you are used to it (or maybe you won't even notice).
- I have written a simple text-editor for creating and editing sets. On Unix the default in Grace is to open 'vi' ('Create new->In text editor'). This option is not portable (at least not in a way I would like). I used an internal text-editor because this seems to be comfortable enough (suggestions for improvements are welcome).
- The pipes may work a bit differently in QtGrace, because I could not include the 'pipe-contents-changed' signal into the main-application loop like Grace does. Pipes also do not work on Windows, yet.


Additions to qtGrace (not present in Grace): see help-page on QtGrace-extensions


Pipes (I include this here and not in the help-file, because I do not want to advertise this feature due to it's incompleteness):

The use of named pipes via the '-npipe' option or starting QtGrace in a pipe via '-pipe' or opening a pipe from Data->Import->Ascii is supported now. The drawback at the moment is: This does not work on Windows, yet. The usage of pipes is a bit different on Windows and I have to find out how to reliably use them there, sorry. Any helpful advice is welcome here! 
For the pipes (also called "Real time input") I included a new window/widget. In 'Window->Monitor real time inputs...' you can see which pipes are currently monitored for input. You can also activate or deactivate the monitoring here via a checkbox. Internally in order not to block the other operations of QtGrace the input via pipes is not real time but in specified intervals. The time between updates of the real time input can be set here. I set it to 1 second by default. You can also open and close pipes via this dialog. I hope this is helpful for some people.

Any suggestions for improvements, any bug reports or other comments are welcome.
Thank you.


Version history:
v0.1.2		initial release on sourceforge.net
-		I made a few silent updates to remove some more or less minor bugs
v0.1.2a		bug-fix-release: major bug in "Arrange graphs"-dialog removed; adjustments on the sliders (especially their resize-behavior); changes in the routine for loading gracerc.user: this file is searched first in the working directory, then in the directory of the executable and last in the users home directory; special-tick-marks-problems solved
v0.1.3		bug-fixes: the transformation between word- and viewport-coordinates has been included in the objects-dialogs (boxes, ellipses ...); some of the dialogs have text-lines in which usually numbers are entered (like the start- and stop-values in the axis-dialog), now more of them accept parametric values as well (like the constant PI), this is a feature of Grace that has been temporarily deactivated during the development phase of QtGrace and I forgot to reactivate it, this has now been done; the native-printer-dialog now uses the set page-orientation as a default; 
		Undo/Redo included (partially and experimental)
v0.1.3a		hotfix for a severe memory-bug in the undo-function; undo is still unfinished and experimental
v0.1.4		first release with complete Undo/Redo; several bug-fixes of mostly minor stuff
v0.1.5		several bug fixes and improvements on the repaint-system (which is especially necessary for newer Qt-versions)
v0.1.6		some bug fixes; addition of immediate updates and support for simple LaTeX-type text-input
v0.1.7		minor bug fixes; addition of copy/paste and drag/drop; minor improvements on automatic script generation
v0.1.8		bug fixes; several improvements; addition of color management; filling the area between sets; rotation of boxes and ellipses
v0.1.9		bug fixes(some severe ones); transparency added for printing in files; spread-sheet editor rewritten
v0.2		Page-zoom, selecting multiple graphs for zooming, reactivating pipes (non-Windows-systems)
v0.2.1		Support for csv-import
v0.2.2		'-free' option and 'immediate update' reactivated, experimental support for non-latin-characters and different fonts;support for multiple files in binary import and the use of a trigger channel ; bug fixes
v0.2.3		minor bugfixes; print-command saved in preferences; selection of odd or even set-ids; addition of fit-button for page-zoom-slider; minor changes to improve UTF-8-support in file-paths
v0.2.4		several bugfixes (major and minor); further improvement in UTF-8-support (very extensive internal changes); made decimal-separator selectable throughout the whole QtGrace-Gui; improvement in the script-generation (execution of scripts with automatically replaced set-ids); slider-value made editable; QtGrace-help-page; QtGrace-examples

Have fun using QtGrace!

Best regards
Andreas Winter (andwin)
 
