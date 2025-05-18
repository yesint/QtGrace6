General Notes on Version 0.2.7:

QtGrace has never been abandoned!

Finally I finished this version after 5 years. There have been many reasons why it took me so long: One reason is that I had lots of work at my regular job including lots of overtime. The second problem was that I did not set a roadmap for this release. Therefore I added new features constantly without finising previous new features completely. This resulted in a bit of a mess that I had to resolve. The next big issue has been the release of Qt version 6 for which I had to change a few things (see below). The last thing was an annoying bug in the spreadsheet-editor that took a very long time to figure out. All of this took a long time (especially testing and updating the documentation). Sorry!

This new release v0.2.7 contains additions to the user interface and additional new functions. I tried to fit the changes in while keeping as much of the Grace-interface unchanged as possible. In some cases I used switches in the preferences where the user can activate or deactivate new features in some other cases I just added new options or had to do some minor rearrangements. I hope this is not too much of an inconvenience to anybody.

New Features:
- The new version of QtGrace is able to compress files. QtGrace-project files may be compressed using the Qt-compression algorithm (based on z-lib). In project files only the data is compressed, not the header-information. This is done in order for QtGrace to be able to simply read the content informations without having to decompress everything. If you need to compress or decompress QtGrace-project files you may use the CompressorDecompressor-application that is included as a side-project to QtGrace. This program lets you work with a bulk of files at once. QtGrace is also able to decompress and read compressed binary files as long as they have been compressed with a z-lib-compatible compressor. Please note: compressed files can not be read by the original Grace / XmGrace. Please use the CompressorDecompressor-application before transfering files between QtGrace and Grace / XmGrace (Or save without compression).
- There is now a new dialog that lets you fit a circle or an ellipse onto points in a set (whole set or inside a region). Data->Transform->Fit 2D-Objects... (If you have suggestions for additional objects that could be fit - please let me know. This dialog is meant for parametric functions that can be written as x(t) and y(t) instead of y(x).)
- In the Data->Data set operations... dialog there is now a new option to create a set with the Y-values beeing the average of several sets Y-values. The averaging can be done by interpolating the sets before or by simply adding the y-columns (which is only possible for sets that have the same lengths).
- For text there is now a possibility to adjust the alignment if more than one line is in the text. You can set left-align, right-align or centered. This has no effect if a text has only a single line (i.e. no new-line-characters). Text-elements can be graph titles, text-annotations (string-objects), axis-labels or tick-labels.
- For string-objects (text) it is now possible to auto-align the whole text-box to the whole page or to a specific graph. This is for example useful if you have a page with several columns of graphs and want to place a title in the center of the page above the graphs.
- For png/bmp/jpg/tiff-files it is now possible to remove the empty areas near the borders of the page. The image begins and ends with pixels that are not the background. To use this feature go to File->File Output Setup... and there is a check-box (Clip empty border areas) you may activate.
- It is now possible to set images as background or filling of boxes and ellipses. This way you may include logos or explanatory images for example. To do so select "image" as filling instead of color pattern or solid fill. Then select an image file. For saving of the project file you may select to store the path absolute (default) or relative to the agr-file (the image is not stored in the project file, just a path were to look for it). The image can also be transformed in several ways to fill the available space: scale up/down and stretch to fill the width or the height (this does not change the aspect ration) or scale to fill the whole space (changing the aspect ratio); you may also use the image multiple times to fill the space by tiling. If the image does not fill the available space then the remaining space is filled with the set background color. If you want the remaining background to be transparent please set the filling to Color pattern and the set alpha-channel to the lowest possible setting and then change the filling to your image again. Please note: Image-export is only possible for pixel-based fileformats. You can not export images in pdf-files or ps-files (so far). 
- In the Quick-Format-selection in the File-Output-Setup-dialog it is now possible to set three user defined output-settings. These settings can be changed via a separate dialog and you can select, that several settings are not to be changed (like the page orientation). In the dialog you can also activate or deactivate the different formats to make the list clearer.
- I wrote functions to work comfortably with input-data in different representations. You can now use every format that is available in the Axis-properties-dialog (Tick properties->Format) for input as well as for output. The format of the data can be automatically recognized. At the moment this feature is very demanding on the cpu and is therefore not activated yet for every occasion. To use it use Data->Import->Ascii... and select block data. In the Block-Data-dialog you have to select the "Extended format input". This opens additional settings on the right hand side. Here you can see what the new format recognizing feature determined to be the most probable column format and which values have been read. You can set different format options here as well and reread the columns. The resulting values are displayed on the bottom. If you want the read data to be imported you have to click the button below to store the values as block data. The original block data is the data that the original Grace-function read from the file (in case of Dates and Times the Grace-import-function usually reads them as garbage). After the data is stored as block-data you may use it as usual. Please also note that the csv-import-dialog is also able to read ascii-data and my be even more capable of doing so for complex data formats. Consider using this instead of the Data->Import->Ascii-dialog.
- The csv-import-dialog has been rewritten completely. This dialog is now able to read any text-file (including normal ascii-text-files). On how to use it please refer to the QtGrace-Extensions-help-file.
- There is now an if-statement for scripts. If you type "#if[<statement>] <command>" then the <command> will only be executed, when the <statement> is true. The command can be any command you like. The statement can be a number (0=false, everything else=true) or a simple equation like a=b, a>b, a<b, a!=b. For example you may write "#if[S#%6=0] y=y*2" and this results in the y-values of every selected set that has a set-id that can be divided by 6 (without residue; like 0,6,12,18,...) to be multiplied by 2.
- In the Commands-dialog the different types of commands are now displayed in different colors: normal operations are displayed in black, QtGrace-specific commands are displayed in pink, comments are green and conditional commands (if-clauses) are displayed in blue.
- QtGrace now offers several options to increase the GUI-elements for screens with high resolution. You can increase the sizes of the font, dialogs and icons. See the QtGrace-Extensions for further informations. In order to get proper scale up of control elements I changed the style of the gui to 'Fusion'. This makes QtGrace look a bit different than other applications, but for the default style I was unable to scale up anything properly. If anyone has an idea on how to do so, let me know. This is especially necessary for 4K-displays and above.
- For line-objects it is now possible to edit the end-points of an exisiting line by clicking and draging. To do so open the line-edit-dialog and use the Line-edit-button at the bottom of the dialog.
- For moving different objects it is possible to use click-and-drag with the mouse cursor. This method can be a bit unprecise. To improve this a bit I included the option to restrict the movement to X- or Y-axis. If 'Alt' is held during draging the movement is only done along the X-axis. If 'Ctrl' is held during draging the movement is only done along the Y-axis.
- In the Axis-properties-dialog there are now further options for applying settings to several axes at once. You now may select the normal and alt axes separately and you may deactivate the transfer of tick-labels or axis-label settings to other graphs. This is especially useful if you are working with packed graphs where several graphs share an axis and labels are deliberately turned off.
- In the Region-Master-Dialog you may now delete or extract points or sets inside a given region ('Operations'-tab) and you may create a region from an existing set ('Create'-tab). This is useful for regions with a complicated shape like a circle or something generated by a parametric function.
- In the feature-extraction-dialog there is now the option to restrict the extraction to a specific region.
- It is now possible to change the separation-character for date-formats. When printing dates Grace / XmGrace uses '/'. In QtGrace you may use '/', '-' or '.'. To change the separation character see Preferences (at the very bottom of Grace-tab).
- The Binary-data-import is now able use the trigger in a more general way: you can set the trigger level to an absolute value (just enter the value) or as a relative value in comparisson to the trigger-sets-min- and max-values. In this case enter the trigger level as a percentage (like '50 %'). QtGrace will then find min and max values and calculate the trigger-value in between according to the percentage. Furthermore the import of data from a ini-type-header is now more versatile. You may create a counter for the number of entries in a group of import settings (e.g. to find the number of channels if the number is not mentioned explicitely). Also you may do some preprocessing on the text-tokens in the ini file in order to separate data-values from one another (e.g. in the case that the settings for one channel are actually stored in a single entry separated by ',' and you need the different settings for the import). Also the trigger-channel can now be any of the channels present in the files regardless whether they are to be imported or not. You do not have to mark a channel as trigger-channel, just set the import channels and tell QtGrace which channel is the trigger channel (you have to specifiy the channel-number in the file, not the number of the imported channels). This should be more obvious that the method before.
- In the Set-Popup that is accessible by right-clicking on sets in any set list there is now an option to automatically label the sets. This is useful if you have a large number of sets and want to label them in a consistent manner. For this you may use text extracted from exisiting labels and/or use a number generated by the set-id or an index.
- The spreadsheet-editor is now able to display the string-column of a set. You may also extract data from the string column if necerssary. The data in the sets can be displayed and entered in different formats now. You may change the column-formats as before by clicking on the column header. But you may now also set a default column format for the spreadsheet editor in the preferences at the bottom of the defaults-tab. This setting will be stored in the ini-file of QtGrace and will be used for every new spreadsheet-dialog. Important comment for invalid data: if your data sets contain datapoints that are either "Not a Number" (NaN) or infinite (Inf) then the spreadsheet is able to display this and you may also enter NaN or Inf, but QtGrace is unable to read these values from files. Therefore saving data with invalid data points in it will result in problems during loading. The only way to remove these errors afterwards is to manually edit the saved file (the only export and import that takes invalid data into acount is the binary export).
- In the Data->Set-operations-dialog you may now swap two columns in a set (e.g. exchange X- with Y-column).
- When selecting a string-element for deleting the question dialog shows the actual text (as a help whether you have selected the correct string to delete).
- When importing several files by drag-and-drog and the question about ',' or '.' as decimal seperator appears there is now an option to say "Yes to all" or "No to all". This is very helpful if you have several files of the same format. If the "Don't ask question" option is activated in the preferences no question will be displayed, but "Yes to all" will be used. Please keep this in mind in case that your file format does not match the preselected decimal separator.
- In the Data->Tranformations->Evaluate expression... dialog you may now select an option for working with regions to evaluate the formula only on the data inside (or outside) the region and keep the remaining data unchanged. The default is that the set will be restricted to the region (the data outside the region is lost).

