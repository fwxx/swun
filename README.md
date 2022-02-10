SWUN (simple SoftWare Update Notifier) 
======================================
Swun is a very simple update notifier intended for the use with Slackware Linux.
It places itself as an an icon into the notification area of your desktop 
environment.



BUILDING and INSTALLING
-----------------------

### Quick
There is an slackware build script located in the subdirectory slack.
You can simply do the following to create a slackware package(you must be root):

    $ cd slack
    $ ./swun.Slackbuild.direct

Now there should be an installable slackware package in your tmp directory, e.g.
 ``tmp/swun-0.0.2-x86_64-1_fwxx.tgz``
Install this package with

    # upgradepkg --install-new /tmp/swun-0.0.2-x86_64-1_fwxx.tgz

That's it.

If you want more detail read on:

### Details
Swun is a cmake project, so building and installing is done this way:

* run cmake 
* run make
* run make install

You might want to create a separate build directory.
This is how I'm working:

    $ mkdir build
    $ cd build
    $ cmake -DCMAKE_BUILD_TYPE=Release ..
    $ make
    $ make install




GETTING STARTED
---------------
Now that you have installed swun, do a setup for the tool:

* First of all you need a working slackpkg setup(-> slackpkg manpage for this.)
* You will find Swun in KDE menu under Applications -> System,
  probably also on other desktops. 
* After Start, you will recognize a new icon in the notification area 
  of your desktop environment. 
* After a few seconds it should turn green or red depending on the 
  actually installed Software on your system.
* In KDE it will be automatically restarted in a later login.
* swun is documented on its man page(man swun)

Have Fun!



ISSUES: Nobody is perfect
-------------------------
Unfortunately I can not test every possible constellation/configuration.
If you found some issue or bug or miss some feature:
Please create an issue on https://github.com/fwxx/swun/issues
(unfortunately you need a github account there)
or simply send an email to fwxx (at) rocketmail.com,
and I will see what I can do for you!
Please Contribute to improve this piece of software!
I'd also appreciate, if you just tell me that you like this software ;-)




Documentation
-------------
Swun is (quite) well documented in its man page, have a look at it(man swun).

### How Swun works
It does not more than check changes in the changelog ---
This is essentially what "slackpkg check-updates" does.
and changes the icon color accordingly:
- red == update available,
- green == system is up to date(no changes to the changelog)

### The menu
Right Click-> There are only 4 Menu Actions:
- Check ChangeLog
 (this is the action, that is automatically triggered from time to time, 
  but you are able to trigger it here)
- CheckUpdateState
  this will do a "Check Changelog" and afterwards check, if all packages
  listed in the cache are really installed(slackpkg upgrade, but no install). 
  if not, it will display these items(root rights required).
- Update System
  This will open a terminal with root rights to give you the chance 
  to run an interactive update(slackpkg upgrade). 
- Quit is self explaining ;-)



### Alternative Setup
You can also add slackpkg tool as a sudo command:
enable sudo package management without passwords questions like in this line:

    $(user)  ALL=(root) NOPASSWD: /usr/sbin/slackpkg

And set the "USE\_SUDO" variable in swun.sh to "yes".
