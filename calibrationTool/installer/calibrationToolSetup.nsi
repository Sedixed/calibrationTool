; The name of the installer
Name "Calibration Tool"

; The file to write
OutFile "Calibration Tool Setup.exe"

; The default installation directory
InstallDir "C:\Program Files (x86)\Calibration Tool"

!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Administrator rights required!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend

Page directory
Page instfiles

Section "Install"

; Uninstaller
WriteUninstaller $INSTDIR\uninstaller.exe

SetOutPath $INSTDIR
File "CalibrationGui.exe" 
File "User Manual.pdf"
File "highgui100.dll"
File "msvcr80.dll"
File  "cxcore100.dll"
File "libguide40.dll"
File "cv100.dll"
CreateDirectory "$SMPROGRAMS\Calibration Tool"
CreateShortCut "$DESKTOP\Calibration Tool.lnk" "$INSTDIR\CalibrationGui.exe" 
CreateShortCut "$SMPROGRAMS\Calibration Tool\Calibration Tool.lnk" "$INSTDIR\CalibrationGui.exe"
CreateShortCut "$SMPROGRAMS\Calibration Tool\User Manual.lnk" "$INSTDIR\User Manual.pdf"
SectionEnd

Section "Uninstall"

Delete $INSTDIR\uninstaller.exe
# Delete the directory
RMDir $INSTDIR

SectionEnd