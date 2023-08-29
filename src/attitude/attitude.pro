#
# $Id: attitude.pro,v 1.2 2003/05/21 13:54:43 nilspace Exp $
#
# Copyright (C) 2002-2003 by Andrew J. Turner.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation under the terms of the GNU General Public License is hereby 
# granted. No representations are made about the suitability of this software 
# for any purpose. It is provided "as is" without express or implied warranty.
# See the GNU General Public License for more details.

HEADERS  	= Attitude.h AttitudeState.h	
SOURCES	 	=	Attitude.cpp AttitudeState.cpp
TARGET	 	= 	osessame_attitude
VERSION  	= 	1.0
DESTDIR		=	../../lib/
INCLUDEPATH	=	../matrix/ ../rotation ../attitude \
			../orbit ../datahandling ../dynamics \
			../environment ../utils
CONFIG 		= 	console staticlib warn_on debug
TEMPLATE 	= 	lib
OBJECTS_DIR 	= 	../../objects
DEPENDPATH  	= 	../matrix/ ../rotation ../attitude \
			../orbit ../datahandling ../dynamics \
			../environment ../utils
