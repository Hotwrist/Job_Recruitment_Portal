# Odey John Ebinyi aka Hotwrist
# This bash script compiles the approved_list.cpp in this folder to list.cgi
g++ -Wall -I/usr/include/cppconn  -o list.cgi approved_list.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc 
