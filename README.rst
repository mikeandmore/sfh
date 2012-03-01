Structured Fault Handling For Linux
===================================

This library is a similar to SEH (Structured Exception Handling) under Windows.  It's implemented using UNIX signal, therefore provide UNIX system capabilities to handle faults (such as SIGSEGV) using a try/catch manner.

Limitations
-----------

C and C++ backends are however separated.  C++ backends was using standard try/catch (which is more modern) while C backends is using setjmp/longjmp.  This brings incapability.  Therefore, you have to make sure:

1. Initialized the correct backends.  Using ``sfh_init_sjlj()`` for C and ``sfh_init_cxx_throw()`` for C++.
2. try/catch is identical to the backend on initialization.  If initialized using ``sfh_init_sjlj()`` you can only try/catch in a C source, otherwise you can only try/catch (both ``__sfh_try`` / ``__sfh_cath`` and ``try`` / ``catch``) in a C++ source.
