/////////////////////////////////////////////////////////////////////////////
// Name:	ohlccandlestickrenderer.h
// Purpose: OHLC candlestick renderer declarations
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// Copyright:	(c) 2008-2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef OHLCCANDDLESTICKRENDERER_H_
#define OHLCCANDDLESTICKRENDERER_H_

#include <wx/ohlc/ohlcrenderer.h>

/**
 * OHLC renderer for displaying japanese candles.
 */
class WXDLLIMPEXP_FREECHART OHLCCandlestickRenderer : public OHLCRenderer
{
public:
	OHLCCandlestickRenderer();
	virtual ~OHLCCandlestickRenderer();

	void DrawItem(wxDC &dc, wxCoord x, wxCoord open, wxCoord high, wxCoord low, wxCoord close);

private:
	wxCoord m_candleRadius;

	wxPen m_outlinePen;
	wxBrush m_lowerBrush;
	wxBrush m_higherBrush;
};

#endif /*OHLCCANDDLESTICKRENDERER_H_*/
