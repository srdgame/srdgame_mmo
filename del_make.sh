make clean
find . -name 'Makefile' -exec rm -f {} \;
find . -name 'Makefile.in' -exec rm -f {} \;
#find . -name '*.o' -exec rm -f {} \;
find . -name '*~' -ok rm -f {} \;
find . -name '*.dump' -ok rm -f {} \;
find . -type d -name '.deps' | xargs rm -fr \;

rm config.*
rm aclocal.m4
rm autom4te.cache -rf
rm COPYING
rm depcomp
rm INSTALL
rm install-sh
rm missing
rm stamp-h1
rm autoscan.log
rm configure
rm .anjuta -rf
rm .tm_project2.cache
rm TODO.tasks
rm libtool
rm ltmain.sh

