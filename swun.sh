#!/bin/sh
###############################################################################
#
# Copyright (c) 2012, Benno Grolik <fwxx@rocketmail.com>
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
# - Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
# - Redistributions in binary form must reproduce the above copyright notice, 
#     this list of conditions and the following disclaimer in the documentation 
#     and/or other materials provided with the distribution.
# - Neither the name of Benno Grolik nor the names of its contributors may be 
#     used to endorse or promote products derived from this software without 
#     specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGE.
#
###############################################################################

# @PARAM USE SUDO:
# set it to "yes" and put a line like the following into /etc/sudoers to 
# enable sudo package management without passwords questions
# $(user)  ALL=(root) NOPASSWD: /usr/sbin/slackpkg
# 
USE_SUDO="no"
###############################################################################
PKG_TOOL="/usr/sbin/slackpkg"
TERMINAL="/usr/bin/xterm -bg black -fg white"
LOG="/tmp/swun.log"
###############################################################################
SUDO=""
if [ "$USE_SUDO" == "yes" ]; then
    SUDO="/usr/bin/sudo"
fi
#echo "sudo=$SUDO"




function doCheck()
{
    #avoid control character with -spinning=off
    OUTPUT=$(/usr/sbin/slackpkg -spinning=off check-updates | grep -v "^$")
    echo "$OUTPUT"
    if [ "$OUTPUT" == "News on ChangeLog.txt" ]; then
	RETURN=1;
    else
	RETURN=0;
    fi
    return $RETURN;
}


function checkPackages()
{
#    OUTPUT=$($PKG_TOOL -dialog=off -batch=on -default_answer=n upgrade-all | grep -v "^$" | grep -v "DONE$" | grep -v "^Total package" | grep -v "^Do you wish to upgrade")
    OUTPUT=$($SUDO $PKG_TOOL -dialog=off -batch=on -default_answer=n upgrade-all | grep '\.t.z')
    for line in $OUTPUT 
    do
	echo $line
    done
    if [ -z "$OUTPUT" ]; then
	RESULT=0
    else
	RESULT=1
    fi
    return $RESULT;
}

function autoUpdate()
{
    $SUDO $PKG_TOOL -batch=on -default_answer=y update
    return $?
}


function autoUpgrade()
{
    $SUDO $PKG_TOOL -dialog=off -batch=on -default_answer=y upgrade-all
    return $?
}


function interactiveUpgrade()
{
    $SUDO $PKG_TOOL -batch=on -default_answer=y update      2>  "$LOG"
    $SUDO $PKG_TOOL upgrade-all 2>> "$LOG"
    RESULT=$?
    exit $RESULT;
}




###############################################################################
#main
case "$1" in
    "check")
	doCheck
	;;
    "checkPackages")
	if [ "$USE_SUDO" == "yes" ]; then
	    checkPackages
	else
	kdesu --noignorebutton -t -c "$0 root checkPackages" 2>/dev/null
        fi
	;;
    "autoUpdate")
	if [ "$USE_SUDO" == "yes" ]; then
	    autoUpdate
	else
	kdesu --noignorebutton -t -c "$0 root autoUpdate"    2>/dev/null
        fi
	;;
    "autoUpgrade")
	if [ "$USE_SUDO" == "yes" ]; then
	    autoUpgrade
	else
	kdesu --noignorebutton -t -c "$0 autoUpgrade" 2>/dev/null
        fi
	;;
    "upgrade")
	rm -f "$LOG"
	touch "$LOG"
	if [ "$USE_SUDO" == "yes" ]; then
	    $TERMINAL -e "$0 root interactiveUpgrade; sleep 3"
	else
	kdesu --noignorebutton -c "$TERMINAL -e \"$0 root interactiveUpgrade; sleep 3\"" 2>/dev/null
        fi
	RESULT=$?
	cat "$LOG"
	rm -f "$LOG"
	exit $RESULT
	;;
    "root")
	case "$2" in
	    "checkPackages")
		checkPackages
		;;
	    "autoUpdate")
		autoUpdate
		;;
	    "autoUpgrade")
		autoUpgrade
		;;
	    "interactiveUpgrade")
		interactiveUpgrade
		;;
	esac
	;;
    *)
	echo "usage: $0 [check|checkPackages|autoUpdate|upgrade|autoUpgrade]"
	;;
esac
