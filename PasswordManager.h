#pragma once
#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H
#include <string>
#include <vector>
#include <iostream>
#include <random>
#include <algorithm>
#include <cstdlib> 
#include "AESEncryption.h"
#include "DatabaseHandler.h"
#include "Account.h"

class PasswordManager
{
private:
	const string MASTER_PASSWORD_ENV = "PM_MASTER_PASSWORD";
	const string MASTER_USER_ENV = "PM_MASTER_USER";
	const int PASSWORD_GEN_LENGTH = 50;
	string databaseName = "password-manager";
	string master_username;
	string master_password;
	DatabaseHandler db;

public: 
	PasswordManager();
	~PasswordManager();
	string get_master_password() const;
	string get_master_username() const;
	void set_master_password(const string& master_password);
	void set_master_user(const string& master_user);
	string encrypt_password(string password);
	string decrypt_password(string cipher_hex_str);
	bool verify_user(string user);
	vector<Account> find_all_credentials();
	vector<Account> find_credentials_of_service(string service);
	vector<Account> find_one_credential_of_service_and_username(string service, string username);
	void add_credential(string service, string username, string password, string description);
	void edit_credential(string service, string username, string new_password);
	void remove_credential(string service, string username);
	void create_user(string username);
	string get_environment_variable(string environment_key);
	void change_master_password(string new_master_password);
	string generate_random_password();
};

#endif // PASSWORD_MANAGER_H


