
#include "xml.h"
#include "dataset.h"

#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    auto data_file = get_datafile_default_path();
    Dataset dataset;
    if (xml_read_data(data_file, dataset) != 0) {
        std::cerr << "Failed to load data file." << endl;
        return -1;
    }
    for (const auto& crop: dataset.get_crops()) {
        auto start_date = crop.get_date(Crop::DateSel::START);
        cout << start_date << ": " << crop.get_repr() << endl;
    }
    return 0;
}
