Overview
========

This library implements TLS stream functionality using native Windows `SSPI/Schannel`_ implementation.

Motivation
----------
Avoid dependency on OpenSSL on Windows as well as being able to use
certificates and keys from the Windows certificate stores directly.


Contents
--------

.. toctree::
   :maxdepth: 2

   examples
   classes
   enumerations
   functions
   type_aliases


.. _SSPI/Schannel: https://docs.microsoft.com/en-us/windows-server/security/tls/tls-ssl-schannel-ssp-overview/
