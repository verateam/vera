if (x) foo();     // bad style
if (x) { foo(); } // OK

if (x)
    foo();        // again bad style

if (x)
{                 // OK
    foo();
}

if (x)
{                 // OK
    foo();
}
else
    foo();        // bad style

if (x)
{                 // OK
    foo();
}
else
{
    foo();        // OK
}

if (x)
{                 // OK
    foo();
}
else if (x)
    foo();        // bad style

if (x) {   // OK
    foo();
} else if (x) { foo(); }        // OK
	

if (x)
    while (y)     // bad style
        foo();    // bad style

if (x)
{                 // OK
    while (y)
    {             // OK
        foo();
    }
}

for (int i = 0; i = 10; ++i);  // oops!
    cout << "Hello\n";

for (int i = 0; i = 10; ++i)   // OK
{
    cout << "Hello\n";
}

do
    cout << "Hello\n";
while (int i = 0; i = 10; ++i);   // not OK

do
{
    cout << "Hello\n";
}
while (int i = 0; i = 10; ++i);   // OK
