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
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <string>

// The MySQL server is hosted locally. To connect to it,
// it uses 127.0.0.1 and port 3306  
#define DB_HOST_URL "tcp://127.0.0.1:3306"

using namespace cgicc;
using namespace std;
using namespace sql;


/* The error_redirect Function
	It takes one parameter for the error message.
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
    	It has a parameter containing the SQL select command.
	After successful execution, it returns SQL::ResultSet
*/
ResultSet *make_query(string select) 
{
    try 
    {
        Driver *driver;
        Connection *con;
        Statement *stmt;
        ResultSet *res;

        driver = get_driver_instance();
        
        // creates a database connection using the driver. The parameters to the driver is the :
	// DB_HOST_URL: MySQL server IP address and port, 
	// the user 'admin' that is allowed to modify the companies_unapproved and companies_approved database, and
	// lastly, its password.
        con = driver->connect(DB_HOST_URL, "admin", "vxbyt&^%rxcdquthy$@#990234&");
        
        /* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        con->setSchema("Users");

        stmt = con->createStatement();
        return (res = stmt->executeQuery(select));
    }
    catch (sql::SQLException& e) 
    {
        error_redirect("ERROR: Session expired");
        exit(EXIT_SUCCESS);
    }
}

/* The display_users Function
	This displays the users(companies and applicants) who are awaiting approval, to the admin
	It takes no parameter.
*/
void display_users()
{
	ResultSet *res = make_query("SELECT * FROM companies_unapproved");
	
	try 
	{
            cout << HTTPHTMLHeader() << endl;
            cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "../css/admin_page.css")) << endl;
            cout << script().set("type", "text/javascript").set("src", "../js/admin_page.js");
            cout << script() << endl;
            
            // Company awaiting approval listing section
            cout << body() << endl;
            cout << h1("Companies Awaiting Approval") << endl;
            cout << table().set("class", "container").set("id", "display-table") << endl;
               
            cout << "<thead>" << endl;
            cout << "<tr>" << endl;
            cout << "<th><h1>Company</h1></th>" << endl;
            cout << "<th><h1>Skill Required</h1></th>" << endl;
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
             	cout << res->getString("skill_required");
                cout << "</td>" << endl;
                cout << "</tr>" << endl;
            }
            cout << "</tbody>" << endl;
            cout << table() << endl;
            cout << form().set("method", "post").set("action", "approved/company/") << endl;
            cout << input().set("type", "text").set("placeholder", "Name to approve").set("name", "approved_name").set("value", "").set("autocomplete", "off").set("class", "form_input") << endl;
            cout << "<br>" << endl; 
            cout << "<br>" << endl; 
            cout << input().set("type", "submit").set("value", "Approve").set("class", "btn") << endl;
            cout << form() << endl;
            cout << "<br>" << endl;    
            
            // Applicants awaiting approval listing section
            res = make_query("SELECT * FROM applicants_unapproved");
            cout << h1("Applicants Awaiting Approval") << endl;
            cout << table().set("class", "container").set("id", "display-table") << endl;
               
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
            cout << form().set("method", "post").set("action", "approved/applicant/") << endl;
            cout << input().set("type", "text").set("placeholder", "Name to approve").set("name", "approved_name").set("value", "").set("autocomplete", "off").set("class", "form_input") << endl;
            cout << "<br>" << endl; 
            cout << "<br>" << endl; 
            cout << input().set("type", "submit").set("value", "Approve").set("class", "btn") << endl;
            cout << form() << endl;
            
            cout << form().set("method", "post").set("action", "list.cgi") << endl;
            cout << input().set("type", "submit").set("value", "Approved list").set("class", "btn") << endl;
            cout << form() << endl;
            
            cout << "<br>" << endl;  
            cout << body() << endl;
            cout << html();
        }
        catch (exception& e) {
            cout << "Please try reloading the webpage." << endl; 
        }
}

/* The main Function
	It calls the display_users function.
*/
int main()
{
	display_users();	
}