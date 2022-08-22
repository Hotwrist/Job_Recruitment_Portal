# Odey John Ebinyi aka Hotwrist
# This bash script compiles the registration.cpp in this folder to registration.cgi
g++ -Wall -I/usr/include/cppconn -o registration.cgi registration.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc
