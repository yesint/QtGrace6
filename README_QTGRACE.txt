General Notes on Version 0.2.3:

This release contains only some minor bug fixes and a few improvements. I added a 'Fit'-button below the page-zoom-slider to adjust the page-size to fit the window-size (thank you jhenin for suggesting this and supplying source code). I added options for selecting sets with odd or even set-ids in the set lists and an option to select and copy odd or even lines in the spreadsheet-editor. The print-command (like 'lpr') is now saved in the users preferences file (qtGrace_Settings.ini) for users who do not use lpr or who want to use more complicated commands with options regularly. I also tried to improve the support for non-ascii-characters in file-names and file-paths. I tested it a bit and it seems to work, but feedback on this is always welcome!

(Just an excuse for not updating QtGrace earlier or not having included more new features: The compiler on my Windows7-system stopped working after a regular Windows update. I tried several different things to get it working again, but there still remains an error in some basic library concerning a typedef with a pointer which I do not understand. I spent more time trying unsuccessfully to resolve this problem than I spent actually doing improvements on QtGrace. This made me very frustrated! I compiled the Windows-executable for this release on an older WindowsXP-system with Qt-4.6.1. I hope this works for the Windows-users out there - I can not do more concerning Windows.)

One further request (as always): If you find bugs or missing feature or if you have suggestions for improvements: Let me know immediately!
Thank you!

Warning: I changed some of the shortcut-keys: In order to use Crtl+Z for Undo I used Alt+Z for Zoom. In order to use Crtl+V for paste-operations I used Alt+V for viewport.
I hope this will be helpful to someone. Don't hesitate to report bugs/suggestions/wishes.


General Notes on QtGrace:

Dear User or Developer;

Thank you for your interest in my Qt-version of Grace!
In case you do not know already: Grace is a program to plot data and analyze it.
Grace is based on xmgrace / xmgr and uses the motif-framework for the graphical user interface.
The Grace-website is: http://plasma-gate.weizmann.ac.il/Grace/
QtGrace is based on grace-5.1.22 but uses the Qt-framework from Nokia as a graphical user interface instead (Qt and their respective logos are trademarks of the Nokia Corporation).
Although I have tested QtGrace for quite a while now there may still be some problems / bugs present. I recommend this version mainly for experienced Grace-users and developers. Everyone else may give it a try, but be sure to read the Grace documentation (and save often).

I hope this will be useful to someone else as well (I like my Qt-version of Grace :-) ).

