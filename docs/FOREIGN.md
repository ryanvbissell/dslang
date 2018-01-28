
#### keywords vs foreign-functions

Keyword handlers are just pointers to free functions, and the only
context they receive is a SexprENV.  Keywords should be your first
choice.  Add them via 'addKeyword()' in your derived Scheme class.
'addKeyword()' will also let you override an existing keyword handler.

Foreign-functions additionally receive a 'this' pointer.  Use these
only when the handlers need access to context that is outside of
the provided SexprENV.


