# Microsoft Developer Studio Project File - Name="libSimpl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=libSimpl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libSimpl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libSimpl.mak" CFG="libSimpl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libSimpl - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "libSimpl - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libSimpl - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "include" /I "src\third_party" /I "src" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "libSimpl - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "include" /I "src\third_party" /I "src" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "libSimpl - Win32 Release"
# Name "libSimpl - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "libSimpl Src"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=.\src\gray.c
# End Source File
# Begin Source File

SOURCE=.\src\image.c
# End Source File
# Begin Source File

SOURCE=.\src\simpl.c
# End Source File
# End Group
# Begin Group "libJpeg Src"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jaricom.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcapimin.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcapistd.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcarith.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jccoefct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jccolor.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcdctmgr.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jchuff.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcinit.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcmainct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcmarker.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcmaster.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcomapi.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcparam.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcprepct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jcsample.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jctrans.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdapimin.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdapistd.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdarith.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdatadst.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdatasrc.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdcoefct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdcolor.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jddctmgr.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdhuff.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdinput.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdmainct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdmarker.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdmaster.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdmerge.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdpostct.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdsample.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdtrans.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jerror.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jfdctflt.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jfdctfst.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jfdctint.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jidctflt.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jidctfst.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jidctint.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jmemansi.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jmemmgr.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jquant1.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jquant2.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jutils.c"
# End Source File
# End Group
# Begin Group "libPNG Src"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\png.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngerror.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngget.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngmem.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngpread.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngread.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngrio.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngrtran.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngrutil.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngset.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngtrans.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngwio.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngwrite.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngwtran.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngwutil.c"
# End Source File
# End Group
# Begin Group "libZip Src"

# PROP Default_Filter "c"
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\adler32.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\compress.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\crc32.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\deflate.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\gzclose.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\gzlib.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\gzread.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\gzwrite.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\infback.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inffast.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inflate.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inftrees.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\trees.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\uncompr.c"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\zutil.c"
# End Source File
# End Group
# Begin Source File

SOURCE=.\test\main.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "libSimpl Hdr"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\include\gray.h
# End Source File
# Begin Source File

SOURCE=.\include\image.h
# End Source File
# Begin Source File

SOURCE=.\include\simpl.h
# End Source File
# End Group
# Begin Group "libJpeg Hdr"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jconfig.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jdct.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jerror.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jinclude.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jmemsys.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jmorecfg.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jpegint.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jpeglib.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\jpeg-8b\jversion.h"
# End Source File
# End Group
# Begin Group "libPNG Hdr"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\png.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\lpng-1.2.49\pngconf.h"
# End Source File
# End Group
# Begin Group "libZip Hdr"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\crc32.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\deflate.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\gzguts.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inffast.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inffixed.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inflate.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\inftrees.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\trees.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\zconf.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\zconf.in.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\zlib.h"
# End Source File
# Begin Source File

SOURCE=".\src\third_party\zlib-1.2.7\zutil.h"
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
