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

#include <iomanip>
#include <string>


#define DB_HOST_URL "tcp://127.0.0.1:3306"

using namespace cgicc;
using namespace std;
using namespace sql;


/* The admin Page Function
	Prints the name of the company that has been approved
	and redirects to the admin page.
*/
void admin_page(string name)
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")).add(meta().set("http-equiv", "refresh").set("content", "0; URL=../../")) << endl;
        	cout << head() << endl;
        	cout << body();
        
       	 	cout << script() << endl;
        	cout << "alert(\"" +name+" has been approved successfully!!\");" << endl;
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


/* The error_redirect Function
	It takes one parameter for the error message.
	It pops the error message on the screen and redirects to the admin page.
*/
void error_redirect(string err_message)
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")).add(meta().set("http-equiv", "refresh").set("content", "0; URL=../../")) << endl;
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


/* The approve_company Function
	It takes only one parametre for the approved name.
	It also performs some database commands and finally stores the 
	approved name and the related skill in the companies_approved database.
*/
void approve_company(string name)
{
	try 
	{
        	Driver *driver;
        	Connection *con;
        	ResultSet *res;
        	PreparedStatement *prep_stmt;
        	
        	driver = get_driver_instance();
        	
        	// creates a database connection using the driver. The parameters to the driver is the :
		// DB_HOST_URL: MySQL server IP address and port, 
		// the user 'admin' that is allowed to modify the companies_unapproved and companies_approved database, and
		// lastly, its password.
        	con = driver->connect(DB_HOST_URL, "admin", "vxbyt&^%rxcdquthy$@#990234&");
        	
        	/* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        	con->setSchema("Users");
        	
        	string skill_required = "";
        	prep_stmt = con->prepareStatement("SELECT skill_required FROM companies_unapproved WHERE name=?");
        	prep_stmt->setString(1, name);
        	res = prep_stmt->executeQuery();
        	
        	while(res->next()) 
        	{
               	  skill_required = res->getString("skill_required");
           	}

		// adds the approved company details(name and skill required) to the approved company database.
        	prep_stmt = con->prepareStatement("INSERT INTO companies_approved(name,skill_required) VALUES(?,?)");
        	prep_stmt->setString(1, name);
        	prep_stmt->setString(2, skill_required);
        	prep_stmt->execute();
        	
        	// Delete the approved company from the unapproved companies database. 
        	prep_stmt = con->prepareStatement("DELETE FROM companies_unapproved WHERE name=?");
        	prep_stmt->setString(1, name);
        	prep_stmt->execute();
        	
        	con->close();
        	delete con;
        	delete prep_stmt;
        	delete res;
    	}
    	catch (sql::SQLException& e) 
    	{
    	    error_redirect("ERROR: Company approval failed!");
    	    exit(EXIT_SUCCESS);
    	}
    	
    	admin_page(name);	// Go back to the admin page.
}


/* The main Function
	It receives the approved name of the company through a POST
	request. It calls approved_company function passing the approved company name as an argument.
*/
int main()
{
	string name = "";

	Cgicc form_data;
	
	cout << "Content-type:text/html\r\n\r\n";
   	cout << "<html>\n";
   	cout << "<head>\n";
   	cout << "<title>Using GET and POST Methods</title>\n";
   	cout << "</head>\n";
   	cout << "<body>\n";
    	form_iterator fi = form_data.getElement("approved_name");
    
    	if (!fi->isEmpty() && fi != (*form_data).end()) 
    	{
        	name = **fi;
    	}
    	
    	cout << "<br/>\n";
   	cout << "</body>\n";
   	cout << "</html>\n";
    	
    	approve_company(name);
	
	exit(EXIT_SUCCESS);
}