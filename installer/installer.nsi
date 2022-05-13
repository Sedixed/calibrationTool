;-------------------------------------------------------------------------------
; Includes
!include "MUI2.nsh"
!include "LogicLib.nsh"
!include "WinVer.nsh"
!include "x64.nsh"


;-------------------------------------------------------------------------------
; Attributes
Name "Calibration Tool"
OutFile "Calibration Tool Setup.exe"
InstallDir "$PROGRAMFILES64\Calibration Tool"
RequestExecutionLevel admin

;-------------------------------------------------------------------------------
; Modern UI Appearance
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\orange-install.ico"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\orange.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\orange.bmp"
!define MUI_FINISHPAGE_NOAUTOCLOSE

;-------------------------------------------------------------------------------
; Installer Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

;-------------------------------------------------------------------------------
; Uninstaller Pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages
!insertmacro MUI_LANGUAGE "English"

;-------------------------------------------------------------------------------
; Installer Sections
Section "Calibration Tool" CalibInstall
	SetOutPath $INSTDIR
	File "..\Calibration Tool.exe"
    SetOutPath $INSTDIR\docs
    File "..\docs\user_manual.pdf"
    File "..\docs\technical_manual.pdf"
    SetOutPath $INSTDIR
    File /r "..\libs\wx\"
    File /r "..\libs\opencv\"
    File /r "..\libs\mingw\"
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    CreateDirectory "$SMPROGRAMS\Calibration Tool"
    CreateShortCut "$DESKTOP\Calibration Tool.lnk" "$INSTDIR\Calibration Tool.exe" 
    CreateShortCut "$SMPROGRAMS\Calibration Tool\Calibration Tool.lnk" "$INSTDIR\Calibration Tool.exe"
SectionEnd

;-------------------------------------------------------------------------------
; Uninstaller Sections
Section "Uninstall" CalibUninstall
    Delete "$INSTDIR\docs\*"
    RMDir "$INSTDIR\docs"
	Delete "$INSTDIR\*"
	RMDir "$INSTDIR"
SectionEnd