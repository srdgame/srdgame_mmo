aclocal
autoconf
autoheader
automake --add-missing
./configure
make
cp ./src/sample/.libs/libsample.so ./src/world/
cp ./src/sample/.libs/libsample.so ./src/realm/
cp ./src/sample/.libs/libsample.so ./src/login/
