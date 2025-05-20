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

// FILE: LayersList.h 
/*---------------------------------------------------------------------------*/
/* EA Pacific                                                                */
/* Confidential Information	                                                 */
/* Copyright (C) 2001 - All Rights Reserved                                  */
/* DO NOT DISTRIBUTE                                                         */
/*---------------------------------------------------------------------------*/
/* Project:    RTS3                                                          */
/* File name:  LayersList.h                                                  */
/* Created:    John K. McDonald, Jr., 5/10/2002                              */
/* Desc:       Layers List Declarations                                      */
/* Revision History:                                                         */
/*		5/10/2002 : Initial creation                                           */
/*---------------------------------------------------------------------------*/

#pragma once
#ifndef _H_LAYERSLIST_
#define _H_LAYERSLIST_

// INCLUDES ///////////////////////////////////////////////////////////////////
#include "Common/AsciiString.h"
#include <list>
#include <string>

// DEFINES ////////////////////////////////////////////////////////////////////

// FORWARD DECLARATIONS ///////////////////////////////////////////////////////
class MapObject;
#ifdef ZH
class PolygonTrigger;
#endif

// TYPE DEFINES ///////////////////////////////////////////////////////////////
typedef std::list<MapObject*> ListMapObjectPtr;
typedef ListMapObjectPtr::iterator ListMapObjectPtrIt;
#ifdef ZH

typedef std::list<PolygonTrigger*> ListPolygonTriggerPtr;
typedef ListPolygonTriggerPtr::iterator ListPolygonTriggerPtrIt;
#endif

struct Layer
{
	AsciiString layerName;
	ListMapObjectPtr objectsInLayer;
#ifdef ZH
	ListPolygonTriggerPtr polygonTriggersInLayer;
#endif
	Bool show;
};

typedef std::list<Layer> ListLayer;
typedef ListLayer::iterator ListLayerIt;

// Class Definition for overridden Tree control that supports Right-click context sensitive menu.
class CLLTreeCtrl : public CTreeCtrl
{
	public:
		AsciiString getLastClickedLayer(void) {	return mLastClickedLayer; }
		AsciiString getLastClickedObject(void) {	return mLastClickedObject; }
	
	protected:
		AsciiString mLastClickedLayer;
		AsciiString mLastClickedObject;
		void buildMoveMenu(CMenu* pPopup, UINT firstID);
		virtual void OnRButtonDown(UINT nFlags, CPoint point);
		DECLARE_MESSAGE_MAP()
		
};


// Class Definition

class LayersList : public CDialog
{

	public:
		enum { IDD = IDD_LAYERSLIST };
		LayersList(UINT nIDTemplate = LayersList::IDD, CWnd *parentWnd = NULL); 
		virtual ~LayersList();

		void resetLayers();
		void addMapObjectToLayersList(IN MapObject *objToAdd, AsciiString layerToAddTo = AsciiString(TheDefaultLayerName.c_str()));
#ifdef ZH
		void addPolygonTriggerToLayersList(IN PolygonTrigger *triggerToAdd, AsciiString layerToAddTo = AsciiString(TheDefaultLayerName.c_str()));
#endif
		AsciiString removeMapObjectFromLayersList(IN MapObject *objToRemove);
#ifdef ZH
		AsciiString removePolygonTriggerFromLayersList(IN PolygonTrigger *triggerToRemove);
#endif

		void changeMapObjectLayer(IN MapObject *objToChange, AsciiString layerToPlaceOn);
#ifdef ZH
		void changePolygonTriggerLayer(IN PolygonTrigger *triggerToChange, AsciiString layerToPlaceOn);
#endif

		void addLayerNamed(IN AsciiString layerToAdd);
		void removeLayerNamed(IN AsciiString layerToRemove);
		void changeLayerName(IN AsciiString oldLayerName, IN AsciiString newLayerName);
		void mergeLayerInto(IN ListLayerIt src, IN ListLayerIt dst);

		Bool isLayerHidden(IN AsciiString layerToTest);

		void updateUIFromList(void);

		void disableUpdates() { m_performUpdates = false; }
		void enableUpdates() { m_performUpdates = true; updateUIFromList(); }

		const ListLayer& GetAllLayers(void) const { return mLayers; }

