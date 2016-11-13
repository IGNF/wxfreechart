/////////////////////////////////////////////////////////////////////////////
// Name:	xylinerenderer.cpp
// Purpose: XY line renderer implementation
// Author:	Moskvichev Andrey V.
// Created:	2008/11/07
// Copyright:	(c) 2008-2010 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/xy/xylinerenderer.h>

IMPLEMENT_CLASS(XYLineRendererBase, XYRenderer)
IMPLEMENT_CLASS(XYLineRenderer, XYLineRendererBase)
IMPLEMENT_CLASS(XYLineStepRenderer, XYLineRendererBase)

//
// XYLineRendererBase
//

XYLineRendererBase::XYLineRendererBase(int defaultPenWidth, int defaultPenStyle)
{
	m_defaultPenWidth = defaultPenWidth;
	m_defaultPenStyle = defaultPenStyle;
}

XYLineRendererBase::~XYLineRendererBase()
{
}

void XYLineRendererBase::SetSeriePen(size_t serie, wxPen *pen)
{
	m_seriePens[serie] = *pen;
	FireNeedRedraw();
}

wxPen *XYLineRendererBase::GetSeriePen(size_t serie)
{
	if (m_seriePens.find(serie) == m_seriePens.end()) {
		return wxThePenList->FindOrCreatePen(GetDefaultColour(serie), m_defaultPenWidth, m_defaultPenStyle);
	}
	return &m_seriePens[serie];
}

void XYLineRendererBase::SetSerieColour(size_t serie, wxColour *colour)
{
	SetSeriePen(serie, wxThePenList->FindOrCreatePen(*colour, m_defaultPenWidth, m_defaultPenStyle));
}

wxColour XYLineRendererBase::GetSerieColour(size_t serie)
{
	if (m_seriePens.find(serie) == m_seriePens.end()) {
		return GetDefaultColour(serie);
	}
	return m_seriePens[serie].GetColour();
}

//
// XYLineRenderer
//

XYLineRenderer::XYLineRenderer(bool drawSymbols, bool drawLines, int defaultPenWidth, int defaultPenStyle)
: XYLineRendererBase(defaultPenWidth, defaultPenStyle)
{
	m_drawSymbols = drawSymbols;
	m_drawLines = drawLines;
}

XYLineRenderer::~XYLineRenderer()
{
}

void XYLineRenderer::Draw(wxDC &dc, wxRect rc, Axis *horizAxis, Axis *vertAxis, XYDataset *dataset)
{
	FOREACH_SERIE(serie, dataset) {
		if (dataset->GetCount(serie) < 2) {
			continue;
		}

		for (size_t n = 0; n < dataset->GetCount(serie) - 1; n++) {
			double x0 = dataset->GetX(n, serie);
			double y0 = dataset->GetY(n, serie);
			double x1 = dataset->GetX(n + 1, serie);
			double y1 = dataset->GetY(n + 1, serie);

			// check whether segment is visible
			if (!horizAxis->IntersectsWindow(x0, x1) &&
					!vertAxis->IntersectsWindow(y0, y1)) {
				continue;
			}

			ClipHoriz(horizAxis, x0, y0, x1, y1);
			ClipHoriz(horizAxis, x1, y1, x0, y0);
			ClipVert(vertAxis, x0, y0, x1, y1);
			ClipVert(vertAxis, x1, y1, x0, y0);

			// translate to graphics coordinates.
			wxCoord xg0, yg0;
			wxCoord xg1, yg1;

			xg0 = horizAxis->ToGraphics(dc, rc.x, rc.width, x0);
			yg0 = vertAxis->ToGraphics(dc, rc.y, rc.height, y0);
			xg1 = horizAxis->ToGraphics(dc, rc.x, rc.width, x1);
			yg1 = vertAxis->ToGraphics(dc, rc.y, rc.height, y1);

			DrawSegment(dc, serie, xg0, yg0, xg1, yg1);
		}
	}
}

void XYLineRenderer::DrawSegment(wxDC &dc, size_t serie, wxCoord x0, wxCoord y0, wxCoord x1, wxCoord y1)
{
	if (m_drawLines) {
		wxPen *pen = GetSeriePen(serie);
		dc.SetPen(*pen);
		dc.DrawLine(x0, y0, x1, y1);
	}

	if (m_drawSymbols) {
		Symbol *symbol = GetSerieSymbol(serie);

		wxColour color = GetSerieColour(serie);
		symbol->Draw(dc, x0, y0, color);
		symbol->Draw(dc, x1, y1, color);
	}
}

