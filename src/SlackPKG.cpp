/******************************************************************************

Copyright (c) 2012, Benno Grolik <fwxx@rocketmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice, 
    this list of conditions and the following disclaimer.
- Redistributions in binary form must reproduce the above copyright notice, 
    this list of conditions and the following disclaimer in the documentation 
    and/or other materials provided with the distribution.
- Neither the name of Benno Grolik nor the names of its contributors may be 
    used to endorse or promote products derived from this software without 
    specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/

#include "SlackPKG.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

std::pair<int,std::string> execute(const std::string& cmd)
{
 std::pair<int,std::string> result;
 result.first=-1;
 result.second="";
 FILE* pipe=popen(cmd.c_str(),"r");
 if(pipe)
    {
     char buffer[128];
     while(!feof(pipe))
	{
	 memset(buffer,0,sizeof(buffer));
	 if(fgets(buffer, 128, pipe) != NULL)
	    {
	     result.second += buffer;
	    }
	}

     int status=pclose(pipe);
     result.first=WEXITSTATUS(status);
    }
 return result;
}

std::string trim(const std::string& input)
{
 std::string in=input;
 /*
 //replace control chars with whitespace(experimental)
 for(int i=0;i<in.length();++i)
    {
     if( in[i] == '' )
	{ 
	 in[i]=' '; 
	 if(i<in.length()-6) 
	    {
	     for(int j=0;j<6;++j)
	     in[i+j]=' ';
	    }
	}
    }
    */
 //trim whitespace at begin and end
 std::size_t begin=in.find_first_not_of(" \t\n");
 std::size_t end=in.find_last_not_of(" \t\n");
 if(begin==std::string::npos || end==std::string::npos)
    {
     return "";
    }
 else
    {
     return in.substr(begin,end-begin+1);
    }
}




bool
SlackPKG::check()
{
 std::string cmdLine="swun.sh check";
 std::pair<int,std::string> ret=execute(cmdLine);
 m_lastCmdOutput=ret.second;
 bool updatesAvailable=(ret.first!=0);
 return updatesAvailable;
}

PackageBackend::ePBState
SlackPKG::checkState()
{
 if(check())
    {//updates are available
     return PackageBackend::ePBS_ChangelogOutdated;
    }
 else
    {
     std::string cmdLine="swun.sh checkPackages";
     std::pair<int,std::string> ret=execute(cmdLine);
     if(ret.first==0)
	{
	 return PackageBackend::ePBS_UpToDate;
	}
     else
	{
	 m_lastCmdOutput="Installable Packages:\n" + ret.second;
	 return PackageBackend::ePBS_PackagesInstallable;
	}
    }
}


bool 
SlackPKG::doUpdate()
{
 std::string cmdLine="swun.sh upgrade";
 int status=system(cmdLine.c_str());
 int retcode=WEXITSTATUS(status);
 m_lastCmdOutput="doUpdate";
 std::cout << "doUpdate: ret=" << retcode  << std::endl;
 return true;
}


std::string
SlackPKG::getOutput() const
{
 return m_lastCmdOutput;
}
