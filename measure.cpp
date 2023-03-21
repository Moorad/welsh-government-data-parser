

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 854378

  This file contains the implementation of the Measure class. Measure is a
  very simple class that needs to contain a few member variables for its name,
  codename, and a Standard Library container for data. The data you need to
  store is values, organised by year. I'd recommend storing the values as
  doubles.

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment.
*/

#include <stdexcept>
#include <string>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "measure.h"

/*
  TODO: Measure::Measure(codename, label);

  Construct a single Measure, that has values across many years.

  All StatsWales JSON files have a codename for measures. You should convert
  all codenames to lowercase.

  @param codename
	The codename for the measure

  @param label
	Human-readable (i.e. nice/explanatory) label for the measure

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);
*/
Measure::Measure(std::string codename, const std::string label) : label(label)
{
	std::transform(codename.begin(), codename.end(), codename.begin(), ::tolower);

	this->codename = codename;
}

/*
  TODO: Measure::getCodename()

  Retrieve the code for the Measure. This function should be callable from a
  constant context and must promise to not modify the state of the instance or
  throw an exception.

  @return
	The codename for the Measure

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
	...
	auto codename2 = measure.getCodename();
*/
const std::string &Measure::getCodename() const noexcept
{
	return this->codename;
}

/*
  TODO: Measure::getLabel()

  Retrieve the human-friendly label for the Measure. This function should be
  callable from a constant context and must promise to not modify the state of
  the instance and to not throw an exception.

  @return
	The human-friendly label for the Measure

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
	...
	auto label = measure.getLabel();
*/

const std::string &Measure::getLabel() const noexcept
{
	return this->label;
}

/*
  TODO: Measure::setLabel(label)

  Change the label for the Measure.

  @param label
	The new label for the Measure

  @example
	Measure measure("pop", "Population");
	measure.setValue(1999, 12345678.9);
	...
	measure.setLabel("New Population");
*/
void Measure::setLabel(const std::string label)
{
	this->label = label;
}
/*
  TODO: Measure::getValue(key)

  Retrieve a Measure's value for a given year.

  @param key
	The year to find the value for

  @return
	The value stored for the given year

  @throws
	std::out_of_range if year does not exist in Measure with the message
	No value found for year <year>

  @return
	The value

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
	...
	auto value = measure.getValue(1999); // returns 12345678.9
*/
const double Measure::getValue(const int key) const
{
	auto element = this->values.find(key);

	// Return measure if found
	if (element != this->values.end())
	{
		return element->second;
	}

	throw std::out_of_range("No value found for year " + std::to_string(key));
}

/*
  TODO: Measure::setValue(key, value)

  Add a particular year's value to the Measure object. If a value already
  exists for the year, replace it.

  @param key
	The year to insert a value at

  @param value
	The value for the given year

  @return
	void

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
*/

void Measure::setValue(int year, double value)
{

	if (this->values.find(year) != this->values.end())
	{
		// language exist, reassign
		this->values[year] = value;
	}
	else
	{
		// language does not exist, insert
		this->values.insert(std::make_pair(year, value));
	}
}

/*
  TODO: Measure::size()

  Retrieve the number of years data we have for this measure. This function
  should be callable from a constant context and must promise to not change
  the state of the instance or throw an exception.

  @return
	The size of the measure

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
	auto size = measure.size(); // returns 1
*/
const int Measure::size() const noexcept
{
	return this->values.size();
}

/*
  TODO: Measure::getDifference()

  Calculate the difference between the first and last year imported. This
  function should be callable from a constant context and must promise to not
  change the state of the instance or throw an exception.

  @return
	The difference/change in value from the first to the last year, or 0 if it
	cannot be calculated

  @example
	Measure measure("pop", "Population");
	measure.setValue(1999, 12345678.9);
	measure.setValue(2001, 12345679.9);
	auto diff = measure.getDifference(); // returns 1.0
*/
const double Measure::getDifference() const noexcept
{
	double first_year = this->values.begin()->second;
	double last_year = std::prev(this->values.end())->second;

	try
	{
		return std::abs(last_year - first_year);
	}
	catch (std::length_error &e)
	{
		return 0;
	}
}

