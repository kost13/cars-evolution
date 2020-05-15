#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <vector>

#include <QString>

#include "CarsPopulationData.h"

namespace json_parser {

std::pair<cer::ParametersMatrix, bool> readParameters(const QString &file_path);

bool writeParameters(const cer::ParametersMatrix &parameters,
                     const QString &file_path);
}  // namespace json_parser

#endif  // JSONPARSER_H
