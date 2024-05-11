.. title:: clang-tidy - misc-representation-leak

misc-representation-leak
===================

Checks whether a class leaks model, view or persistent component representation
via either a public field or function return type.

When such a component is exposed, it is possible that other components
take advantage of this, and directly use them instead of via the correct structural layers.

For each component, 2 matchers are registered. The first one matches
the C++ method declarations that are public, are not the ``operator=`` functions,
and the return type is either a pointer or a non-const reference to a object of
classes that match the model, view or persistence components' regular expression.

.. code-block:: c++
  class MyClass {
  public:
	Model publicModel;
  };

The second matcher filters the public fields of C++ classes,
except when the class name starts with ``Ui_``,
where the type of the field matches any of components' regular expression,
including references and pointers.

.. code-block:: c++
  class MyClass {
  public:
    Model & refFunc();
  };

All matched nodes are reported to the diagnostics with the corresponding explanation.
