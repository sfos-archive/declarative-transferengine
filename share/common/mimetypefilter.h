/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include <QList>
#include <QString>

#ifndef MIMETYPEFILTER_H
#define MIMETYPEFILTER_H

namespace MimeTypeFilter {

bool allAllowed(const QStringList &capabilities);
bool validMimeTypeFilter(const QString &filter);
bool filterAcceptsMimeType(const QString &filter, const QString &mimeType);

} // MimeTypeFilter

#endif // MIMETYPEFILTER_H
