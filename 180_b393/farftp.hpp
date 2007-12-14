#ifndef __FARFTP_HPP__
#define __FARFTP_HPP__
/*
farftp.hpp

����� wininet.h - ���� ����� ������, ������������ � message.cpp
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

extern "C" {

#define INTERNETAPI DECLSPEC_IMPORT
typedef LPVOID HINTERNET;

INTERNETAPI void WINAPI FtpConnect(HANDLE *hConnect,LPCSTR lpszServerName,
            LPCSTR lpszUserName,LPCSTR lpszPassword);

INTERNETAPI BOOL WINAPI FtpFindFirstFile(HINTERNET hConnect,
            LPCSTR lpszSearchFile,LPWIN32_FIND_DATA lpFindFileData,
            DWORD ResetCache,DWORD *Mode, LPSTR lpszOwner);

INTERNETAPI BOOL WINAPI FtpFindNextFile(HINTERNET hConnect,
            LPVOID lpvFindData,DWORD *Mode, LPSTR lpszOwner);

INTERNETAPI BOOL WINAPI FtpGetCurrentDirectory(HINTERNET hConnect,
            LPSTR lpszCurrentDirectory,LPDWORD lpdwCurrentDirectory);

INTERNETAPI BOOL WINAPI FtpSetCurrentDirectory(HINTERNET hConnect,
            LPCSTR lpszDirectory);

INTERNETAPI BOOL WINAPI FtpCreateDirectory(HINTERNET hConnect,
            LPCSTR lpszDirectory);

INTERNETAPI BOOL WINAPI FtpRemoveDirectory(HINTERNET hConnect,
            LPCSTR lpszDirectory);

INTERNETAPI BOOL WINAPI FtpRenameFile(HINTERNET hConnect,
            LPCSTR lpszExisting,LPCSTR lpszNew);

INTERNETAPI BOOL WINAPI FtpDeleteFile(HINTERNET hConnect,
            LPCSTR lpszFileName);

INTERNETAPI BOOL WINAPI FtpChmod(HINTERNET hConnect,
            LPCSTR lpszFileName,DWORD Mode);

INTERNETAPI BOOL WINAPI FtpDisconnect(HINTERNET hInternet);

INTERNETAPI BOOL WINAPI FtpGetFile(HINTERNET hConnect,
            LPCSTR lpszRemoteFile,LPCSTR lpszNewFile,int Reget,
            int AsciiMode,int (*GetCallback)(int Size));

INTERNETAPI BOOL WINAPI FtpPutFile(HINTERNET hConnect,
            LPCSTR lpszLocalFile,LPCSTR lpszNewRemoteFile,long Position,
            int AsciiMode,int (*PutCallback)(int Size));

INTERNETAPI BOOL WINAPI FtpSystemInfo(HINTERNET hConnect,
            char *Buffer,int MaxSize);

INTERNETAPI BOOL WINAPI FtpStartReply(HINTERNET hConnect,
            char *Buffer,int MaxSize);

INTERNETAPI void WINAPI FtpResetLastReply();

INTERNETAPI void WINAPI FtpLastReply(char *Buffer1,int MaxSize1,
                                     char *Buffer2,int MaxSize2);
};


#define INTERNET_ERROR_BASE                     12000

#define ERROR_INTERNET_OUT_OF_HANDLES           (INTERNET_ERROR_BASE + 1)
#define ERROR_INTERNET_TIMEOUT                  (INTERNET_ERROR_BASE + 2)
#define ERROR_INTERNET_EXTENDED_ERROR           (INTERNET_ERROR_BASE + 3)
#define ERROR_INTERNET_INTERNAL_ERROR           (INTERNET_ERROR_BASE + 4)
#define ERROR_INTERNET_INVALID_URL              (INTERNET_ERROR_BASE + 5)
#define ERROR_INTERNET_UNRECOGNIZED_SCHEME      (INTERNET_ERROR_BASE + 6)
#define ERROR_INTERNET_NAME_NOT_RESOLVED        (INTERNET_ERROR_BASE + 7)
#define ERROR_INTERNET_PROTOCOL_NOT_FOUND       (INTERNET_ERROR_BASE + 8)
#define ERROR_INTERNET_INVALID_OPTION           (INTERNET_ERROR_BASE + 9)
#define ERROR_INTERNET_BAD_OPTION_LENGTH        (INTERNET_ERROR_BASE + 10)
#define ERROR_INTERNET_OPTION_NOT_SETTABLE      (INTERNET_ERROR_BASE + 11)
#define ERROR_INTERNET_SHUTDOWN                 (INTERNET_ERROR_BASE + 12)
#define ERROR_INTERNET_INCORRECT_USER_NAME      (INTERNET_ERROR_BASE + 13)
#define ERROR_INTERNET_INCORRECT_PASSWORD       (INTERNET_ERROR_BASE + 14)
#define ERROR_INTERNET_LOGIN_FAILURE            (INTERNET_ERROR_BASE + 15)
#define ERROR_INTERNET_INVALID_OPERATION        (INTERNET_ERROR_BASE + 16)
#define ERROR_INTERNET_OPERATION_CANCELLED      (INTERNET_ERROR_BASE + 17)
#define ERROR_INTERNET_INCORRECT_HANDLE_TYPE    (INTERNET_ERROR_BASE + 18)
#define ERROR_INTERNET_INCORRECT_HANDLE_STATE   (INTERNET_ERROR_BASE + 19)
#define ERROR_INTERNET_NOT_PROXY_REQUEST        (INTERNET_ERROR_BASE + 20)
#define ERROR_INTERNET_REGISTRY_VALUE_NOT_FOUND (INTERNET_ERROR_BASE + 21)
#define ERROR_INTERNET_BAD_REGISTRY_PARAMETER   (INTERNET_ERROR_BASE + 22)
#define ERROR_INTERNET_NO_DIRECT_ACCESS         (INTERNET_ERROR_BASE + 23)
#define ERROR_INTERNET_NO_CONTEXT               (INTERNET_ERROR_BASE + 24)
#define ERROR_INTERNET_NO_CALLBACK              (INTERNET_ERROR_BASE + 25)
#define ERROR_INTERNET_REQUEST_PENDING          (INTERNET_ERROR_BASE + 26)
#define ERROR_INTERNET_INCORRECT_FORMAT         (INTERNET_ERROR_BASE + 27)
#define ERROR_INTERNET_ITEM_NOT_FOUND           (INTERNET_ERROR_BASE + 28)
#define ERROR_INTERNET_CANNOT_CONNECT           (INTERNET_ERROR_BASE + 29)
#define ERROR_INTERNET_CONNECTION_ABORTED       (INTERNET_ERROR_BASE + 30)
#define ERROR_INTERNET_CONNECTION_RESET         (INTERNET_ERROR_BASE + 31)
#define ERROR_INTERNET_FORCE_RETRY              (INTERNET_ERROR_BASE + 32)
#define ERROR_INTERNET_INVALID_PROXY_REQUEST    (INTERNET_ERROR_BASE + 33)
#define ERROR_INTERNET_NEED_UI                  (INTERNET_ERROR_BASE + 34)

#define ERROR_INTERNET_HANDLE_EXISTS            (INTERNET_ERROR_BASE + 36)
#define ERROR_INTERNET_SEC_CERT_DATE_INVALID    (INTERNET_ERROR_BASE + 37)
#define ERROR_INTERNET_SEC_CERT_CN_INVALID      (INTERNET_ERROR_BASE + 38)
#define ERROR_INTERNET_HTTP_TO_HTTPS_ON_REDIR   (INTERNET_ERROR_BASE + 39)
#define ERROR_INTERNET_HTTPS_TO_HTTP_ON_REDIR   (INTERNET_ERROR_BASE + 40)
#define ERROR_INTERNET_MIXED_SECURITY           (INTERNET_ERROR_BASE + 41)
#define ERROR_INTERNET_CHG_POST_IS_NON_SECURE   (INTERNET_ERROR_BASE + 42)
#define ERROR_INTERNET_POST_IS_NON_SECURE       (INTERNET_ERROR_BASE + 43)
#define ERROR_INTERNET_CLIENT_AUTH_CERT_NEEDED  (INTERNET_ERROR_BASE + 44)
#define ERROR_INTERNET_INVALID_CA               (INTERNET_ERROR_BASE + 45)
#define ERROR_INTERNET_CLIENT_AUTH_NOT_SETUP    (INTERNET_ERROR_BASE + 46)
#define ERROR_INTERNET_ASYNC_THREAD_FAILED      (INTERNET_ERROR_BASE + 47)
#define ERROR_INTERNET_REDIRECT_SCHEME_CHANGE   (INTERNET_ERROR_BASE + 48)

//
// FTP API errors
//

#define ERROR_FTP_TRANSFER_IN_PROGRESS          (INTERNET_ERROR_BASE + 110)
#define ERROR_FTP_DROPPED                       (INTERNET_ERROR_BASE + 111)

#endif  // __FARFTP_HPP__
