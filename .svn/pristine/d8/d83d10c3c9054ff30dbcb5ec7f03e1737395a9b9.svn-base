# ====================== Duilib NSIS ===========================
# 2016.07.04 - Created by Linzw
# 7Z打开空白
!system '>blank set/p=MSCF<nul'
!packhdr temp.dat 'cmd /c Copy /b temp.dat /b +blank&&del blank'
SetCompressor LZMA
# ====================== 自定义宏 ==============================
!define PRODUCT_NAME 								"DaDaJiaSuSetup"
!define PRODUCT_VERSION 						"2.0.16.401"
!define PRODUCT_PUBLISHER 					"DaDaJiaSu.com"
!define PRODUCT_FILE_DESC  					"哒哒加速器"
!define PRODUCT_WEB_SITE 						"www.dadajiasu.com"
!define PRODUCT_LEGAL		 						"深圳幻美网络科技有限公司"
!define PRODUCT_LEGAL_RIGHT					"(C) DaDaJiaSu.Com All Rights Reserved."
!define PRODUCT_PATHNAME  					"DaDaJiaSu"
!define PRODUCT_UNINST_ROOT_KEY 		"HKLM"
!define PRODUCT_UNINST_KEY 					"Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

# ===================== 外部插件以及宏 =============================
!include	"MUI2.nsh"
!include	"x64.nsh"
!include 	"FileFunc.nsh"
!include	"nsWindows.nsh"
!AddPluginDir "nsPlugin"

# ===================== 自定义页面==================================
Page custom					DUI_InitWindow
Page instfiles	"" 	NSIS_InstPage

# ===================== 安装包版本 =============================
!insertmacro 	 			MUI_LANGUAGE 			 "SimpChinese"
VIProductVersion											 "${PRODUCT_VERSION}"
VIAddVersionKey		 "ProductVersion"    "${PRODUCT_VERSION}"
VIAddVersionKey		 "ProductName"       "${PRODUCT_NAME}"
VIAddVersionKey		 "CompanyName"       "${PRODUCT_PUBLISHER}"
VIAddVersionKey		 "FileVersion"       "${PRODUCT_VERSION}"
VIAddVersionKey		 "FileDescription" 	 "${PRODUCT_FILE_DESC}"
VIAddVersionKey		 "LegalCopyright"    "${PRODUCT_LEGAL_RIGHT}"
VIAddVersionKey		 "LegalTrademarks"   "${PRODUCT_LEGAL}"

# ==================== NSIS属性 ================================
# 安装包名字.
Name "${PRODUCT_FILE_DESC}"
# 安装程序文件名.
OutFile "bin\${PRODUCT_NAME}.${PRODUCT_VERSION}.exe"
# 默认安装位置.
InstallDir "$PROGRAMFILES\${PRODUCT_PATHNAME}"
# 针对Vista和win7 的UAC进行权限请求.
RequestExecutionLevel admin
# 图标
Icon "image\logo.ico"
# 必要变量
Var hInstallDlg ;Dui窗口句柄
Var Param       ;当前运行参数

#=========================安装过程===================================
Section MainSetup
  nsDui::ReSize 600 600
	Sleep 100
	Sleep 100
	Sleep 100
	Sleep 100
	Sleep 100
	nsDui::ReSize 600 400
SectionEnd

#=========================初始化===================================
Function .onInit
	${Getparameters} $Param
	InitPluginsDir
	File /oname=$PLUGINSDIR\skin.zip "image\skin.zip"
	nsDui::NewDUISetup "${PRODUCT_FILE_DESC}安装向导" "install.xml"
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

#=========================控件绑定===================================
Function  DUI_Bind_Function
	# 中断响应绑定
	GetFunctionAddress $0 OnError
  nsDui::BindNSIS "dn_error" $0
  
	# 控件响应绑定
  GetFunctionAddress $0 OnBtnExit
  nsDui::BindNSIS "btn_close" $0

  GetFunctionAddress $0 OnBtnFastinst
  nsDui::BindNSIS "btn_fastinst" $0

  GetFunctionAddress $0 OnBtnBack
  nsDui::BindNSIS "btn_back" $0
  
FunctionEnd
#=========================中断、控件响应=============================
Function OnError
	MessageBox MB_OK "安装已中断！"
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




