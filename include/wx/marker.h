/////////////////////////////////////////////////////////////////////////////
// Name:	marker.h
// Purpose: markers declarations
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// Copyright:	(c) 2008-2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef MARKER_H_
#define MARKER_H_

#include <wx/wxfreechartdefs.h>
#include <wx/drawobject.h>

#include <wx/axis/axis.h>

#include <wx/dynarray.h>

//
// TODO: markers are not implemented!
//

/**
 * Markers base class.
 */
class WXDLLIMPEXP_FREECHART Marker : public DrawObject
{
public:
	Marker();
	virtual ~Marker();

	/**
	 * Performs marker drawing.
	 * @param dc device context
	 * @param rcData data area rectangle
	 * @param horizAxis horizontal axis
	 * @param vertAxis vertical axis
	 */
	virtual void Draw(wxDC &dc, wxRect rcData, Axis *horizAxis, Axis *vertAxis) = 0;
};

WX_DECLARE_USER_EXPORTED_OBJARRAY(Marker *, MarkerArray, WXDLLIMPEXP_FREECHART);

/**
 * Point marker.
 */
class WXDLLIMPEXP_FREECHART PointMarker : public Marker
{
public:
	PointMarker();
	virtual ~PointMarker();

	virtual void Draw(wxDC &dc, wxRect rcData, Axis *horizAxis, Axis *vertAxis);

	void SetText(const wxString &text)
	{
		m_text = text;
		FireNeedRedraw();
	}

private:
	wxString m_text;
	wxFont m_textFont;
	wxColour m_textColour;
};

#if defined(USE_LINE_MARKER)
class WXDLLIMPEXP_FREECHART LineMarker : public Marker
{
public:
	LineMarker(wxPen linePen);
	virtual ~LineMarker();

	virtual void Draw(wxDC &dc, wxRect rcData, Axis *horizAxis, Axis *vertAxis);
};
#endif


#if defined(USE_RANGE_MARKER)
class WXDLLIMPEXP_FREECHART RangeMarker : public Marker
{
public:
	RangeMarker(wxBrush brush, wxPen pen);
	virtual ~RangeMarker();

	virtual void Draw(wxDC &dc, wxRect rcData, Axis *horizAxis, Axis *vertAxis);

	/**
	 * Sets range for marker.
	 * @param minValue range minimal value
	 * @param maxValue range maximal value
	 */
	void SetRange(double minValue, double maxValue)
	{
		m_minValue = minValue;
		m_maxValue = maxValue;
		FireNeedRedraw();
	}

private:
	double m_minValue;
	double m_maxValue;
};
#endif

#endif /*MARKER_H_*/
