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

using namespace std;
using namespace cgicc;


/*  The register_page Function
    This prints the HTML code of the registration index page and it takes no parameter.
*/
void register_page()
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "../css/main.css")) << endl;
		cout << body();
		cout << cgicc::div().set("class", "login-box") << endl;
		cout <<  h1("Register");
		cout << form().set("method", "post").set("action", "registration.cgi") << endl;
		cout << input().set("type", "text").set("name", "username").set("placeholder", "username").set("value", "").set("autocomplete", "off").set("required") << endl;
		cout << "<br>" << endl;
		cout << "<br>" << endl;
		
		cout << input().set("type", "text").set("name", "password").set("placeholder", "password").set("value", "").set("autocomplete", "off").set("required") << endl;
		cout << "<br>" << endl;
		cout << "<br>" << endl;
		cout << input().set("type", "email").set("name", "email").set("placeholder", "e-mail").set("value", "").set("autocomplete", "off") << endl;
		cout << "<br>" << endl;
		cout << "<br>" << endl;
		cout << input().set("type", "text").set("name", "fullname").set("placeholder", "full name").set("value", "").set("autocomplete", "off").set("required") << endl;
		cout << "<br>" << endl;
		cout << "<br>" << endl;
		cout << input().set("type", "text").set("name", "skill").set("placeholder", "skill").set("value", "").set("autocomplete", "off") << endl;
		
		cout << input().set("type", "submit").set("value", "Register").set("name", "").set("class", "btn") << endl;
		cout << "<br>" << endl;
		cout << form() <<endl;
		cout << cgicc::div() << endl;
		cout << body() << endl;
		cout << html() << endl;
	}
	catch (exception &e)
	{
		cout << "Error encountered. Try reloading the page.." << endl;
	}
}

/*  The Main Function
    It calls the function register_page.

*/
int main() 
{
	register_page();

   	return 0;
}
