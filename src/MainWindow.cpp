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

#include "MainWindow.h"
#include "SlackPKG.h"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QCloseEvent>
#include <iostream>
#include <stdlib.h>

static const std::string sVersion= SWUN_VERSION ;


MainWindow::MainWindow()
{
 m_packBE=new SlackPKG();
 m_state=PackageBackend::ePBS_Unknown;

 m_checkCLAction = new QAction(tr("&Check ChangeLog"), this);
 m_checkSAction  = new QAction(tr("CheckUpdate&State"), this);
 m_updateAction  = new QAction(tr("&Update System"), this);
 m_restoreAction = new QAction(tr("&Restore"), this);
 m_quitAction    = new QAction(tr("&Quit"), this);
 m_helpAction    = new QAction(tr("&Help"), this);
 connect( m_restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
 connect( m_quitAction,    SIGNAL(triggered()), qApp, SLOT(quit()));
 connect( m_checkCLAction, SIGNAL(triggered()), this, SLOT(triggeredCheckChangelog()));
 connect( m_checkSAction,  SIGNAL(triggered()), this, SLOT(checkState()));
 connect( m_updateAction,  SIGNAL(triggered()), this, SLOT(doUpdate()));
 connect( m_helpAction,    SIGNAL(triggered()), this, SLOT(showHelp()) );

 createTrayIcon();

 connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
	 this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
 m_timer = new QTimer(this);
 m_timer->start(5000);
 connect( m_timer, SIGNAL(timeout()), this, SLOT(autoCheckChangelog()));

 m_trayIcon->show();
 setWindowTitle(tr("Swun"));
 resize(400, 400);
}


MainWindow::~MainWindow()
{
 delete m_packBE;
}


void 
MainWindow::closeEvent(QCloseEvent *event)
{
 if(m_trayIcon->isVisible()) 
    {
     hide();
     event->ignore();
    }
}


void 
MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
 const std::string& output=m_packBE->getOutput();
 switch(reason) 
    {
     case QSystemTrayIcon::Trigger:
          m_trayIcon->showMessage("Swun",output.c_str(),
                                 QSystemTrayIcon::Information,1000);
         break;
     case QSystemTrayIcon::DoubleClick:
          m_trayIcon->showMessage("Swun",sVersion.c_str(),
                                  QSystemTrayIcon::Information,1000);
          break;
     case QSystemTrayIcon::MiddleClick:
          m_trayIcon->showMessage("Swun",sVersion.c_str(),
                                  QSystemTrayIcon::Information,1000);
          break;
     default:
          break;
    }
}


void
MainWindow::autoCheckChangelog()
{
 static bool firstCall=true;
 std::cout << "autoCheckChangelog" << std::endl;
 if(checkChangelog())
    {
     std::string output=m_packBE->getOutput();
     if(firstCall && m_state==PackageBackend::ePBS_UpToDate)
        {//do nothing in this case
        }
     else
        {
         m_trayIcon->showMessage("Swun",output.c_str());
        }
    }
 if(firstCall)
    {
     m_timer->start(3600000);
     firstCall=false;
    }
}

void 
MainWindow::triggeredCheckChangelog()
{
 std::cout << "triggeredCheckChangelog" << std::endl;
 checkChangelog();
 std::string output=m_packBE->getOutput();
 m_trayIcon->showMessage("Swun",output.c_str());
}


void
MainWindow::checkState()
{
 std::cout << "check update State" << std::endl;
 m_state=m_packBE->checkState();
 std::string output=m_packBE->getOutput();
 m_trayIcon->showMessage("Swun",output.c_str());
 updateIcon();
}


void
MainWindow::doUpdate()
{
 std::cout << "do an update" << std::endl;
 m_packBE->doUpdate();
 m_state=PackageBackend::ePBS_Unknown;
 autoCheckChangelog();
}


void
MainWindow::showHelp()
{
 std::cout << "show help" << std::endl;
 static const std::string TERMINAL="xterm -bg black -fg grey";
 std::string cmd=TERMINAL + " -e 'man swun'";
 system(cmd.c_str());
}


bool 
MainWindow::checkChangelog()
{
 bool outdated=m_packBE->check();
 bool stateChange=false;
 if(outdated)
    {
     stateChange=true;
     m_state=PackageBackend::ePBS_ChangelogOutdated;
    }
 else
    {
     if(m_state==PackageBackend::ePBS_Unknown)
	{
	 stateChange=true;
	 m_state=PackageBackend::ePBS_UpToDate;
	}
    }
 updateIcon();
 return stateChange;
}



void 
MainWindow::createTrayIcon()
{
 m_trayIconMenu = new QMenu(this);
 m_trayIconMenu->addAction(m_checkCLAction);
 m_trayIconMenu->addAction(m_checkSAction);
 m_trayIconMenu->addAction(m_updateAction);
 //m_trayIconMenu->addAction(m_restoreAction);
 m_trayIconMenu->addAction(m_helpAction);
 m_trayIconMenu->addAction(m_quitAction);

 m_trayIcon = new QSystemTrayIcon(this);
 m_trayIcon->setContextMenu(m_trayIconMenu);
 updateIcon();
}


void
MainWindow::updateIcon()
{
 switch(m_state)
    {
     case PackageBackend::ePBS_UpToDate:
	 m_trayIcon->setIcon(QIcon(":/images/uptodate.png"));
	 break;
     case PackageBackend::ePBS_ChangelogOutdated:
	 m_trayIcon->setIcon(QIcon(":/images/newupdates.png"));
	 break;
     case PackageBackend::ePBS_PackagesInstallable:
	 m_trayIcon->setIcon(QIcon(":/images/packages.png"));
	 break;
     default:
	 m_trayIcon->setIcon(QIcon(":/images/unknown.png"));
	 break;
    }
}
