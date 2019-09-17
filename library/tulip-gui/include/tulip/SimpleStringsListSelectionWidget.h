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

#ifndef SIMPLESTRINGSLISTSELECTIONWIDGET_H_
#define SIMPLESTRINGSLISTSELECTIONWIDGET_H_

#include <QWidget>

#include "StringsListSelectionWidgetInterface.h"

namespace Ui {
class SimpleStringsListSelectionData;
}

class QListWidgetItem;

namespace tlp {

class TLP_QT_SCOPE SimpleStringsListSelectionWidget : public QWidget,
                                                      public StringsListSelectionWidgetInterface {

  Q_OBJECT

  Ui::SimpleStringsListSelectionData *_ui;

public:
  SimpleStringsListSelectionWidget(QWidget *parent = nullptr,
                                   const unsigned int maxSelectedStringsListSize = 0);
  ~SimpleStringsListSelectionWidget() override;

  void setUnselectedStringsList(const std::vector<std::string> &unselectedStringsList) override;

  void setSelectedStringsList(const std::vector<std::string> &selectedStringsList) override;

  void clearUnselectedStringsList() override;

  void clearSelectedStringsList() override;

  void setMaxSelectedStringsListSize(const unsigned int maxSelectedStringsListSize) override;

  std::vector<std::string> getSelectedStringsList() const override;

  std::vector<std::string> getUnselectedStringsList() const override;

  void selectAllStrings() override;

  void unselectAllStrings() override;

private slots:

  void pressButtonSelectAll();
  void pressButtonUnselectAll();
  void listItemClicked(QListWidgetItem *item);
  void pressButtonUp();
  void pressButtonDown();

private:
  void qtWidgetsConnection();

  unsigned int maxSelectedStringsListSize;
};
} // namespace tlp

#endif /* SIMPLESTRINGSLISTSELECTIONWIDGET_H_ */
///@endcond
