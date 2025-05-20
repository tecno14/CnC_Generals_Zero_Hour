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

// FILE: CommandSetUpgrade.h /////////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, September 2002
// Desc:	 UpgradeModule that sets a new override string for Command Set look ups
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef _COMMAND_SET_UPGRADE_H
#define _COMMAND_SET_UPGRADE_H

#include "GameLogic/Module/UpgradeModule.h"

//-----------------------------------------------------------------------------
class CommandSetUpgradeModuleData : public UpgradeModuleData
{
public:
	AsciiString m_newCommandSet;
#ifdef ZH
	AsciiString m_newCommandSetAlt;
	AsciiString m_triggerAlt;
#endif

	CommandSetUpgradeModuleData()
	{
		m_newCommandSet = AsciiString::TheEmptyString;
#ifdef ZH
		m_newCommandSetAlt	= AsciiString::TheEmptyString;
		m_triggerAlt				= "none";
#endif
	}

	static void buildFieldParse(MultiIniFieldParse& p);
};

//-----------------------------------------------------------------------------
class CommandSetUpgrade : public UpgradeModule
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( CommandSetUpgrade, "CommandSetUpgrade" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( CommandSetUpgrade, CommandSetUpgradeModuleData );

public:

	CommandSetUpgrade( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype defined by MemoryPoolObject

protected:
	virtual void upgradeImplementation( ); ///< Here's the actual work of Upgrading
	virtual Bool isSubObjectsUpgrade() { return false; }

};
#endif // _COMMAND_SET_UPGRADE_H


