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

// FILE: UpgradeModule.h /////////////////////////////////////////////////////////////////////////////////
// Author: Graham Smallwood, March 2002
// Desc:	 A Module that responds to PlayerUpgrade Bitsetting by executing code once and only once
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __UPGRADE_MODULE_H_
#define __UPGRADE_MODULE_H_

#include "Common/Module.h"
#include "Common/STLTypedefs.h"
#ifdef ZH
#include "Common/Upgrade.h"
#endif

#include "GameClient/Drawable.h"
#include "GameClient/FXList.h"

#include "GameLogic/Module/BehaviorModule.h"

// FORWARD REFERENCES /////////////////////////////////////////////////////////////////////////////
class Player;

//-------------------------------------------------------------------------------------------------
/** OBJECT DIE MODULE base class */
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
class UpgradeModuleInterface
{
public:

 	virtual Bool isAlreadyUpgraded() const = 0;
#ifdef OG
	virtual Bool attemptUpgrade( Int64 keyMask ) = 0;
	virtual Bool wouldUpgrade( Int64 keyMask ) const = 0;
	virtual Bool resetUpgrade( Int64 keyMask ) = 0;
#endif
#ifdef ZH
	virtual Bool attemptUpgrade( UpgradeMaskType keyMask ) = 0;
	virtual Bool wouldUpgrade( UpgradeMaskType keyMask ) const = 0;
	virtual Bool resetUpgrade( UpgradeMaskType keyMask ) = 0;
#endif
	virtual Bool isSubObjectsUpgrade() = 0;
	virtual void forceRefreshUpgrade() = 0;
#ifdef OG
	virtual Bool testUpgradeConditions( Int64 keyMask ) const = 0;
#endif
#ifdef ZH
	virtual Bool testUpgradeConditions( UpgradeMaskType keyMask ) const = 0;
#endif

};

//-------------------------------------------------------------------------------------------------
class UpgradeMuxData	// does NOT inherit from ModuleData.
{
public:
#ifdef ZH
	mutable std::vector<AsciiString>	m_triggerUpgradeNames;
#endif
	mutable std::vector<AsciiString>	m_activationUpgradeNames;
	mutable std::vector<AsciiString>	m_conflictingUpgradeNames;
#ifdef ZH
	mutable std::vector<AsciiString>	m_removalUpgradeNames;
	
#endif
	mutable const FXList*							m_fxListUpgrade;
#ifdef OG
	mutable Int64		m_activationMask;				///< Activation only supports a single name currently
	mutable Int64		m_conflictingMask;			///< Conflicts support multiple listings, and they are an OR
#endif
#ifdef ZH
	mutable UpgradeMaskType						m_activationMask;				///< Activation only supports a single name currently
	mutable UpgradeMaskType						m_conflictingMask;			///< Conflicts support multiple listings, and they are an OR
#endif
	mutable Bool    m_requiresAllTriggers;

	UpgradeMuxData()
	{
#ifdef OG
		m_activationMask = 0;
		m_conflictingMask = 0;

#endif
#ifdef ZH
		m_triggerUpgradeNames.clear();
		m_activationUpgradeNames.clear();
		m_conflictingUpgradeNames.clear();
		m_removalUpgradeNames.clear();

#endif
		m_fxListUpgrade = NULL;
#ifdef ZH
		m_activationMask.clear();
		m_conflictingMask.clear();
#endif
		m_requiresAllTriggers = false;
	}

	static const FieldParse* getFieldParse() 
	{
		static const FieldParse dataFieldParse[] = 
		{
			{ "TriggeredBy",		INI::parseAsciiStringVector, NULL, offsetof( UpgradeMuxData, m_activationUpgradeNames ) },
			{ "ConflictsWith",	INI::parseAsciiStringVector, NULL, offsetof( UpgradeMuxData, m_conflictingUpgradeNames ) },
#ifdef ZH
			{ "RemovesUpgrades",INI::parseAsciiStringVector, NULL, offsetof( UpgradeMuxData, m_removalUpgradeNames ) },
#endif
			{ "FXListUpgrade",	INI::parseFXList, NULL, offsetof( UpgradeMuxData, m_fxListUpgrade ) },
			{ "RequiresAllTriggers", INI::parseBool, NULL, offsetof( UpgradeMuxData, m_requiresAllTriggers ) },
			{ 0, 0, 0, 0 }
		};
		return dataFieldParse;
	}
	Bool requiresAllActivationUpgrades() const;
#ifdef OG
	void getUpgradeActivationMasks(Int64& activation, Int64& conflicting) const;	///< The first time someone looks at my mask, I'll figure it out.
#endif
#ifdef ZH
	void getUpgradeActivationMasks(UpgradeMaskType& activation, UpgradeMaskType& conflicting) const;	///< The first time someone looks at my mask, I'll figure it out.
#endif
	void performUpgradeFX(Object* obj) const;
#ifdef ZH
	void muxDataProcessUpgradeRemoval(Object* obj) const;
	Bool isTriggeredBy(const std::string &upgrade) const;
#endif
};

