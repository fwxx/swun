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

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QSystemTrayIcon>
#include <QDialog>
#include "PackageBackend.h"


class MainWindow : public QDialog
{
Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void autoCheckChangelog();
    void triggeredCheckChangelog();
    void checkState();
    void doUpdate();
    void showHelp();

private:
    /// returns if state changes and we should show a message
    bool checkChangelog(); 
    void createTrayIcon();
    void updateIcon();

private:
    QAction* m_restoreAction;
    QAction* m_helpAction;
    QAction* m_quitAction;
    QAction* m_checkCLAction;
    QAction* m_checkSAction;
    QAction* m_updateAction;

    QSystemTrayIcon* m_trayIcon;
    QMenu*           m_trayIconMenu;

    QTimer*  m_timer;

    PackageBackend* m_packBE;
    PackageBackend::ePBState m_state;
};

#endif //MAIN_WINDOW_H
