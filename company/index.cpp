/* Copyright(c) 2022 Odey John Ebinyi a.k.a Hotwrist
*  All Rights Reserved
*/

#include <iostream>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <string>

// The MySQL server is hosted locally. To connect to it,
// it uses 127.0.0.1 and port 3306 
#define DB_HOST_URL "tcp://127.0.0.1:3306"

using namespace cgicc;
using namespace std;
using namespace sql;


/*The error Function
	Takes one parameter for the error message.
	It diplays the error message  alerting the user of the error.
*/
void error(string err_message)
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")) << endl;
        	cout << head() << endl;
        	cout << body();
        
       	 	cout << script() << endl;
        	cout << "alert('"+ err_message +"');" << endl;
        	cout << script() << endl;

        	cout << body() << endl;

       		cout << html();	
        }
        catch(exception &e)
        {
        	cout << "Try reloading the webpage." << endl; 
        }
       
        exit(EXIT_SUCCESS);
}

/* The search_applicants Function
	It takes one parametre: username of type string. This is displayed alongside the online notification dot.
	It provides a form where the company can enter its skill, to search for applicats having the required skill.
*/
void search_applicants(string username)
{
	try 
	{
        	cout << HTTPHTMLHeader() << endl;
        	cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "../css/main.css")) << endl;
        	
        	cout << body();
        	
        	cout << cgicc::div().set("class", "header-box") << endl;
        	cout << p("Username: "+username);
        	cout << cgicc::div() << endl;
        	
        	cout << cgicc::div().set("class", "online-dot") << endl;
        	cout << cgicc::div() << endl;
        	
        	cout << cgicc::div().set("class", "login-box") << endl;
       
        	cout << form().set("method", "post").set("action", "applicant_list.cgi") << endl;
        	cout << p("Search For Applicants") << endl;
        	cout << input().set("type", "text").set("name", "skill").set("placeholder", "Enter skill").set("autocomplete", "off").set("required") << endl;
        	cout << "<br>" << endl;
     
        	cout << input().set("type", "submit").set("value", "Search").set("class", "btn") << endl;
        	cout << "<br>" << endl;
        	cout << form() << endl;
        	cout << cgicc::div() << endl;

        	cout << body() << endl;
        	cout << html();
    	}
    	catch (exception& e) 
    	{
        	error("Try reloading the webpage.");
    	}
}


/* The main Function
	This receives the username through a POST request.
	It then calls the search_applicant function with the argument of the username received.
	This is done to search for applicants having the required skill.
*/
int main()
{
	Cgicc form_data;
	string username = "";
	
	form_iterator fi = form_data.getElement("username");  
   	if( !fi->isEmpty() && fi != (*form_data).end()) 
   	{  
      		username = **fi; 
      		search_applicants(username);
   	}
}