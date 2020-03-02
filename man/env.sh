#!/usr/bin/env bash

open https://en.wikipedia.org/wiki/Environment_variable

#    Unix
#
#    See also: Path (computing)
$PATH
#    Contains a colon-separated list of directories that the shell searches for commands that do not contain a slash in their name (commands with slashes are interpreted as file names to execute, and the shell attempts to execute the files directly). It is equivalent to the DOS, OS/2 and Windows %PATH% variable.
$HOME
#    Contains the location of the user's home directory. Although the current user's home directory can also be found out through the C-functions getpwuid and getuid, $HOME is often used for convenience in various shell scripts (and other contexts). Using the environment variable also gives the user the possibility to point to another directory.
$PWD
#    This variable points to the current directory. Equivalent to the output of the command pwd when called without arguments.
$DISPLAY
#    Contains the identifier for the display that X11 programs should use by default.
$LD_LIBRARY_PATH
#    On many Unix systems with a dynamic linker, contains a colon-separated list of directories that the dynamic linker should search for shared objects when building a process image after exec, before searching in any other directories.
$LIBPATH or $SHLIB_PATH
#    Alternatives to $LD_LIBRARY_PATH typically used on older Unix versions.
$LANG, $LC_ALL, $LC_...
#    LANG is used to set to the default locale. For example, if the locale values are pt_BR, then the language is set to (Brazilian) Portuguese and Brazilian practice is used where relevant. Different aspects of localization are controlled by individual $LC_-variables ($LC_CTYPE, $LC_COLLATE, $LC_DATE etc.). $LC_ALL can be used to force the same locale for all aspects.
$TZ
#    Refers to time zone. It can be in several formats, either specifying the timezone itself or referencing a file (in /usr/share/zoneinfo).

