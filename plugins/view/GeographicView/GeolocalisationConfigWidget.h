/**
 *
 * Copyright (C) 2019-2020  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#ifndef GEOLOCALISATION_CONFIG_WIDGET_H
#define GEOLOCALISATION_CONFIG_WIDGET_H

#include <QWidget>

namespace Ui {
class GeolocalisationConfigWidget;
}

namespace tlp {

class Graph;

class GeolocalisationConfigWidget : public QWidget {

  Q_OBJECT
  Ui::GeolocalisationConfigWidget *_ui;

public:
  GeolocalisationConfigWidget(QWidget *parent = nullptr);
  ~GeolocalisationConfigWidget() override;

  void setGraph(Graph *graph);

  void setLatLngGeoLocMethod(const std::string &latitudePropertyName,
                             const std::string &longitudePropertyName);

  void setEdgesPathsPropertyName(const std::string &edgesPathsPropertyName);

  bool geolocateByAddress() const;

  bool createLatAndLngProperties() const;

  bool resetLatAndLngValues() const;

  std::string getAddressGraphPropertyName() const;

  std::string getLatitudeGraphPropertyName() const;

  std::string getLongitudeGraphPropertyName() const;

  bool useEdgesPaths() const;

  std::string getEdgesPathsPropertyName() const;

public slots:

  void enableDisableComboBoxes();

signals:
  void computeGeoLayout();
};
}

#endif // GEOLOCALISATION_CONFIG_WIDGET_H
