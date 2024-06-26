#pragma once
#ifndef DATABASE_HANDLER_H
#define DATABASE_HANDLER_H
#include <string>
#include <iostream>
#include <mongocxx/client.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/exception/exception.hpp>
#include <bson/bson.h>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/exception/exception.hpp>
#include <mongocxx/pool.hpp>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <iostream>
#include <vector>
#include <assert.h>
#include "SHAHashing.h"
#include "Account.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

#define MONGODB_URI "MONGODB_URI"

enum CollectionIndex { CREDENTIALS, USERS };

class DatabaseHandler
{
private:
	std::string databaseName;
	std::vector<std::string> collection_names = { "credentials", "masterkeys"};
	mongocxx::instance inst;
	mongocxx::uri mongoURI;
	mongocxx::client conn;
	mongocxx::database db;

public:
	DatabaseHandler(std::string databaseName);
	~DatabaseHandler();
	std::string get_environment_variable(std::string environmentVarKey);
	std::string to_json(bsoncxx::document::view view);
	std::string query_master_password_of_user(std::string username );
	void create_user(std::string username, std::string master_password);
	void edit_master_password(std::string master_username, std::string new_master_password);
	void set_collection_name(std::string collectionName);
	std::vector<Account> query_credentials(std::string user, std::string service);
	std::vector<Account> query_all_credentials(std::string user);
	std::vector<Account> query_credential(std::string user, std::string service, std::string username);
	void add_credential(std::string user, std::string service, std::string username, std::string password, std::string description);
	void edit_credential(std::string user, std::string service, std::string username, std::string new_password);
	void remove_credential(std::string user, std::string service, std::string username);
};

#endif // DATABASE_HANDLER_H