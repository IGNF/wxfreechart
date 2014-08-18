/////////////////////////////////////////////////////////////////////////////
// Name:	xysimpledataset.cpp
// Purpose: xy simple dataset implementation
// Author:	Moskvichev Andrey V.
// Created:	2009/11/25
// Copyright:	(c) 2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/xy/xysimpledataset.h>

#include "wx/arrimpl.cpp"

WX_DEFINE_EXPORTED_OBJARRAY(XYSerieArray);

IMPLEMENT_CLASS(XYSimpleDataset, XYDataset)

//
// XYSerie
//

XYSerie::XYSerie(double *data, size_t count)
{
	m_data = new double[2 * count];
	memcpy(m_data, data, 2 * count * sizeof(double));
	m_count = count;
}

XYSerie::~XYSerie()
{
	wxDELETEA(m_data);
}

double XYSerie::GetX(size_t index)
{
	wxCHECK_MSG(index < m_count, 0, wxT("XYSerie::GetX"));
	return m_data[index * 2];
}

double XYSerie::GetY(size_t index)
{
	wxCHECK_MSG(index < m_count, 0, wxT("XYSerie::GetY"));
	return m_data[index * 2 + 1];
}

size_t XYSerie::GetCount()
{
	return m_count;
}

const wxString &XYSerie::GetName()
{
	return m_name;
}

void XYSerie::SetName(const wxString &name)
{
	m_name = name;
}

//
// XYSimpleDataset
//

XYSimpleDataset::XYSimpleDataset()
{
}

XYSimpleDataset::~XYSimpleDataset()
{
	for (size_t n = 0; n < m_series.Count(); n++) {
		wxDELETE(m_series[n]);
	}
}

void XYSimpleDataset::AddSerie(double *data, size_t count)
{
	AddSerie(new XYSerie(data, count));
}

void XYSimpleDataset::AddSerie(XYSerie *serie)
{
	m_series.Add(serie);
	DatasetChanged();
}

double XYSimpleDataset::GetX(size_t index, size_t serie)
{
	wxCHECK(serie < m_series.Count(), 0);
	return m_series[serie]->GetX(index);
}

double XYSimpleDataset::GetY(size_t index, size_t serie)
{
	wxCHECK(serie < m_series.Count(), 0);
	return m_series[serie]->GetY(index);
}

size_t XYSimpleDataset::GetSerieCount()
{
	return m_series.Count();
}

size_t XYSimpleDataset::GetCount(size_t serie)
{
	return m_series[serie]->GetCount();
}

wxString XYSimpleDataset::GetSerieName(size_t serie)
{
	wxCHECK(serie < m_series.Count(), wxEmptyString);
	return m_series[serie]->GetName();
}

void XYSimpleDataset::SetSerieName(size_t serie, const wxString &name)
{
	m_series[serie]->SetName(name);
	DatasetChanged();
}
