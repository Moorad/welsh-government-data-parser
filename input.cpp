

/*
  +---------------------------------------+
  | BETH YW? WELSH GOVERNMENT DATA PARSER |
  +---------------------------------------+

  AUTHOR: 854378

  This file contains the code responsible for opening and closing file
  streams. The actual handling of the data from that stream is handled
  by the functions in areas.cpp. See the header file for additional comments.

  Each function you must implement has a TODO in its comment block. You may
  have to implement additional functions. You may implement additional
  functions not specified.
 */

#include "input.h"
#include <iostream>
#include <fstream>

/*
  TODO: InputSource::InputSource(source)

  Constructor for an InputSource.

  @param source
	A unique identifier for a source (i.e. the location).
*/
InputSource::InputSource(const std::string &source) : source(source) {}

/*
  TODO: InputSource::getSource()

  This function should be callable from a constant context.

  @return
	A non-modifable value for the source passed into the construtor.
*/

const std::string &InputSource::getSource()
{
	return this->source;
}

/*
  TODO: InputFile:InputFile(path)

  Constructor for a file-based source.

  @param path
	The complete path for a file to import.

  @example
	InputFile input("data/areas.csv");
*/
InputFile::InputFile(const std::string &filePath) : InputSource(filePath) {}

/*
  TODO: InputFile::open()

  Open a file stream to the file path retrievable from getSource()
  and return a reference to the stream.

  @return
	A standard input stream reference

  @throws
	std::runtime_error if there is an issue opening the file, with the message:
	InputFile::open: Failed to open file <file name>

  @example
	InputFile input("data/areas.csv");
	input.open();
*/

std::istream &InputFile::open()
{
	static std::ifstream input_file;

	input_file.open(this->getSource());

	// Throw exception if an error occurs while reading
	if (input_file.fail())
	{
		throw std::runtime_error("InputFile::open: Failed to open file " + this->getSource());
	}

	input_file.close();

	return input_file;
}