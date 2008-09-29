#include "stdafx.h"
#pragma hdrstop

#include "ftp_Int.h"
#include <boost/algorithm/string/trim.hpp>
#include "utils/winwrapper.h"

/* ANY state */
bool FTP::ExecCmdLineANY(const std::wstring& str, bool Prefix)
{  
	bool      iscmd = ShowHosts || Prefix;
	FTPUrlPtr ui = FTPUrlPtr(new FTPUrl);

	std::wstring::const_iterator itr = str.begin();
	Parser::skipNSpaces(itr, str.end());
	if(itr != str.end())
		++itr;
	std::wstring cmd(str.begin(), itr);
	Parser::skipSpaces(itr, str.end());

	if(boost::iequals(cmd, "help") || cmd == L"?")
	{
		FARWrappers::getInfo().ShowHelp(FARWrappers::getModuleName(), L"FTPCommandLineHelp", FHELP_SELFHELP);
		return true;
	}

	if(iscmd == false)
		return false;

	if(boost::iequals(cmd, L"qadd"))
	{
		UrlInit(ui);
		if(itr != str.end())
		{
			ui->SrcPath.assign(itr, str.end());
			if(EditUrlItem(ui))
				AddToQueque(ui);
		}
		return true;
	}

	if(boost::iequals(cmd, L"xadd"))
	{
		UrlInit(ui);
		if(itr != str.end())
		{
			ui->SrcPath.assign(itr, str.end());
			if(EditUrlItem(ui))
				AddToQueque(ui);
		}
		QueueExecOptions op;
		SetupQOpt(&op);
		if(!urlsQueue_.empty() && WarnExecuteQueue(&op))
		{
			ExecuteQueue(&op);
			if(!urlsQueue_.empty())
				QuequeMenu();
		}

		return true;
	}

	if(boost::iequals(cmd, L"q"))
	{
		QuequeMenu();
		return true;
	}

	if(boost::iequals(cmd, L"qx"))
	{
		QueueExecOptions op;
		SetupQOpt( &op );
		if(!urlsQueue_.empty() && WarnExecuteQueue(&op))
		{
			ExecuteQueue(&op);
			if(!urlsQueue_.empty())
				QuequeMenu();
		}
		return true;
	}
	return false;
}

/* HOSTS state */
bool FTP::ExecCmdLineHOST(const std::wstring &str, bool prefix)
{
	if(boost::iequals(str, L"EXIT") || boost::iequals(str, L"QUIT"))
	{
		FARWrappers::getInfo().Control(this, FCTL_CLOSEPLUGIN, NULL);
		return true;
	}

	//Connect to ftp
	if(boost::starts_with(str, L"//"))
	{
		FtpHostPtr h = FtpHostPtr(new FTPHost);
		h->Init();
		h->SetHostName(std::wstring(L"ftp:") + str, L"", L"");
		FullConnect(h);
		return true;
	}
	
	//Connect to http
	if(boost::istarts_with(str, L"HTTP://"))
	{
		FtpHostPtr h = FtpHostPtr(new FTPHost);
		h->Init();
		h->SetHostName(str, L"", L"");
		FullConnect(h);
		return true;
	}

	if(boost::istarts_with(str, L"CD "))
	{
		std::wstring::const_iterator itr = str.begin()+3;
		Parser::skipSpaces(itr, str.end());
		return FtpFilePanel_.SetDirectory(std::wstring(itr, str.end()), 0);
	}
	return false;
}

#define FCMD_SINGLE_COMMAND 0
#define FCMD_FULL_COMMAND   1

const int FCMD_SHOW_MSG = 0x0001;
const int FCMD_SHOW_EMSG = 0x0002;

bool FTP::DoCommand(const std::wstring& str, int type, DWORD flags)
{
	FARWrappers::Screen scr;

	bool	ext = getConnection().getHost()->ExtCmdView,
			dex = g_manager.opt.DoNotExpandErrors;
	std::wstring m;

	getConnection().getHost()->ExtCmdView = TRUE;
	g_manager.opt.DoNotExpandErrors     = FALSE;

	m = str;
	Connection::Result res;

	//Process command types
	switch( type ) 
	{
	case FCMD_SINGLE_COMMAND: 
		res = getConnection().command(m);
		break;

	case FCMD_FULL_COMMAND: //Convert string quoting to '\x1'
		{
			std::wstring::iterator itr = m.begin();
			while(itr != m.end())
			{
				if(*itr == L'\\')
				{
					if(++itr == m.end())
						break;
				}
				else
				{
					if(*itr == L'\"')
						*itr = quoteMark;
				}
				++itr;
			}
			res = getConnection().ProcessCommand(m);
		}
		break;
	}

	if ( (res == Connection::Done && is_flag(flags, FCMD_SHOW_MSG)) ||
		(res != Connection::Done && is_flag(flags,FCMD_SHOW_EMSG)) )
		getConnection().ConnectMessage(MOk, L"", res != Connection::Done, MOk);

	//Special process of known commands
	if(type == FCMD_SINGLE_COMMAND)
		if(boost::istarts_with(str, L"CWD "))
		{
			FtpFilePanel_.resetFileCache();
		}

	getConnection().getHost()->ExtCmdView = ext;
	g_manager.opt.DoNotExpandErrors     = dex;
	return res == Connection::Done;
}

