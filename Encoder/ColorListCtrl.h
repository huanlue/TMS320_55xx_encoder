#pragma once

#include "ColorHeaderCtrl.h"

// CStatListCtrl

class CColorListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CColorListCtrl)

public:
	CColorListCtrl();
	virtual ~CColorListCtrl();

	CColorHeaderCtrl m_ctlHeader;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual void PreSubclassWindow();
};


