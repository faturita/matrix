#
# $Id: math.pro,v 1.3 2003/06/06 00:34:47 nilspace Exp $
#
# Copyright (C) 2002-2003 by Andrew J. Turner.
#
# Permission to use, copy, modify, and distribute this software and its
# documentation under the terms of the GNU General Public License is hereby 
# granted. No representations are made about the suitability of this software 
# for any purpose. It is provided "as is" without express or implied warranty.
# See the GNU General Public License for more details.

HEADERS  	= 	Integrator.h RungeKuttaIntegrator.h \
			RungeKuttaFehlbergIntegrator.h\
			Interpolator.h LinearInterpolator.h \
			Functor.h
SOURCES	 	=	RungeKuttaIntegrator.cpp RungeKuttaFehlbergIntegrator.cpp \
			LinearInterpolator.cpp 
TARGET	 	= 	osessame_math
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
