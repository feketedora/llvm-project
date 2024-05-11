.. title:: clang-tidy - misc-illegal-layer-access

misc-illegal-layer-access
=========================

Checks the correct containment of the components: view contains model, model contains persistence.

All the other containments are considered illegal, either because
view component omits the model layer to access the persistence component directly,
or the inclusion is reversed, resulting in a structural logic error.

Each illegal access type consists of 2 components, where from one (container)
to the other (contained) the access is not allowed.
For each of these types 2 matchers are registered.
In both cases the container and the contained class also match derived classes, too.

The first binds fields in classes, where the classes are of the container type,
and the fields are of the contained type, including references and pointers.

.. code-block:: c++
  class View {
	Persistence * persistence;
  };

The second matcher binds variables in C++ class methods (including function parameters),
where the class is of the container type and the variables are of the
contained type, including references and pointers.

.. code-block:: c++
  class Persistence {
	void fun (Model * M);
  };

All matched nodes are reported to the diagnostics with the corresponding explanation.
