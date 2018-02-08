﻿#include "stdafx.h"
#include "StringHelper.hpp"

namespace System
{
	//功  能: 在lpszSour中查找字符串lpszFind，lpszFind中可以包含通配字符‘?’
	//参  数: nStart为在lpszSour中的起始查找位置
	//返回值: 成功返回匹配位置，否则返回-1
	//注  意: Called by “bool MatchingString()”
	int StringHelper::FindingString(LPCTSTR lpszSour,LPCTSTR lpszFind, int nStart)
	{
		if(lpszSour == NULL || lpszFind == NULL || nStart < 0) return -1;

		int m = (int)(_tcslen(lpszSour));
		int n = (int)(_tcslen(lpszFind));

		if( nStart+n > m ) return -1;
		if(n == 0) return nStart;

		//KMP算法
		int* next = new int[n];
		//得到查找字符串的next数组
		{	
			n--;
			int j, k;
			j = 0;
			k = -1;
			next[0] = -1;

			while(j < n)
			{	
				if(k == -1 || lpszFind[k] ==_T('?') || lpszFind[j] == lpszFind[k])
				{	
					j++;
					k++;
					next[j] = k;
				}
				else
					k = next[k];
			}

			n++;
		}

		int i = nStart, j = 0;
		while(i < m && j < n)
		{
			if(j == -1 || lpszFind[j] == _T('?') || lpszSour[i] == lpszFind[j])
			{	i++;
			j++;
			}
			else
				j = next[j];
		}

		delete []next;

		if(j >= n)
			return i-n;
		else
			return -1;
	}
	//功	  能: 带通配符的字符串匹配
	//参	  数: lpszSour是一个普通字符串；
	//			  lpszMatch是一可以包含通配符的字符串；
	//			  bMatchCase为0，不区分大小写，否则区分大小写。
	//返  回  值: 匹配，返回1；否则返回0。
	//通配符意义: 
	//		‘*’	代表任意字符串，包括空字符串；
	//		‘?’	代表任意一个字符，不能为空；
	bool StringHelper::Match(LPCTSTR lpszSour,LPCTSTR lpszMatch, bool bMatchCase)
	{
		//	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszMatch));
		if(lpszSour == NULL || lpszMatch == NULL)
			return false;

		if(lpszMatch[0] == 0) return lpszSour[0] == 0;//Is a empty string

		int i = 0, j = 0;
		//生成比较用临时源字符串'szSource'
		TCHAR* szSource = new TCHAR[ (j = (int)(_tcslen(lpszSour)+1)) ];

		if( bMatchCase ){//memcpy(szSource, lpszSour, j);
			while( (*(szSource+i) = *(lpszSour+i))!=0 ) i++;//while( *(szSource+i) = *(lpszSour+i++) );
		}
		else
		{	//Lowercase 'lpszSour' to 'szSource'
			i = 0;
			while(lpszSour[i])
			{	
				if(lpszSour[i] >= _T('A') && lpszSour[i] <= _T('Z'))
					szSource[i] = lpszSour[i] - _T('A') + _T('a');
				else
					szSource[i] = lpszSour[i];
				i++;
			}
			szSource[i] = 0;
		}

		//生成比较用临时匹配字符串'szMatcher'
		TCHAR* szMatcher = new TCHAR[_tcslen(lpszMatch)+1];

		//把lpszMatch里面连续的“*”并成一个“*”后复制到szMatcher中
		i = j = 0;
		while(lpszMatch[i])
		{
			szMatcher[j++] = (!bMatchCase) ?
				( 
				   (lpszMatch[i] >= _T('A') && lpszMatch[i] <= _T('Z')) ?//Lowercase lpszMatch[i] to szMatcher[j]
					lpszMatch[i] - _T('A') + _T('a') :
					lpszMatch[i]
				) :
				lpszMatch[i];//Copy lpszMatch[i] to szMatcher[j]

			if(lpszMatch[i] == _T('*'))
				while(lpszMatch[++i] == _T('*'));
			else
				i++;
		}
		szMatcher[j] = 0;

		//开始进行匹配检查

		int nMatchOffset, nSourOffset;

		bool bIsMatched = true;
		nMatchOffset = nSourOffset = 0;
		while(szMatcher[nMatchOffset])
		{
			if(szMatcher[nMatchOffset] == _T('*'))
			{
				if(szMatcher[nMatchOffset+1] == 0)
				{	//szMatcher[nMatchOffset]是最后一个字符

					bIsMatched = true;
					break;
				}
				else
				{	//szMatcher[nMatchOffset+1]只能是_T('?')或普通字符

					int nSubOffset = nMatchOffset+1;

					while(szMatcher[nSubOffset])
					{	
						if(szMatcher[nSubOffset] == _T('*')) break;
						nSubOffset++;
					}

					if( _tcslen(szSource+nSourOffset) < size_t(nSubOffset-nMatchOffset-1) )
					{	//源字符串剩下的长度小于匹配串剩下要求长度
						bIsMatched = false; //判定不匹配
						break;			//退出
					}

					if(!szMatcher[nSubOffset])//nSubOffset is point to ender of 'szMatcher'
					{	//检查剩下部分字符是否一一匹配

						nSubOffset--;
						int nTempSourOffset = (int)(_tcslen(szSource)-1);
						//从后向前进行匹配
						while(szMatcher[nSubOffset] != _T('*'))
						{
							if(szMatcher[nSubOffset] == _T('?'));
							else
							{	
								if(szMatcher[nSubOffset] != szSource[nTempSourOffset])
								{	
									bIsMatched = false;
									break;
								}
							}
							nSubOffset--;
							nTempSourOffset--;
						}
						break;
					}
					else//szMatcher[nSubOffset] == _T('*')
					{	
						nSubOffset -= nMatchOffset;
						TCHAR* szTempFinder = new TCHAR[nSubOffset];
						nSubOffset--;
						memcpy(szTempFinder, szMatcher+nMatchOffset+1, nSubOffset*sizeof(TCHAR));
						szTempFinder[nSubOffset] = 0;

						int nPos = FindingString(szSource+nSourOffset, szTempFinder, 0);
						delete []szTempFinder;

						if(nPos != -1)//在'szSource+nSourOffset'中找到szTempFinder
						{	
							nMatchOffset += nSubOffset;
							nSourOffset += (nPos+nSubOffset-1);
						}
						else
						{	
							bIsMatched = false;
							break;
						}
					}
				}
			}		//end of "if(szMatcher[nMatchOffset] == _T('*'))"
			else if(szMatcher[nMatchOffset] == _T('?'))
			{
				if(!szSource[nSourOffset])
				{	
					bIsMatched = false;
					break;
				}
				if(!szMatcher[nMatchOffset+1] && szSource[nSourOffset+1])
				{	//如果szMatcher[nMatchOffset]是最后一个字符，
					//且szSource[nSourOffset]不是最后一个字符
					bIsMatched = false;
					break;
				}
				nMatchOffset++;
				nSourOffset++;
			}
			else//szMatcher[nMatchOffset]为常规字符
			{
				if(szSource[nSourOffset] != szMatcher[nMatchOffset])
				{	
					bIsMatched = false;
					break;
				}
				if(!szMatcher[nMatchOffset+1] && szSource[nSourOffset+1])
				{	
					bIsMatched = false;
					break;
				}
				nMatchOffset++;
				nSourOffset++;
			}
		}

		delete []szSource;
		delete []szMatcher;
		return bIsMatched;
	}

