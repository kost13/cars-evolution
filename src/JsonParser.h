#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QString>

namespace cer {
class CarParameters;
}

namespace json_parser {

std::pair<std::vector<cer::CarParameters>, bool> readParameters(
    const QString &file_path);

bool writeParameters(const std::vector<cer::CarParameters> &parameters,
                     const QString &file_path);
}

#endif  // JSONPARSER_H
