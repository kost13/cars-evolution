#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <vector>

#include <QString>

#include "CarsEvolutionCore/CarsPopulationData.h"

namespace json_parser {

std::pair<std::vector<cer::CarParameters>, bool> readParameters(
    const QString &file_path);

bool writeParameters(const std::vector<cer::CarParameters> &parameters,
                     const QString &file_path);
}  // namespace json_parser

#endif  // JSONPARSER_H
