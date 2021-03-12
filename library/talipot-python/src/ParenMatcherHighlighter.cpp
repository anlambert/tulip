/**
 *
 * Copyright (C) 2019-2021  The Talipot developers
 *
 * Talipot is a fork of Tulip, created by David Auber
 * and the Tulip development Team from LaBRI, University of Bordeaux
 *
 * See the AUTHORS file at the top-level directory of this distribution
 * License: GNU General Public License version 3, or any later version
 * See top-level LICENSE file for more information
 *
 */

#include <algorithm>

#include "talipot/ParenMatcherHighlighter.h"

ParenInfoTextBlockData::ParenInfoTextBlockData() = default;

QVector<ParenInfo> ParenInfoTextBlockData::parens() {
  return _parenInfo;
}

void ParenInfoTextBlockData::insert(const ParenInfo &parenInfo) {
  _parenInfo.append(parenInfo);
}

void ParenInfoTextBlockData::sortParenInfo() {
  std::sort(_parenInfo.begin(), _parenInfo.end());
}

ParenMatcherHighlighter::ParenMatcherHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent) {
  _leftParensToMatch.append('(');
  _leftParensToMatch.append('[');
  _leftParensToMatch.append('{');
  _rightParensToMatch.append(')');
  _rightParensToMatch.append(']');
  _rightParensToMatch.append('}');
}

void ParenMatcherHighlighter::highlightBlock(const QString &text) {
  ParenInfoTextBlockData *data = new ParenInfoTextBlockData;

  QString modifiedText = text;
  QRegExp dblQuotesRegexp("\"[^\"]*\"");
  QRegExp simpleQuotesRegexp("'[^']*'");

  int pos = dblQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos; i < pos + dblQuotesRegexp.matchedLength(); ++i) {
      modifiedText[i] = ' ';
    }

    pos = dblQuotesRegexp.indexIn(modifiedText, pos + dblQuotesRegexp.matchedLength());
  }

  pos = simpleQuotesRegexp.indexIn(modifiedText);

  while (pos != -1) {
    for (int i = pos; i < pos + simpleQuotesRegexp.matchedLength(); ++i) {
      modifiedText[i] = ' ';
    }

    pos = simpleQuotesRegexp.indexIn(modifiedText, pos + simpleQuotesRegexp.matchedLength());
  }

  for (int i = 0; i < _leftParensToMatch.size(); ++i) {
    int leftPos = modifiedText.indexOf(_leftParensToMatch.at(i));

    while (leftPos != -1) {
      ParenInfo info;
      info.character = _leftParensToMatch.at(i);
      info.position = currentBlock().position() + leftPos;
      data->insert(info);
      leftPos = modifiedText.indexOf(_leftParensToMatch.at(i), leftPos + 1);
    }
  }

  for (int i = 0; i < _rightParensToMatch.size(); ++i) {
    int rightPos = modifiedText.indexOf(_rightParensToMatch.at(i));

    while (rightPos != -1) {
      ParenInfo info;
      info.character = _rightParensToMatch.at(i);
      info.position = currentBlock().position() + rightPos;
      data->insert(info);
      rightPos = modifiedText.indexOf(_rightParensToMatch.at(i), rightPos + 1);
    }
  }

  data->sortParenInfo();
  setCurrentBlockUserData(data);
}
