/* Copyright(c) 2022 Odey John Ebinyi a.k.a Hotwrist
*  All Rights Reserved
*/

#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

using namespace cgicc;
using namespace std;

/* The start_page Function.
   It prints the HTML code of the login index page and it takes no parametres.
*/
void start_page() 
{
    try 
    {
        cout << HTTPHTMLHeader() << endl;
        cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "css/main.css")) << endl;
        cout << body();
        cout << cgicc::div().set("class", "login-register-box") << endl;
        cout << h1("Job Recruitment");
        cout << form().set("method", "post").set("action", "authenticator/") << endl;
        cout << input().set("type", "text").set("name", "username").set("placeholder", "username").set("value", "").set("autocomplete", "off") << endl;
        cout << "<br>" << endl;
        cout << "<br>" << endl;
        cout << input().set("type", "password").set("name", "password").set("placeholder", "password").set("value", "").set("autocomplete", "off") << endl;
        cout << "<br>" << endl;
            	
        cout << input().set("type", "submit").set("value", "Login").set("class", "btn") << endl;
        cout << "<br>" << endl;
        cout << form() << endl;
        cout << form().set("method", "post").set("action", "register/") << endl;
        cout << input().set("type", "submit").set("value", "Register").set("class", "btn") << endl;
        cout << form() << endl;
        cout << cgicc::div() << endl;
        cout << body() << endl;
        cout << html();
    }
    catch (exception& e) 
    {
        cout << "This should not have happend. Please try to reload the webpage." << endl; 
    }
}


/*  The Main Function
    It calls the function start_page to display the login index page.
*/
int main()
{
    start_page();
}

