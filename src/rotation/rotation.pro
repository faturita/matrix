#
# $Id: rotation.pro,v 1.6 2003/05/27 14:39:12 nilspace Exp $
#
# Copyright (C) 2002-2003 by Andrew J. Turner.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation under the terms of the GNU General Public License is hereby 
# granted. No representations are made about the suitability of this software 
# for any purpose. It is provided "as is" without express or implied warranty.
# See the GNU General Public License for more details.

HEADERS  	= 	Rotation.h #Matrix.h
SOURCES	 	=	Rotation.cpp
TARGET	 	= 	osessame_rotation
VERSION  	= 	1.0

INCLUDEPATH 	= 	../matrix ../utils
DESTDIR		=	../../lib/
CONFIG 		= 	staticlib warn_on debug
TEMPLATE 	= 	lib
OBJECTS_DIR 	= 	../../objects
DEPENDPATH  	= 	../matrix/ ../attitude ../orbit \
			../datahandling ../dynamics \
			../environment ../utils
