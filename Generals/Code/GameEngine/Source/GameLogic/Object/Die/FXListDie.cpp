/*
**	Command & Conquer Generals(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// FILE: FXListDie.cpp ///////////////////////////////////////////////////////////////////////////
// Author: Steven Johnson, Jan 2002
// Desc:   Simple Die module
///////////////////////////////////////////////////////////////////////////////////////////////////


// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#ifdef OG
#define DEFINE_DAMAGE_NAMES
#endif
#include "Common/INI.h"
#ifdef ZH
#include "Common/Player.h"
#endif
#include "Common/ThingTemplate.h"
#include "Common/Xfer.h"
#include "GameClient/FXList.h"
#include "GameLogic/Damage.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
#include "GameLogic/Module/FXListDie.h"
#include "GameLogic/Module/AIUpdate.h"

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
FXListDie::FXListDie( Thing *thing, const ModuleData* moduleData ) : DieModule( thing, moduleData )
#ifdef ZH
{
	if( getFXListDieModuleData()->m_initiallyActive )
#endif
{
#ifdef ZH
		giveSelfUpgrade();
	}
#endif
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
FXListDie::~FXListDie( void )
{

}

//-------------------------------------------------------------------------------------------------
/** The die callback. */
//-------------------------------------------------------------------------------------------------
void FXListDie::onDie( const DamageInfo *damageInfo )
{
#ifdef ZH
	if (!isUpgradeActive())
		return;
#endif
	if (!isDieApplicable(damageInfo))
		return;
	const FXListDieModuleData* d = getFXListDieModuleData();
#ifdef ZH

	UpgradeMaskType activation, conflicting;
	getUpgradeActivationMasks( activation, conflicting );
	Object *obj = getObject();
	if( obj->getObjectCompletedUpgradeMask().testForAny( conflicting ) )
	{
		return;
	}
	if( obj->getControllingPlayer() && obj->getControllingPlayer()->getCompletedUpgradeMask().testForAny( conflicting ) )
	{
		return;
	}

#endif
	if (d->m_defaultDeathFX)
	{
#ifdef OG
		// if the object has any ambient sound(s), kill 'em now.
		TheAudio->stopAllAmbientsBy(getObject());
		
#endif
		if (d->m_orientToObject)
		{
			Object *damageDealer = TheGameLogic->findObjectByID( damageInfo->in.m_sourceID );
			FXList::doFXObj(getFXListDieModuleData()->m_defaultDeathFX, getObject(), damageDealer);
		}
		else
		{
			FXList::doFXPos(getFXListDieModuleData()->m_defaultDeathFX, getObject()->getPosition());
		}
	}
}

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void FXListDie::crc( Xfer *xfer )
{

	// extend base class
	DieModule::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version */
// ------------------------------------------------------------------------------------------------
void FXListDie::xfer( Xfer *xfer )
{

	// version
	XferVersion currentVersion = 1;
	XferVersion version = currentVersion;
	xfer->xferVersion( &version, currentVersion );

	// extend base class
	DieModule::xfer( xfer );

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void FXListDie::loadPostProcess( void )
{

	// extend base class
	DieModule::loadPostProcess();

}  // end loadPostProcess
