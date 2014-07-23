// OK
catch (...)
{
#pragma omp parallel for
     for (int i = 0; i != 10; ++i)
     {
          if (foo(i))
          {
               while (getline(cin, line))
               {
                    switch (i % 3)
                    {
                    case 0:
                         bar(line);
                         break;
                    // ...
                    }
               }
          }
     }
}
// not OK
catch(...)
{
     for(int i = 0; i != 10; ++i)
     {
          if(foo(i))
          {
               while(getline(cin, line))
               {
                    switch (i % 3)
                    {
                    case 0:
                         bar(line);
                         break;
                    // ...
                    }
               }
          }
     }
}
