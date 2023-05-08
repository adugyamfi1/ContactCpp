#include <vector>
#include <iostream>
#include <mariadb/conncpp.hpp>
#include "contactDB.h"
#include "contactEntry.h"



contactDB::contactDB() {
  	// Instantiate Driver
  	driver = sql::mariadb::get_driver_instance();
  	
 	// Configure Connection
  	// The URL or TCP connection string format is
  	// ``jdbc:mariadb://host:port/database``.
  	sql::SQLString url(db_url);

    // Use a properties map for the other connection options
  	sql::Properties my_properties({{"user", user}, {"password",pass}});
  	// Save properties in object
  	properties = my_properties;

    // Establish Connection
  	std::unique_ptr<sql::Connection> my_conn(driver->connect(db_url, properties));
    
    // Check success
    if (!my_conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
   	
   	// Save connection in object
   	conn = std::move(my_conn);
	#include <iostream>
#include <mysql.h>

// function to update a contact by id
void updateContact(MYSQL* conn, int id, std::string first_name, std::string last_name, std::string email, std::string phone_number, std::string address, std::string city, std::string state, std::string zip, std::string country) {
    // create the SQL update statement
    std::string sql = "UPDATE Contacts SET first_name='" + first_name + "', last_name='" + last_name + "', email='" + email + "', phone_number='" + phone_number + "', address='" + address + "', city='" + city + "', state='" + state + "', zip='" + zip + "', country='" + country + "' WHERE id=" + std::to_string(id);

    // execute the SQL statement
    int status = mysql_query(conn, sql.c_str());

    // check for errors
    if (status != 0) {
        std::cout << "Error updating contact: " << mysql_error(conn) << std::endl;
    }
    else {
        std::cout << "Contact updated successfully!" << std::endl;
    }
}

int main() {
    // connect to the MySQL database
    MYSQL* conn = mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "username", "password", "database", 0, NULL, 0);

    // call the updateContact function with the new contact details
    updateContact(conn, 1, "John", "Doe", "johndoe@example.com", "1234567890", "123 Main St", "Anytown", "CA", "12345", "USA");

    // close the database connection
    mysql_close(conn);

    return 0;
   	
}

vector<contactEntry> contactDB::find(string search) {

	vector<contactEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery(
			"SELECT * FROM contacts WHERE Last like '%"+search+"%' OR "+
    		 + "First like '%"+search+"%' OR " +
    		 + "Type like '%"+search+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<contactEntry> contactDB::findByLast(string last) {

	vector<contactEntry> list;
    
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE Last like '%"+last+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

vector<contactEntry> contactDB::findByFirst(string first) {

	vector<contactEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE First like '%"+first+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;
}

vector<contactEntry> contactDB::findByType(string type) {
	vector<contactEntry> list;
	
    // Make sure the connection is still valid
    if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
   	}	
    // Create a new Statement
	std::unique_ptr<sql::Statement> stmnt(conn->createStatement());
    
    // Execute query
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE Type like '%"+type+"%'");
    
    // Loop through and print results
    while (res->next()) {
    	contactEntry entry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
	    	
	    list.push_back(entry);

    }
    return list;

}

void contactDB::addEntry(string first,string last,string phone, string type){

	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
  	stmnt->executeQuery("INSERT INTO contacts(First,Last,Phone,Type) VALUES ('"+first+"','"+last+"','"+phone+"','"+type+"')");
}

contactEntry contactDB::fetchEntry(string id){

	contactEntry entry;	
	
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	
    sql::ResultSet *res = stmnt->executeQuery("SELECT * FROM contacts WHERE ID = '"+id+"'");
    
    // Get first entry
    if (res->next()) {
    	entry = contactEntry(res->getString("First"),res->getString("Last"),
			res->getString("Phone"),res->getString("Type"),
	    	res->getString("ID"));
    }
    return entry;
}

void contactDB::editEntry(string idnum,string first,string last,string phone, string type){
	if (!conn) {
   		cerr << "Invalid database connection" << endl;
   		exit (EXIT_FAILURE);
  	}

  	std::auto_ptr<sql::Statement> stmnt(conn->createStatement());

  	if (type != "Friend" && type != "Family" && type!="Business"){
     	 type="Other";
  	}
  	
  	stmnt->executeQuery("UPDATE contacts SET First = '"+first+"', Last ='"+last+"', Phone ='"+phone+"', Type ='"+type+"' WHERE ID='"+idnum+"'");
  	
}


void contactDB::deleteEntry(string idnum){
  // Establish Connection
  std::unique_ptr<sql::Connection>  conn(driver->connect(db_url, properties));
    
  if (!conn) {
   	cerr << "Invalid database connection" << endl;
   	exit (EXIT_FAILURE);
  }

  std::auto_ptr<sql::Statement> stmt(conn->createStatement());

  stmt->execute("DELETE FROM contacts WHERE ID='"+idnum+"'");
}
