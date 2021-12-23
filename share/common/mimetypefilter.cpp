/*
 * Copyright (c) 2021 Open Mobile Platform LLC.
 *
 * License: Proprietary.
 */

#include "mimetypefilter.h"

namespace {

const auto Wildcard = QStringLiteral("*");

}

// Checks for wildcard
bool MimeTypeFilter::allAllowed(const QStringList &capabilities)
{
    return capabilities.isEmpty() || capabilities.contains(Wildcard);
}

// Tests that filter is one of the following formats:
// "A/B"
// "A/*"
// "*"
bool MimeTypeFilter::validMimeTypeFilter(const QString &filter)
{
    if (filter == Wildcard) {
        return true;
    } else {
        const int slashIndex = filter.indexOf('/');
        if (slashIndex > 0) {
            // '/' is at least second character -> "A/?"
            const int starIndex = filter.indexOf('*');
            if (starIndex < 0) {
                // '*' does not exist, so there must be something after '/' -> "A/B"
                return slashIndex < filter.length()-1;
            } else {
                // '*' exists, so it must be after '/' and it must be the last character -> "A/*"
                return starIndex == slashIndex + 1 && starIndex == filter.length()-1;
            }
        }
    }
    return false;
}

// Check that mimeType is allowed by filter.
//
// Technically both of them are "filters" and must be checked first with
// validMimeTypeFilter(). Also check first if everything is accepted with
// allAccepted(). This assumes "A/B" and "A/*" type filters.
bool MimeTypeFilter::filterAcceptsMimeType(const QString &filter, const QString &mimeType)
{
    if (filter == mimeType)
        return true;
    if (filter.endsWith('*') && filter.left(filter.indexOf('/')) == mimeType.left(filter.indexOf('/')))
        return true;
    return false;
}
