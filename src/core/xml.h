
#ifndef XML_H
#define XML_H

#include <string>

class Dataset;

/**
 * Returns the default path of xml file storing application data.
 *
 * @return empty if the path cannot be infered
 */
const std::string get_datafile_default_path();

/**
 * Reads xml data from given file.
 *
 * @return 0 if success, or an <0 error code
 */
int xml_read_data(std::string filename, Dataset& dataset);

/**
 * Writes xml data to given file.
 *
 * @return 0 if success, or an <0 error code
 */
int xml_write_data(std::string filename,const Dataset& dataset);

#endif //XML_H
