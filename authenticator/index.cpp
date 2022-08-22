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
#include <cppconn/statement.h>
#include <cppconn/connection.h>
#include <cppconn/prepared_statement.h>
#include <openssl/sha.h>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <chrono>

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

/* The itob32 Function	
	This is an INT to BASE32 Function.
       	It converts a long to base32, which is used for the TOTP seed

      	It takes one parametre of type long and returns the base32 string.
*/
string itob32(long i)
{
    string b32 = "";
        int d = i % 32;
        if (d < 26)
        {
            b32.append(1,char((int('a')+d)));
        }
        else
        {
            b32.append(1, char(d-26+1+49));
        }

    return b32;
}

/* The error Function
	It takes one parameter for the error message to display.
	It pops the error message on the screen for the user to see.
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


/*  The check_login Function
    	It checks if the password and username match
    	it also hashes the password by calling sha256() and compared to the password hash
    	stored in the database.
    	
    	It uses the database user 'login'.

   	The parameters username and password are checked against the username and password in 
   	the database. If there is a match, it returns true.
*/
bool check_login(string username, string password) 
{
    password = sha256(password);
    try 
    {
        Driver *driver;
        Connection *con;
        ResultSet *res;
        PreparedStatement *prep_stmt;

        driver = get_driver_instance();
        
        // creates a database connection using the driver. The parameters to the driver is the :
	// DB_HOST_URL: MySQL server IP address and port, 
	// the user 'login' that is allowed access to the login database, and
	// lastly, its password.
        con = driver->connect(DB_HOST_URL, "login", "xcv6ertwq2#$!@098ujtyvbxmsk");
        
        /* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        con->setSchema("Users");
        
        prep_stmt = con->prepareStatement("SELECT username,password FROM login WHERE online='0' AND username=?");
        prep_stmt->setString(1, username);
        
        res = prep_stmt->executeQuery();
        
        // fetch data related to the query.
        if (res->next()) {
            string dbname = res->getString("username");
            string dbpassword = res->getString("password");
            if (dbname == username && dbpassword == password) {
                delete res;
                con->close();
                delete con;
                delete prep_stmt;
                return true;
            }
            else
                return false;
        }
        else {
            delete res;
            con->close();
            delete con;
            delete prep_stmt;
            return false;
        }  
    }
    catch (sql::SQLException& e) {
        return false;
    }
    return false;
}


/* The safe_sessionID  Function
	This saves the SessionID to the database for Sessions.
    	It uses the database user 'login'.

	It takes the parameters: 
	sessionID: The sessionId created,
	username: username the session belongs to, 
	life_time: the expiry time of the session which is 10 minutes, and 
	the IP addressof the user.
	
	Returns true on success.
*/
bool safe_sessionID(string sessionID, string username,string life_time, string IP) 
{
    try 
    {
		Driver *driver;
        	Connection *con;
        	PreparedStatement *prep_stmt;

        	driver = get_driver_instance();
        	
        	// creates a database connection using the driver. The parameters to the driver is the :
		// DB_HOST_URL: MySQL server IP address and port, 
		// the user 'login' that is allowed access to the Sessions database, and
		// lastly, its password.
        	con = driver->connect(DB_HOST_URL, "login", "xcv6ertwq2#$!@098ujtyvbxmsk");
        	
        	/* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        	con->setSchema("Users");
	
        	prep_stmt = con->prepareStatement("INSERT INTO Sessions (SessionID,fk_user,expires,IP,online) VALUES(?,?,?,?,?)");
        	prep_stmt->setString(1, sessionID);
        	prep_stmt->setString(2, username);
        	prep_stmt->setString(3, life_time);
        	prep_stmt->setString(4, IP);
        	prep_stmt->setBoolean(5, false);
        	prep_stmt->execute();
        	
        	con->close();
        	delete con;
        	delete prep_stmt;
        
        	return true;
    }
    catch (sql::SQLException& e) 
    {
        return false;
    }

}


/* The get_sessionID Function
    	It creates a SessionID for the user and calls safe_sessionID() to save it.
    	
    	It takes a parameter: username of type string. The username will belong to the sessionID.
    	It returns the sessionID.

	The time function used is deprecated, but had to be used to get the format needed for MySQL. It makes
	use of the library <chrono>

*/
string get_sessionID(string username) 
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    time_t tt = std::chrono::system_clock::to_time_t(now);
    string nowstring = ctime(&tt);
    time_t     ten = time(0) + 600;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&ten);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);

    // uncomment the following lines for a production environment. This makes it dynamic, getting unique IP addresses.
    //Cgicc form_data;
    //const CgiEnvironment& env = form_data.getEnvironment();
    //string ENV = env.getRemoteAddr();
    
    
    string ENV = "localhost"; // since we are working locally, we need a hard-coded value, localhost.
    srand(time(0));
    string random = to_string(rand());
    string sessionID = sha256(username + random + nowstring + ENV + random);
    if (!safe_sessionID(sessionID,username, buf,ENV))
        error("ERROR: An error just occured. Please login again");
    return sessionID;
}

/* The set_cookie Function
	This sets two Cookies, one with the Username and one with the SesssionID, for this it calls getSessionID().
	The username is the user to login.
*/
void set_cookie(string username, string sID) 
{
    cout << "Set-Cookie:SessionID=";
    cout << sID;
    cout << "; Domain=localhost; ";
    cout << "Path = /; ";
    cout << "HTTPOnly=true; ";
    cout << "SameSite=Strict; ";
    cout << "Max-Age=600;\n";
    cout << "Set-Cookie:username=";
    cout << username;
    cout << "; Domain=localhost; ";
    cout << "Path=/; ";
    cout << "HTTPOnly=true; ";
    cout << "SameSite=Strict; ";
    cout << "Max-Age=600;\n";
}


/*  The send_email Function
    	This sends an email containing an OTP to the adress and username given.
 	It takes 3 parameters:
    	name: the full name of the user
	email: the email of the user, and
	code:  the OTP code
	
	NB: The send email function is simulated and the data are stored in the sent_emails database.
	This is because the relevant mail library could not be installed.
*/
void send_email(string name, string email, string code) 
{
	try 
    	{
        	Driver *driver;
        	Connection *con;
        	PreparedStatement *prep_stmt;

        	driver = get_driver_instance();
        	
        	// creates a database connection using the driver. The parameters to the driver is the :
		// DB_HOST_URL: MySQL server IP address and port, 
		// the user 'login' that is allowed access to the sent_emails database, and
		// lastly, its password.
        	con = driver->connect(DB_HOST_URL, "login", "xcv6ertwq2#$!@098ujtyvbxmsk");
        	
        	/* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        	con->setSchema("Users");
	
        	prep_stmt = con->prepareStatement("INSERT INTO sent_emails(email,name,code) VALUES(?,?,?)");
        	prep_stmt->setString(1, email);
        	prep_stmt->setString(2, name);
        	prep_stmt->setString(3, code);
        	prep_stmt->executeUpdate();
        
        	con->close(); // close the connection to the MySQL server
        	// clean up
        	delete con;
        	delete prep_stmt;

    	}
    	catch (sql::SQLException& e) 
    	{
    		cout << "ERROR: SQLException in " << __FILE__;
		cout << " (" << __func__<< ") on line " << __LINE__ << endl;
		cout << "ERROR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << ")" << endl;
    	}	

}


/* The create_2fa Function
	This creates an OTP, saves it to the database(2FA) and calls send_email() to send it to the specific user.
    	It uses the database user 'register'
	It takes a single parameter, username.
*/
bool create_2fa(string username) 
{
    time_t     ten = time(0) + 600;
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&ten);
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
    srand(time(0));
    int random = rand()%10000;
    string email = "";
    try 
    {
        Driver *driver;
        Connection *con;
        ResultSet *res;
        PreparedStatement *prep_stmt;
        

        driver = get_driver_instance();
        
        // creates a database connection using the driver. The parameters to the driver is the :
	// DB_HOST_URL: MySQL server IP address and port, 
	// the user 'login' that is allowed access to the 2FA and the login database, and
	// lastly, its password.
        con = driver->connect(DB_HOST_URL, "login", "xcv6ertwq2#$!@098ujtyvbxmsk");
        
        /* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        con->setSchema("Users");

        prep_stmt = con->prepareStatement("INSERT INTO 2FA(code,fk_user,expires) VALUES(?,?,?)");
        prep_stmt->setString(1, to_string(random));
        prep_stmt->setString(2, username);
        prep_stmt->setString(3, buf);
     	prep_stmt->execute();

        prep_stmt = con->prepareStatement("SELECT email FROM login WHERE username=?");
        prep_stmt->setString(1, username);
        res = prep_stmt->executeQuery();
      
      	// fetch data from the database based on the query above
        if (res->next()) 
        {
            email = res->getString("email");
        }
        else
        {
            throw "No Email";
	}
        send_email(username,email,to_string(random));
        
        con->close();
        delete con;
        delete res;
        delete prep_stmt;
        return true;
    }
    catch (sql::SQLException& e) 
    {
        return false;
    }
}



/* The set_TOTP Function
    	This creates a random seed by calling itob32() for the TOTP generation, and saves it to the database(TOTP)
    	It uses the database user 'register'

   	It takes a single parameter, username.
*/
string set_TOTP(string username) {
    username = username;
    string secret;
    srand(time(0));
    for (int i = 0; i < 56; i++) {
        int random = rand() % 100;
        secret.append(itob32(random));
    }

    try 
    {
        Driver* driver;
        Connection* con;
        PreparedStatement *prep_stmt;

        driver = get_driver_instance();
        
        // creates a database connection using the driver. The parameters to the driver is the :
	// DB_HOST_URL: MySQL server IP address and port, 
	// the user 'register' that is allowed access to the TOTP database, and
	// lastly, its password.
        con = driver->connect(DB_HOST_URL, "register", "xvbtyttrabcegtt&$22789%$&#@");
        
        /* select appropriate database schema. (We are making use of the 'Users' schema created using create_database.sql file) */
        con->setSchema("Users");

        prep_stmt = con->prepareStatement("INSERT INTO TOTP(seed,fk_user,online) VALUES(?,?,?)");
        prep_stmt->setString(1, secret);
        prep_stmt->setString(2, username);
        prep_stmt->setBoolean(3, false);
        prep_stmt->execute();
        
        con->close();
        delete con;
        delete prep_stmt;

    }
    catch (sql::SQLException& e) 
    {
    	cout << "ERROR: SQLException in " << __FILE__;
	cout << " (" << __func__<< ") on line " << __LINE__ << endl;
	cout << "ERROR: " << e.what();
	cout << " (MySQL error code: " << e.getErrorCode();
	cout << ", SQLState: " << e.getSQLState() << ")" << endl;
    }

    return secret;
}


/* The display_2fa Function
    This prints the 2FA page for the regular users(companies and applicants)
*/
void display_2fa()
{
	try 
	{
        	cout << HTTPHTMLHeader() << endl;
        	cout << html() << head(title("Job Recruitment")).add(link().set("rel", "stylesheet").set("href", "../css/main.css")) << endl;
        	
        	cout << body();
        	
        	cout << cgicc::div().set("class", "login-box") << endl;
        	cout << p("2FA");
       
        	cout << form().set("method", "post").set("action", "confirm_users.cgi") << endl;
        	cout << p("OTP Code sent by E-Mail") << endl;
        	cout << input().set("type", "password").set("name", "2FA").set("placeholder", "OTP code") << endl;
        	cout << "<br>" << endl;
     
        	cout << input().set("type", "submit").set("value", "Login").set("class", "btn") << endl;
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

/* The display_3fa Function
    This prints the 3FA page for the admin.
*/
void display_3fa(string username= "")
{
	string secret = set_TOTP(username);
	try 
	{
        	cout << HTTPHTMLHeader() << endl;
       		cout << html() << head(title("Job Approval")) << endl;
        	cout << link().set("rel", "stylesheet").set("href", "../css/qrcode.css") << endl;
        	cout << script().set("src", "../js/qrcode.js") << endl;
        	cout << script() << endl;
        	cout << head() << endl;
        	cout << body();
        	
        	
        	cout << cgicc::div().set("id", "qrcode").set("class", "qrcode") << endl;
        	cout << script() << endl;
        	cout << "new QRCode(document.getElementById('qrcode'),'"+secret+"');" << endl;
        	cout << "MakeQRCode('"+secret+"');" << endl;
        	cout << script() << endl;
        	cout << p("Scan the code above with a TOTP App like QR Scanner or Google Authenticator") << endl;
        
        	cout << h1("Confirm Email Sent");

        	cout << form().set("method", "post").set("action", "confirm_users.cgi") << endl;
        	cout << p("OTP Code sent by E-Mail") << endl;
        	cout << input().set("type", "password").set("name", "2FA").set("placeholder", "OTP code").set("required") << endl;
        	cout << "<br>" << endl;
        	cout << "<br>" << endl;
        	cout << p("OTP Code generated by App") << endl;
        	cout << input().set("type", "password").set("name", "3FA").set("placeholder", "TOTP") << endl;
        	cout << "<br>" << endl;
        	cout << "<br>" << endl;
        	
        	cout << input().set("type", "submit").set("value", "Confirm").set("class", "btn") << endl;
        	cout << "<br>" << endl;
        	cout << form() << endl;
        	cout << cgicc::div() << endl;

        	cout << body() << endl;

        	cout << html();
    	}
    	catch (exception& e) 
    	{
      	  	cout << "Try reloading the webpage." << endl; 
    	}

}

/* The main Function
	It receives the username and password through the POST request.
	It performs a login check. If successul, it creates a 2FA(for companies/applicants) or 3FA(for admin),
	sets cookie and session, and send the OTP code to the user through his/her email.
*/
int main()
{	
	string username = "";
	string password = "";
    	
    	Cgicc form_data;

    	form_iterator fi = form_data.getElement("username");
   	if (!fi->isEmpty() && fi != (*form_data).end()) 
    	{
        	username = **fi;
        	fi = form_data.getElement("password");
        	if (!fi->isEmpty() && fi != (*form_data).end()) 
        	{
            		password = **fi;
        	}
    	}
    	
    	else 
        {
		error_redirect("ERROR: username missing!.");
                exit(EXIT_SUCCESS);
        }
        
        if (check_login(username, password)) 
        {
                if (username == "admin") 
                {
                    create_2fa(username);
                    set_cookie(username, get_sessionID(username));          
                    display_3fa(username);
                    exit(EXIT_SUCCESS);
                }
                else if(string_starts_with(username, "comp"))
                {
                    create_2fa(username);
                    set_cookie(username, get_sessionID(username));
                    display_2fa();
                    exit(EXIT_SUCCESS);
                }
                else
                {
                	if(string_starts_with(username, "app"))
                	{
                		create_2fa(username);
                	        set_cookie(username, get_sessionID(username));
                    		display_2fa();
                    		exit(EXIT_SUCCESS);
                	}
                }
        }
        else 
        {
		error_redirect("ERROR: Password or Username incorrect");
                exit(EXIT_SUCCESS);
        }
}