/*
  TODO: Measure::getDifferenceAsPercentage()

  Calculate the difference between the first and last year imported as a
  percentage. This function should be callable from a constant context and
  must promise to not change the state of the instance or throw an exception.

  @return
	The difference/change in value from the first to the last year as a decminal
	value, or 0 if it cannot be calculated

  @example
	Measure measure("pop", "Population");
	measure.setValue(1990, 12345678.9);
	measure.setValue(2010, 12345679.9);
	auto diff = measure.getDifferenceAsPercentage();
*/
const double Measure::getDifferenceAsPercentage() const noexcept
{
	auto years = this->getYears();
	// Sort numerically
	std::sort(years.begin(), years.end(),
			  [](const int &a, const int &b) -> bool
			  {
				  return a < b;
			  });

	double first_year = this->getValue(years[0]);
	double last_year = this->getValue(years[years.size() - 1]);

	try
	{
		return (std::abs(last_year - first_year) / first_year) * 100;
	}
	catch (std::logic_error &e)
	{
		return 0;
	}
}

/*
  TODO: Measure::getAverage()

  Calculate the average/mean value for all the values. This function should be
  callable from a constant context and must promise to not change the state of
  the instance or throw an exception.

  @return
	The average value for all the years, or 0 if it cannot be calculated

  @example
	Measure measure("pop", "Population");
	measure.setValue(1999, 12345678.9);
	measure.setValue(2001, 12345679.9);
	auto diff = measure.getAverage(); // returns 12345678.4
*/
const double Measure::getAverage() const noexcept
{
	double sum = 0;
	for (auto it = this->values.begin(); it != this->values.end(); it++)
	{
		sum += it->second;
	}

	return sum / this->size();
}

// getYears will display the list of all the years in the container.
// Used for comparing maps
const std::vector<int> Measure::getYears() const noexcept
{
	std::vector<int> keys;
	for (auto it = this->values.begin(); it != this->values.end(); ++it)
	{
		keys.push_back(it->first);
	}

	return keys;
}

/*
  TODO: operator<<(os, measure)

  Overload the << operator to print all of the Measure's imported data.

  We align the year and value outputs by padding the outputs with spaces,
  i.e. the year and values should be right-aligned to each other so they
  can be read as a table of numerical values.

  Years should be printed in chronological order. Three additional columns
  should be included at the end of the output, correspodning to the average
  value across the years, the difference between the first and last year,
  and the percentage difference between the first and last year.

  If there is no data in this measure, print the name and code, and
  on the next line print: <no data>

  See the coursework specification for more information.

  @param os
	The output stream to write to

  @param measure
	The Measure to write to the output stream

  @return
	Reference to the output stream

  @example
	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	measure.setValue(1999, 12345678.9);
	std::cout << measure << std::end;
*/
std::ostream &operator<<(std::ostream &os, Measure &measure)
{

	auto years = measure.getYears();

	// Sort numerically
	std::sort(years.begin(), years.end(),
			  [](const int &a, const int &b) -> bool
			  {
				  return a < b;
			  });

	os << measure.getLabel() << " (" << measure.getCodename() << ")" << std::endl;

	int space_count;
	for (unsigned int i = 0; i < years.size(); i++)
	{
		space_count = std::to_string(measure.getValue(years[i])).size() - std::to_string(years[i]).size();

		std::cout
			<< std::string(space_count, ' ') << std::to_string(years[i]) + " ";
	}

	space_count = std::to_string(measure.getAverage()).size() - std::string("Average").size();
	os << std::string(space_count, ' ')
	   << "Average ";

	space_count = std::to_string(measure.getDifference()).size() - std::string("Diff.").size();
	os << std::string(space_count, ' ')
	   << "Diff. ";

	space_count = std::to_string(measure.getDifferenceAsPercentage()).size() - std::string("% Diff.").size();
	os << std::string(space_count, ' ')
	   << "% Diff." << std::endl;

	for (unsigned int i = 0; i < years.size(); i++)
	{
		os << std::to_string(measure.getValue(years[i])) << " ";
	}

	os << std::to_string(measure.getAverage()) << " ";
	os << std::to_string(measure.getDifference()) << " ";
	os << std::to_string(measure.getDifferenceAsPercentage()) << std::endl;

	os
		<< std::endl;

	return os;
}
/*
  TODO: operator==(lhs, rhs)

  Overload the == operator for two Measure objects. Two Measure objects
  are only equal when their codename, label and data are all equal.

  @param lhs
	A Measure object

  @param rhs
	A second Measure object

  @return
	true if both Measure objects have the same codename, label and data; false
	otherwise
*/
bool operator==(const Measure &m1, const Measure &m2)
{
	if (m1.getCodename() != m2.getCodename() ||
		m1.getLabel() != m2.getLabel() ||
		m1.size() != m2.size())
	{
		return false;
	}

	for (auto it = m1.getYears().begin(); it != m1.getYears().end(); it++)
	{
		try
		{
			if (m1.getValue(*it) != m2.getValue(*it))
			{
				return false;
			}
		}
		catch (std::out_of_range &e)
		{
			return false;
		}
	}

	return true;
}