void XYLineRenderer::DrawLegendSymbol(wxDC &dc, wxRect rcSymbol, size_t serie)
{
	if (m_drawLines) {
		dc.SetPen(*GetSeriePen(serie));
		dc.DrawLine(rcSymbol.x, rcSymbol.y + rcSymbol.height / 2,
				rcSymbol.x + rcSymbol.width, rcSymbol.y + rcSymbol.height / 2);
	}

	if (m_drawSymbols) {
		wxColour colour = GetSerieColour(serie);
		Symbol *symbol = GetSerieSymbol(serie);

		symbol->Draw(dc, rcSymbol.x + rcSymbol.width / 2, rcSymbol.y + rcSymbol.height / 2, colour);
	}
}

//
// XYLineStepRenderer
//

XYLineStepRenderer::XYLineStepRenderer(int defaultPenWidth, int defaultPenStyle)
: XYLineRendererBase(defaultPenWidth, defaultPenStyle)
{
}

XYLineStepRenderer::~XYLineStepRenderer()
{
}

void XYLineStepRenderer::Draw(wxDC &dc, wxRect rc, Axis *horizAxis, Axis *vertAxis, XYDataset *dataset)
{
	FOREACH_SERIE(serie, dataset) {
		if (dataset->GetCount(serie) < 2) {
			continue;
		}

		// find first visible index
		size_t first = GetFirstVisibleIndex(horizAxis, vertAxis, dataset, serie);
		if (first == (size_t) -1) {
			continue; // nothing visible
		}

		// Set serie pen
		wxPen *pen = GetSeriePen(serie);
		dc.SetPen(*pen);

		// iterate until two points will be invisible
		for (size_t n = first; n < dataset->GetCount(serie) - 1; n++) {
			double x0 = dataset->GetX(n, serie);
			double y0 = dataset->GetY(n, serie);
			double x1 = dataset->GetX(n + 1, serie);
			double y1 = dataset->GetY(n + 1, serie);

			if (!horizAxis->IntersectsWindow(x0, x1) ||
					!vertAxis->IntersectsWindow(y0, y1)) {
				break;
			}

			ClipHoriz(horizAxis, x0, y0, x1, y1);
			ClipHoriz(horizAxis, x1, y1, x0, y0);
			ClipVert(vertAxis, x0, y0, x1, y1);
			ClipVert(vertAxis, x1, y1, x0, y0);

			// translate to graphics coordinates.
			wxCoord xg0, yg0;
			wxCoord xg1, yg1;

			xg0 = horizAxis->ToGraphics(dc, rc.x, rc.width, x0);
			yg0 = vertAxis->ToGraphics(dc, rc.y, rc.height, y0);
			xg1 = horizAxis->ToGraphics(dc, rc.x, rc.width, x1);
			yg1 = vertAxis->ToGraphics(dc, rc.y, rc.height, y1);

			dc.DrawLine(xg0, yg0, xg1, yg1);
		}
	}
}

void XYLineStepRenderer::DrawLegendSymbol(wxDC &dc, wxRect rcSymbol, size_t serie)
{
	dc.SetPen(*GetSeriePen(serie));
	dc.DrawLine(rcSymbol.x, rcSymbol.y + rcSymbol.height / 2,
			rcSymbol.x + rcSymbol.width, rcSymbol.y + rcSymbol.height / 2);
}

size_t XYLineStepRenderer::GetFirstVisibleIndex(Axis *horizAxis, Axis *vertAxis, XYDataset *dataset, size_t serie)
{
	for (size_t n = 0; n < dataset->GetCount(serie) - 1; n++) {
		double x0 = dataset->GetX(n, serie);
		double y0 = dataset->GetY(n, serie);
		double x1 = dataset->GetX(n + 1, serie);
		double y1 = dataset->GetY(n + 1, serie);

		// check whether segment is visible
		if (horizAxis->IntersectsWindow(x0, x1) ||
				vertAxis->IntersectsWindow(y0, y1)) {
			return n;
		}
	}
	return (size_t) -1; // nothing visible
}
