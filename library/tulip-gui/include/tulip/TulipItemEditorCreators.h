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

#ifndef TULIPITEMEDITORCREATORS_H
#define TULIPITEMEDITORCREATORS_H

#include <tulip/tulipconf.h>
#include <tulip/PropertyTypes.h>
#include <tulip/TlpQtTools.h>
#include <tulip/TulipViewSettings.h>

#include <QVariant>
#include <QSize>

class QWidget;
class QPainter;
class QStyleOptionViewItem;
class QModelIndex;

namespace tlp {

class PropertyInterface;

class TLP_QT_SCOPE TulipItemEditorCreator {
public:
  virtual ~TulipItemEditorCreator() {}
  virtual QWidget *createWidget(QWidget *) const = 0;
  virtual bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
                     const QModelIndex &) const;
  virtual QString displayText(const QVariant &) const {
    return "";
  }

  virtual QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const;

  virtual void setEditorData(QWidget *, const QVariant &, bool isMandatory,
                             tlp::Graph *g = nullptr) = 0;
  virtual QVariant editorData(QWidget *, tlp::Graph *g = nullptr) = 0;
  virtual void setPropertyToEdit(tlp::PropertyInterface *) {}
};

template <typename T>
class TLP_QT_SCOPE StringDisplayEditorCreator : public TulipItemEditorCreator {
public:
  inline QString displayText(const QVariant &v) const override {
    return tlpStringToQString(T::toString(v.value<typename T::RealType>()));
  }
};

class TLP_QT_SCOPE ColorEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
};

template <typename T>
class NumberEditorCreator : public StringDisplayEditorCreator<T> {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
};

template <typename T>
class LineEditEditorCreator : public StringDisplayEditorCreator<T> {
public:
  QWidget *createWidget(QWidget *) const;
  virtual void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *);
  virtual QVariant editorData(QWidget *, tlp::Graph *);
};

template <typename T>
class MultiLinesEditEditorCreator : public StringDisplayEditorCreator<T> {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &) const override;
};

class TLP_QT_SCOPE BooleanEditorCreator : public TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE CoordEditorCreator : public StringDisplayEditorCreator<tlp::PointType> {
  bool editSize;

public:
  CoordEditorCreator() : editSize(false) {}
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  void setPropertyToEdit(tlp::PropertyInterface *prop) override;
};

template <typename PROP>
class PropertyEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE PropertyInterfaceEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE NumericPropertyEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE ColorScaleEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
};

class TLP_QT_SCOPE StringCollectionEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE TulipFileDescriptorEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &) const override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
};

class TLP_QT_SCOPE TextureFileEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
};

class TLP_QT_SCOPE TulipFontIconCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &) const override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE NodeShapeEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
};

class TLP_QT_SCOPE EdgeExtremityShapeEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
  bool paint(QPainter *, const QStyleOptionViewItem &, const QVariant &,
             const QModelIndex &index) const override;
  QSize sizeHint(const QStyleOptionViewItem &, const QModelIndex &) const override;
};

class TLP_QT_SCOPE EdgeShapeEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

template <typename ElementType>
class VectorEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *editor, const QVariant &data, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *editor, tlp::Graph *) override;
  QString displayText(const QVariant &data) const override;
};

class QVectorBoolEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *editor, const QVariant &data, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *editor, tlp::Graph *) override;
  QString displayText(const QVariant &data) const override;
};

struct TLP_QT_SCOPE TulipFontEditorCreator : public tlp::TulipItemEditorCreator {
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *, const QVariant &, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE TulipLabelPositionEditorCreator : public tlp::TulipItemEditorCreator {

public:
  QWidget *createWidget(QWidget *) const override;
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE GraphEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE EdgeSetEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE QStringEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
  void setPropertyToEdit(tlp::PropertyInterface *prop) override;

protected:
  std::string propName;
};

class TLP_QT_SCOPE StdStringEditorCreator : public QStringEditorCreator {
public:
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

class TLP_QT_SCOPE QStringListEditorCreator : public tlp::TulipItemEditorCreator {
public:
  QWidget *createWidget(QWidget *parent) const override;
  void setEditorData(QWidget *w, const QVariant &var, bool, tlp::Graph *) override;
  QVariant editorData(QWidget *, tlp::Graph *) override;
  QString displayText(const QVariant &) const override;
};

TLP_QT_SCOPE void addIconToPool(const QString &iconName, const QIcon &icon);
} // namespace tlp

#if defined(__clang__)
template class tlp::StringDisplayEditorCreator<tlp::PointType>;
template class tlp::StringDisplayEditorCreator<tlp::SizeType>;
#endif

inline QString &truncateText(QString &text, const QString &trailingChars = " ...",
                             int maxChars = 45) {
  if (text.size() > maxChars) {
    text.truncate(maxChars - trailingChars.length());
    text.append(trailingChars);
  }

  return text;
}

#include "cxx/TulipItemEditorCreators.cxx"

#endif // TULIPITEMEDITORCREATORS_H
///@endcond
