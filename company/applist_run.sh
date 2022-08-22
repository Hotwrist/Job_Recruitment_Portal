# Odey John Ebinyi aka Hotwrist
# This bash script compiles the applicant_list.cpp in this folder to applicant_list.cgi
g++ -Wall -I/usr/include/cppconn  -o applicant_list.cgi applicant_list.cpp -lcrypto -L/usr/lib -lmysqlcppconn  -lcgicc 
