.. title:: clang-tidy - misc-structured-namespace

misc-structured-namespace
=========================

Checks whether the components (model, view, persistence) belong to
the correct corresponding namespace structure, if any namespaces exist.

For each component, 1 matcher is registered. It binds C++ record declarations
of class definitions where the name of the class matches the components'
regular expressions, except if name starts with ``Ui_``.

The different variants of the component names are checked individually,
in every case the matched nodes are processed further. For each of them
the full qualified name contains all the namespaces of the class.
Splitting them by the ``::`` separators enables comparing all the
namespace levels to current variant. If a namespace contains the variant,
it is reported to the diagnostics with the corresponding explanation.

.. code-block:: c++
  namespace view {
	class Model {};
  }

