# Microsoft Developer Studio Project File - Name="QalibServer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=QalibServer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "QalibServer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "QalibServer.mak" CFG="QalibServer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "QalibServer - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QalibServer - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QalibServer - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QalibServer - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QalibServer - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "QalibServer - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "QalibServer - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "C:\OPTIVEC\INCLUDE" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib ovvcmdd.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /libpath:"C:\OPTIVEC\LIB" /delayload:mtxex.dll
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=.\Debug\QalibServer.dll
InputPath=.\Debug\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "QalibServer - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\QalibServer.dll
InputPath=.\DebugU\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "QalibServer - Win32 Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /I "C:\OPTIVEC\INCLUDE" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# SUBTRACT MTL /Oicf
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib ovvcmdr.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"C:\OPTIVEC\LIB" /delayload:mtxex.dll
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=.\ReleaseMinSize\QalibServer.dll
InputPath=.\ReleaseMinSize\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "QalibServer - Win32 Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /machine:I386 /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\QalibServer.dll
InputPath=.\ReleaseMinDependency\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "QalibServer - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /machine:I386 /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\QalibServer.dll
InputPath=.\ReleaseUMinSize\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "QalibServer - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 mtx.lib mtxguid.lib delayimp.lib /nologo /subsystem:windows /dll /machine:I386 /delayload:mtxex.dll
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\QalibServer.dll
InputPath=.\ReleaseUMinDependency\QalibServer.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Execute mtxrereg.exe before using MTS components in MTS 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "QalibServer - Win32 Debug"
# Name "QalibServer - Win32 Unicode Debug"
# Name "QalibServer - Win32 Release MinSize"
# Name "QalibServer - Win32 Release MinDependency"
# Name "QalibServer - Win32 Unicode Release MinSize"
# Name "QalibServer - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Albumin.cpp
# End Source File
# Begin Source File

SOURCE=.\AnalyteTypeClass.cpp
# End Source File
# Begin Source File

SOURCE=.\AssignedValues.cpp
# End Source File
# Begin Source File

SOURCE=.\BarCodes.cpp
# End Source File
# Begin Source File

SOURCE=.\BestFitMethods.cpp
# End Source File
# Begin Source File

SOURCE=.\BusinessRules.cpp
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesEnforce.cpp
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesSupport.cpp
# End Source File
# Begin Source File

SOURCE=.\CalibrationCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\ChemistryDependent.cpp
# End Source File
# Begin Source File

SOURCE=.\Chloride.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientData.cpp
# End Source File
# Begin Source File

SOURCE=.\Controls.cpp
# End Source File
# Begin Source File

SOURCE=.\DbAccess.cpp
# End Source File
# Begin Source File

SOURCE=.\DbUtils.cpp
# End Source File
# Begin Source File

SOURCE=.\derivatives.cpp
# End Source File
# Begin Source File

SOURCE=.\dlldatax.c
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ErrorProcessing.cpp
# End Source File
# Begin Source File

SOURCE=.\ExponentialCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\ExponentialDerivatives.cpp
# End Source File
# Begin Source File

SOURCE=.\FitParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\FixedSlope.cpp
# End Source File
# Begin Source File

SOURCE=.\FunctionArray.cpp
# End Source File
# Begin Source File

SOURCE=.\HelperFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearFitParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearResults.cpp
# End Source File
# Begin Source File

SOURCE=.\Loging.cpp
# End Source File
# Begin Source File

SOURCE=.\MinMaxDecayResults.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFitParameters.cpp
# End Source File
# Begin Source File

SOURCE=.\OutlierCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\Outliers.cpp
# End Source File
# Begin Source File

SOURCE=.\QalibConstants.cpp
# End Source File
# Begin Source File

SOURCE=.\QalibServer.cpp
# End Source File
# Begin Source File

SOURCE=.\QalibServer.def
# End Source File
# Begin Source File

SOURCE=.\QalibServer.idl
# ADD MTL /tlb ".\QalibServer.tlb" /h "QalibServer.h" /iid "QalibServer_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\QalibServer.rc
# End Source File
# Begin Source File

