/****************************************************************************************
** Copyright (c) 2021 Open Mobile Platform LLC.
** Copyright (c) 2023 Jolla Ltd.
**
** All rights reserved.
**
** This file is part of Sailfish Transfer Engine component package.
**
** You may use this file under the terms of BSD license as follows:
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
** 1. Redistributions of source code must retain the above copyright notice, this
**    list of conditions and the following disclaimer.
**
** 2. Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with the distribution.
**
** 3. Neither the name of the copyright holder nor the names of its
**    contributors may be used to endorse or promote products derived from
**    this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
** FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
****************************************************************************************/

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
