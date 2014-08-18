/////////////////////////////////////////////////////////////////////////////
// Name:	timeseriesdemos.cpp
// Purpose: time series demos code
// Author:	Moskvichev Andrey V.
// Created:	30.11.2008
// Copyright:	(c) 2009 Moskvichev Andrey V.
// Licence:	wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "democollection.h"

#include <wx/xy/xyplot.h>
#include <wx/xy/xylinerenderer.h>

#include <wx/axis/numberaxis.h>
#include <wx/axis/dateaxis.h>

/**
 * Time series demo dataset.
 */
class TimeSeriesDataset : public XYDataset, public DateTimeDataset
{
	DECLARE_CLASS(TimeSeriesDataset)
public:
	TimeSeriesDataset(double *data, time_t *times, size_t count);
	virtual ~TimeSeriesDataset();

	virtual size_t GetSerieCount();

	virtual wxString GetSerieName(size_t serie);

	virtual size_t GetCount(size_t serie);

	virtual double GetX(size_t index, size_t serie);

	virtual double GetY(size_t index, size_t serie);

	virtual DateTimeDataset *AsDateTimeDataset();

	//
	// DateTimeDataset
	//
	virtual time_t GetDate(size_t index);

	virtual size_t GetCount();


private:
	double *m_data;
	time_t *m_times;
	size_t m_count;
};

IMPLEMENT_CLASS(TimeSeriesDataset, XYDataset)

TimeSeriesDataset::TimeSeriesDataset(double *data, time_t *times, size_t count)
{
	m_data = new double[count];
	m_times = new time_t[count];
	m_count = count;

	for (size_t n = 0; n < count; n++) {
		m_data[n] = data[n];
		m_times[n] = times[n];
	}
}

TimeSeriesDataset::~TimeSeriesDataset()
{
	m_count = 0;
	wxDELETEA(m_data);
	wxDELETEA(m_times);
}

size_t TimeSeriesDataset::GetSerieCount()
{
	return 1;
}

size_t TimeSeriesDataset::GetCount(size_t WXUNUSED(serie))
{
	return m_count;
}

size_t TimeSeriesDataset::GetCount()
{
	return m_count;
}

wxString TimeSeriesDataset::GetSerieName(size_t WXUNUSED(serie))
{
	return wxT("Time series");
}

double TimeSeriesDataset::GetX(size_t index, size_t WXUNUSED(serie))
{
	return index;
}

double TimeSeriesDataset::GetY(size_t index, size_t WXUNUSED(serie))
{
	return m_data[index];
}

time_t TimeSeriesDataset::GetDate(size_t index)
{
	return m_times[index];
}

DateTimeDataset *TimeSeriesDataset::AsDateTimeDataset()
{
	return this;
}


/**
 *
 */
class TimeSeriesDemo1 : public ChartDemo
{
public:
	TimeSeriesDemo1()
	: ChartDemo(wxT("Time series Demo 1"))
	{
	}

	virtual Chart *Create()
	{
		// data
		double data[] = {
			 64.09,
			 63.34,
			 61.41,
			 62.00,
			 61.71,
			 63.39,
			 63.64,
			 63.61,
			 65.11,
			 65.72,
			 66.89,
			 66.68,
			 66.51,
			 66.40,
			 67.27,
			 67.66,
		};
		// dates
		const wxChar *strDates[] = {
			wxT("20060317"),
			wxT("20060320"),
			wxT("20060321"),
			wxT("20060322"),
			wxT("20060323"),
			wxT("20060324"),
			wxT("20060327"),
			wxT("20060328"),
			wxT("20060329"),
			wxT("20060330"),
			wxT("20060331"),
			wxT("20060403"),
			wxT("20060404"),
			wxT("20060405"),
			wxT("20060406"),
			wxT("20060407"),
		};

		time_t times[WXSIZEOF(strDates)];

		wxDateTime dt;
		for (size_t n = 0; n < WXSIZEOF(strDates); n++) {
			dt.ParseFormat(strDates[n], wxT("%Y%m%d"));
			times[n] = dt.GetTicks();
		}

		// first step: create plot
		XYPlot *plot = new XYPlot();

		TimeSeriesDataset *dataset = new TimeSeriesDataset(data, times, WXSIZEOF(data));

		dataset->SetRenderer(new XYLineRenderer());

		plot->AddDataset(dataset);

		// add left number and bottom date axes
		NumberAxis *leftAxis = new NumberAxis(AXIS_LEFT);
		DateAxis *bottomAxis = new DateAxis(AXIS_BOTTOM);

		// setup window
		//bottomAxis->SetWindow(0, 10);
		//bottomAxis->SetUseWindow(true);

		bottomAxis->SetVerticalLabelText(true);
		bottomAxis->SetDateFormat(wxT("%d-%m"));

		// add axes to first plot
		plot->AddAxis(leftAxis);
		plot->AddAxis(bottomAxis);

		// link axes and dataset
		plot->LinkDataVerticalAxis(0, 0);
		plot->LinkDataHorizontalAxis(0, 0);

		// and finally create chart
		Chart *chart = new Chart(plot, GetName());

		//chart->SetScrolledAxis(bottomAxis);
		return chart;
	}
};

ChartDemo *timeSeriesDemos[] = {
	new TimeSeriesDemo1(),
};
int timeSeriesDemosCount = WXSIZEOF(timeSeriesDemos);
