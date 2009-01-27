/*
 * This file is part of TelepathyQt4
 *
 * Copyright (C) 2008 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright (C) 2008 Nokia Corporation
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _TelepathyQt4_cli_connection_internal_h_HEADER_GUARD_
#define _TelepathyQt4_cli_connection_internal_h_HEADER_GUARD_

#include <TelepathyQt4/Client/Connection>

#include <TelepathyQt4/Client/PendingOperation>

namespace Telepathy
{
namespace Client
{

class Connection::PendingConnect : public PendingOperation
{
    Q_OBJECT

public:
    PendingConnect(Connection *parent, Connection::Features features);

private Q_SLOTS:
    void onConnectReply(QDBusPendingCallWatcher *);
    void onBecomeReadyReply(Telepathy::Client::PendingOperation *);

private:
    Connection::Features features;
};

} // Telepathy::Client
} // Telepathy

#endif