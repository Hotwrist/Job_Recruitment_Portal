# Odey John Ebinyi aka Hotwrist
# This bash script compiles the confirm_registration.cpp in this folder to confirm.cgi
g++ -Wall -I/usr/include/cppconn  -I/usr/local/include/Poco -o confirm.cgi confirm_registration.cpp -L/usr/lib -lmysqlcppconn -lPocoNet -lcrypto -lcgicc -L/home/hot-wrist/Downloads/chilkat-9.5.0-x86_64-linux-gcc/lib -lchilkat-9.5.0 -lresolv -lpthread