		static MapObject *findObjectByUID(AsciiString objectIDToFind);
#ifdef ZH
		static PolygonTrigger *findPolygonTriggerByUID(AsciiString objectIDToFind);
		static Bool findAndSelectMapObject(AsciiString selectedItemAsciiString);
		static Bool findAndSelectPolygonTrigger(AsciiString selectedItemAsciiString);
		static void unselectAllMapObjects(void);
		static void unselectAllPolygonTriggers(void);
#endif
		
#ifdef ZH
	public:
		// This is a string because making it an AsciiString makes us barf on construction. :-(
		static std::string TheDefaultLayerName;
		static std::string ThePolygonTriggerLayerName;
		static std::string TheDefaultNewLayerName;
		static std::string TheActiveLayerName;
		static const std::string TheUnmutableDefaultLayerName;

#endif
	protected:
		AsciiString mCurrentlyEditingLabel;
		ListLayer mLayers;
		CLLTreeCtrl *mTree;
		CImageList mImageList;
		Bool m_performUpdates;
#ifdef ZH
		Bool m_activatedLayer;
#endif


		HTREEITEM findTreeLayerNamed(const AsciiString& nameToFind);
		HTREEITEM findTreeObjectNamed(const AsciiString& objectToFind, HTREEITEM layerItem);

#ifdef OG
		// This is a string because making it an AsciiString makes us barf on construction. :-(
		static std::string TheDefaultLayerName;
		static std::string TheDefaultNewLayerName;
		static const std::string TheUnmutableDefaultLayerName;

#endif
		// This function takes an MapObject, and does one of the following: 
		// 1) Return true if the MapObject can be found, and 
		//    layerIt points to a valid layer iterator in which the MapObject was found 
		//    MapObjectIt points to a valid MapObject iterator on the layerIts MapObjectsInLayer member 
		// 2) Returns false if the MapObject cannot be found.
		Bool findMapObjectAndList(IN MapObject *MapObjectToFind, OUT ListLayerIt *layerIt = NULL, OUT ListMapObjectPtrIt *MapObjectIt = NULL);
#ifdef ZH
		Bool findPolygonTriggerAndList(IN PolygonTrigger *PolygonTriggerToFind, OUT ListLayerIt *layerIt = NULL, OUT ListPolygonTriggerPtrIt *PolygonTriggerIt = NULL);
#endif
		
		// This function takes a layer name, and does one of the following:
		// 1) Return true if the layer can be found, and 
		//		layerIt points to a valid layer iterator named layerName
		// 2) Returns false if the layer cannot be found.
		Bool findLayerNamed(IN AsciiString layerName, OUT ListLayerIt *layerIt = NULL);

		void addMapObjectToLayer(IN MapObject *objToAdd, IN ListLayerIt *layerIt);
#ifdef ZH
		void addPolygonTriggerToLayer(IN PolygonTrigger *objToAdd, IN ListLayerIt *layerIt);
#endif
		void removeMapObjectFromLayer(IN MapObject *objToRemove, IN ListLayerIt *layerIt = NULL, IN ListMapObjectPtrIt *MapObjectIt = NULL);
#ifdef ZH
		void removePolygonTriggerFromLayer(IN PolygonTrigger *triggerToRemove, IN ListLayerIt *layerIt = NULL, IN ListPolygonTriggerPtrIt *PolygonTriggerIt = NULL);
		void updateObjectRenderFlags(IN ListLayerIt *layerIt);
		void updateTreeImages();
#endif

	protected:
		virtual void OnOK();
		virtual void OnCancel();
		virtual BOOL OnInitDialog();

		afx_msg void OnBeginEditLabel(NMHDR *pNotifyStruct, LRESULT* pResult);
		afx_msg void OnEndEditLabel(NMHDR *pNotifyStruct, LRESULT* pResult);

		afx_msg void OnMergeLayer(UINT commandID);
		afx_msg void OnMergeObject(UINT commandID);
		afx_msg void OnMergeViewSelection(UINT commandID);
#ifdef ZH
		afx_msg void OnSelectActiveLayer();
#endif

		
#ifdef ZH
		afx_msg void OnSelectLayerObject();
#endif
		afx_msg void OnNewLayer();
		afx_msg void OnDeleteLayer();
		afx_msg void OnHideShowLayer();
		DECLARE_MESSAGE_MAP()
};

extern LayersList *TheLayersList;

#endif /* _H_LAYERSLIST_ */
