# Odey John Ebinyi aka Hotwrist
# This bash script compiles the confirm_users.cpp in this folder to confirm_users.cgi
g++ -Wall -I/usr/include/cppconn  -o confirm_users.cgi confirm_users.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc 
