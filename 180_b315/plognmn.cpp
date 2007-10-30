/*
plognmn.cpp

class PreserveLongName
*/
/*
Copyright (c) 1996 Eugene Roshal
Copyright (c) 2000 Far Group
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
3. The name of the authors may not be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "headers.hpp"
#pragma hdrstop

#include "plognmn.hpp"
#include "fn.hpp"

PreserveLongName::PreserveLongName(const wchar_t *ShortName,int Preserve)
{
	PreserveLongName::Preserve=Preserve;
	if (Preserve)
	{
		FAR_FIND_DATA_EX FindData;

		if ( apiGetFindDataEx(ShortName, &FindData) )
			strSaveLongName = FindData.strFileName;
		else
			strSaveLongName = L"";

		strSaveShortName = ShortName;
	}
}


PreserveLongName::~PreserveLongName()
{
	if (Preserve && GetFileAttributesW(strSaveShortName)!=0xFFFFFFFF)
	{
		FAR_FIND_DATA_EX FindData;

		if ( !apiGetFindDataEx (strSaveShortName, &FindData) || StrCmp(strSaveLongName,FindData.strFileName)!=0)
		{
			string strNewName;

			strNewName = strSaveShortName;

			wchar_t *lpwszNewName = strNewName.GetBuffer ();

			lpwszNewName = wcsrchr (lpwszNewName, '\\'); //BUGBUG

			if ( lpwszNewName )
				*lpwszNewName = 0;

			strNewName.ReleaseBuffer ();

			strNewName += "\\";
			strNewName += strSaveLongName;

			MoveFileW (strSaveShortName, strNewName);
		}
	}
}
