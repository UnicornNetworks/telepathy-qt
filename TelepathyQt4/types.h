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

#ifndef _TelepathyQt4_types_h_HEADER_GUARD_
#define _TelepathyQt4_types_h_HEADER_GUARD_

#ifndef IN_TELEPATHY_QT4_HEADER
#error IN_TELEPATHY_QT4_HEADER
#endif

#include <TelepathyQt4/_gen/types.h>

#include <TelepathyQt4/SharedPtr>

namespace Tp
{

class AbstractClientHandler;
class Account;
class AccountManager;
class Channel;
class ChannelRequest;
class ClientObject;
class ClientRegistrar;
class Connection;
class ConnectionManager;
class FileTransfer;
class RoomList;
class MediaStream;
class StreamedMediaChannel;
class TextChannel;

typedef SharedPtr<AbstractClientHandler> AbstractClientPtr;
typedef SharedPtr<Account> AccountPtr;
typedef SharedPtr<AccountManager> AccountManagerPtr;
typedef SharedPtr<Channel> ChannelPtr;
typedef SharedPtr<ChannelRequest> ChannelRequestPtr;
typedef SharedPtr<ClientObject> ClientObjectPtr;
typedef SharedPtr<ClientRegistrar> ClientRegistrarPtr;
typedef SharedPtr<Connection> ConnectionPtr;
typedef SharedPtr<ConnectionManager> ConnectionManagerPtr;
typedef SharedPtr<FileTransfer> FileTransferPtr;
typedef SharedPtr<RoomList> RoomListPtr;
typedef SharedPtr<MediaStream> MediaStreamPtr;
typedef SharedPtr<StreamedMediaChannel> StreamedMediaChannelPtr;
typedef SharedPtr<TextChannel> TextChannelPtr;

} // Tp

#endif
