

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 854378

  This file contains the implementation for the Area class. Area is a relatively
  simple class that contains a local authority code, a container of names in
  different languages (perhaps stored in an associative container?) and a series
  of Measure objects (also in some form of container).

  This file contains numerous functions you must implement. Each function you
  must implement has a TODO block comment.
*/

#include <stdexcept>
#include <algorithm>
#include "area.h"
#include "bethyw.h"

/*
  TODO: Area::Area(localAuthorityCode)

  Construct an Area with a given local authority code.

  @param localAuthorityCode
	The local authority code of the Area

  @example
	Area("W06000023");
*/
Area::Area(const std::string localAuthorityCode) : localAuthorityCode(localAuthorityCode) {}

/*
  TODO: Area::getLocalAuthorityCode()

  Retrieve the local authority code for this Area. This function should be
  callable from a constant context and not modify the state of the instance.

  @return
	The Area's local authority code

  @example
	Area area("W06000023");
	...
	auto authCode = area.getLocalAuthorityCode();
*/
const std::string &Area::getLocalAuthorityCode() const
{
	return this->localAuthorityCode;
}

/*
  TODO: Area::getName(lang)

  Get a name for the Area in a specific language.  This function should be
  callable from a constant context and not modify the state of the instance.

  @param lang
	A three-leter language code in ISO 639-3 format, e.g. cym or eng

  @return
	The name for the area in the given language

  @throws
	std::out_of_range if lang does not correspond to a language of a name stored
	inside the Area instance

  @example
	Area area("W06000023");
	std::string langCode  = "eng";
	std::string langValue = "Powys";
	area.setName(langCode, langValue);
	...
	auto name = area.getName(langCode);
*/
const std::string &Area::getName(const std::string &lang) const
{
	for (auto it = this->names.begin(); it != this->names.end(); ++it)
	{
		if (strcasecmp(it->first.c_str(), lang.c_str()) == 0)
		{
			return it->second;
		}
	}

	throw std::out_of_range("Lang does not correspond to a language");
}

/*
  TODO: Area::setName(lang, name)

  Set a name for the Area in a specific language.

  @param lang
	A three-letter (alphabetical) language code in ISO 639-3 format,
	e.g. cym or eng, which should be converted to lowercase

  @param name
	The name of the Area in `lang`

  @throws
	std::invalid_argument if lang is not a three letter alphabetic code

  @example
	Area area("W06000023");
	std::string langCodeEnglish  = "eng";
	std::string langValueEnglish = "Powys";
	area.setName(langCodeEnglish, langValueEnglish);

	std::string langCodeWelsh  = "cym";
	std::string langValueWelsh = "Powys";
	area.setName(langCodeWelsh, langValueWelsh);
*/
void Area::setName(std::string lang, std::string name)
{
	if (lang.size() != 3)
	{
		throw std::invalid_argument("Area::setName: Language code must be three alphabetical letters only");
	}

	// Check if each character in language code is an alphabetical letter
	for (size_t i = 0; i < lang.size(); i++)
	{
		if (!isalpha(lang[i]))
		{
			throw std::invalid_argument("Area::setName: Language code must be three alphabetical letters only");
		}
	}

	std::transform(lang.begin(), lang.end(), lang.begin(), tolower);

	for (auto it = this->names.begin(); it != this->names.end(); ++it)
	{
		if (strcasecmp(it->first.c_str(), lang.c_str()) == 0)
		{
			this->names[lang] = name;
			return;
		}
	}

	// language does not exist, insert
	this->names.insert(std::make_pair(lang, name));
}

/*
  TODO: Area::getMeasure(key)

  Retrieve a Measure object, given its codename. This function should be case
  insensitive when searching for a measure.

  @param key
	The codename for the measure you want to retrieve

  @return
	A Measure object

  @throws
	std::out_of_range if there is no measure with the given code, throwing
	the message:
	No measure found matching <codename>

  @example
	Area area("W06000023");
	Measure measure("Pop", "Population");
	area.setMeasure("Pop", measure);
	...
	auto measure2 = area.getMeasure("pop");
*/
Measure &Area::getMeasure(const std::string &key) const
{
	for (auto it = this->measures.begin(); it != this->measures.end(); ++it)
	{
		if (strcasecmp(it->first.c_str(), key.c_str()) == 0)
		{
			return it->second;
		}
	}

	throw std::out_of_range("No measure found matching " + key);
}

/*
  TODO: Area::setMeasure(codename, measure)

  Add a particular Measure to this Area object. Note that the Measure's
  codename should be converted to lowercase.

  If a Measure already exists with the same codename in this Area, overwrite any
  values contained within the existing Measure with those in the new Measure
  passed into this function. The resulting Measure stored inside the Area
  instance should be a combination of the two Measures instances.

  @param codename
	The codename for the Measure

  @param measure
	The Measure object

  @return
	void

  @example
	Area area("W06000023");

	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	double value = 12345678.9;
	measure.setValue(1999, value);

	area.setMeasure(codename, measure);
*/
void Area::setMeasure(const std::string codename, Measure measure)
{

	for (auto it = this->measures.begin(); it != this->measures.end(); ++it)
	{
		if (strcasecmp(it->first.c_str(), codename.c_str()) == 0)
		{
			// If an existing measure is found
			// replace the label and all the years + value
			it->second.setLabel(measure.getLabel());

			std::vector<unsigned int> years = measure.getAllYears();
			for (size_t i = 0; i < years.size(); i++)
			{
				it->second.setValue(years[i], measure.getValue(years[i]));
			}

			// Exit method, dont run insertion code
			return;
		}
	}

	// If the value was not overwritten then we insert
	// We copy the name and convert it to lowercase (we cant modify original because const)
	// This has to be lowercase for getMeasure and sorting to work correctly
	std::string codenameLower(codename.size(), 0);
	std::transform(codename.begin(), codename.end(), codenameLower.begin(), ::tolower);
	this->measures.insert(std::make_pair(codenameLower, measure));
}

