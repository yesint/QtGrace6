General Notes on Version 0.2.5:

This release contains major and minor bug-fixes. I also included several improvements:
- rework of the binary import (this makes it possible to import binary files by drag-and-drop), see in the Qt-Extensions-help-file for an explanation on how it works
- additions to the regions-dialog (more possibilities to edit regions): the region-master: activate and deactivate regions, edit regions and edit the way how regions are displayed
- additions to the commands-dialog to extend the scripting possibilities (linear regression, creating of new sets and graphs)
- increased the number of fit-parameters to 20 and a dialog to edit the parameter values; please remember that all parameter a0-a19 are usable as general purpose variables during all calculations
- redesign of the file-output-dialog, separation of file-output from physical printer, adding a quick-selection-options to set the resolution
- added the possibility to use libHaru (for pdf-output) and libFFTW3 (for better fourier-transformation) - either as dlls or statically compileable into QtGrace (see below)
- further extension of the preferences dialog (many things are now user-adjustable)
- file-import-selection-lines accept drag-and-drop now
- dragging-and-dropping a Grace-project-file (agr) into the QtGrace-window now opens a separate dialog to let you either select to open the file or just extract single sets from the file
- in data-set-operations there is now the possibility to restrict sets to regions and to swap columns of a set (like swap X- and Y-axis)
- extension of the feature-extraction-dialog (more features like special value crossing settings)
- project description now visible in the open-projects-dialog and can be edited in the plot-appearance-dialog; the open-project-dialog now also shows how many graphs and sets there are in a selected project file and you can open a dialog that lets you extract single sets from the file
- apart from the time-stamp you can now add a file-path-stamp to the project
- you also can change the global font-size by a multiplier in the plot-appearance dialog
- further options for placing the legend-box (manual and automatic alignment with the graphs borders)
- changed the apply-to-option for the axis-dialog (the apply-button only applies the settings to the current axis, there is now a separate button if you want to apply settings to different axis as well - just for safety)
- addition of QTcpSocket-support for remote control (by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB), see qtclient-example in example folder, documentation still very basic
- added a new dialog to be able to determine intersection points between sets as well as intersection angles (Geometric evaluation...)
- added a new option in the preferences: "QtGrace-Behavior"->"General Behavior like" QtGrace or Grace; at the moment this only effects the question that is asked when QtGrace is closed and some changes have not been saved. With "Grace" the question is "Exit losing unsaved changes?" with QtGrace it is "Content of current project changed! Save project?" (Beware of the changed meaning!). The question now has the additional option to save the changes with a Save-button.
- some minor stuff I do not remember in every detail has been changed / improved

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
Although I have tested QtGrace for quite a while now there may still be some problems / bugs present. Therefore give it a try, but be sure to read the Grace documentation (and save often) - and tell me when you find a bug so that I may remove it.

I hope this will be useful to someone else as well (I like my Qt-version of Grace :-) ).

