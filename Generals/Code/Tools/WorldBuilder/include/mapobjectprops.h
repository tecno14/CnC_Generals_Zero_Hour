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

#if !defined(AFX_MAPOBJECTPROPS_H__44517B9E_12AB_4E2C_B49B_D6BB65C59649__INCLUDED_)
#define AFX_MAPOBJECTPROPS_H__44517B9E_12AB_4E2C_B49B_D6BB65C59649__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// mapobjectprops.h : header file
//

#include "OptionsPanel.h"
#include "Common/Dict.h"
#include "WBPopupSlider.h"

class MapObject;
class ModifyObjectUndoable;
#ifdef ZH
class MultipleUndoable;
class CWorldBuilderDoc;
class MapObject;
#endif

/////////////////////////////////////////////////////////////////////////////
// External Defines
extern const char* NEUTRAL_TEAM_UI_STR;
extern const char* NEUTRAL_TEAM_INTERNAL_STR;


/////////////////////////////////////////////////////////////////////////////
// MapObjectProps dialog

class MapObjectProps : public COptionsPanel, public PopupSliderOwner
{
// Construction
public:
	MapObjectProps(Dict* dictToEdit = NULL, const char* title = NULL, CWnd* pParent = NULL);   // standard constructor
	~MapObjectProps();
	void makeMain();

// Dialog Data
	//{{AFX_DATA(MapObjectProps)
	enum { IDD = IDD_MAPOBJECT_PROPS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapObjectProps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void getAllSelectedDicts(void);

	static MapObjectProps *TheMapObjectProps; 

#ifdef ZH
	// Data common to all property pages
#endif
	Dict* m_dictToEdit;
	std::vector<Dict*> m_allSelectedDicts;
	const char* m_title;
	MapObject *m_selectedObject;
#ifdef ZH
	MapObject *m_dictSource; // Source object for m_dictToEdit. m_selectedObject is not always the current source
	                         // of m_dictToEdit, and I don't understand why, so I'm making another MapObject pointer
	                         // which is always kept in sync.

	Real m_angle;
	Real m_height;
	Real m_scale;

	WBPopupSliderButton m_heightSlider;
	WBPopupSliderButton m_angleSlider;
	WBPopupSliderButton m_scaleSlider;
#endif

#ifdef ZH
	Int              m_defaultEntryIndex; //< Index in the sound combobox of the entry labelled "default"
	Bool             m_defaultIsNone; //< The default for this object is no sound
	AsciiString      m_defaultEntryName; //< The original name of the default entry

	ModifyObjectUndoable *m_posUndoable;
	Coord3D m_position;

	void deletePages();
#endif
	void updateTheUI(void);
	void enableButtons();
	int getSel();
#ifdef OG
	WBPopupSliderButton m_heightSlider;
	WBPopupSliderButton m_angleSlider;
	Real m_angle;
	Real m_height;
	ModifyObjectUndoable *m_posUndoable;
#endif

