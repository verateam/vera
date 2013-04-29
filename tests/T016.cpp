x = max(y, z); // wrong, vulnerable to accidental macro substitution
x = (max)(y, z); // OK
x = max BOOST_PREVENT_MACRO_SUBSTITUTION (y, z); // OK