My motivation: I had problems using Grace on different operating systems. Since I had written some applications using the Qt-framework (which can be found on http://www.qt.io/) I decided to modify the latest stable version of Grace (5.1.22) and replace all dialogs by equally looking Qt-replacements. I wanted to conserve the "look and feel" of Grace 5.1.22 because I wanted all Grace-users to be able to work with QtGrace immediately without having to adjust their workflow.

I altered most of the source-files and therefore changes in Grace have to be implemented manually. Since there seems to be no major development around Grace-5.1.22 I believe that this approach is acceptable for now. All development on Grace seems to be focused on version 6 (or 5.99) which has a fundamentally different internal architecture. QtGrace is not compatible with Grace 5.99. There are other implementations of Grace 5.99 with newer graphic-libraries (like Qt and GTK) - just do a websearch if you want these.

This version 0.2.5 of QtGrace is capable of nearly all of the operations Grace 5.1.22 is able to do. Some minor functions are missing or work only on some platforms (see below). I have inserted a few extra functions I found helpful (see help-page on QtGrace-Extensions). There are a few minor known problems (see below).

The internal procedures are the ones from Grace 5.1.22 (internal data management, calculations and so on). I have only recreated the user interface in Qt which makes it possible to compile QtGrace natively (!) on Windows, Mac OS X and Linux. You can load your old Grace-project files (usual extension: '.agr'). With Qt it is also possible to easily translate the user interface and make the language of the interface selectable (I have made an attempt for a test-translation to German. The file is included but very far (!!) from being complete. If there is someone out there willing to complete the german translation or generate a translation to any other language feel free to do so using the "Linguist" program included in the Qt-installation. If you send me the translation I will include it into the downloadable zip-file.).

Don't alter the structure or remove content from the folders of QtGrace. The executable relies on the relative positions of the files it needs. Just use symbolic links to start QtGrace from your desktop.

I am always interested in hearing your opinion on QtGrace or in your bug-reports. Suggestions for improvements are welcome as well!
To contact me please use email: andreas.f.winter@web.de

I hope you enjoy QtGrace!

Best regards
Andreas Winter (andwin)


Copyright and License-Terms:

Copyright (C) 2008-2015 by Andreas Winter
andreas.f.winter@web.de
QtGraceTcpServer- and -Client-code (C) 2014-2105 by by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB

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

libharu and fftw3 are included unmodified to be easily accessable. See QTGRACE_EXTENSIONS.html for further informations and copyright notice.
For the recompiled releases for Windows and MacOSX I include dlls and frameworks that are part of Qt. These files are under the same copyright as the Qt-library. I included the files for convenience unmodified.

How to build QtGrace:

QtGrace is based on the Qt-Framework which has to be obtained via http://www.qt.io/.
Please use at least version 4.6 of Qt to compile QtGrace (Qt5.5 or higher, is recommended, especially for QTcpSocket, please do not use 5.0 because it seems problematic)!
Please install the Qt-SDK which includes the Qt-Creator IDE. If you are an experienced developer, you may also be able to compile QtGrace without the Qt-Creator, but I will not describe how to do so here.
Since you have extracted this folder you may notice, that there are some sub-folders. It is important for QtGrace to keep the structure of this folder in order to find some files at runtime. I have also included the original source-code-zip-file from grace-5.1.22 as a tar-archive. You will not have to extract it. This is included just for completeness and for developers use in case someone needs it. 
I assume in the following part, that you have installed Qt and you have got a fully functional Qt-Creator. 
To compile QtGrace: 
First step: Open Qt-Creator and do File->"Open File or Project..." (you may have to specify the file format as "Qt Project file (*.pro)"). Select the file "src.pro" in the 'src'-sub-folder. On the left hand side of the IDE-window you should now be able to see all source-files that are part of QtGrace. (On newer version of QtCreator you may be asked to specify the target to compile QtGrace for - use the 'Desktop'-version in this case.)
Second step: At the left hand side of the Qt-Creator-window you find some icons. Click on the "Projects"-icon and make sure a suitable path for the Qt-Installation is set at the general section. (You may have to consult the Qt-Manual on some systems on how to do so correctly. Especially in newer versions of QtCreator you have to prepare the compiler settings in a special way, so please consult the Qt-documentation.) You have two options now: either set the build-path in the general build settings to the 'bin'-subfolder or remember the build path that has been set automatically and copy the executable after the compilation to the 'bin'-subfolder. You will also have to set the destination of the executable to be executed from within Qt Creator in the settings in order to run QtGrace after the compilation from within Qt Creator. On newer version of the QtCreator you have to manage the different kits.
Third step: Click the 'Edit'-icon on the left side. Click on the 'Build All'-Icon (the one with the hammer in the left hand lower corner). This will initiate the process of compilation and may take a while. You may also see a few thousand warnings which can be ignored (hopefully) but there should be no errors (if you get errors, report them to me and I will provide information on how to solve this problem). (Special platform specific notes: see below)
Fourth step: Everything has been compiled now and you should be able to start QtGrace. Either you double-click on the qtgrace icon (or qtgrace.exe) in the bin-folder or you set the execution path in the project settings to this file and run it from within the Qt Creator.
Done! 

On Windows:
You have to set the path to the Qt-dlls in your PATH-variable or copy the dlls to the bin-folder (if you have a complete Qt-installation you can run the 'windeployqt'-tool -- see the Qt-documentation for how to do so)

On Linux:
If you continue getting messages like: invalid viewport coordinates --> please contact me.

On Mac OS X:
Executables are organized as disguised folders on such systems. Therefore after compilation in order to work in a MacOS-way you have to open the package-contents of qtgrace (right-click on qtgrace in the 'bin'-folder and select 'show package contents'). Then go to 'Contents'. Copy the contents of the original unzipped folder except the 'bin'-subfolder and the grace-5.1.22.tar.gz into this 'Contents-folder'. Copy the contents (except 'qtgrace') of the 'bin'-subfolder into the 'MacOS'-folder. Now you should be able to execute qtgrace by clicking on qtgrace in the 'bin'-subfolder. It should also be possible to move/copy QtGrace in every folder you like just like any other MacOS-application (put it for example in your Applications-folder). There is also a deployment-tool: 'macdeployqt' (see Qt-documentation).


Activate FFTW3- and libHaru(PDF)-support:
See Preferences-->Micellaneous and see whether the libraries are usable and activated.
If the libraries are not compiled into QtGrace statically you have to tell QtGrace where the dynamic library is located. This should usually be the bin-subfolder in the QtGrace-directory. The name of the library is usually libfftw3-3.dll and libhpdf.dll. If everything is ok you should see green instead of red boxes on the right hand side of the dialog.

Known Problems:

- Printing directly to a printer via the native printer-dialog is possible but I have not tested it extensively (but it seem to work in an acceptable way). Most settings are not mirrored in the native dialog. Direct printing without the native printer-dialog is only possible on systems that support the 'lpr' printing command (or any other printer-command like 'kprinter' on some linux-systems) and accept postscript files (i.e. a system command like 'lpr file.ps' will actually print the contents of 'file.ps' on the standard-(line)-printer). Printing to files is always possible. You have to physically print the file-contents via a different/external program (like OpenOffice/LibreOffice). You may also copy the whole page in the clipboard via View->PageSetup and paste it into your external program. Printing on Mac-system without a physical printer present can be tricky if you use Qt5 - sorry (previously the pdf-option was always accessable)
- On some Linux-systems the scrollbars are not displayed correctly but they still stay usable (I don't know why, yet. Seems to be a Qt-problem...; I also don't know whether this bug is still present, since I have not seen it in a while).
- Pipes not working on Windows.
- The switching between Grace-fonts and QtFonts is not adviceable. It is possible in most cases, but especially for the Symbol-font problems are to be expected - sorry. This has several reasons especially with the ordering of characters in the T1-Symbol-font in Grace and UTF-8-fonts. At the moment I do not know how to solve this problem. I made an attempt by making the symbol-font special (a separate setting in the preferences).
- Loading files (especially old ones) relies heavily on the encoding setting in the Preferences-dialog (QtGrace->Behavior). For loading example-files it is often adviceable to use the Latin1-encoding (or something similar). Newer files should contain the encoding used during saving and should be loadable without problems.


Things that work in Grace, but not in QtGrace:

- I have rewritten the spreadsheet-editor. Therefore it may not work as you are used to it (or maybe you won't even notice).
- I have written a simple text-editor for creating and editing sets. On Unix the default in Grace is to open 'vi' ('Create new->In text editor'). This option is not portable (at least not in a way I would like). I used an internal text-editor because this seems to be comfortable enough (suggestions for improvements are welcome).
- The pipes may work a bit differently in QtGrace, because I could not include the 'pipe-contents-changed' signal into the main-application loop like Grace does. Pipes also do not work on Windows, yet.


Additions to QtGrace (not present in Grace): see help-page on QtGrace-Extensions


Pipes (I include this here and not in the help-file, because I do not want to advertise this feature due to it's incompleteness):

The use of named pipes via the '-npipe' option or starting QtGrace in a pipe via '-pipe' or opening a pipe from Data->Import->Ascii is supported now. The drawback at the moment is: This does not work on Windows, yet. The usage of pipes is a bit different on Windows and I have to find out how to reliably use them there, sorry. Any helpful advice is welcome here! 
For the pipes (also called "Real time input") I included a new dialog. In 'Window->Monitor real time inputs...' you can see which pipes are currently monitored for input. You can also activate or deactivate the monitoring here via a checkbox. Internally in order not to block the other operations of QtGrace the input via pipes is not real time but in specified intervals. The time between updates of the real time input can be set here. I set it to 1 second by default. You can also open and close pipes via this dialog. I hope this is helpful for some people.

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
v0.2.2		'-free' option and 'immediate update' reactivated, experimental support for non-latin-characters and different fonts; support for multiple files in binary import and the use of a trigger channel ; bug fixes
v0.2.3		minor bugfixes; print-command saved in preferences; selection of odd or even set-ids; addition of fit-button for page-zoom-slider; minor changes to improve UTF-8-support in file-paths
v0.2.4		several bugfixes (major and minor); further improvement in UTF-8-support (very extensive internal changes); made decimal-separator selectable throughout the whole QtGrace-Gui; improvement in the script-generation (execution of scripts with automatically replaced set-ids); slider-value made editable; QtGrace-help-page; QtGrace-examples
v0.2.5		rework of the binary import (incl. drag-and-drop); addition of regions-master-dialog; additions to the commands-dialog to extend the scripting possibilities; increased the number of fit-parameters to 20; redesign of the output-dialog, separation of file-output from physical printer, adding a quick-selection-options to set the resolution; added the possibility to use libHaru (for pdf-output) and libFFTW3 (for better fourier-transformation); further extension of the preferences dialog (many things are now user-adjustable); drag-and-drop for file-import-selection-lines; more comfortable dragging-and-dropping of Grace-project-files (agr); separate dialog to let you extract single sets from an agr-file; restrict sets to regions and swap columns of a set (like swap X- and Y-axis); extension of the feature-extraction-dialog (more features); project description now visible in the open-projects-dialog and editable in the plot-appearance-dialog; addition of a file-path-stamp to the project; global font-size-multiplier; further options for placing the legend-box (manual and automatic alignment with the graphs borders); changed the apply-to-option for the axis-dialog; additional dialog to determine set-intersection-points and -angles; addition of QTcpSocket-support for remote control (by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB); bug-fixes

Have fun using QtGrace!

Best regards
Andreas Winter (andwin)
 
