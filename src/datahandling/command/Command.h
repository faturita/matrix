//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file Command.h
*  \brief Interface to the Command class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*!  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_COMMAND_H__
#define __OSESSAME_COMMAND_H__

using namespace O_SESSAME; 

namespace O_SESSAME {

/*! \brief Class interface for creating commands in Open-SESSAME.
* \ingroup DataHandling
*
*
*/
class Command 
{
public:
    virtual ~Command();

    virtual void Execute() = 0;
protected:
    Command();

private:

};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/