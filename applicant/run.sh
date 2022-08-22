# Odey John Ebinyi aka Hotwrist
# This bash script compiles the index.cpp in this folder to index.cgi
g++ -Wall -I/usr/include/cppconn -o index.cgi index.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc 