SOURCE=.\QalibServerMain.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadraticCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\QuadraticResults.cpp
# End Source File
# Begin Source File

SOURCE=.\Results.cpp
# End Source File
# Begin Source File

SOURCE=.\Rotor.cpp
# End Source File
# Begin Source File

SOURCE=.\SigmoidalCalculations.cpp
# End Source File
# Begin Source File

SOURCE=.\SigmoidalResults.cpp
# End Source File
# Begin Source File

SOURCE=.\Species.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusTest.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Support.cpp
# End Source File
# Begin Source File

SOURCE=.\T4.CPP
# End Source File
# Begin Source File

SOURCE=.\Triglycerides.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Albumin.h
# End Source File
# Begin Source File

SOURCE=.\AnalyteTypeClass.h
# End Source File
# Begin Source File

SOURCE=.\AssignedValues.h
# End Source File
# Begin Source File

SOURCE=.\BarCodes.h
# End Source File
# Begin Source File

SOURCE=.\BestFitMethods.h
# End Source File
# Begin Source File

SOURCE=.\BusinessRules.h
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesEnforce.h
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesProcess.h
# End Source File
# Begin Source File

SOURCE=.\BusinessRulesSupport.h
# End Source File
# Begin Source File

SOURCE=.\CalibrationCalculations.h
# End Source File
# Begin Source File

SOURCE=.\ChemistryDependent.h
# End Source File
# Begin Source File

SOURCE=.\Chloride.h
# End Source File
# Begin Source File

SOURCE=.\ClientData.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\Controls.h
# End Source File
# Begin Source File

SOURCE=.\Creatinine.h
# End Source File
# Begin Source File

SOURCE=.\DbAccess.h
# End Source File
# Begin Source File

SOURCE=.\dboGetAllowedSampleSpecies.H
# End Source File
# Begin Source File

SOURCE=.\dboGetAnyFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboGetApplyOutlierLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetAssignedFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboGetAssignedValueLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetAssignedValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetAssignedValuesToPlot.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalculatedAssignedValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationAlgorithm.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationBarCode.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationData.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationDataResults.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationMode.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationSpecies.H
# End Source File
# Begin Source File

SOURCE=.\dboGetCalibrationValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetChemistryBusinessRules.H
# End Source File
# Begin Source File

SOURCE=.\dboGetChemistryName.H
# End Source File
# Begin Source File

SOURCE=.\dboGetChemistryNameFromPartNumber.H
# End Source File
# Begin Source File

SOURCE=.\dboGetChemistrySpeciesDependencies.H
# End Source File
# Begin Source File

SOURCE=.\dboGetChemistryWavelength.H
# End Source File
# Begin Source File

SOURCE=.\dboGetDataStatus.H
# End Source File
# Begin Source File

SOURCE=.\dboGetDiluent.H
# End Source File
# Begin Source File

SOURCE=.\dboGetEditableFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboGetFitParameterFactors.H
# End Source File
# Begin Source File

SOURCE=.\dboGetFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboGetFitParametersLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetFixedSlopeValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetGoodnessOfFitTests.H
# End Source File
# Begin Source File

SOURCE=.\dboGetListOfRequiredSamples.H
# End Source File
# Begin Source File

SOURCE=.\dboGetListOfSamplesToPlot.H
# End Source File
# Begin Source File

SOURCE=.\dboGetLowerOutlierLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetOutlierLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetPartNumber.H
# End Source File
# Begin Source File

SOURCE=.\dboGetPartNumberFromChemistryName.H
# End Source File
# Begin Source File

SOURCE=.\dboGetPlotData.H
# End Source File
# Begin Source File

SOURCE=.\dboGetPlotResults.H
# End Source File
# Begin Source File

SOURCE=.\dboGetPlotSampleSizes.H
# End Source File
# Begin Source File

SOURCE=.\dboGetRecoveredValuesFromRates.H
# End Source File
# Begin Source File

SOURCE=.\dboGetRotorFactors.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleIdsWithType.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleInstruments.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleLevel.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleSizeExcludeOutliers.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleSizeIncludeOutliers.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleSizes.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleSpecies.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleStatus.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleTypeIDs.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleTypes.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSampleValuesIDs.H
# End Source File
# Begin Source File

SOURCE=.\dboGetSigmoidalEditableFitParametersValues.H
# End Source File
# Begin Source File

SOURCE=.\dboGetUpperOutlierLimits.H
# End Source File
# Begin Source File

SOURCE=.\dboGetValuesAndStatus.H
# End Source File
# Begin Source File

SOURCE=.\dboGetWeightMethod.H
# End Source File
# Begin Source File

SOURCE=.\dboInsertLinearFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboInsertQuadraticFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboInsertRecoveredValuesFromRates.H
# End Source File
# Begin Source File

SOURCE=.\dboInsertSampleData.H
# End Source File
# Begin Source File

SOURCE=.\dboInsertSigmoidalFitParameters.H
# End Source File
# Begin Source File

SOURCE=.\dboRotorCuvetteAttributes.H
# End Source File
# Begin Source File

SOURCE=.\dboSetComment.H
# End Source File
# Begin Source File

SOURCE=.\dboTestSampleType.H
# End Source File
# Begin Source File

SOURCE=.\dboUpdateDataStatus.H
# End Source File
# Begin Source File

SOURCE=.\dboUserAllowedAccess.H
# End Source File
# Begin Source File

SOURCE=.\DbUtils.h
# End Source File
# Begin Source File

SOURCE=.\derivatives.h
# End Source File
# Begin Source File

SOURCE=.\dlldatax.h
# PROP Exclude_From_Scan -1
# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ErrorProcessing.h
# End Source File
# Begin Source File

SOURCE=.\ExponentialCalculations.h
# End Source File
# Begin Source File

SOURCE=.\ExponentialDerivatives.h
# End Source File
# Begin Source File

SOURCE=.\FitParameters.h
# End Source File
# Begin Source File

SOURCE=.\FixedSlope.h
# End Source File
# Begin Source File

SOURCE=.\FunctionArray.h
# End Source File
# Begin Source File

SOURCE=.\globals.h
# End Source File
# Begin Source File

SOURCE=.\HelperFunctions.h
# End Source File
# Begin Source File

SOURCE=.\LinearCalibration.h
# End Source File
# Begin Source File

SOURCE=.\LinearFitParameters.h
# End Source File
# Begin Source File

SOURCE=.\LinearResults.h
# End Source File
# Begin Source File

SOURCE=.\Loging.h
# End Source File
# Begin Source File

SOURCE=.\MinMaxDecayResults.h
# End Source File
# Begin Source File

SOURCE=.\NewFitParameters.h
# End Source File
# Begin Source File

SOURCE=.\OutlierCalculations.h
# End Source File
# Begin Source File

SOURCE=.\Outliers.h
# End Source File
# Begin Source File

SOURCE=.\ProcessBusinessRules.h
# End Source File
# Begin Source File

SOURCE=.\QalibConstants.h
# End Source File
# Begin Source File

SOURCE=.\QalibServerMain.h
# End Source File
# Begin Source File

SOURCE=.\QalibStructs.h
# End Source File
# Begin Source File

SOURCE=.\QuadraticCalculations.h
# End Source File
# Begin Source File

SOURCE=.\QuadraticResults.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Results.h
# End Source File
# Begin Source File

SOURCE=.\Rotor.h
# End Source File
# Begin Source File

SOURCE=.\RotorFactors.h
# End Source File
# Begin Source File

SOURCE=.\SigmoidalCalculations.h
# End Source File
# Begin Source File

SOURCE=.\SigmoidalResults.h
# End Source File
# Begin Source File

SOURCE=.\Species.h
# End Source File
# Begin Source File

SOURCE=.\Status.h
# End Source File
# Begin Source File

SOURCE=.\StatusTest.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Support.h
# End Source File
# Begin Source File

SOURCE=.\T4.H
# End Source File
# Begin Source File

SOURCE=.\Triglycerides.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\QalibServerMain.rgs
# End Source File
# End Group
# End Target
# End Project
