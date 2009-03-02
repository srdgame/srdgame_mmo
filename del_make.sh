find . -name 'Makefile' -exec rm -f {} \;
find . -name 'Makefile.in' -exec rm -f {} \;
find . -name '*.o' -exec rm -f {} \;
find . -name '*~' -ok rm -f {} \