I had problems using Grace on different operating systems. Since I had written some applications using the Qt-framework (which can be found on http://qt.nokia.com) I decided to modify the latest stable version of Grace (5.1.22) and replace all dialogs by equally looking Qt-replacements. I wanted to conserve the "look and feel" of Grace 5.1.22 because I wanted all Grace-users to be able to work with QtGrace immediately without having to adjust their workflow.

I altered most of the source-files and therefore changes in Grace have to be implemented manually. Since there seems to be no major development around Grace-5.1.22 I believe that this approach is acceptable for now. All development on Grace seems to be focused on version 6 (or 5.99) which has a fundamentally different internal architecture. QtGrace is not compatible with Grace 5.99.

This version 0.2.3 of QtGrace is capable of nearly all of the operations Grace 5.1.22 is able to do. Some minor functions are missing or work only on some platforms (see below). I have inserted a few extra functions I found helpful (see below). There are a few minor known problems (see below).

The internal procedures are the ones from Grace 5.1.22 (internal data management, calculations and so on). I have only recreated the user interface in Qt which makes it possible to compile QtGrace natively (!) on Windows, Mac OS X and Linux. You can load your old Grace-project files (usual extension: '.agr'). With Qt it is also possible to easily translate the user interface and make the language of the interface selectable (I have made an attempt for a test-translation to German. The file is included but very far (!!) from being complete. If there is someone out there willing to complete the german translation or generate a translation to any other language feel free to do so using the "Linguist" program included in the Qt-installation. If you send me the translation I will include it into the downloadable zip-file.).

I mainly provide the source code for QtGrace since I am not 100% sure which files are needed for every platform. If you want to compile QtGrace you have to download Qt from Nokia as well (see below). As an additional download I also provide a binary-version for Windows for all Windows-users who just want to use QtGrace and are no developers. The dll-files in this zip-file are copyright by Nokia and part of Qt. I provide them just for convenience. Don't alter the structure or remove content from the folders. I also provide a disk-image for Mac-users on an experimental basis. Any advices on how to generate a reliable rpm-file for Linux-users are welcome!

I am always interested in hearing your opinion on QtGrace or in your bug-reports. Suggestions for improvements are welcome also!
To contact me please use email: andreas.f.winter@web.de

I hope you enjoy QtGrace!

Best regards
Andreas Winter (andwin)


Copyright and License-Terms:

Copyright (C) 2008-2012 by Andreas Winter
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

QtGrace is based on the Qt-Framework which has to be obtained via http://qt.nokia.com.
Please use at least version 4.6 (or newer) of Qt to compile QtGrace!
Please install the Qt-SDK which includes the Qt-Creator IDE. If you are an experienced developer, you may also be able to compile QtGrace without the Qt-Creator, but I will not describe how to do so here.
Since you have extracted this folder you may notice, that there are some sub-folders. It is important for QtGrace to keep the structure of this folder in order to find some files at runtime. I have also included the original source-code-zip-file from grace-5.1.22 as a tar-archive. You will not have to extract it. This is included just for completeness and for developers use in case someone needs it. 
I assume in the following part, that you have installed Qt and you have got a fully functional Qt-Creator. 
To compile QtGrace: 
First step: Open Qt-Creator and do File->"Open File or Project..." (you may have to specify the file format as "Qt Project file (*.pro)"). Select the file "src.pro" in the 'src'-sub-folder. On the left hand side of the IDE-window you should now be able to see all source-files that are part of QtGrace.
Second step: At the left hand side of the Qt-Creator-window you find some icons. Click on the "Projects"-icon and make sure a suitable path for the Qt-Installation is set at the general section. (You may have to consult the Qt-Manual on some systems on how to do so correctly.) You have two options now: either set the build-path in the general build settings to the 'bin'-subfolder or remember the build path that has been set automatically and copy the executable after the compilation to the 'bin'-subfolder. You will also have to set the destination of the executable to be executed from within Qt Creator in the settings in order to run QtGrace after the compilation from within Qt Creator.
Third step: Click the 'Edit'-icon on the left side. Click on the 'Build All'-Icon (the one with the hammer in the left hand lower corner). This will initiate the process of compilation and may take a while. You may also see a few thousand warnings which can be ignored (hopefully) but there should be no errors (if you get errors, report them to me and I will provide information on how to solve this problem). (Special platform specific notes: see below)
Fourth step: Everything has been compiled now and you should be able to start QtGrace. Either you double-click on the qtgrace icon (or qtgrace.exe) in the bin-folder or you set the execution path in the project settings to this file and run it from within the Qt Creator.
Done! 

On Windows:
You have to set the path to the Qt-dlls in your PATH-variable or copy the dlls to the bin-folder (look for 'QtCore4.dll' and use the path thereof). Otherwise you can only run qtgrace from within the QtCreator. (You need the following dlls: QtCore4.dll, QtGui4.dll, QtNetwork4.dll, mingwm10.dll, libgcc_s_dw2-1.dll; The path where they can be found is usually something like 'C:\Qt\2010.05\qt\bin\'.)

On Linux:
Make sure the environment variables do not contain an entry like LANG=de_DE.UTF-8 (or similar containing UTF-8 and de or other settings that force the decimal separator to be ',' instead of '.'). In this case either remove this entry (unset LANG or set LANG=C) or set LC_NUMERIC=C (preferred solution).
Otherwise you will always get an error message saying: invalid viewport coordinates.

On Mac OS X:
Executables are organized as disguised folders on such systems. Therefore after compilation in order to work in a MacOS-way you have to open the package-contents of qtgrace (right-click on qtgrace in the 'bin'-folder and select 'show package contents'). Then go to 'Contents'. Copy the contents of the original unzipped folder except the 'bin'-subfolder and the grace-5.1.22.tar.gz into this 'Contents-folder'. Copy the contents (except 'qtgrace') of the 'bin'-subfolder into the 'MacOS'-folder. Now you should be able to execute qtgrace by clicking on qtgrace in the 'bin'-subfolder. It should also be possible to move/copy qtGrace in every folder you like just like any other MacOS-application (put it for example in your Applications-folder). 


Known Problems:

- On systems that have the environment variable "LANG" defined and the value contains 'UTF8' and 'de' or 'fr' (system-language set to german or french or other settings that involve a decimal separator of ','): The Grace/Qt-core of qtgrace has problems interpreting some decimal numbers that contain '.' instead of ',' as decimal separator. In this case you will not be able to display any data since the viewport-coordinates are always invalid. I don't know how to solve this yet. Workaround: unset "LANG" or set "LC_NUMERIC=C" (this overwrites language-settings and always '.' is used as decimal separator). 
- Printing directly to a printer via the native printer-dialog is possible but I have not tested it extensively (but it seem to work in an acceptable way). Most settings are not mirrored in the native dialog. Direct printing without the native printer-dialog is only possible on systems that support the 'lpr' printing command (or any other printer-command like 'kprinter' on some linux-systems) and accept postscript files (i.e. a system command like 'lpr file.ps' will actually print the contents of 'file.ps' on the standard-(line)-printer). Printing to files is always possible. You have to physically print the file-contents via a different/external program (like OpenOffice). 
- On some Linux-systems the scrollbars are not displayed correctly but they still stay usable (I don't know why, yet. Seems to be a Qt-problem...; I also don't know whether this bug is still present, since I have not seen it in a while).
- On Windows systems the usage of the internal html-viewer for help-files has been disabled (the option is present, but internally discarded). I had to do this, because the webkit- and phonon-packages of Qt don't seem to work together on Windows. (For developers who want to give it a try: remove the preprocess directives in 'replacement_main.cpp' at lines 1602, 1605, 1607 and 1615.) This may also be true for some Linux-systems (In this case remove everything between the directives mentioned).
- Pipes not working on Windows.


Things that work in Grace, but not in qtGrace:

- I have rewritten the spreadsheet-editor. Therefore it may not work as you are used to it (or maybe you won't even notice).
- I have written a simple text-editor for creating and editing sets. On Unix the default in Grace is to open 'vi' ('Create new->In text editor'). This option is not portable (at least not in a way I would like). I used an internal text-editor because this seems to be comfortable enough (suggestions for improvements are welcome).
- The pipes may work a bit differently in QtGrace, because I could not include the pipe-contents-changed signal into the main-application loop like Grace does. Pipes also do not work on Windows, yet.


Additions to qtGrace (not present in Grace):

- I have included an import-filter for binary data (Grace only supports ascii-files). Usage: see below. There is also a binary export for single sets and the possibility to import such sets later by using the 'grace_binary_format.fmt' for the binary import-filter.
- In all I/O-dialogs I have inserted a button on the upper left hand side (the one with the arrow pointing upwards). Clicking on it will set the current path on the parent-path of the current folder (like clicking on '..').
- In all I/O-dialogs I have inserted a button on the right hand side in the middle of the dialog (the one showing a folder). Clicking on it will open the native file selection dialog of your operating system (This is very useful, because this dialog is more comfortable than the Grace-I/O-dialog in my opinion. You don't have to use it if you don't think so.).
- I inserted an option for ascii-import to use ',' instead of '.' as decimal separator in ascii-files.
- I have inserted a history-menu showing the files you have opened recently.
- The QtGrace-specific settings, the preferences and the history are stored in an ini-file that is read at startup and written when QtGrace is closed. (If you have a wish for a parameter to be restored in every session of QtGrace: let me know and I may insert it in the ini-file.)
- I have inserted another data-transformation-dialog: Data->Transformation->Filters. With this dialog you are able to use low-pass-, high-pass- and band-pass-filters. They are based on fourier-transformations. This dialog is currently in an early stage of development. Not all types of filters will work as expected (Butterworth- and Brickwall-filter should work; you have been warned). The conversion of the x-axis to an axis showing time in seconds is necessary for interpreting the frequency correctly (this conversation is only used internally - your x-axis is not altered). Any suggestions for improvements are welcome! 
- In the properties-dialog I have inserted a button in the bottom right hand corner called 'Extra' opening a separate dialog for the QtGrace-specific settings. (Changing the Grace Home directory will probable not work reliably, this is still in an experimental stage.) As a default, all help-files are displayed in an internal html-viewer. You may also use your favorite html-viewer by setting the path to it in this dialog (you have to set a command that opens your viewer). The drawback is, that not every external html-viewer accepts the html-extension (the part of the html-address after #). You may also set a different language here, but there are no complete translations of the user interface yet (help is welcome here). The show/hide-workaround has been included because on some linux-systems the behavior of Qt is a bit more complicated than on other systems. Paint events are not done completely sometimes (at least not every time or I am doing something wrong). Sometimes using this workaround helps, but not always. This can be very(!) annoying (there has been a time in which I liked linux - about 10 years ago - but the linux-distributions I tried over the last 3 year have all been very disappointing. They tend to have a lot of fancy stuff like window decorations, transparencies and so on, but as far as I am concerned the usability has decreased). I don't have a solution for this so far. Finally in the QtGrace-settings-dialog you can increase or decrease the number of recent files displayed in the 'File' menu. 
- In the print-dialog I inserted a few file-formats for export. I also inserted a button that lets you switch to the native printer-dialog of your operating system. This is still in an early stage of development. I am not sure whether it works on all circumstances (bug-reports, suggestions, wishes are welcome).
- UnDo/ReDo-feature: see below.
- Copy/Paste and (Drag)/Drop functions (no real 'drag' at the moment) 
- Changing color palette via the user-interface (see also below)
- filling area between sets
- rotation of boxes and ellipses
- Monitoring the real time input via pipes (activating, deactivating, managing more than one pipe) - not in Windows, yet
- Zooming several graphs simultaneously
- Zooming of the displayed page
- Import of csv (comma separated values)
- Description of additional features is incomplete due to laziness ... (see also below)


How to use the binary import filter:

To import data from a binary file go to: Data->Import->Binary...
This will open a dialog. Be aware, that you have to know exactly (!) how the structure of the file is that you want to open. You have to tell the import-filter the exact format once and afterwards you may chose to save this format-information to reuse it later. I have included the file-format-specification of wav-files: Click 'Load File Format' and select 'wav_format.fmt' in the bin-folder. To select the actual binary file click on 'Browse...' on the right hand side of 'Datafile:'. By default it is assumed that the binary file also contains a header that has some information about the data present (this is the case for wav-files). 
The first part of telling the filter about the file format is to define the header-contents (byte for byte). This is done by adding a token to the list (click on '+') and tell the filter the format of the token (usually this is the kind of numerical representation of a number in a C-like format). The 'Bytes' box is only used for 'Offset' tokens that are ignored during data import later. It is possible (but not strictly necessary for every file type) to tell the filter how to use the header-information via the ComboBox 'InputAs'. In case you have done something wrong you are able to delete a token by clicking on '-'.
By clicking 'TestLoad'  you can check whether your input has been correct (I always recommend clicking 'TestLoad' since it is needed for some files). The 'File-Info'-tab contains the data read from the header. 
The second part is setting the Data-Format. It is assumed, that the data is present after the header in the file as a stream of bytes (no additional information in-between). It is also assumed, that the data-points are either present one after the other or one channel after the other. You have to set the format of every channel present in the file and whether or not this channel shall be imported.
The last step is to set the destination for the import: set-type and graph-number. You may only import one data set at a time.
It is also possible for data and header to be present in different files. You may select both of them in this case. For the header-information two versions are supported yet: header-information in binary format or in the format of ini-files (like system settings: '[Data] t0=5 dt=0.1' separated by new lines).
Experiment with binary import on wav-files. This may give you a good idea of how I meant it to work.
You may import more than one set at once. In order to do so set more than one import-channel to the same column (like one 'X' and three times 'Y').
Beginning with v0.2.2 it is possible to select several files for binary import at the same time. Doing so, the import filter will assume the files to have the same format and import the selected data from every file into a new set.
One channel in a file can be declared as a trigger channel. This channel is used as a reference for the x-axis. Currently only a rising edge is supported and the import dialog will ask for a trigger level if a trigger channel is selected. After import the x-position of the rising edge is detected and all imported sets are shifted to set the rising edge to x=0.
Any suggestions for improvements are welcome.


How to use the csv import filter:

To import data from a binary file go to: Data->Import->CSV...
This will open a dialog. In the Filename box you may enter the name of the file you want to load data from. Although: The usual method would be for you to click on "Select CSV file" and use the operating systems file selection dialog. After selecting a file or clicking on "Re-guess columns" the first lines (ending with a new-line-character) of the files are loaded and shown in the File contents box. You may also notice a "-#-#-#-#-#-#-#-#-" somewhere in this box. This indicates the separation between a header (which is ignored) and the actual data columns below.
On the right hand side of the dialog you can select the target graph data will be imported into.
In the middle of the dialog you have to set the file format yourself (since there is no standard for csv, the files are different depending on how they have been generated). 
At first you have to set how many lines at the beginning of the file are to be ignored (please note, that these header lines have to be separated by newline-characters or they may not be recognized as actual lines). The Data-set-separator specifies the ending of every data point. It is assumed, that every point consists of a number of values and in the file the end of one point and the beginning of a new point is announced by a special character like a newline-character. A newline has to be entered as '\n'; a tab-character as '\t'; except this all separators have to be single characters! Different (column-)values of one data point are separated by the Column-separator; Strings are indicated by a text-separator-character at the beginning an the end of the string (internally the string separator for Grace is always " therefore the use of the Text-separator or " inside the strings is very dangerous).
After entering all these format settings it is strictly advised to click on "Re-guess columns" since this will show whether the import filter works correctly. The guessed column-formats are shown in the contents-box below the first six data lines below '...'. 
It is possible to use ',' instead of '.' as decimal separator during import operation (usage of ',' as a column- or Data-set-separator is not advisable in this case). The guessing also sets the presumed number of data sets present in the file (=Number of data columns-1) and the formats in the column import selectors above the Apply/Accept/Close-buttons. The standard-import targets are set to: import first column as X-value for all new sets, import all following columns as Y-Values for new sets. If you set higher target columns (like Y1, Y2, ...) the set-type of the new sets will be adjusted automatically in order for the set to represent enough columns. 
If you need more sets then the guessed ones you can alter the "Number of sets to read". Strings can not be imported, they can only be recognized as columns between real values.
In short: suggested workflow:
Select a csv-file; Enter the separator-characters and the number of header-lines and press "Re-guess columns"; Set the import-format and import-targets for every column in the file; Press "Apply".


Undo/Redo-function:

Starting with version v0.1.3 I have included a feature to Undo and later Redo some operations the user does with graphs and data sets. It is deactivated as a default because it is of an experimental nature at this point. It will be activated as a default in a later release when I am sure that no harm is done by this function. To activate it: Go to Edit->UndoList and set the "Undo active"-option. This option will be saved in the ini-preference-file and will be restored at every startup of QtGrace (keep this in mind: if you activate Undo/Redo, it will stay active until you deactivate it again!). I do not recommend activating this option for Graphs that include large datasets (>100.000 points) since it may consume a lot of memory because the undo saves up to 100 states of your work.
Since you now have been warned, let us get to the nice stuff: What is possible with this feature?
When "Undo active" is activated, every supported operation is recorded as can be seen in the UndoList. In this list you will find a description of what you have done (the last one is the one on the bottom) and at the beginning of one line the letter 'U' or the letter 'R'. Actions marked as such are the actions that will be Undone or Redone when you use Edit->UnDo or Edit->ReDo respectively. You may also select one or more actions in the list and click on "Undo" below the list. Undo will undo the actions form the last one upwards. Redo will undo the actions from upper most selected action downwards. Every UnDo- or ReDo- operation is considered as some sort of toggle-operation. After an action has been undone a "(*)" will appear at the beginning of the description in the UndoList. This means, that you already have made this undone. Clicking on it and clicking Undo afterwards will effectively redo it. This is also the case, when you use Edit->UnDo. An action that has already been made undone will be redone, when it is reached by the Edit->Undo-operation. Doing a normal operation in QtGrace will set the 'U'-marker to the last operation in the list. This means that by clicking Edit->UnDo the last operation will be made undone and the 'U' will go to the operation before this one. The 'R' for Redo will then be set at the last operation. Clicking multiple times on Edit-UnDo will set the 'U'-marker one operation back(=upwards) until the end of the list is reached or a new normal operation is done in QtGrace. You can always do a manual undo via the list. Manual operations do not alter the 'U'-marker. Using Edit->ReDo will Undo the Undo-operations until the end of the list is reached. Give it a try and you will see what I mean. The shortcuts Ctrl+Z and Ctrl+Shift+Z are used for Undo and Redo. Please note: Ctrl+Z is normally used for Zoom-operations. If Undo is possible, Ctrl+Z will result in an undo; if not, a zoom is initiated.
One further warning: Operations that change the ordering of the sets in a graph or the ordering of graphs (like swap-operations or pack-sets) are a bit dangerous if you do manual undo. At the moment the undo-function does not check whether the set-number has been changed since the operation you want to undo. Doing a chronological undo with Edit->Undo should be save, manual undo can sometime have unexpected results.
Simple description of what Undo/Redo does: Undo does set the state before the action has been performed, Undo again will set the state after the action has been performed. (Undo and Redo are only different with respect to the chronological order in which operations are made undone.) Undo/Redo does usually NOT really do the action again (there are some exceptions), only data is restored. Please use the manual undo-operation only for emergencies or if you really know what you want. Don't manually undo and redo several operations in arbitrary order to often, because unexpected results will be guaranteed. Chronological Undo via Edit->Undo or Edit-Redo is preferred.
Any suggestions are as always very welcome! Bug-reports as well.


Other additions:

In the Preferences dialog in the Extra preferences you will find two new options. One for immediate updates and one for LaTeX-support. Both are still a bit experimental.
Immediate updates means, that changes you make in dialogs (like change font, line-width, patterns and so on) will immediately be represented in the graphics. You have to activate this feature (it is deactivated by default). Although this feature can be quite helpful it produces lots of entries in the undo-list because every small change is recorded (in the normal mode the undo feature stores the current state of the project only when apply is clicked. With immediate updates, the changed state is stored every time a small alteration is made). You have been warned.
The LaTeX-support is meant for the input of simple LaTeX-commands declared by '$$' at the beginning and the end of the command (without'). Only text-input-widgets that are ment for comments/labels do accept the LaTeX-commands and only if the option is activated in the Extra-Preferences. This should be helpful to write formulas for people who know how to use LaTeX. For Example: You can enter something like '$$E=(\frac{1}{2}\pm\nu)\cdot\hbar\omega$$' and this command will internally be translated in the Grace-typesetting language (there is no real LaTeX-typesetter included - just some sort of makro-translator). At the moment the translation is one-way only. The original input is stored and you can switch between the original and the translated version. Usually the original will be shown. If you want to edit the translated text click on the label on the left of the text-input-widget. The text will be shown in red then. If you edit something in the converted version and click on Apply or press Enter while Immediate Update is on the text containing the Grace-font-setting-commands will be stored as original and your LaTeX-input is lost.
I did not copy every command from a LaTeX-handbook - I only included commands I thought could be useful and that are simple enough to include. (Take a look into ListOfLatexCommands.cpp if you want to know which commands are recognized at the moment.) Complex commands (like special braces like \underbrace or \overbrace) are not recognized at the moment and the overall translation is not completely fail-save. Don't use it in critical situations and save often. (If you enter a command and it is not recognized correctly try experimenting with spaces before or after it.) Nevertheless I would be very interested in your experiences in using this new feature. I also would like to know which commands would be most useful to you if they were included. Font attributes are copied. Please notice: there is no font-command for making a font bold. Therefore characters that use the symbol font can not be displayed in bold. Sorry.

Concerning the default printing device I included an option in the Extra-preferences. You may select, which device should be set at startup of QtGrace next time. You may select the last printing device you have set in the current session or a fixed device every time (like use 'jpeg' format for printing). It should now also be possible to load QtGrace and load a project-file and immediately click on 'File->Print'. The file is stored in the directory of the project-file loaded with the default extension of the default-printing-device.


Automatic script generation:

I included an option in the Undo-List-dialog: If some of the entries in the undo-list are selected the button below the list may be pressed. This will convert the undo-entries into appropriate commands in the Command-dialog. By doing so it is possible to record a script of what has been done (Do something in the normal way while Undo is active. Then afterwards select the actions that should make up a script and then press the button. In the Commands-dialog you may then alter the commands, save and load them.). Unfortunately this feature is still incomplete at the moment. Loading and saving data is not fully supported yet. The scripting works concerning style-changes in things like Graph-Appearance, Set-Appearance and so on.


Color managements:

In 'Window->Color Management...' you can change the current Color palette used for the project. In the upper part you can select  a color from the current palette and edit it. You may enter the red/green/blue-color-components and the color name. Below you will see a preview of the edited color. You have three options for the single color: delete the color with the specified number from the palette, add the color as a new color to the palette or replace the color with the specified number with the new color (Edit color). The option 'Define color' lets you select a color from the color-selector-dialog of your operating system. If you want to add this new color to the current palette use add or edit.
You may also edit the whole palette in one go. You may set the palette for example to a specified spectrum of colors. You may use predefined spectra or set your own spectrum. By selecting 'Custom...' and setting a number of path-points you may define the spectrum by specified colors that have to be part of the spectrum. For each path point you have to specify a color via 'Current path point' and 'Color'. Below you will see a preview of the spectrum. Please note, that the first two colors in the spectrum will always be white and black because otherwise you will get very irritating results. If you don't want this: set a spectrum and edit the first two colors afterwards.
Please note also: everything you edit in the Color management dialog only effects the color-palette of the color selector in this dialog. If you want the change to take effect in the current project you have to click Apply or Accept.


Font sizes in Grace/QtGrace:

Because the coordinates in Grace/QtGrace are usually relative to the smaller page-dimension (viewport-coordinates) there is no simple transformation formula between Grace/QtGrace and other applications. Let us assume a Page in landscape orientation with the x-y-dimensions a x b in pixels (like 792 x 612 pixels for a Letter-page in 72dpi). In this case b is the smaller dimension and Grace/QtGrace will scale everything in relation to this value (viewport-coordinate '1' corresponds to b pixels in this case; generally: viewport-coordinate '1' corresponds to the smaller page dimension).
In comparison with other applications which give font-sizes in points (pt) I have derived a rough formula for converting font-sizes:
(Grace-char-size)=445/9*fontsize(pt)*resolution(dpi)/(lower page dimension in Grace)
Or for example in two simple cases:
(Grace-char-size)=5.817*fontsize(pt) (for letter-pages in 72 dpi)
(Grace-char-size)=5.983*fontsize(pt) (for A4-pages in 72 dpi)
If you use a letter-page in 72dpi and want a font-size of 20pt you have to set the char-size in QtGrace/Grace to 116.
As mentioned before: This is a rough conversion based on experimental values. Suggestions for improvements are welcome here as well.


Selecting multiple Graphs for zooming:

On the upper left hand side of the QtGrace-window there is now a new graph-selector. This selector shows all graphs present and the currently active graph is selected. Any change in graph-selection is represented here. You can also change the current graph by using this selector. The main purpose of this is to be able to select more than one graph for zoom-in and zoom-out. All zoom-operations that are shown in the toolbar use this selector now for determining on what graph(s) they should be used. Please mind, that internally Grace supports only one active/selected graph. This one is displayed with focus-markers usually. This new graph-selector will only work with the zoom-buttons in the toolbar and is meant for people who use superimposed graphs and want to zoom in and out simultaneously.


Zoom-Slider:

I added zoom-slider in the toolbar on the lower left hand side of the QtGrace-window. You will find it just above the 'Exit'-Button. The slider-value is changed logarithmically and can be set between 0.1 and 10.0. (The internal values of the slider range between -100 and +100 and the actual zoom-factor is calculated by 10^(value*0.01). The current value of the slider is saved in the QtGrace-ini-file as "PageViewZoom". If you have problems resetting the page zoom to 1, set this value in the ini-file manually back to 0.) The setting only effects the visual display on screen. Print-commands are unaffected by this. There is also no Undo for changing the slider setting because this in nondestructive. The setting is saved when QtGrace is closed and will be reset to the last value at the next startup. I intended this for people who have uncommon display-resolutions. QtGrace calculates the size of the visual page according to the resolution of the computer-screen used. This is ok for most users. It is very inconvenient for people using notebooks with small screens (like 12") because they will not be able to see the whole page. With this new zoom-slider you can reduce the page size for the display according to your needs. You are still able to produce hardcopies (in Print or in Files) with an exact resolution.
To reset the slider to factor 1.00 you may use the shortcut 'Crtl+1'.
Please note: The Zoom-Slider is deactivated if QtGrace is started with the '-free' option, because it is useless in this case!
The 'Fit'-Button below the page-zoom-slider sets the zoom-slider in order to fit the page-size to the available space of the window. (Thank you jhenin for suggesting this and supplying the source-code which I modified a bit.)


Qt-fonts / non-ascii-characters:

I added an experimental support for character sets that are not Latin. In the preferences dialog in the Extra-preferences you will find now a Combo-box to select the available file-encodings on your system. This setting is used to interpret the project files in order to use characters for example from cyrillic or chinese fonts. (I have not tested this very extensively, because I do not understand any language that uses non-latin-characters - sorry - feedback welcome.) The default setting is the 'System'-setting - which means "use the default encoding currently used by your operating system".
I also included an experimental startup-option: '-QtFonts'. If you start QtGrace with this option (i.e. 'qtgrace -QtFonts') the font selectors will change their behavior. The label on the left side saying 'Font:' will change to a command button and pressing on it will display fonts that are available for use with the Qt-drivers (This should include most of the fonts available on your operating system. This is not restricted to the T1-fonts in the QtGrace-folder.). These fonts are only used for printing when Qt is used. This is the case for the display/screen, for using the native printer dialog and printing to 'jpg', 'png' and 'bmp' files. The T1-standard-fonts are used for printing on all other devices (like 'ps'). At the beginning the combo-box will only show the standard fonts supported by the original Grace. If you want to add other fonts: click on the command button saying 'Font:' and select a font from the font dialog of your operating system (usually the font size can be selected here as well, but this setting is ignored at the moment).
When using the '-QtFonts'-option the Font-Tool will be considerably slower (I hope to improve this in future releases). Using the Symbol-font will not have the same effect as it has with the T1-fonts: In T1 the Symbol-font contains the greek letters (only). In the Qt-fonts every font contains the greek letters (but not at the same positions as the latin-letters). That means if you want to use the QtFonts and latin and greek letters at the same time you have to enter the unicode-numbers of the greek letters or copy and paste the letters from the Font tool.
PLEASE NOTE: To actually use Qt-Fonts activate "Use device fonts" in the PageSetup-dialog!
I can not guarantee that all special font commands like "\s", "\S" or similar will work all the time (due to differences between UTF-8 and Ascii). Using the LaTeX-shortcuts will also be unreliable while using QtFonts because the character positioning has been optimized for T1-fonts.
Conversion of path-names to ascii-characters is done using the default system-encoding, because this is necessary for the internal file-operation-functions.

Pipes:

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

Have fun using QtGrace!

Best regards
Andreas Winter (andwin)
 
