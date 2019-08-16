# Custom Template Library (CTL)

The Custom Template Librar (CTL) is a collecton of C++ utility functions
and classes that follows three basic principles.

1. Non compiled
2. Templated
3. Usable with *factory functions*

## Bioinformatics Template Library (BTL)

The *Bioinformatics Template Library (BTL)* is a collection of (templated)
functions and classes to deal with bioinformatics tasks like: loading and
writing files, generating sequences and reads, ...

Although not strictly part of the CTL library, the BTL is included for
convenience. Due to templating mechanism, if none of the BTL functionality
is used, no code is generating during compilation. Moreover, the BTL
files: ``btl.h`` and files in ``btl/`` directory can be removed when only
using CTL (vice-versa is not true since BTL relies on facilities in CTL).