/* FTP state */
bool FTP::ExecCmdLineFTP(const std::wstring& str, bool Prefix)
{
	std::wstring s = str;
	if(Prefix && !s.empty() && s[0] == L'/')
	{
		FtpHostPtr host = FtpHostPtr(new FTPHost);
		if(boost::istarts_with(s, L"FTP:") == false)
			s = L"ftp:" + s;
		if(host->SetHostName(s, L"", L""))
		{
			if(getConnection().getHost()->url_.compare(host->url_))
			{
				FtpFilePanel_.SetDirectory(host->url_.directory_, 0);
				return true;
			}
			FullConnect(host);
			return true;
		}
	}

	std::wstring::const_iterator itr = s.begin();
	Parser::skipNSpaces(itr, s.end());
	std::wstring cmd(s.begin(), itr);
	Parser::skipSpaces(itr, s.end());
	s.assign(itr, s.end());

	//Switch to hosts
	if(boost::iequals(cmd, L"TOHOSTS"))
	{
		getConnection().disconnect();
		return true;
	}

	//CMD
	if(boost::iequals(cmd, L"CMD"))
	{
		if(!s.empty())
			DoCommand(s, FCMD_FULL_COMMAND, FCMD_SHOW_MSG|FCMD_SHOW_EMSG );
		return true;
	}

	BOOST_LOG(INF, L"ProcessCmd" << getConnection().getHost()->ProcessCmd);
	if (!getConnection().getHost()->ProcessCmd)
		return false;

	//CD
	if(boost::iequals(cmd, L"CD"))
	{
		if(!s.empty())
		{
			FtpFilePanel_.SetDirectory(s, 0);
			return true;
		} 
		else
			return false;
	}

	//Manual command line to server
	if(!Prefix)
	{
		DoCommand(s, FCMD_SINGLE_COMMAND, FCMD_SHOW_MSG|FCMD_SHOW_EMSG);
		return true;
	}

	return false;
}

bool FTP::ExecCmdLine(const std::wstring& str, bool wasPrefix)
{
	BOOL      isConn = getConnection().isConnected();
	FARWrappers::Screen scr;

	if(str.empty())
		return false;

	std::wstring::const_iterator itr = str.begin();
	bool prefix = Parser::checkString(itr, str.end(), L"FTP:", false) || wasPrefix;

	do{
		//Any state
		if(ExecCmdLineANY(std::wstring(itr, str.end()), prefix))
			break;

		//HOSTS state
		if(ShowHosts && ExecCmdLineHOST(str, prefix))
			break;

		//CONNECTED state
		if(!ShowHosts && ExecCmdLineFTP(str, prefix))
			break;

		//Unprocessed
		if(prefix)
		{
			FARWrappers::getInfo().Control(this, FCTL_SETCMDLINE, (void*)L"");
			FARWrappers::getInfo().ShowHelp(FARWrappers::getModuleName(), L"FTPCommandLineHelp", FHELP_SELFHELP);
			return true;
		} else
			return false;
	}while(0);

	//processed
	FARWrappers::getInfo().Control(this, FCTL_SETCMDLINE, (void*)L"");

	if(isConn && (!getConnection().isConnected()))
		BackToHosts();

	Invalidate();

	return true;
}

int FTP::ProcessCommandLine(wchar_t *CommandLine)
{
	if(!CommandLine)
		return false;

	FTPUrl_ url;
	std::wstring cmdLine = boost::trim_copy(std::wstring(CommandLine));
	if(cmdLine.empty())
		return false;

	//Hosts
	if(cmdLine[0] != L'/')
		return ExecCmdLine(cmdLine, true);

	if(url.parse(cmdLine))
		return false;

	//Connect
	FTPUrlPtr ui = FTPUrlPtr(new FTPUrl);
	std::wstring::iterator UrlName = 
		(*cmdLine.rbegin() != '/' && cmdLine.find(L'/') != std::wstring::npos)? 
			cmdLine.begin() + cmdLine.rfind(cmdLine, L'/') : cmdLine.end();

	FtpHostPtr h = FtpHostPtr(new FTPHost);
	h->Init();
	UrlInit(ui);

	if(UrlName != cmdLine.end())
		ui->SrcPath = L"ftp://" + cmdLine;

	//Get URL
	BOOST_ASSERT(0 && "TODO");
	if(!ui->SrcPath.empty())
	{
		if (!EditUrlItem(ui))
		{
			static const wchar_t* itms[] = 
			{ 
				getMsg(MRejectTitle),
				getMsg(MQItemCancelled),
				getMsg(MYes),
				getMsg(MNo)
			};
			if(FARWrappers::message(itms, 2, FMSG_WARNING) != 0)
				return false;
			*UrlName = 0;
			UrlName++;
		} else {
			//User confirm downloading: execute queque
			AddToQueque(ui);

			QueueExecOptions  op;
			SetupQOpt(&op);
			if (!urlsQueue_.empty() && WarnExecuteQueue(&op) )
			{
				ExecuteQueue(&op);
				return TRUE;
			}

			ClearQueue();

			return FALSE;
		}
	}

	//Connect to host
	ClearQueue();

	//Fill`n`Connect
	h->SetHostName(CommandLine, L"", L"");

	if(!FullConnect(h))
		return false;

	FARWrappers::Screen::fullRestore();

	if(UrlName != cmdLine.end() && !ShowHosts)
		FtpFilePanel_.setSelectedFile(std::wstring(UrlName, cmdLine.end()));
	return true;
}
