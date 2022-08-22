# Odey John Ebinyi aka Hotwrist
# This bash script compiles the company_list.cpp in this folder to company_list.cgi
g++ -Wall -I/usr/include/cppconn  -o company_list.cgi company_list.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc 