Changes:
- To make the page background transparent you have to select a transparent background from a combo-box instead of checking a check-box.
- In the File-Output-Setup-dialog the Quick-Format-selection is now a menu instead of a combo-box.
- I made the fill-pattern-preview-icons rectangular and larger. (I think this looks better and you can see the pattern more clearly.)
- Warning: Due to changes in Qt6 it will in the future not be possible to use different text-encodings for non-ascii-charactes. In the future QtGrace will only support UTF-8. Sorry. For this release it will be possible to read old files with non-ascii-text encoding, but files that are saved with this version of QtGrace will be saved in UTF-8.
- Internal change: when drawing sets with error-bars the error-bars are now drawn before the set itself, this way the line is always on top of the error-bars
- In the Text-Edit-Dialog (for string-items) the selector for the graph the sting-object is linked to is now always accessible (not just for alignment and world-coordinates)
- In order to have reliable upscaling of the GUI-elements I had to switch the style of the QtGrace-application to "Fusion". This is a setting that changes the look (and scaling behaviour) of the GUI-elements. The result is that QtGrace looks different from other applications. If you do not like this you have to start QtGrace with the option '-style' followed by a style-name. Which styles are present on your system depent on your Qt-installation. For example on Windows you should have "fusion", "windows" and "windowsvista". So you may use something like "qtgrace -style windows". If you want the default style of the operating system please use an invalid style-name like "qtgrace -style dummy", this will result in an error-message on the command-line but will also display the available styles.

