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

// FILE: GrantUpgradeCreate.cpp ////////////////////////////////////////////////////////////////////////
// Author: Kris Morness, April 2002
// Desc:   GrantUpgrade create module
///////////////////////////////////////////////////////////////////////////////////////////////////

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "PreRTS.h"	// This must go first in EVERY cpp file int the GameEngine

#define DEFINE_OBJECT_STATUS_NAMES
#include "Common/Player.h"
#include "Common/Upgrade.h"
#include "Common/Xfer.h"
#include "GameLogic/Module/GrantUpgradeCreate.h"
#include "GameLogic/Object.h"

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
GrantUpgradeCreateModuleData::GrantUpgradeCreateModuleData()
{
	m_upgradeName = "";
#ifdef OG
	m_exemptStatus = OBJECT_STATUS_NONE;
#endif
}

// ------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------
void GrantUpgradeCreateModuleData::buildFieldParse(MultiIniFieldParse& p)
{
  CreateModuleData::buildFieldParse(p);

	static const FieldParse dataFieldParse[] = 
	{
		{ "UpgradeToGrant",	INI::parseAsciiString,		NULL, offsetof( GrantUpgradeCreateModuleData, m_upgradeName ) },
#ifdef OG
		{ "ExemptStatus",	INI::parseBitString32,	TheObjectStatusBitNames, offsetof( GrantUpgradeCreateModuleData, m_exemptStatus ) },
#endif
#ifdef ZH
		{ "ExemptStatus",		ObjectStatusMaskType::parseFromINI, NULL, offsetof( GrantUpgradeCreateModuleData, m_exemptStatus ) },
#endif
		{ 0, 0, 0, 0 }
	};

  p.add(dataFieldParse);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
GrantUpgradeCreate::GrantUpgradeCreate( Thing *thing, const ModuleData* moduleData ) : CreateModule( thing, moduleData )
{
}  // end GrantUpgradeCreate

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
GrantUpgradeCreate::~GrantUpgradeCreate( void )
{

}  // end ~GrantUpgradeCreate

//-------------------------------------------------------------------------------------------------
/** The create callback. */
//-------------------------------------------------------------------------------------------------
void GrantUpgradeCreate::onCreate( void )
{

#ifdef OG
	ObjectStatusBits exemptStatus = (ObjectStatusBits)getGrantUpgradeCreateModuleData()->m_exemptStatus;
	ObjectStatusBits currentStatus = (ObjectStatusBits)getObject()->getStatusBits();
	if( BitTest( exemptStatus, OBJECT_STATUS_UNDER_CONSTRUCTION ) == TRUE )
#endif
#ifdef ZH
	ObjectStatusMaskType exemptStatus = getGrantUpgradeCreateModuleData()->m_exemptStatus;
	ObjectStatusMaskType currentStatus = getObject()->getStatusBits();
	if( exemptStatus.test( OBJECT_STATUS_UNDER_CONSTRUCTION ) )
#endif
	{
#ifdef OG
		if(	BitTest( currentStatus, OBJECT_STATUS_UNDER_CONSTRUCTION ) == FALSE ) 
#endif
#ifdef ZH
		if(	!currentStatus.test( OBJECT_STATUS_UNDER_CONSTRUCTION ) ) 
#endif
		{
			const UpgradeTemplate *upgradeTemplate = TheUpgradeCenter->findUpgrade( getGrantUpgradeCreateModuleData()->m_upgradeName );
			if( !upgradeTemplate )
			{
				DEBUG_ASSERTCRASH( 0, ("GrantUpdateCreate for %s can't find upgrade template %s.", getObject()->getName(), getGrantUpgradeCreateModuleData()->m_upgradeName ) );
				return;
			}

#ifdef ZH
			Player *player = getObject()->getControllingPlayer();
#endif
			if( upgradeTemplate->getUpgradeType() == UPGRADE_TYPE_PLAYER )
			{
				// get the player
#ifdef OG
				Player *player = getObject()->getControllingPlayer();
#endif
				player->addUpgrade( upgradeTemplate, UPGRADE_STATUS_COMPLETE );
			}
			else
			{
				getObject()->giveUpgrade( upgradeTemplate );
			}
#ifdef ZH
			
			player->getAcademyStats()->recordUpgrade( upgradeTemplate, TRUE );
#endif
		}
	}

}  // end onCreate

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
void GrantUpgradeCreate::onBuildComplete( void )
{
	if( ! shouldDoOnBuildComplete() )
		return;

	CreateModule::onBuildComplete(); // extend

	const UpgradeTemplate *upgradeTemplate = TheUpgradeCenter->findUpgrade( getGrantUpgradeCreateModuleData()->m_upgradeName );
	if( !upgradeTemplate )
	{
		DEBUG_ASSERTCRASH( 0, ("GrantUpdateCreate for %s can't find upgrade template %s.", getObject()->getName(), getGrantUpgradeCreateModuleData()->m_upgradeName ) );
		return;
	}

	if( upgradeTemplate->getUpgradeType() == UPGRADE_TYPE_PLAYER )
	{
		// get the player
		Player *player = getObject()->getControllingPlayer();
		player->addUpgrade( upgradeTemplate, UPGRADE_STATUS_COMPLETE );
	}
	else
	{
		getObject()->giveUpgrade( upgradeTemplate );
	}
}  // end onBuildComplete

// ------------------------------------------------------------------------------------------------
/** CRC */
// ------------------------------------------------------------------------------------------------
void GrantUpgradeCreate::crc( Xfer *xfer )
{

	// extend base class
	CreateModule::crc( xfer );

}  // end crc

// ------------------------------------------------------------------------------------------------
/** Xfer method
	* Version Info:
	* 1: Initial version */
// ------------------------------------------------------------------------------------------------
void GrantUpgradeCreate::xfer( Xfer *xfer )
{

	// version
	XferVersion currentVersion = 1;
	XferVersion version = currentVersion;
	xfer->xferVersion( &version, currentVersion );

	// extend base class
	CreateModule::xfer( xfer );

}  // end xfer

// ------------------------------------------------------------------------------------------------
/** Load post process */
// ------------------------------------------------------------------------------------------------
void GrantUpgradeCreate::loadPostProcess( void )
{

	// extend base class
	CreateModule::loadPostProcess();

}  // end loadPostProcess
