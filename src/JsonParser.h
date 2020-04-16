#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>

namespace cer {
class CarsPopulationData;
}

namespace json_parser {

bool readParameters(cer::CarsPopulationData *population,
                    const QString &file_path);

bool writeParameters(const cer::CarsPopulationData *population,
                     const QString &file_path);
}

#endif  // JSONPARSER_H
