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

/* The error_redirect Function
	It takes one parameter for the error message to display.
	It pops the error message on the screen and redirects the user to the start page.
*/
void error_redirect(string err_message)
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")).add(meta().set("http-equiv", "refresh").set("content", "0; URL=../")) << endl;
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

/* The make_query Function
	This is used to execute SELECT statements, which returns an SQL result.
    	It has 2 parameters containing the SQL query and the arg parameter(representing the skill passed to it).
	After successful execution, it returns SQL::ResultSet
*/
ResultSet *make_query(string query, string arg) 
{
    try 
    {
        Driver *driver;
        Connection *con;
        PreparedStatement *prep_stmt;
        ResultSet *res;

        driver = get_driver_instance();
        
        // creates a database connection using the driver. The parameters to the driver is the :
	// DB_HOST_URL: MySQL server IP address and port, 
	// the user 'user' that is allowed only SELECT access to the applicants_unapproved database, and
	// lastly, its password.
        con = driver->connect(DB_HOST_URL, "user", "ib546@#$$o0dhfbvxcggg984Am6");
        
        /* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        con->setSchema("Users");

        prep_stmt = con->prepareStatement(query);
        prep_stmt->setString(1, arg);
        res = prep_stmt->executeQuery();
        return res;
    }
    catch (sql::SQLException& e) 
    {
        error_redirect("ERROR: Error Connecting to Database!.");
        exit(EXIT_SUCCESS);
    }
}


/* The display_applicants Function
	This displays the applicants having the skill required by the company.
	It takes one parameter, skill representing the skill required by the company.
*/
void display_applicants(string skill)
{	
	ResultSet *res = make_query("SELECT * FROM applicants_unapproved WHERE skill=?", skill);
	
	try 
	{
            cout << HTTPHTMLHeader() << endl;
            cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "../css/company_page.css")) << endl;
            
            cout << body() << endl;
            cout << h1("Applicants with the Required Skill") << endl;
            cout << table().set("class", "container") << endl;
               
            cout << "<thead>" << endl;
            cout << "<tr>" << endl;
            cout << "<th><h1>Applicant</h1></th>" << endl;
            cout << "<th><h1>Skill</h1></th>" << endl;
            cout << "</tr>" << endl;
            cout << "</thead>" << endl;
                
            cout << "<tbody>" << endl;
            
            /* fetch the data : retrieve all the rows in the result set */
            while(res->next())
            {
                cout << "<tr>" << endl;
                cout << "<td>" << endl;
                cout << res->getString("name");
               	cout << "</td>" << endl;
             	cout << "<td>" << endl;
             	cout << res->getString("skill");
                cout << "</td>" << endl;
                cout << "</tr>" << endl;
            }
            cout << "</tbody>" << endl;
            cout << table() << endl;
            cout << "<br>" << endl;
   
            cout << body() << endl;
            cout << html();
        }
        catch (exception& e) {
            cout << "Please try reloading the webpage." << endl; 
        }
}

/* The main Function
	This receives the skill searched by the company from the POST request 
	and then calls the display_applicants function 
	passing the skill as the argument to the function.
*/
int main()
{
	Cgicc form_data;
	string skill = "";
	
	form_iterator fi = form_data.getElement("skill");  
   	if( !fi->isEmpty() && fi != (*form_data).end()) 
   	{  
      		skill = **fi; 
      		
      		display_applicants(skill);	
   	}
}