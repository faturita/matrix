#
# $Id: rotation.pro,v 1.1 2003/05/20 17:53:19 nilspace Exp $
#
# Copyright (C) 2002-2003 by Andrew J. Turner.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation under the terms of the GNU General Public License is hereby 
# granted. No representations are made about the suitability of this software 
# for any purpose. It is provided "as is" without express or implied warranty.
# See the GNU General Public License for more details.

HEADERS  	= 	 
SOURCES	 	=	*.c 

OBJECTS		=	
TARGET	 	= 	matrix
VERSION  	= 	1.0

INCLUDEPATH 	= 	camblas camlapack
CONFIG 		= 	console staticlib warn_on debug
TEMPLATE 	= 	lib
OBJECTS_DIR 	= 	./
DEPENDPATH  	= 	