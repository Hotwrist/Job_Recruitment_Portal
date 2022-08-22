/* Copyright(c) 2022 Odey John Ebinyi a.k.a Hotwrist
*  All Rights Reserved
*/

#include <iostream>
#include <fstream>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include "mysql_connection.h"

#include <openssl/sha.h>
#include <iomanip>

// The MySQL server is hosted locally. To connect to it,
// it uses 127.0.0.1 and port 3306  
#define DB_HOST_URL "tcp://127.0.0.1:3306"

using namespace cgicc;
using namespace std;
using namespace sql;


/* The string_starts_with Function.
	It takes 2 parametres: main_str and to_match
	It uses the c++ string 'find method' to check for a match between the main_str and the to_match string.
	If the main_str begins with the exact words found in the to_match string, then it returns true.
	It returns false if it finds no match.
*/
bool string_starts_with(string main_str, string to_match)
{
	if(main_str.find(to_match) == 0)
		return true;
	else 
		return false;
}

/* The error Function
	It takes one parameter for the error message to display.
	It pops the error message on the screen and redirects the user to the register page.
*/
void error(string err_message) 
{
    	cout << HTTPHTMLHeader() << endl;
	cout << html() << head(title("Job Recruitment")).add(meta().set("http-equiv", "refresh").set("content", "0; URL=.")) << endl;
       	cout << head() << endl;
       	cout << body();
        
       	cout << script() << endl;
       	cout << "alert('"+ err_message +"');" << endl;
       	cout << script() << endl;

       	cout << body() << endl;

   	cout << html();	
   	
   	exit(EXIT_SUCCESS);
}

/* The redirect_to_login Function
	This function redirects any user to the login page after successful registration.
	It alerts the user on successful registration and uses the HTTP redirect method
	to redirect the user to the login page.
*/
void redirect_to_login()
{
	try
	{
		cout << HTTPHTMLHeader() << endl;
		cout << html() << head(title("Job Recruitment")).add(meta().set("http-equiv", "refresh").set("content", "0; URL=../")) << endl;
        	cout << head() << endl;
        	cout << body();
        
       	 	cout << script() << endl;
        	cout << "alert(\"You have been registered successfully!!\");" << endl;
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

/*  The SHA256 Function
    This Generates a SHA256 hash from the input string and returns it.

    It takes one parameter: input_str
    this input_str, will be hashed and returned to the calling function.
*/
string sha256(string input_str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input_str.c_str(), input_str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }
    return ss.str();
}


/*  The register_user Function
    This saves the username, password, email, fullname and skill of the users to the database.
    It uses prepared statement to validate the inputs preventing SQL Injections.
    After saving the valid inputs, it redirects any of the users it is working on, to the login page.
*/
void register_user(string username, string password, string email, string full_name, string skill)
{
   	password = sha256(password);	
	
	try
	{
		Driver *driver;
		Connection *con;
		PreparedStatement *prep_stmt = NULL;

		driver = get_driver_instance();
		
		// creates a database connection using the driver. The parameters to the driver is the :
		// DB_HOST_URL: MySQL server IP address and port, 
		// the user 'register' that is allowed to modify the login database, and
		// lastly, its password.
		con = driver->connect(DB_HOST_URL, "register", "xvbtyttrabcegtt&$22789%$&#@"); 
		
		/* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
		con->setSchema("Users");
		
		if(username == "admin")
		{
			// add admin details to the login database
			prep_stmt = con -> prepareStatement ("INSERT INTO login (online,password,email,fullname,username) VALUES (?,?,?,?,?)");
		
			prep_stmt -> setBoolean (1, 0);
			prep_stmt -> setString (2, password);
			prep_stmt -> setString (3, email);
			prep_stmt -> setString (4, full_name);
			prep_stmt -> setString (5, username);
			prep_stmt -> executeUpdate();
			
			prep_stmt = con -> prepareStatement ("INSERT INTO admin_credentials (admin_id, name) VALUES (?,?)");
			prep_stmt -> setUInt (1, 1);
			prep_stmt -> setString (2, full_name);
			prep_stmt -> executeUpdate();
			
			redirect_to_login();
			exit(0);
		}
		
		else if(string_starts_with(username, "comp"))
		{
			// add company details to the login database.
			prep_stmt = con -> prepareStatement ("INSERT INTO login (online,password,email,fullname,username) VALUES (?,?,?,?,?)");
		
			prep_stmt -> setBoolean (1, 0);
			prep_stmt -> setString (2, password);
			prep_stmt -> setString (3, email);
			prep_stmt -> setString (4, full_name);
			prep_stmt -> setString (5, username);
			prep_stmt -> execute();

			prep_stmt = con -> prepareStatement ("INSERT INTO companies_unapproved (name, username, skill_required) VALUES (?,?,?)");
		
			prep_stmt -> setString (1, full_name);
			prep_stmt -> setString (2, username);
			prep_stmt -> setString (3, skill);
			prep_stmt -> execute();
			
			redirect_to_login();
			exit(EXIT_SUCCESS);
		}
		
		else
		{
			if(string_starts_with(username, "app"))
			{
				// add applicant details to the login database.
				prep_stmt = con -> prepareStatement ("INSERT INTO login (online,password,email,fullname,username) VALUES (?,?,?,?,?)");
				
				prep_stmt -> setBoolean (1, 0);
				prep_stmt -> setString (2, password);
				prep_stmt -> setString (3, email);
				prep_stmt -> setString (4, full_name);
				prep_stmt -> setString (5, username);
				prep_stmt -> executeUpdate();				

				prep_stmt = con -> prepareStatement ("INSERT INTO applicants_unapproved (name, username, skill) VALUES (?,?,?)");
		
				prep_stmt -> setString (1, full_name);
				prep_stmt -> setString (2, username);
				prep_stmt -> setString (3, skill);
				
				prep_stmt -> executeUpdate();
				
				redirect_to_login();
				exit(EXIT_SUCCESS);
			}
			
			else { error("ERROR: Empty Database"); }
		}
		
		/* Clean up */
		delete prep_stmt;
		con->close();
		delete con;
	}
	catch (sql::SQLException& e) 
	{
        	error("ERROR: connection to database was not successful");
    	}
	
	return;
}


/*  The main Funcion
    
    It receives the GET, POST and allows for the registration of the users.
    It receives the user's input: username, password, email, full name, and skill(apart from the admin).
    It then calls the register_user function to register the user based on the details provided.
*/
int main()
{	
	string username = "";
    	string password = "";
    	string email = "";
    	string fullname = "";
    	string skill = "";

	Cgicc form_data;
    	form_iterator fi = form_data.getElement("username");
    
    	if (!fi->isEmpty() && fi != (*form_data).end()) 
    	{
        	username = **fi;
	
        	fi = form_data.getElement("password");
        	if (!fi->isEmpty() && fi != (*form_data).end()) 
           		password = **fi;

        	fi = form_data.getElement("email");
        	if (!fi->isEmpty() && fi != (*form_data).end())
       		     	email = **fi;

		fi = form_data.getElement("fullname");
        	if (!fi->isEmpty() && fi != (*form_data).end())
            		fullname = **fi;
            		
            	fi = form_data.getElement("skill");
            	if (!fi->isEmpty() && fi != (*form_data).end())
            		skill = **fi;
            	register_user(username, password, email, fullname, skill);
    	}

	return 0;
}