/*
  TODO: Area::size()

  Retrieve the number of Measures we have for this Area. This function should be
  callable from a constant context, not modify the state of the instance, and
  must promise not throw an exception.

  @return
	The size of the Area (i.e., the number of Measures)

  @example
	Area area("W06000023");
	std::string langCode  = "eng";
	std::string langValue = "Powys";
	area.setName(langCode, langValue);

	std::string codename = "Pop";
	std::string label = "Population";
	Measure measure(codename, label);

	area.setMeasure(code, measure);
	auto size = area.size();
*/
const int Area::size() const
{
	return this->measures.size();
}

// Auxiliary method to get all area names sorted alphabetically
// e.g. eng and cym
const std::vector<std::string> Area::getAllNames() const noexcept
{
	std::vector<std::string> keys;
	for (auto it = this->names.begin(); it != this->names.end(); ++it)
	{
		keys.push_back(it->first);
	}

	// Sort keys alphabetically
	std::sort(keys.begin(), keys.end(),
			  [](const std::string &a, const std::string &b) -> bool
			  {
				  return a > b;
			  });

	return keys;
}

// Auxiliary method to get all measurement codenames sorted alphabetically
// e.g. pop and dens
const std::vector<std::string> Area::getAllMeasureCodenames() const noexcept
{
	std::vector<std::string> keys;
	for (auto it = this->measures.begin(); it != this->measures.end(); ++it)
	{
		keys.push_back(it->first);
	}

	// Sort keys alphabetically
	std::sort(keys.begin(), keys.end(),
			  [](const std::string &a, const std::string &b) -> bool
			  {
				  return a < b;
			  });

	return keys;
}

/*
  TODO: operator<<(os, area)

  Overload the stream output operator as a free/global function.

  Output the name of the Area in English and Welsh, followed by the local
  authority code. Then output all the measures for the area (see the coursework
  worksheet for specific formatting).

  If the Area only has only one name, output this. If the area has no names,
  output the name "Unnamed".

  Measures should be ordered by their Measure codename. If there are no measures
  output the line "<no measures>" after you have output the area names.

  See the coursework specification for more examples.

  @param os
	The output stream to write to

  @param area
	Area to write to the output stream

  @return
	Reference to the output stream

  @example
	Area area("W06000023");
	area.setName("eng", "Powys");
	std::cout << area << std::endl;
*/
std::ostream &operator<<(std::ostream &os, Area &area)
{
	auto areaNames = area.getAllNames();

	// if no english or welsh name output "Unnamed"
	if (areaNames.empty())
	{
		os << "Unnamed";
	}

	// If english or/and welsh is set, output them
	for (size_t i = 0; i < areaNames.size(); i++)
	{
		os << area.getName(areaNames[i]);

		if (i < areaNames.size() - 1)
		{
			os << " / ";
		}
	}

	os << " (" << area.getLocalAuthorityCode() << ")" << std::endl;

	auto measureCodenames = area.getAllMeasureCodenames();

	// If no measurement code (i.e. no measures) output "<no measures>"
	if (measureCodenames.empty())
	{
		os << "<no measures>\n"
		   << std::endl;

		return os;
	}

	for (size_t i = 0; i < measureCodenames.size(); i++)
	{
		auto measure = area.getMeasure(measureCodenames[i]);

		os << measure;
	}

	return os;
}

/*
  TODO: operator==(lhs, rhs)

  Overload the == operator for two Area objects as a global/free function. Two
  Area objects are only equal when their local authority code, all names, and
  all data are equal.

  @param lhs
	An Area object

  @param rhs
	A second Area object

  @return
	true if both Area instanes have the same local authority code, names
	and data; false otherwise.

  @example
	Area area1("MYCODE1");
	Area area2("MYCODE1");

	bool eq = area1 == area2;
*/
bool operator==(const Area &a1, const Area &a2)
{
	if (a1.getLocalAuthorityCode() != a2.getLocalAuthorityCode() ||
		a1.size() != a2.size())
	{
		return false;
	}

	std::vector<std::string> names = a1.getAllNames();
	for (size_t i = 0; i < names.size(); i++)
	{
		try
		{
			if (a1.getName(names[i]) != a2.getName(names[i]))
			{
				return false;
			}
		}
		catch (const std::out_of_range &e)
		{
			return false;
		}
	}

	auto measureCodenames = a1.getAllMeasureCodenames();
	for (size_t i = 0; i < measureCodenames.size(); i++)
	{
		try
		{
			if (!(a1.getMeasure(measureCodenames[i]) == a2.getMeasure(measureCodenames[i])))
			{
				return false;
			}
		}
		catch (const std::out_of_range &e)
		{
			return false;
		}
	}

	return true;
}