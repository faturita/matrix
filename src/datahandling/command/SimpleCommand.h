//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file SimpleCommand.h
*  \brief Interface to the SimpleCommand class.
*  \author $Author: nilspace $
*  \version $Revision: 1.9 $
*  \date    $Date: 2003/05/20 17:50:01 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*!  
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __OSESSAME_SIMPLECOMMAND_H__
#define __OSESSAME_SIMPLECOMMAND_H__

using namespace O_SESSAME; 

namespace O_SESSAME {

/*! \brief Interface for creating simple, templated commands in Open-SESSAME.
* \ingroup DataHandling
*
*
*/
template <class Receiver>
class SimpleCommand : public Command 
{
public:
    typedef void(Receiver::* Action)();
    SimpleCommand(Receiver* _pnewReceiver, Action _newAction) : m_pReceiver(_pnewReceiver), m_Action(_newAction) {}
    virtual ~Command();

    virtual void Execute()
        {(m_pReceiver->*m_Action)();}
protected:
    Action m_Action;
    Receiver* m_pReceiver;
private:

};
} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: AttitudeHistory.h,v $
*
******************************************************************************/