64-bit-changes:
- Since QtGrace is now a 64bit-application on Windows (as well as on Mac OS X or Linux) any external libraries have to be 64bit as well. For the Windows-executable-package I included the fftw3-library in 64bit as can be found on the fftw3-homepage (unchanged, I just included this for conveniance).
- The pdf-library haru is not availabe as 64bit (at least I did not find any). Therefore I used the source-code of the haru-library to build a static library using Qt. This static library may be linked to QtGrace statically in order to be able to use the haru-pdf-library with 64bit-QtGrace (see description for compiling below). If you find a 64bit dll you have to deselect the static library in the src.pro file before compiling and afterwards tell QtGrace in the miscellaneous-tab in the preferences where to find the dll.


Bug-Fixes:
- Several minor problems (I can not remember every one).
- Problems with modern compilers on Mac OS X and newer Linux-distributions.
- Calculating intersection points is now more accurate. But: the algorith is not very efficient. This means that the amount of work needed is proportional to the multiplied number of points of the two sets you selected for the search of intersection points.  I.e. the amount of time it takes to find intersection points increases rapidely. This also effects the visible-intersection-shortcut Ctrl+I. Sorry. (This may be improved in a future release.)


One further request (as always): If you find bugs or missing features or if you have suggestions for improvements: Let me know immediately!
Thank you!

