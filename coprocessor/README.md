# Coprocessor - a CNF preprocessor

For the current version of the tool, please visit the web page
<http://tools.computational-logic.org>
Please send bug reports to norbert.manthey@tu-dresden.de

## Development

In this directory, the new version of the preprocessor Coprocessor is
developped.

The class is a friend class of the class Minisat, so that it has access to all
its structures.

The data that is used to handle all clauses, literal counts, ... is implemented
in the file CoprocessorTypes.h. The corresponding object should be passed to
preprocessing methods, so that they have full access to all members of the
structure.

Simplification methods, that should be integrated into Coprocessor shoud be
implemented into an extra class (e.g. subsumption, variable elimination, ... ).
Since the data about the formula is handled in an extra object, it could be
passed to each technique separately without the single techniques knowing each
other. Each technique implementation shoud decide by itself which objects should
be stored permamnently (working queues), and which elements should be created
only for performing the technique ones. Run time statistics should be
implemented into the class of the specific technique.

Because the techniques require very heterogenous calls, there is no nice global
interface. Every technique has to take care about the correct oder of method
calls.

## Class descriptions

**Coprocessor**:
Major preprocessor class, represents the interface between preprocessor and
solver class

**CoprocessorData**:
Represents all clauses and related data for all preprocessing techniques Should
provide methods for accessing this data, adding/deleting clauses from occurrence
lists, updating counters, ...

**Technique**:
Super class for any class implementation of techniques. This class already
copies the clause allocator and gives some more methods that are necessary for a
nice interaction with the main preprocessor class. The method Technique::reset()
should be overwritten by all classes the inherit from Technique
