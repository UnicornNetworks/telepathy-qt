/**
 * This file is part of TelepathyQt
 *
 * Copyright © 2009-2012 Collabora Ltd. <http://www.collabora.co.uk/>
 * Copyright © 2009 Nokia Corporation
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

#include <TelepathyQt/Farstream/Channel>

#include "TelepathyQt/Farstream/_gen/channel.moc.hpp"

#include "TelepathyQt/debug-internal.h"

#include <TelepathyQt/CallChannel>
#include <TelepathyQt/Connection>

#include <telepathy-farstream/telepathy-farstream.h>
#include <telepathy-glib/channel.h>
#include <telepathy-glib/connection.h>
#include <telepathy-glib/dbus.h>

namespace Tp {
namespace Farstream {

struct TP_QT_FS_NO_EXPORT PendingChannel::Private
{
    Private()
        : mTfChannel(0)
    {
    }

    static void onTfChannelNewFinish(GObject *sourceObject, GAsyncResult *res, gpointer userData);

    TfChannel *mTfChannel;
};

PendingChannel::PendingChannel(const CallChannelPtr &channel)
    : Tp::PendingOperation(channel),
      mPriv(new PendingChannel::Private)
{
    if (!channel->handlerStreamingRequired()) {
        setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE,
                QLatin1String("Handler streaming not required"));
        return;
    }

    TpDBusDaemon *dbus = tp_dbus_daemon_dup(0);
    if (!dbus) {
        setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE,
                QLatin1String("Unable to connect to D-Bus"));
        return;
    }

    Tp::ConnectionPtr connection = channel->connection();
    if (connection.isNull()) {
        setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE,
                QLatin1String("Connection not available"));
        g_object_unref(dbus);
        return;
    }

    TpConnection *gconnection = tp_connection_new(dbus,
            connection->busName().toAscii(),
            connection->objectPath().toAscii(),
            0);
    g_object_unref(dbus);
    dbus = 0;
    if (!gconnection) {
        setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE,
                QLatin1String("Unable to construct TpConnection"));
        return;
    }

    TpChannel *gchannel = tp_channel_new(gconnection,
            channel->objectPath().toAscii(),
            TP_QT_IFACE_CHANNEL_TYPE_CALL.latin1(),
            (TpHandleType) channel->targetHandleType(),
            channel->targetHandle(),
            0);
    g_object_unref(gconnection);
    gconnection = 0;
    if (!gchannel) {
        setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE,
                QLatin1String("Unable to construct TpChannel"));
        return;
    }

    tf_channel_new_async(gchannel, PendingChannel::Private::onTfChannelNewFinish, this);
    g_object_unref(gchannel);
}

PendingChannel::~PendingChannel()
{
    delete mPriv;
}

void PendingChannel::Private::onTfChannelNewFinish(GObject *sourceObject,
        GAsyncResult *res, gpointer userData)
{
    PendingChannel *self = reinterpret_cast<PendingChannel *>(userData);

    GError *error = NULL;
    TfChannel *ret = tf_channel_new_finish(sourceObject, res, &error);
    if (error) {
        debug() << "Fs::PendingChannel::Private::onTfChannelNewFinish: error " << error->message;
        self->setFinishedWithError(TP_QT_ERROR_NOT_AVAILABLE, QLatin1String(error->message));
        g_clear_error(&error);
        return;
    }

    self->mPriv->mTfChannel = ret;
    self->setFinished();
}

TfChannel *PendingChannel::tfChannel() const
{
    return mPriv->mTfChannel;
}

CallChannelPtr PendingChannel::callChannel() const
{
    return CallChannelPtr::staticCast(object());
}

PendingChannel *createChannel(const CallChannelPtr &channel)
{
    PendingChannel *ptf = new PendingChannel(channel);
    return ptf;
}

} // Farstream
} // Tp
