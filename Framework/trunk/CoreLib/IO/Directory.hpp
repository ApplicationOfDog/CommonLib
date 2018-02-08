#pragma once

namespace System
{
	namespace IO
	{
		///for searchOpts
		#define SEARCH_FILE 0x1
		#define SEARCH_DIRECTORY 0x2
		#define SEARCH_FILTERATTR_AND 0x4
		#define SEARCH_ERROR_EXIT 0x8
		#define SEARCH_IS_INCLUDE_FILTER 0x10	//是否为包含过滤
		#define SEARCH_ALL_DIRECTORIES -1
		#define SEARCH_DIR_FIRST_TRAVER 0 //先序遍历
		#define SEARCH_DIR_LAST_TRAVER 1 //后序遍历

		///for backOpts
		#define BACKOPT_FILE 0x1	   //返回文件
		#define BACKOPT_DIRECTORY 0x2 //返回文件夹
		#define BACKOPT_FULL_PATH 0x4 //该位1表示返回全路径,0表示返回文件名称

		///<summary>搜索选项</summary>
		struct SearchOption 
		{
			enum _DirOptions
			{
				TContinue=0,//继续往下遍历
				TStopInDir,//结束对该文件夹遍历即不遍历该文件夹内部
				TStopInLayer,//结束当前层的遍历即该层后面的文件夹或文件全部跳过
				TExit //退出整个遍历
			};

			/// <param name="flag">
			/// bit0:是否遍历文件 
			/// bit1:是否遍历文件夹。如果bit0和bit1都为0跟0x3是一样的，即默认都要遍历
			/// bit2:设置FilterAttrs的语义:
			///		1表示and操作否则为or操作.and操作:同时满足所有条件才是过滤对象;or操作:只要满足一个条件就是过滤对象
			/// bit3:是否遇到访问失败就立即返回，不再遍历文件树			
			/// bit4:设置filterNames的语义:
			///		0表示排除文件(即匹配文件不加入结果集,其他加入),1表示选择文件(即将匹配文件加入结果集,其他不加入)
			/// </param>
			IN BYTE flag;
			/// <param name="filterAttrs">
			/// 过滤的文件属性,0表示不设置,多个属性用|连接。符合过滤文件属性的文件或文件夹经都不会调用fileHandler
			/// 例如:windows:FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_HIDDEN表示过滤掉隐藏的系统文件。
			/// unix:S_IRWXU,S_IRUSR,S_IWUSR,S_IXUSR等属性的集合
			/// </param>
			IN DWORD filterAttrs;
			/// <param name="filterNames">
			/// 过滤的文件文件名称,支持通配符。多个匹配项用|隔开
			/// 例如:*.pdb|*.ncb|*.obj|*.pch|*.idb
			/// </param>
			IN String filterNames;
			/// <param name="filterMaxFileSize">
			/// 过滤的文件大小，大于该文件大小的直接过滤掉.以byte为单位.0表示不限制.
			/// 例如:1024表示最大不能超过1k的文件
			/// </param>
			IN DWORD filterMaxFileSize;
			/// <param name="Level">
			/// 遍历的最深层数,-1表示不限制,0:只遍历最顶层
			/// </param>
			IN int maxVisitLevel;
			/// 附加参数
			IN LPVOID lpParameter;
			/// <param name="FailedCount">
			/// 返回遍历过程访问失败的文件个数
			/// </param>
			OUT int failedCount;

			SearchOption()
			{
				flag=SEARCH_FILE|SEARCH_DIRECTORY;
#if IS_WINDOWS_OS
				filterAttrs=FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_TEMPORARY;
#else
                filterAttrs=0;
#endif
				filterNames=String::Empty;
				maxVisitLevel=SEARCH_ALL_DIRECTORIES;
				failedCount=0;
				lpParameter=NULL;
				filterMaxFileSize=0;
			}
		};
		typedef SearchOption::_DirOptions DSearchOption;

		///<summary>文件处理器（返回值决定了该文件树的遍历如何进行）</summary>
		///<param name="path">文件或文件夹全路径</param>
		///<param name="isDir">path为一个文件夹,false:文件</param>
		///<param name="lpParameter">附加参数</param>
		///<param name="level">当前文件所在的文件树的第几层，即遍历深度</param>
		///<param name="pFindData">文件[夹]信息结构,win系统是WIN32_FIND_DATA指针否则是stat指针</param>
		typedef DSearchOption(*FileHandler)(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData);
		//磁盘处理器,返回值true：继续往下遍历，false：结束这层的遍历（即停止当前的搜索）
		//type表示磁盘类型，例如DRIVE_FIXED,DRIVE_CDROM,DRIVE_REMOVABLE...
		//isSys:是否为系统盘
		typedef bool (*DiskHandler)(IN String& drivePath,IN UINT type,IN bool isSys,IN PVOID lpParameter);

