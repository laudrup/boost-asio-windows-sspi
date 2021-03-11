.. figure:: logo.jpg
   :alt: Boost.Wintls logo

Overview
========

This library implements TLS stream functionality using native Windows `SSPI/Schannel`_ implementation.

Motivation
----------
Avoid dependency on OpenSSL on Windows as well as being able to use
certificates and keys from the Windows certificate stores directly.

Examples
========

Full code to the examples can be found in the `examples`_ directory.

HTTPS Client
------------
This example demonstrates a basic synchronous HTTPS client using
boost::beast.

.. literalinclude:: ../examples/https_client.cpp
   :lines: 9-

Classes
=======

context
-------
.. doxygenclass:: boost::wintls::context
   :members:

stream
------
.. doxygenclass:: boost::wintls::stream
   :members:

Enumerations
============

handshake_type
--------------
.. doxygenenum:: boost::wintls::handshake_type

method
------
.. doxygenenum:: boost::wintls::method

Functions
=========

x509_to_cert_context
--------------------
.. doxygenfunction:: boost::wintls::x509_to_cert_context(const net::const_buffer &x509, file_format format)
.. doxygenfunction:: boost::wintls::x509_to_cert_context(const net::const_buffer &x509, file_format format, boost::system::error_code& ec)

Type aliases
============

cert_context_ptr
----------------
.. doxygentypedef:: boost::wintls::cert_context_ptr

.. _SSPI/Schannel: https://docs.microsoft.com/en-us/windows-server/security/tls/tls-ssl-schannel-ssp-overview/
.. _examples: https://github.com/laudrup/boost-wintls/tree/master/examples
.. _CERT_CONTEXT: https://docs.microsoft.com/en-us/windows/win32/api/wincrypt/ns-wincrypt-cert_context
