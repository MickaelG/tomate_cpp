
#ifndef XML_H
#define XML_H

#include <string>
using namespace std;
class Dataset;

int xml_read_data(string filename, Dataset& dataset);
int xml_write_data(string filename,const Dataset& dataset);

#endif //XML_H
