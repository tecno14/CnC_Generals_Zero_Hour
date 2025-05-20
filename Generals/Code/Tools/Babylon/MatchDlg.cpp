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

// MatchDlg.cpp : implementation file
//

#include "stdafx.h"
#ifdef OG
#include "noxstring.h"
#endif
#ifdef ZH
#include "Babylon.h"
#endif
#include "MatchDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef OG
NoxText *MatchingNoxText = NULL;
NoxText *MatchOriginalText;
NoxLabel *MatchLabel;
#endif
#ifdef ZH
BabylonText *MatchingBabylonText = NULL;
BabylonText *MatchOriginalText;
BabylonLabel *MatchLabel;
#endif

#define MAX_MATCH 256
#ifdef OG
static NoxText *current_match = NULL;
#endif
#ifdef ZH
static BabylonText *current_match = NULL;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMatchDlg dialog


CMatchDlg::CMatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMatchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMatchDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMatchDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMatchDlg, CDialog)
	//{{AFX_MSG_MAP(CMatchDlg)
	ON_BN_CLICKED(IDC_NOMATCH, OnNomatch)
	ON_BN_CLICKED(IDC_MATCH, OnMatch)
	ON_BN_CLICKED(IDC_SKIP, OnSkip)
	ON_CBN_SELCHANGE(IDC_MATCHCOMBO, OnSelchangeMatchcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMatchDlg message handlers

void CMatchDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
#ifdef OG
	MatchingNoxText = NULL;	
#endif
#ifdef ZH
	MatchingBabylonText = NULL;	
#endif
	CDialog::OnCancel();
}

void CMatchDlg::OnNomatch() 
{
	// TODO: Add your control notification handler code here
#ifdef OG
	MatchingNoxText = NULL;	
#endif
#ifdef ZH
	MatchingBabylonText = NULL;	
#endif
	CDialog::OnOK ();
}

void CMatchDlg::OnMatch() 
{
	// TODO: Add your control notification handler code here
#ifdef OG
	if ( (MatchingNoxText = current_match ) )
#endif
#ifdef ZH
	if ( (MatchingBabylonText = current_match ) )
#endif
	{
		CButton *check = (CButton *) GetDlgItem ( IDC_CHECKRETRANSLATE );

		current_match->SetRetranslate ( check->GetCheck ());
	}
	CDialog::OnOK ();
}

BOOL CMatchDlg::OnInitDialog() 
{
#ifdef OG
	NoxText *text;
#endif
#ifdef ZH
	BabylonText *text;
#endif
	ListSearch sh;
	int index;
	CStatic *newtext;
	CComboBox *combo;
	static char buffer[4*1024];


	sprintf ( buffer, "Resolve umatched text from \"%s\" on line %d", MatchLabel->NameSB(), 
							MatchOriginalText->LineNumber() );
	SetWindowText ( buffer );
	CDialog::OnInitDialog();

	current_match = NULL;

	newtext = (CStatic *) GetDlgItem ( IDC_NEWTEXT );
	newtext->SetWindowText ( MatchOriginalText->GetSB());

	combo = (CComboBox *) GetDlgItem ( IDC_MATCHCOMBO );
	CButton *check = (CButton *) GetDlgItem ( IDC_CHECKRETRANSLATE );
	check->SetCheck ( 1 );

	text = MatchLabel->FirstText ( sh );
	index = 0;

	while ( text )
	{
		if ( !text->Matched ())
		{
			int result;

			result = combo->InsertString ( index, text->GetSB ());
			result = combo->SetItemDataPtr ( index, text );

			if ( result == CB_ERR  )
			{
				result = 0;
			}
			if ( result == CB_ERRSPACE )
			{
				result = 0;
			}
			index++; 
		}
		
		text = MatchLabel->NextText ( sh );
	}

	combo->SetCurSel ( 0 );
	OnSelchangeMatchcombo();
#ifdef OG
	MatchingNoxText = NULL;	
#endif
#ifdef ZH
	MatchingBabylonText = NULL;	
#endif
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMatchDlg::OnSelchangeMatchcombo() 
{
	// TODO: Add your control notification handler code here
	int index;
	CComboBox *combo = (CComboBox *) GetDlgItem ( IDC_MATCHCOMBO );

	index = combo->GetCurSel ();

	if ( index >= 0  )
	{
		CStatic *newtext = (CStatic *) GetDlgItem ( IDC_MATCHTEXT );
#ifdef OG
		current_match = (NoxText *) combo->GetItemDataPtr ( index );
#endif
#ifdef ZH
		current_match = (BabylonText *) combo->GetItemDataPtr ( index );
#endif
		newtext->SetWindowText ( current_match->GetSB());
	}
	else
	{
		current_match = NULL;
	}
}

void CMatchDlg::OnSkip() 
{
	// TODO: Add your control notification handler code here
		 EndDialog ( IDSKIP );
}