	// Generated message map functions
	//{{AFX_MSG(MapObjectProps)
#ifdef ZH
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
#endif
	afx_msg void OnSelchangeProperties();
#ifdef OG
	virtual BOOL OnInitDialog();
#endif
	afx_msg void OnEditprop();
	afx_msg void OnNewprop();
	afx_msg void OnRemoveprop();
	afx_msg void OnDblclkProperties();
#ifdef OG
	virtual void OnOK();
	virtual void OnCancel();
#endif
	afx_msg void _TeamToDict(void);
	afx_msg void _NameToDict(void);
#ifdef ZH
	afx_msg void _ScriptToDict(void);
	afx_msg void _WeatherToDict(void);
	afx_msg void _TimeToDict(void);
	afx_msg void _ScaleToDict(void);
	afx_msg void SetZOffset(void);
	afx_msg void SetAngle(void);
	afx_msg void SetPosition(void);
	afx_msg void OnScaleOn();
	afx_msg void OnScaleOff();
	afx_msg void OnKillfocusMAPOBJECTXYPosition();
	afx_msg void _PrebuiltUpgradesToDict(void);
#endif
	afx_msg void _HealthToDict(void);
	afx_msg void _EnabledToDict(void);
#ifdef OG
	afx_msg void _ScriptToDict(void);
#endif
	afx_msg void _IndestructibleToDict(void);
	afx_msg void _UnsellableToDict(void);
	afx_msg void _TargetableToDict();
	afx_msg void _PoweredToDict(void);
	afx_msg void _AggressivenessToDict(void);
	afx_msg void _VisibilityToDict(void);
	afx_msg void _VeterancyToDict(void);
	afx_msg void _ShroudClearingDistanceToDict(void);
	afx_msg void _RecruitableAIToDict(void);
	afx_msg void _SelectableToDict(void);
#ifdef OG
	afx_msg void _WeatherToDict(void);
	afx_msg void _TimeToDict(void);
	afx_msg void _PrebuiltUpgradesToDict(void);
	afx_msg void SetZOffset(void);
	afx_msg void SetAngle(void);
#endif
	afx_msg void _HPsToDict();
	afx_msg void _StoppingDistanceToDict(void);
#ifdef ZH
	afx_msg void attachedSoundToDict(void);
	afx_msg void customizeToDict(void);
	afx_msg void enabledToDict(void);
	afx_msg void loopingToDict(void);
	afx_msg void loopCountToDict(void);
	afx_msg void minVolumeToDict(void);
	afx_msg void volumeToDict(void);
	afx_msg void minRangeToDict(void);
	afx_msg void maxRangeToDict(void);
	afx_msg void priorityToDict(void);
#endif
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

#ifdef ZH
	void _DictToName(void);
#endif
	void _DictToTeam(void);
#ifdef OG
	void _DictToName(void);

#endif
#ifdef ZH
	void _DictToScript(void);
	void _DictToScale(void);
	void _DictToWeather(void);
	void _DictToTime(void);
	void _DictToPrebuiltUpgrades(void);
#endif
	void _DictToHealth(void);
	void _DictToHPs(void);
	void _DictToEnabled(void);
#ifdef OG
	void _DictToScript(void);
#endif
	void _DictToDestructible(void);
	void _DictToUnsellable(void);
	void _DictToTargetable(void);
	
	void _DictToPowered(void);
	void _DictToAggressiveness(void);
	void _DictToVisibilityRange(void);
	void _DictToVeterancy(void);
	void _DictToShroudClearingDistance(void);
	void _DictToRecruitableAI();
	void _DictToSelectable(void);
#ifdef OG
	void _DictToWeather(void);
	void _DictToTime(void);
#endif
#ifdef ZH
	void _DictToStoppingDistance(void);

#endif
	void ShowZOffset(MapObject* pMapObj);
	void ShowAngle(MapObject* pMapObj);
#ifdef OG
	void _DictToStoppingDistance(void);
	void _DictToPrebuiltUpgrades(void);

#endif
#ifdef ZH
	void ShowPosition(MapObject* pMapObj);
	void dictToAttachedSound(void);
	void dictToCustomize(void);
	void dictToEnabled(void);
	void dictToLooping(void);
	void dictToLoopCount(void);
	void dictToMinVolume(void);
	void dictToVolume(void);
	void dictToMinRange(void);
	void dictToMaxRange(void);
	void dictToPriority(void);

	void clearCustomizeFlag( CWorldBuilderDoc* pDoc, MultipleUndoable * ownerUndoable );
#endif

#ifdef OG
public:
#endif
#ifdef ZH
	// Implementation of PopupSliderOwner callbacks
#endif
	virtual void GetPopSliderInfo(const long sliderID, long *pMin, long *pMax, long *pLineSize, long *pInitial);
	virtual void PopSliderChanged(const long sliderID, long theVal);
	virtual void PopSliderFinished(const long sliderID, long theVal);

#ifdef ZH
public:
#endif
	static MapObject *getSingleSelectedMapObject(void);
	static void update(void);

#ifdef ZH
private:
  /// Disallow copying: Object is not set up to be copied
  MapObjectProps( const MapObjectProps & other ); // Deliberately undefined
  MapObjectProps & operator=( const MapObjectProps & other ); // Deliberately undefined
	void updateTheUI(MapObject *pMapObj);
	void InitSound(void);
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPOBJECTPROPS_H__44517B9E_12AB_4E2C_B49B_D6BB65C59649__INCLUDED_)
