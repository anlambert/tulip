/**
 *
 * Copyright (C) 2019  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */
///@cond DOXYGEN_HIDDEN
#ifndef CSVCONTENTHANDLER_H_
#define CSVCONTENTHANDLER_H_

#include <string>
#include <vector>

#include <tulip/tulipconf.h>

namespace tlp {
/**
 * @brief Interface to inherit to get and treat data from csv files with CSVParser object.
 */
class TLP_QT_SCOPE CSVContentHandler {
public:
  virtual ~CSVContentHandler() {}
  /**
   * Function called at the beginning of the file parsing.
   */
  virtual bool begin() = 0;

  /**
   * Function called for each line in the file.
   * @param row The number of the row.
   * @param lineTokens The tokens.
   */
  virtual bool line(unsigned int row, const std::vector<std::string> &lineTokens) = 0;

  /**
   * Function called at the end of the parsing.
   * @param rowNumber the number of row read in the file.
   * @param columnNumber The column number for the line with the greatest column number.
   */
  virtual bool end(unsigned int rowNumber, unsigned int columnNumber) = 0;
};
} // namespace tlp
#endif /* CSVCONTENTHANDLER_H_ */
///@endcond