Warning: I changed some of the shortcut-keys in contrast to Grace: In order to use Crtl+Z for Undo I used Alt+Z for Zoom. In order to use Crtl+V for paste-operations I used Alt+V for viewport.
I hope this will be helpful to someone. Don't hesitate to report bugs/suggestions/wishes.


General Notes on QtGrace:

Dear User or Developer;

Thank you for your interest in my Qt-version of Grace!
In case you do not know already: Grace is a program to plot data and analyze it.
Grace is based on xmgrace / xmgr and uses the motif-framework for the graphical user interface.
The Grace-website is: http://plasma-gate.weizmann.ac.il/Grace/
QtGrace is based on grace-5.1.22 but uses the Qt-framework (formerly by Nokia, now Open-Source) as a graphical user interface instead (Qt and their respective logos were formerly trademarks of the Nokia Corporation but are now Open-Source as far as I know; see http://www.qt.io/).
Although I have tested QtGrace for quite a while now there may still be some problems / bugs present. Therefore give it a try, but be sure to read the Grace documentation and the documentation of the QtGrace-extensions (and save often) - and tell me when you find a bug so that I may remove it.

I hope this will be useful to someone else as well (I like my Qt-version of Grace :-) ).

My motivation: I had problems using Grace on different operating systems. Since I had written some applications using the Qt-framework (which can be found on http://www.qt.io/) I decided to modify the latest stable version of Grace (5.1.22) and replace all dialogs by equally looking Qt-replacements. I wanted to conserve the "look and feel" of Grace 5.1.22 because I wanted all Grace-users to be able to work with QtGrace immediately without having to adjust their workflow. Over the years I also included new features as I found them useful or after users suggested them.

I altered most of the source-files and therefore changes in Grace have to be implemented in QtGrace manually. Since there seems to be no major development around Grace-5.1.22 I believe that this approach is acceptable for now. All development on Grace seems to be focused on version 6 (or 5.99) which has a fundamentally different internal architecture. QtGrace is not compatible with Grace 5.99. There are other implementations of Grace 5.99 with newer graphic-libraries (like Qt and GTK) - just do a websearch if you want these.

This version 0.2.7 of QtGrace is capable of nearly all of the operations Grace 5.1.22 is able to do. Some minor functions are missing or work only on some platforms (see below). I have inserted extra functions and interface additions I found helpful (see help-page on QtGrace-Extensions). There are a few minor known problems (see below).

The internal procedures are the ones from Grace 5.1.22 (internal data management, calculations and so on). I have only recreated the user interface in Qt which makes it possible to compile QtGrace natively (!) on Windows, Mac OS X and Linux. You can load your old Grace-project files (usual extension: '.agr' / '.xgr'). With Qt it is also possible to easily translate the user interface and make the language of the interface selectable (I have made an attempt for a test-translation to German. The file is included but very far (!!) from being complete. If there is someone out there willing to complete the german translation or generate a translation to any other language feel free to do so using the "Linguist" program included in the Qt-installation. If you send me the translation I will include it into the downloadable zip-file.).

Don't alter the structure or remove content from the folders of QtGrace. The executable relies on the relative positions of the files it needs. Just use symbolic links to start QtGrace from your desktop.

I am always interested in hearing your opinion on QtGrace or in your bug-reports. Suggestions for improvements are welcome as well!
To contact me please use email: andreas.f.winter@web.de

I hope you enjoy QtGrace!

Best regards
Andreas Winter (andwin)


Copyright and License-Terms:

Copyright (C) 2008-2022 by Andreas Winter
andreas.f.winter@web.de
QtGraceTcpServer- and -Client-code (C) 2014-2017 by by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB

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

libharu and fftw3 are included unmodified to be easily accessible. See QTGRACE_EXTENSIONS.html for further informations and copyright notice.
For the precompiled releases for Windows and Mac OS X I include dlls and frameworks that are part of Qt. These files are under the same copyright as the Qt-library. I included the files for convenience unmodified.

How to build QtGrace:

QtGrace is based on the Qt-Framework which has to be obtained via http://www.qt.io/.
Please use at least version 4.6 of Qt to compile QtGrace (Qt5.5 or higher, is recommended, especially for QTcpSocket, please do not use 5.0 because it seems problematic, Qt6 should work as well)! Newer versions are needed for some features of QtGrace (latest version of Qt5 recommended).
Please install the Qt-SDK including the Qt-Creator IDE. If you are an experienced developer, you may also be able to compile QtGrace without the Qt-Creator, but I will not describe how to do so here.
Since you have extracted this folder you may notice, that there are some sub-folders. It is important for QtGrace to keep the structure of this folder in order to find some files at runtime. I have also included the original source-code-zip-file from grace-5.1.22 as a tar-archive. You will not have to extract it. This is included just for completeness and for developers use in case someone needs it. 
I assume in the following part, that you have installed Qt and you have got a fully functional Qt-Creator. 
To compile QtGrace: 
First step: Open Qt-Creator and do File->"Open File or Project..." (you may have to specify the file format as "Qt Project file (*.pro)"). Select the file "src.pro" in the 'src'-sub-folder. Please select to configure the 64bit-version if asked. On the left hand side of the IDE-window you should now be able to see all source-files that are part of QtGrace. (On newer version of QtCreator you may be asked to specify the target to compile QtGrace for - use the 'Desktop'-version in this case.)
Second step: At the left hand side of the Qt-Creator-window you find some icons. Click on the "Projects"-icon and make sure a suitable path for the Qt-Installation is set at the general section. (You may have to consult the Qt-Manual on some systems on how to do so correctly. Especially in newer versions of QtCreator you have to prepare the compiler settings in a special way, so please consult the Qt-documentation.) You have two options now: either set the build-path in the general build settings to the 'bin'-subfolder or remember the build path that has been set automatically and copy the executable after the compilation to the 'bin'-subfolder. You will also have to set the destination of the executable to be executed from within Qt Creator in the settings in order to run QtGrace after the compilation from within Qt Creator. On newer version of the QtCreator you have to manage the different kits (see the Qt-documentation). Please deactivate the "Shadow build"-option.
Third step: Click the 'Edit'-icon on the left side. Click on the 'Build All'-Icon (the one with the hammer in the lower left hand corner). This will initiate the process of compilation and may take a while. You may also see a few thousand warnings which can be ignored (hopefully) but there should be no errors (if you get errors, report them to me and I will provide information on how to solve this problem). (Special platform specific notes: see below)
Fourth step: Everything has been compiled now and you should be able to start QtGrace. Either you double-click on the qtgrace icon (or qtgrace.exe) in the bin-folder or you set the execution path in the project settings to this file and run it from within the Qt Creator.
Done! 

On Windows:
You have to set the path to the Qt-dlls in your PATH-variable or copy the dlls to the bin-folder (if you have a complete Qt-installation you can run the 'windeployqt'-tool -- see the Qt-documentation for how to do so)

On Linux:
If you continue getting messages like: invalid viewport coordinates --> please contact me.

On Mac OS X:
Executables are organized as disguised folders on such systems. Therefore after compilation in order to work in a MacOS-way you have to open the package-contents of qtgrace (right-click on qtgrace in the 'bin'-folder and select 'show package contents'). Then go to 'Contents'. Copy the contents of the original unzipped folder except the 'bin'-subfolder and the grace-5.1.22.tar.gz into this 'Contents-folder'. Copy the contents (except 'qtgrace') of the 'bin'-subfolder into the 'MacOS'-folder. Now you should be able to execute qtgrace by clicking on qtgrace in the 'bin'-subfolder. It should also be possible to move/copy QtGrace in every folder you like just like any other MacOS-application (put it for example in your Applications-folder). There is also a deployment-tool: 'macdeployqt' (see Qt-documentation) whih is very helpful, but does not copy the Grace-specific files needed for QtGrace.


Activate FFTW3- and libHaru(PDF)-support:
See QTGRACE_EXTENSIONS.html for further informations, go to Preferences-->Micellaneous and see whether the libraries are usable and activated.
If the libraries are not compiled into QtGrace statically you have to tell QtGrace where the dynamic library is located. This should usually be the bin-subfolder in the QtGrace-directory. The name of the library is usually libfftw3-3.dll and libhpdf.dll. If everything is ok you should see green instead of red boxes on the right hand side of the dialog. Please remember: you will need the correct version of the libraries that are either 32-bit or 64-bit like QtGrace.
If the Haru-PDF-library is activated you have to use the "PDF(Haru)"-option as the output format in the Export Setup Dialog to export pdf-files. Do only use the PDF-output-format (without "(Haru)") if the Haru-pdf-library is not available or if you have to use the QtFonts. 
Compile Haru-pdf-library as 64bit static library:
I included a subdirectory ("libharu64") in the QtGrace folder. I just included the source-files from the haru-pdf-library (without the usual config-files) and a Qt-project file. To compile libharu open the QtCreator and select libhpdf.pro from the libharu64/libhpdf-folder. Select the 64bit version if asked. Then just create everything. If this is successfull you should get a file called 'libhpdf.a' in the libharu64/libhpdf-folder. This is the static library. To tell the QtCreator to use it close the libhpdf project and open the src.pro-file in the src-subfolder with QtCreator. Then edit the following lines: remove the '#' at the beginning of the line for the 4 lines beginning with "DEFINES += USE_STATIC_EXT_LIB_HARU". You also have to enter the complete path to the new libhpdf.a-file here. Now just (re-)compile QtGrace.


Known Problems:

- Printing directly to a printer via the native printer-dialog is possible but I have not tested it extensively (but it seem to work in an acceptable way). Most settings are not mirrored in the native dialog. Direct printing without the native printer-dialog is only possible on systems that support the 'lpr' printing command (or any other printer-command like 'kprinter' on some linux-systems) and accept postscript files (i.e. a system command like 'lpr file.ps' will actually print the contents of 'file.ps' on the standard-(line)-printer). Printing to files is always possible. You have to physically print the file-contents via a different/external program (like OpenOffice/LibreOffice). You may also copy the whole page in the clipboard via View->PageSetup and paste it into your external program. Printing on Mac-system without a physical printer present can be tricky if you use Qt5 - sorry (previously the pdf-option was always accessible).
- On some Linux-systems the scrollbars are not displayed correctly but they still stay usable (I don't know why, yet. Seems to be a Qt-problem...; I also don't know whether this bug is still present, since I have not seen it in a while).
- Pipes not working on Windows (at least not in the same way as on unix-systems).
- The switching between Grace-fonts and QtFonts is not advisable. It is possible in most cases, but especially for the Symbol-font problems are to be expected - sorry. This has several reasons especially with the ordering of characters in the T1-Symbol-font in Grace and UTF-8-fonts. At the moment I do not know how to solve this problem. I made an attempt by making the symbol-font special (a separate setting in the preferences). This should help - but this approach may not be flawless.
- Loading files (especially old ones) relies heavily on the encoding set in the Preferences-dialog (QtGrace-Behavior). For loading example-files it is often advisable to use the Latin1-encoding (or something similar). Newer files should contain the encoding used during saving and should be loadable without problems.
- Via the undo-function you can also create scripts. When using the feature extraction dialog you can select as X-values to convert the legend to a value. This option can not be used in scripts (yet) because there is no script-function to convert strings to values. The automatically generated script commands use the Set-ID instead of the value from the legend.
- With the haru-pdf-library I had sometimes problems when trying to overwrite existing pdf-files that have already been opened externally. I do not know the reason for this. The only solution is to delete the existing file manually.


Things that work in Grace, but not in QtGrace:

- I have rewritten the spreadsheet-editor. Therefore it may not work as you are used to it (or maybe you won't even notice).
- I have written a simple text-editor for creating and editing sets. On Unix the default in Grace is to open 'vi' ('Create new->In text editor'). This option is not portable (at least not in a way I would like). I used an internal text-editor because this seems to be comfortable enough (suggestions for improvements are welcome).
- The pipes may work a bit differently in QtGrace, because I could not include the 'pipe-contents-changed' signal into the main-application loop like Grace does. Pipes also do not work on Windows, yet.


Additions to QtGrace (not present in Grace): see help-page on QtGrace-Extensions (because there are a lot)


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

v0.2.2		'-free' option and 'immediate update' reactivated, experimental support for non-latin-characters and different fonts; support for multiple files in binary import and the use of a trigger channel; bug fixes

v0.2.3		minor bugfixes; print-command saved in preferences; selection of odd or even set-ids; addition of fit-button for page-zoom-slider; minor changes to improve UTF-8-support in file-paths

v0.2.4		several bugfixes (major and minor); further improvement in UTF-8-support (very extensive internal changes); made decimal-separator selectable throughout the whole QtGrace-Gui; improvement in the script-generation (execution of scripts with automatically replaced set-ids); slider-value made editable; QtGrace-help-page; QtGrace-examples

v0.2.5		rework of the binary import (incl. drag-and-drop); addition of regions-master-dialog; additions to the commands-dialog to extend the scripting possibilities; increased the number of fit-parameters to 20; redesign of the output-dialog, separation of file-output from physical printer, adding a quick-selection-options to set the resolution; added the possibility to use libHaru (for pdf-output) and libFFTW3 (for better fourier-transformation); further extension of the preferences dialog (many things are now user-adjustable); drag-and-drop for file-import-selection-lines; more comfortable dragging-and-dropping of Grace-project-files (agr); separate dialog to let you extract single sets from an agr-file; restrict sets to regions and swap columns of a set (like swap X- and Y-axis); extension of the feature-extraction-dialog (more features); project description now visible in the open-projects-dialog and editable in the plot-appearance-dialog; addition of a file-path-stamp to the project; global font-size-multiplier; further options for placing the legend-box (manual and automatic alignment with the graphs borders); changed the apply-to-option for the axis-dialog; additional dialog to determine set-intersection-points and -angles; addition of QTcpSocket-support for remote control (by Vadim Engelson and Nimalendiran Kailasanathan, Wolfram MathCore AB); bug-fixes

v0.2.5a		Bugfixes

v0.2.6		Lots of interface improvements: Setup-Wizard, Apply single settings on multiple sets, text editable in main drawing area, Icons in set lists, Highlighting, Extended context menu in main drawing area, Transparency, Pan and Move, Page-size-change by click and drag, Error-bars in legends, connected errorbars, Ignore a set during autoscaling, Syncing selection of source- and destination, Size of tool bar changeable, New polar plot, Mouse-wheel-zooming and -size-changes, title- and subtitle-position adjustable, New viewport stack control, Antialiasing for all objects (not only text), Auto "Pack all sets", More icons on buttons/in lists, Error-bars concidered in autoscale, Popup-menu cleared up and extended with sub-menus, Multiple commands in "evaluate expression" allowed now (again), Undo setting separated in current- and startup-setting, More explanations and unified messages on incomplete command line arguments, Auto selection of new sets changed, "All Sets"/"All Graphs" only displayed if more than one entry in list, New formula preprocessor, Letters as symbols for set-points can now be selected from a list (no ascii-code needed any more), Dialog sizes stored in ini-file, Select legend by double-click, Popup for the color selectors and linestyles can be closed by pressing Esc or by clicking outside, Shift viewing area with Ctrl+Arrow keys, delete sets or graphs or lines in spreadsheet with Del-key, Tool-buttons stay pressed as long as the action is active, Import data with drag and drog with CR / LF / CR+LF as line-end-characters, Additional icons, Error-bars now visible even if set point outside viewing area, minimal support for touch-gestures, gracerc-file fully usable again, improved support for relative paths for files, Esc-key halts all operations (as intended), slider-values no longer unstable (no shifting when apply is pressed, no more autonomous movements), Code revision.

v0.2.7          File compression; Improved data import for different complicated data formats like Date/Time; improved and extended csv-import; fitting of Circles and Ellipses; generating sets as average of several sets, column-swapping, set restriction to areas; new option for alignment (left/right/center) of multi-line-text; auto-alignment of text-objects to graph or page; clipping of the empty areas at the page borders for file-output in pixel-formats (bmp/jpg/png/tiff); images can be used as background or for filling of objects; Quick-format-selection for output now allows three user defined settings; several new options for increasing the sizes of GUI-elements; if-clause for scripts; different types of commands in a script are displayed in different colors; spreadsheet editor now able to display and manipulate string-column;encoding only in UTF-8 possible due to Qt6; impovement for increased screen resolutions; edit line objects by dragging end-points; moving objects may be locked to X- or Y-axis by pressing 'Alt' or 'Crtl'; applying axis-settings to several axes with fine-tuning; extended options for regions (data extraction, creation of regions from sets); feature-extraction may be restricted to a region; kill string question shows text; YesToAll and NoToAll for drag-and-drop; evaluate expression for data inside region, but keep outside uncahnged; separation character in date-format changable ('/', '-' or '.'); improvments for binary import: trigger-channel is normal channel now, trigger-level may be set relativ (not just absolut), counters and prepocessors for ini-headers; autolabel-function in set-contex-menu


Have fun using QtGrace!

Best regards
Andreas Winter (andwin)