	//功  能: 多重匹配，不同匹配字符串之间用‘|’隔开
	//			如: “*.h|*.cpp”将依次匹配“*.h”和“*.cpp”
	//参  数: nMatchLogic = 0, 不同匹配求或，else求与；bMatchCase, 是否大小敏感
	//返回值: 如果bRetReversed = false, 匹配返回true；否则不匹配返回true
	bool StringHelper::MultiMatch(LPCTSTR lpszSour,LPCTSTR lpszMatch, int nMatchLogic, bool bRetReversed, bool bMatchCase)
	{
		//	ASSERT(AfxIsValidString(lpszSour) && AfxIsValidString(lpszMatch));
		if(lpszSour == NULL || lpszMatch == NULL) return false;

		TCHAR* szSubMatch = new TCHAR[_tcslen(lpszMatch)+1];
		bool bIsMatch;

		if(nMatchLogic == 0)//求或
		{	
			bIsMatch = 0;
			int i = 0;
			int j = 0;
			while(1)
			{	
				if(lpszMatch[i] != 0 && lpszMatch[i] != _T('|'))
					szSubMatch[j++] = lpszMatch[i];
				else
				{	
					szSubMatch[j] = 0;
					if(j != 0)
					{
						bIsMatch = Match(lpszSour, szSubMatch, bMatchCase);
						if(bIsMatch) break;
					}
					j = 0;
				}

				if(lpszMatch[i] == 0)
					break;
				i++;
			}
		}
		else//求与
		{	
			bIsMatch = 1;
			int i = 0;
			int j = 0;
			while(1)
			{	
				if(lpszMatch[i] != 0 && lpszMatch[i] != _T('|'))
					szSubMatch[j++] = lpszMatch[i];
				else
				{	
					szSubMatch[j] = 0;
					bIsMatch = Match(lpszSour, szSubMatch, bMatchCase);
					if(!bIsMatch) break;

					j = 0;
				}

				if(lpszMatch[i] == 0)
					break;
				i++;
			}
		}

		delete []szSubMatch;

		if(bRetReversed) return !bIsMatch;
		return bIsMatch;
	}

}