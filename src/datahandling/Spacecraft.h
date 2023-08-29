//////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file Spacecraft.h
*  \brief Interface to spacecraft class.
*  \author $Author: nilspace $
*  \version $Revision: 1.13 $
*  \date    $Date: 2003/06/06 00:34:16 $
*//////////////////////////////////////////////////////////////////////////////////////////////////
/*  
*
*/
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __O_SESSAME_SPACECRAFT_H__
#define __O_SESSAME_SPACECRAFT_H__

namespace O_SESSAME {

class Spacecraft
{
public:
    Spacecraft();
    virtual ~Spacecraft();
 
    void SetPropagator(Propagator* _pNewPropagator)	{m_pPropagator = _pNewPropagator;}
    
    void SetOrbit(Orbit* _pNewOrbit)			{m_pOrbit = _pNewOrbit;}
    
    void SetAttitude(Attitude* _pNewAttitude)		{m_pAttitude = _pNewAttitude;}
    
    
private:
    Propagator* m_pPropagator;
    Orbit*	m_pOrbit;
    Attitude*	m_pAttitude;
        
};

} // close namespace O_SESSAME

#endif


// Do not change the comments below - they will be added automatically by CVS
/*****************************************************************************
*	$Log: History.h,v $
*
******************************************************************************/