		// 摘要:
		//     提供用于创建、复制、删除、移动和打开文件的静态方法，并协助创建 System.IO.FileStream 对象。
		class API_CORE Directory
		{
		public:
			///<summary>判断一个文件夹是否存在</summary>
			///<param name="path">待判断的文件夹的全路径</param>
			///<returns>true:文件夹已存在,false:未找到目标文件夹</returns>
			static bool Exists(IN String path);

			///<summary>创建一个文件夹,如果文件夹已经存在则直接退出(返回TRUE)</summary>
			///<param name="path">要创建文件夹的全路径名</param>
			///<returns>true:文件夹创建成功,false:文件夹创建失败</returns>
			static bool Create(IN String path);

			///<summary>逐层创建文件夹</summary>
			///<param name="path">欲创建文件夹的全路径名</param>
			///<returns>是否创建成功</returns>
			static bool CreateInLayers(IN String path);

			///<summary>从指定路径删除空目录,PS.删除非空目录返回false（或使用errno获取错误码）</summary>
			///<param name="path">要移除的空目录的名称。 此目录必须为可写或为空</param>
			///<returns>是否删除成功</returns>
			static bool Delete(IN String path);
			static bool Delete(IN String path, bool recursive);
			///<summary>删除文件夹内容</summary>
			static bool DeleteContent(IN String path);

			///<summary>获取应用程序的当前工作目录</summary>
			///<returns>包含当前工作目录的路径的字符串，并不以反斜杠 (\) 结尾</returns>
			static String GetCurrentDirectory();
#if IS_WINDOWS_OS
			///<summary>将应用程序的当前工作目录设置为指定的目录</summary>
			///<param name="path">设置为当前工作目录的路径</param>
			static void SetCurrentDirectory(IN String path);
			
			/// <summary>
			/// 设备磁盘遍历
			/// </summary>
			/// <returns>返回操作标志,成功返回:ERROR_SUCCESS.(SetLastError)</returns>
			static DWORD SearchDisk(IN DiskHandler diskHandler,IN LPVOID lpParameter=NULL);						
#endif
			/// <summary>
			/// 返回文件夹下的符合条件的所有文件或文件夹(默认返回第一层的所有子文件名称)
			/// </summary>
			/// <param name="path">目标目录</param>
			/// <param name="result">文件名称列表,PS.只是文件名称不是文件全路径</param>
			/// <param name="searchOpts">搜索选项</param>
			/// <param name="backOpts">返回值选项,该参数与searchOpts.flag的bit[0|1]不同.flag表示是否进入文件夹遍历。
			/// bit0:是否返回文件,即将文件加入结果集
			/// bit1:是否返回文件夹,即将文件夹加入结果集
			/// bit2:是返回文件名称(0)还是文件全路径(1)
			///</param>
			/// <returns>返回操作标志,成功返回:TRUE，错误返回FALSE，可以用GetLastError获取错误信息</returns>
			static bool GetFiles(IN String path,IN OUT vector<String>& result,IN SearchOption* searchOpts=NULL,IN BYTE backOpts=BACKOPT_FILE);
			/// <summary>
			/// 文件夹(连同文件夹内容)复制
			/// </summary>
			/// <param name="src">源目录</param>
			/// <param name="dest">目标目录</param>
			/// <param name="overwrite">重写类型
			/// 0:重名文件重写,1:先清空文件夹内容，其他值:如果文件夹存在则返回false
			/// </param>
			/// <param name="options">搜索选项</param>
			/// <returns>是否执行成功</returns>
			static bool Copy(IN String src,IN String dest,IN int overwrite=0,IN SearchOption* options=NULL);
            /// <summary>
			/// 文件夹遍历/搜索
			/// </summary>
			/// <param name="dir">目标目录</param>
			/// <param name="fileHandler">文件处理器</param>
			/// <param name="options">搜索选项</param>
			/// <returns>返回操作是否成功</returns>
			static bool Search(IN String dir,IN FileHandler fileHandler,IN SearchOption* options=NULL);

        private:
			enum THandlerFunction{ 
				HFFile=0x1,
				HFDirFirst=0x2,//先序遍历中访问文件夹
				HFDirLast=0x4  //后序遍历中访问文件夹
			};
			///flag:遍历标志位,bit0:启用文件访问回调,bit1:启用文件夹先序遍历回调,bit2:启用文件夹后序遍历回调
			static bool InnerSearch(IN String dir,IN FileHandler fileHandler,IN SearchOption* options,
				IN int* pCurrentVisitLevel,IN int flag=HFFile|HFDirFirst|HFDirLast);
            static DSearchOption InnerCopyFileHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData);
			static DSearchOption InnerDeleteHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData);
			static DSearchOption InnerGetHandler(IN String& path,IN bool isDir,IN PVOID lpParameter,IN int level,IN PVOID pFindData);
		};
	}	
}