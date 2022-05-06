; The name of the installer
Name "Calibration Tool"

;The File to Write
OutFile "Calibration Tool Setup.exe"

;The default installation directory
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

Section ""
SetOutPath $INSTDIR
File "..\Calibration Tool.exe"
File "..\docs\user_manual.pdf"
SetOutPath $INSTDIR\libs
File /r "..\libs\"
SetOutPath $INSTDIR
CreateDirectory "$SMPROGRAMS\Calibration Tool"
CreateShortCut "$DESKTOP\Calibration Tool.lnk" "$INSTDIR\Calibration Tool.exe" 
CreateShortCut "$SMPROGRAMS\Calibration Tool\Calibration Tool.lnk" "$INSTDIR\Calibration Tool.exe"
SectionEnd