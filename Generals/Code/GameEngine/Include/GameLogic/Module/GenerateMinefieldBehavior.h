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

// FILE: GenerateMinefieldBehavior.h /////////////////////////////////////////////////////////////////////////
// Author: Colin Day, December 2001
// Desc:   Update that will count down a lifetime and destroy object when it reaches zero
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef __GenerateMinefieldBehavior_H_
#define __GenerateMinefieldBehavior_H_

// INCLUDES ///////////////////////////////////////////////////////////////////////////////////////
#include "GameLogic/Module/BehaviorModule.h"
#include "GameLogic/Module/DieModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#ifdef ZH
#include "GameLogic/Module/UpdateModule.h"
#endif

//-------------------------------------------------------------------------------------------------
class GenerateMinefieldBehaviorModuleData : public BehaviorModuleData
{
public:
	UpgradeMuxData				m_upgradeMuxData;
	AsciiString						m_mineName;
#ifdef ZH
	AsciiString						m_mineNameUpgraded;
	AsciiString						m_mineUpgradeTrigger;
#endif
	const FXList*					m_genFX;
	Real									m_distanceAroundObject;
	Real									m_minesPerSquareFoot;
	Real									m_randomJitter;
	Real									m_skipIfThisMuchUnderStructure;
	Bool									m_onDeath;
	Bool									m_borderOnly;
	Bool									m_alwaysCircular;
#ifdef ZH
	Bool									m_upgradable;
#endif
	Bool									m_smartBorder;
	Bool									m_smartBorderSkipInterior;

	GenerateMinefieldBehaviorModuleData();

	static void buildFieldParse(MultiIniFieldParse& p);

private:

};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#ifdef OG
class GenerateMinefieldBehavior : public BehaviorModule, 
#endif
#ifdef ZH
class GenerateMinefieldBehavior : public UpdateModule, 
#endif
																	public DieModuleInterface,
																	public UpgradeMux
{

	MEMORY_POOL_GLUE_WITH_USERLOOKUP_CREATE( GenerateMinefieldBehavior, "GenerateMinefieldBehavior" )
	MAKE_STANDARD_MODULE_MACRO_WITH_MODULE_DATA( GenerateMinefieldBehavior, GenerateMinefieldBehaviorModuleData )

public:

	GenerateMinefieldBehavior( Thing *thing, const ModuleData* moduleData );
	// virtual destructor prototype provided by memory pool declaration

	// module methods
#ifdef OG
	static Int getInterfaceMask() { return (MODULEINTERFACE_DIE) | (MODULEINTERFACE_UPGRADE); }
#endif
#ifdef ZH
	static Int getInterfaceMask() { return UpdateModule::getInterfaceMask() | (MODULEINTERFACE_DIE) | (MODULEINTERFACE_UPGRADE); }
#endif

	// BehaviorModule
	virtual DieModuleInterface* getDie() { return this; }
	virtual UpgradeModuleInterface* getUpgrade() { return this; }
#ifdef ZH
	virtual UpdateSleepTime update();
#endif

	// DamageModuleInterface
	virtual void onDie( const DamageInfo *damageInfo );

	void setMinefieldTarget(const Coord3D* pos);

protected:

	virtual void upgradeImplementation();
	virtual Bool isSubObjectsUpgrade() { return false; }

#ifdef OG
	virtual void getUpgradeActivationMasks(Int64& activation, Int64& conflicting) const
#endif
#ifdef ZH
	virtual void getUpgradeActivationMasks(UpgradeMaskType& activation, UpgradeMaskType& conflicting) const
#endif
	{
		getGenerateMinefieldBehaviorModuleData()->m_upgradeMuxData.getUpgradeActivationMasks(activation, conflicting);
	}
	virtual void performUpgradeFX()
	{
		getGenerateMinefieldBehaviorModuleData()->m_upgradeMuxData.performUpgradeFX(getObject());
#ifdef ZH
	}
	virtual void processUpgradeRemoval()
	{
		// I can't take it any more.  Let the record show that I think the UpgradeMux multiple inheritence is CRAP.
		getGenerateMinefieldBehaviorModuleData()->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject());
#endif
	}
	virtual Bool requiresAllActivationUpgrades() const
	{
		return getGenerateMinefieldBehaviorModuleData()->m_upgradeMuxData.m_requiresAllTriggers;
	}


private:
	Coord3D m_target;
	Bool m_hasTarget;
	Bool m_generated;
#ifdef ZH
	Bool								m_upgraded;
	std::list<ObjectID> m_mineList;
#endif
	
	const Coord3D* getMinefieldTarget() const;
	void placeMines();
	void placeMinesInFootprint(const GeometryInfo& geom, const ThingTemplate* mineTemplate);
	void placeMinesAroundCircle(const Coord3D& pos, Real radius, const ThingTemplate* mineTemplate);
	void placeMinesAlongLine(const Coord3D& posStart, const Coord3D& posEnd, const ThingTemplate* mineTemplate, Bool skipOneAtStart);
	void placeMinesAroundRect(const Coord3D& pos, Real majorRadius, Real minorRadius, const ThingTemplate* mineTemplate);
	Object* placeMineAt(const Coord3D& pt, const ThingTemplate* mineTemplate, Team* team, const Object* producer);
};

#endif // __GenerateMinefieldBehavior_H_

