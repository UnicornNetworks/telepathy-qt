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

#ifndef _TelepathyQt4_cli_connection_manager_h_HEADER_GUARD_
#define _TelepathyQt4_cli_connection_manager_h_HEADER_GUARD_

/**
 * \addtogroup clientsideproxies Client-side proxies
 *
 * Proxy objects representing remote service objects accessed via D-Bus.
 *
 * In addition to providing direct access to methods, signals and properties
 * exported by the remote objects, some of these proxies offer features like
 * automatic inspection of remote object capabilities, property tracking,
 * backwards compatibility helpers for older services and other utilities.
 */

/**
 * \defgroup clientcm Connection manager proxies
 * \ingroup clientsideproxies
 *
 * Proxy objects representing remote Telepathy ConnectionManager objects.
 */

#include <TelepathyQt4/_gen/cli-connection-manager.h>

#include <TelepathyQt4/Client/DBus>
#include <TelepathyQt4/Client/DBusProxy>
#include <TelepathyQt4/Client/OptionalInterfaceFactory>

namespace Telepathy
{
namespace Client
{


class ProtocolInfo
{
public:
    ~ProtocolInfo();

    /**
     * Get the short name of the connection manager (e.g. "gabble").
     *
     * \return The name of the connection manager
     */
    QString cmName() const;

    /**
     * Get the untranslated name of the protocol as described in the Telepathy
     * D-Bus API Specification (e.g. "jabber").
     */
    QString protocolName() const;

    /**
     * Return the names of all supported parameters. The parameters' names
     * may either be the well-known strings specified by the Telepathy D-Bus
     * API Specification (e.g. "account" and "password"), or
     * implementation-specific strings.
     *
     * \return A list of parameter names
     */
    QStringList parameters() const;

    /**
     * Return whether a given parameter can be passed to the connection
     * manager when creating a connection to this protocol.
     *
     * \param param The name of a parameter
     * \return true if the given parameter exists
     */
    bool hasParameter(const QString& param) const;

    /**
     * Return the D-Bus signature of the given parameter. Commonly used
     * signatures include "s" (string) and "u" (32-bit unsigned integer).
     *
     * \return A D-Bus signature
     */
    QDBusSignature parameterDBusSignature(const QString& param) const;

    /**
     * Return the QVariant::Type corresponding to the D-Bus signature of
     * the given parameter, if a suitable type is known. Otherwise return
     * QVariant::Invalid.
     *
     * \param param The name of a parameter
     * \return A QVariant type, or QVariant::Invalid
     */
    QVariant::Type parameterType(const QString& param) const;

    /**
     * Return whether the given parameter is required.
     *
     * The set of required parameters can be different depending on the value
     * of the special parameter <code>register</code>, which indicates
     * whether we are registering for a new account, or connecting to an
     * existing account. Normally, <code>register</code> is false.
     *
     * \param param The name of a parameter
     * \param registering The value that will be given to the special
     *                    parameter <code>register</code>
     * \return true if this parameter must always be given
     */
    bool parameterIsRequired(const QString& param,
            bool registering = false) const;

    /**
     * Return whether this parameter is marked as "secret". Secret parameters
     * (such as passwords) should be stored in an appropriate password store
     * (such as KWallet) rather than in normal configuration files, and should
     * not appear in debugging logs.
     *
     * \param param The name of a parameter
     * \return true if this parameter is a password, key or other secret
     */
    bool parameterIsSecret(const QString& param) const;

    /**
     * Return whether this parameter corresponds to a D-Bus property with
     * the same name. These parameters should be handled specially by the
     * AccountManager.
     *
     * \param param The name of a parameter
     * \return true if this parameter is a D-Bus property
     */
    bool parameterIsDBusProperty(const QString& param) const;

    /**
     * Return whether this parameter has a default value.
     *
     * \param param The name of a parameter
     * \return true if omitting this parameter when creating a connection
     *         is equivalent to using getParameterDefault()
     */
    bool parameterHasDefault(const QString& param) const;

    /**
     * If parameterHasDefault() returns true for the given parameter, return
     * the default value of the parameter. Otherwise, return an invalid
     * variant.
     *
     * \param param The name of a parameter
     * \return The default value of the given parameter, if any
     */
    QVariant getParameterDefault(const QString& param) const;

    /**
     * Return whether it might be possible to register new accounts on this
     * protocol via Telepathy, by setting the special parameter named
     * <code>register</code> to <code>true</code>.
     *
     * \return The same thing as hasParameter("register")
     */
    bool canRegister() const;

private:
    ProtocolInfo(const QString& cmName, const QString& protocol);

    struct Private;
    friend struct Private;
    friend class ConnectionManager;
    Private *mPriv;
};


/**
 * \class ConnectionManager
 * \ingroup clientcm
 * \headerfile TelepathyQt4/cli-connection-manager.h <TelepathyQt4/Client/ConnectionManager>
 *
 * Object representing a Telepathy connection manager. Connection managers
 * allow connections to be made on one or more protocols.
 *
 * Most client applications should use this functionality via the
 * %AccountManager, to allow connections to be shared between client
 * applications.
 */
class ConnectionManager : public StatelessDBusProxy,
        private OptionalInterfaceFactory
{
    Q_OBJECT

public:
    ConnectionManager(const QString& name, QObject* parent = 0);
    ConnectionManager(const QDBusConnection& bus,
            const QString& name, QObject* parent = 0);

    virtual ~ConnectionManager();

    QString cmName() const;

    QStringList interfaces() const;

    QStringList supportedProtocols() const;

    const ProtocolInfo* protocolInfo(const QString& protocol) const;

    /**
     * Convenience function for getting a Properties interface proxy. The
     * Properties interface is not necessarily reported by the services, so a
     * <code>check</code> parameter is not provided, and the interface is
     * always assumed to be present.
     */
    inline DBus::PropertiesInterface* propertiesInterface() const
    {
        return OptionalInterfaceFactory::interface<DBus::PropertiesInterface>(
                *baseInterface());
    }

    bool isReady() const;

Q_SIGNALS:
    void ready(ConnectionManager*);

protected:
    /**
     * Get the ConnectionManagerInterface for this ConnectionManager. This
     * method is protected since the convenience methods provided by this
     * class should generally be used instead of calling D-Bus methods
     * directly.
     *
     * \return A pointer to the existing ConnectionManagerInterface for this
     *         ConnectionManager
     */
    ConnectionManagerInterface* baseInterface() const;

private Q_SLOTS:
    void onGetParametersReturn(QDBusPendingCallWatcher*);
    void onListProtocolsReturn(QDBusPendingCallWatcher*);
    void onGetAllConnectionManagerReturn(QDBusPendingCallWatcher*);
    void continueIntrospection();

private:
    struct Private;
    friend struct Private;
    Private *mPriv;
};


}
}

#endif
