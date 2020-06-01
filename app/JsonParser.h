// module: App
// author: Lukasz Kostrzewa

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <vector>

#include <QString>

#include "CarsPopulationData.h"

/// json_parser contains functions to load and save parameters to/from files in
/// the JSON format.
namespace json_parser {

///
/// \brief readParameters from the JSON file
/// \param file_path to the JSON file with parameters
/// \return pair of the ParametersMatrix and status bool
std::pair<cer::ParametersMatrix, bool> readParameters(const QString &file_path);

///
/// \brief writeParameters to the JSON file
/// \param parameters
/// \param file_path
/// \return true if succeeded
bool writeParameters(const cer::ParametersMatrix &parameters,
                     const QString &file_path);
}  // namespace json_parser

#endif  // JSONPARSER_H