//-------------------------------------------------------------------------------------------------
// implements some handy mix-in guts, but doesn't descend from ObjectModule... useful for Behaviors
// that want to implement Upgrades
class UpgradeMux : public UpgradeModuleInterface
{
public:

	UpgradeMux();

	virtual Bool isAlreadyUpgraded() const ;
	// ***DANGER! DANGER! Don't use this, unless you are forcing an already made upgrade to refresh!!
	virtual void forceRefreshUpgrade();
#ifdef OG
	virtual Bool attemptUpgrade( Int64 keyMask );
	virtual Bool wouldUpgrade( Int64 keyMask ) const;
	virtual Bool resetUpgrade( Int64 keyMask );
	virtual Bool testUpgradeConditions( Int64 keyMask ) const;
#endif
#ifdef ZH
	virtual Bool attemptUpgrade( UpgradeMaskType keyMask );
	virtual Bool wouldUpgrade( UpgradeMaskType keyMask ) const;
	virtual Bool resetUpgrade( UpgradeMaskType keyMask );
	virtual Bool testUpgradeConditions( UpgradeMaskType keyMask ) const;
#endif

protected:

	void setUpgradeExecuted(Bool e) { m_upgradeExecuted = e; }
	virtual void upgradeImplementation( ) = 0; ///< Here's the actual work of Upgrading
#ifdef OG
	virtual void getUpgradeActivationMasks(Int64& activation, Int64& conflicting) const = 0; ///< Here's the actual work of Upgrading
#endif
#ifdef ZH
	virtual void getUpgradeActivationMasks(UpgradeMaskType& activation, UpgradeMaskType& conflicting) const = 0; ///< Here's the actual work of Upgrading
#endif
	virtual void performUpgradeFX() = 0;	///< perform the associated fx list
	virtual Bool requiresAllActivationUpgrades() const = 0;
	virtual Bool isSubObjectsUpgrade() = 0;
#ifdef ZH
	virtual void processUpgradeRemoval() = 0;
#endif
	
#ifdef OG
	void giveSelfUpgrade()
	{
		// If I have an activation condition, and I haven't activated, and this key matches my condition.
		performUpgradeFX();
		upgradeImplementation();
		setUpgradeExecuted(true);
	}
#endif
#ifdef ZH
	void giveSelfUpgrade();

#endif

	//
	// this is not a snapshot class itself, but it is a base class used in conjunction with
	// multiple inheritance for modules, so those modules need to be able to poke inside
	// this class and xfer the data here during a snapshot process
	//
	virtual void upgradeMuxCRC( Xfer *xfer );
	virtual void upgradeMuxXfer( Xfer *xfer);
	virtual void upgradeMuxLoadPostProcess( void );

private:
	Bool m_upgradeExecuted;				///< Upgrade only executes once

};

//-------------------------------------------------------------------------------------------------
struct UpgradeModuleData : public BehaviorModuleData
{
public:
	UpgradeMuxData				m_upgradeMuxData;

	static void buildFieldParse(MultiIniFieldParse& p) 
	{
    ModuleData::buildFieldParse(p);
		p.add(UpgradeMuxData::getFieldParse(), offsetof( UpgradeModuleData, m_upgradeMuxData ));
	}
};

//-------------------------------------------------------------------------------------------------
class UpgradeModule : public BehaviorModule, public UpgradeMux
{

	MEMORY_POOL_GLUE_ABC( UpgradeModule )
	MAKE_STANDARD_MODULE_MACRO_ABC( UpgradeModule )
	MAKE_STANDARD_MODULE_DATA_MACRO_ABC(UpgradeModule, UpgradeModuleData)

public:

	UpgradeModule( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype defined by MemoryPoolObject

	// module methods
	static Int getInterfaceMask() { return MODULEINTERFACE_UPGRADE; }

	// BehaviorModule
	virtual UpgradeModuleInterface* getUpgrade() { return this; }

#ifdef ZH
	bool isTriggeredBy(const std::string & upgrade) const { return getUpgradeModuleData()->m_upgradeMuxData.isTriggeredBy(upgrade); }

#endif
protected:
#ifdef ZH

	virtual void processUpgradeRemoval()
	{
		// I can't take it any more.  Let the record show that I think the UpgradeMux multiple inheritence is CRAP.
		getUpgradeModuleData()->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject());
	}
#endif

	virtual Bool requiresAllActivationUpgrades() const
	{
		return getUpgradeModuleData()->m_upgradeMuxData.m_requiresAllTriggers;
	}

#ifdef OG
	virtual void getUpgradeActivationMasks(Int64& activation, Int64& conflicting) const
#endif
#ifdef ZH
	virtual void getUpgradeActivationMasks(UpgradeMaskType& activation, UpgradeMaskType& conflicting) const
#endif
	{
		getUpgradeModuleData()->m_upgradeMuxData.getUpgradeActivationMasks(activation, conflicting);
	}

	virtual void performUpgradeFX()
	{
		getUpgradeModuleData()->m_upgradeMuxData.performUpgradeFX(getObject());
	}

};
inline UpgradeModule::UpgradeModule( Thing *thing, const ModuleData* moduleData ) : BehaviorModule( thing, moduleData ) { }
inline UpgradeModule::~UpgradeModule() { }


#endif
