# ====================== Duilib NSIS ===========================
# 2016.07.04 - Created by Linzw
# 7Z�򿪿հ�
!system '>blank set/p=MSCF<nul'
!packhdr temp.dat 'cmd /c Copy /b temp.dat /b +blank&&del blank'
SetCompressor LZMA
# ====================== �Զ���� ==============================
!define PRODUCT_NAME 								"DaDaJiaSuSetup"
!define PRODUCT_VERSION 						"2.0.16.401"
!define PRODUCT_PUBLISHER 					"DaDaJiaSu.com"
!define PRODUCT_FILE_DESC  					"���ռ�����"
!define PRODUCT_WEB_SITE 						"www.dadajiasu.com"
!define PRODUCT_LEGAL		 						"���ڻ�������Ƽ����޹�˾"
!define PRODUCT_LEGAL_RIGHT					"(C) DaDaJiaSu.Com All Rights Reserved."
!define PRODUCT_PATHNAME  					"DaDaJiaSu"
!define PRODUCT_UNINST_ROOT_KEY 		"HKLM"
!define PRODUCT_UNINST_KEY 					"Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

# ===================== �ⲿ����Լ��� =============================
!include	"MUI2.nsh"
!include	"x64.nsh"
!include 	"FileFunc.nsh"
!include	"nsWindows.nsh"
!AddPluginDir "nsPlugin"

# ===================== �Զ���ҳ��==================================
Page custom					DUI_InitWindow
Page instfiles	"" 	NSIS_InstPage

# ===================== ��װ���汾 =============================
!insertmacro 	 			MUI_LANGUAGE 			 "SimpChinese"
VIProductVersion											 "${PRODUCT_VERSION}"
VIAddVersionKey		 "ProductVersion"    "${PRODUCT_VERSION}"
VIAddVersionKey		 "ProductName"       "${PRODUCT_NAME}"
VIAddVersionKey		 "CompanyName"       "${PRODUCT_PUBLISHER}"
VIAddVersionKey		 "FileVersion"       "${PRODUCT_VERSION}"
VIAddVersionKey		 "FileDescription" 	 "${PRODUCT_FILE_DESC}"
VIAddVersionKey		 "LegalCopyright"    "${PRODUCT_LEGAL_RIGHT}"
VIAddVersionKey		 "LegalTrademarks"   "${PRODUCT_LEGAL}"

# ==================== NSIS���� ================================
# ��װ������.
Name "${PRODUCT_FILE_DESC}"
# ��װ�����ļ���.
OutFile "bin\${PRODUCT_NAME}.${PRODUCT_VERSION}.exe"
# Ĭ�ϰ�װλ��.
InstallDir "$PROGRAMFILES\${PRODUCT_PATHNAME}"
# ���Vista��win7 ��UAC����Ȩ������.
RequestExecutionLevel admin
# ͼ��
Icon "image\logo.ico"
# ��Ҫ����
Var hInstallDlg ;Dui���ھ��
Var Param       ;��ǰ���в���

#=========================��װ����===================================
Section MainSetup
  nsDui::ReSize 600 600
	Sleep 100
	Sleep 100
	Sleep 100
	Sleep 100
	Sleep 100
	nsDui::ReSize 600 400
SectionEnd

#=========================��ʼ��===================================
Function .onInit
	${Getparameters} $Param
	InitPluginsDir
	File /oname=$PLUGINSDIR\skin.zip "image\skin.zip"
	nsDui::NewDUISetup "${PRODUCT_FILE_DESC}��װ��" "install.xml"
	Pop $hInstallDlg
FunctionEnd

Function DUI_InitWindow
	Call DUI_Bind_Function
 	nsDui::ShowPage
FunctionEnd

Function NSIS_InstPage
  ShowWindow $HWNDPARENT ${SW_HIDE}
	${NSW_SetWindowSize} $HWNDPARENT 0 0
	nsDui::InstBindNSIS
FunctionEnd

#=========================�ؼ���===================================
Function  DUI_Bind_Function
	# �ж���Ӧ��
	GetFunctionAddress $0 OnError
  nsDui::BindNSIS "dn_error" $0
  
	# �ؼ���Ӧ��
  GetFunctionAddress $0 OnBtnExit
  nsDui::BindNSIS "btn_close" $0

  GetFunctionAddress $0 OnBtnFastinst
  nsDui::BindNSIS "btn_fastinst" $0

  GetFunctionAddress $0 OnBtnBack
  nsDui::BindNSIS "btn_back" $0
  
FunctionEnd
#=========================�жϡ��ؼ���Ӧ=============================
Function OnError
	MessageBox MB_OK "��װ���жϣ�"
FunctionEnd

Function OnBtnExit
	nsDui::ExitDUISetup
FunctionEnd

Function OnBtnFastinst
	nsDui::NextPage 2
	nsDui::InstPage 0
FunctionEnd

Function OnBtnBack
	nsDui::PrePage 1
FunctionEnd




