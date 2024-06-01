#include <iostream>
#include <string>
#include <iomanip>
#include "PasswordManager.h"
#include "lib/color.hpp"
#include "lib/tabulate.hpp"

using std::cin;
using std::cout;

string take_password_from_user();
void print_accounts(const vector<Account> &accounts);
bool login(PasswordManager &app);
bool login_with_system_variables(PasswordManager &app);
void transform_to_lower_case(string &str);
int get_account_index_from_user_input(const vector<Account> &accounts);
void print_find_result(const string &username, const string &password);
void get_new_account_info(string &service, string &username, string &password, string &description, const int &argc, char *argv[]);
void query_credentials(PasswordManager &app, int argc, char *argv[]);
void get_account_info_to_edit(PasswordManager &app, string &service, string &username, string &new_password, const int &argc, char *argv[]);
void get_account_info_to_remove(PasswordManager &app, string &service, string &username, const int &argc, char *argv[]);
void remove_spaces(string &str);
void ask_for_sure();

int main(int argc, char *argv[])
{
	PasswordManager app;
	string tabs = "\t";
	string user, master_password;

	if (argc == 1)
	{
		cout << "Password Manager is a simple tool that allows you to store and retrieve passwords securely." << endl
			 << endl;
		cout << "Usage:" << endl
			 << "  pm [command]" << endl
			 << endl;
		cout << "Available commands: " << endl;
		cout << "  find  " << tabs << "Find a password" << endl;
		cout << "  add   " << tabs << "Add a password" << endl;
		cout << "  edit  " << tabs << "Edit a password" << endl;
		cout << "  remove" << tabs << "Remove a password" << endl;
		cout << "  reset " << tabs << "Reset the master password" << endl;
		cout << "  gen   " << tabs << "Generate a password" << endl
			 << endl;
		cout << "Commands usage:" << endl;
		cout << "  pm find [service] [username]" << endl;
		cout << "  pm add [service] [username]" << endl;
		cout << "  pm edit [service] [username]" << endl;
		cout << "  pm remove [service] [username]" << endl;
		cout << "  pm reset" << endl;
		cout << "  pm gen" << endl;
	}

	else
	{
		try
		{
			if (login_with_system_variables(app))
			{
				string command = argv[1];
				if (command.find("find") != string::npos)
				{
					query_credentials(app, argc, argv);
				}

				else if (command == "add")
				{
					string service, username, password, description;
					get_new_account_info(service, username, password, description, argc, argv);
					transform_to_lower_case(service);
					app.add_credential(service, username, password, description);
					cout << dye::green("[SUCCESS] Credential added successfully.") << endl;
				}

				else if (command == "edit")
				{
					string service, username, new_password;
					get_account_info_to_edit(app, service, username, new_password, argc, argv);
					transform_to_lower_case(service);
					ask_for_sure();
					app.edit_credential(service, username, new_password);
					cout << dye::green("[SUCCESS] Credential edited successfully.") << endl;
				}

				else if (command == "remove")
				{
					string service, username;
					get_account_info_to_remove(app, service, username, argc, argv);
					transform_to_lower_case(service);
					ask_for_sure();
					app.remove_credential(service, username);
					cout << dye::green("[SUCCESS] Credential removed successfully.") << endl;
				}

				else if (command == "reset")
				{
					cout << "-> New master password: ";
					string new_master_password;
					cin >> new_master_password;

					if (new_master_password.empty())
					{
						throw std::runtime_error("Master password cannot be empty.");
					}

					app.change_master_password(new_master_password);
				}

				else if (command == "gen")
				{
					cout << dye::green("[SUCCESS] Generated password: ") << dye::yellow(app.generate_random_password()) << endl;
				}

				else
				{
					throw std::runtime_error("Invalid command.");
				}
			}
		}
		catch (std::exception &e)
		{
			cout << dye::red("[ERROR] " + string(e.what()) + ".") << endl;
		}
	}
	return 0;
}

string take_password_from_user()
{
	HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;

	// Create a restore point Mode, is know 503
	GetConsoleMode(hStdInput, &mode);

	// Enable echo input, set to 499
	SetConsoleMode(hStdInput, mode & (~ENABLE_ECHO_INPUT));

	// Take input
	string ipt;
	getline(cin, ipt);

	// Otherwise next cout will print into the same line
	cout << endl;

	// Restore the mode
	SetConsoleMode(hStdInput, mode);
	return ipt;
}

void print_accounts(const vector<Account> &accounts)
{
	tabulate::Table credentials;
	credentials.add_row({"Index", "Service", "Username", "Description"});
	for (size_t i = 0; i < 4; ++i)
	{
		credentials[0][i].format().font_color(tabulate::Color::yellow).font_style({tabulate::FontStyle::bold});
	}

	size_t i = 0;
	for (Account account : accounts)
	{
		string index = std::to_string(i);
		string service = account.get_service();
		string username = account.get_username();
		string description = account.get_description();
		// cout << std::left << std::setfill('.') << std::setw(10) << index << std::left << std::setw(20) << service << std::setw(50) << username << description << endl;
		credentials.add_row({index, service, username, description});
		i++;
	}
	cout << credentials << endl;
}

bool login(PasswordManager &app)
{
	string master_username, master_password;
	do
	{
		cout << "-> Master username: ";
		cin >> master_username;
		cout << "-> Master password: ";
		cin.ignore();
		master_password.assign(take_password_from_user());
		if (!master_password.empty() || !master_username.empty())
		{
			app.set_master_user(master_username);
			app.set_master_password(master_password);
			return app.verify_user(master_username);
		}
		else
		{
			cout << "[FAIL] Master password and username cannot be empty." << endl;
			return false;
		}
	} while (master_password.empty());
}

bool login_with_system_variables(PasswordManager &app)
{
	string master_username = app.get_master_username();
	string master_password = app.get_master_password();
	if (!app.get_master_password().empty() || !master_username.empty())
	{
		app.set_master_user(master_username);
		app.set_master_password(master_password);
		cout << "Stay calm! I'm checking your master username & master password..." << endl;
		if (!app.verify_user(master_username))
		{
			throw std::runtime_error("Oops! Your master username or master password is incorrect.");
		}
		else
		{
			cout << dye::green("[SUCCESS] Okay, welcome back!") << endl;
			return true;
		}
	}
	else
	{
		// cout << "[FAIL] Master password and username cannot be empty." << endl;
		throw std::runtime_error("Master password and username cannot be empty.");
	}
	return false;
}

void transform_to_lower_case(string &str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

int get_account_index_from_user_input(const vector<Account> &accounts)
{
	std::cout << "Choose an account: ";
	string choice_str;
	cin >> choice_str;

	// Check if the input is a number only
	if (!std::all_of(choice_str.begin(), choice_str.end(), ::isdigit))
	{
		throw std::runtime_error("Invalid input. Please enter a number.");
	}

	// Convert the string to an integer
	int choice = std::stoi(choice_str);

	if (choice < 0 || choice >= accounts.size())
	{
		throw std::runtime_error("Invalid choice.");
	}
	else
	{
		return choice;
	}
}

void print_find_result(const string &username, const string &password)
{
	tabulate::Table result;
	result.add_row({"Username", username});
	result.add_row({"Password", password});
	result.column(0).format().font_color(tabulate::Color::yellow).font_style({tabulate::FontStyle::bold});
	result.column(1).format().font_color(tabulate::Color::green).font_style({tabulate::FontStyle::bold});
	cout << result << endl;
}

void get_new_account_info(string &service, string &username, string &password, string &description, const int &argc, char *argv[])
{
	if (argc == 2) // user enter: pm add
	{
		cout << "Service: ";
		std::getline(cin, service);
		remove_spaces(service);
		cout << "Username: ";
		std::getline(cin, username);
		remove_spaces(username);
		cout << "Password: ";
		password.assign(take_password_from_user());
		cout << "Description: ";
		std::getline(cin, description);
	}
	else if (argc == 3) // user enter: pm add [service]
	{
		service = argv[2];
		cout << "Username: ";
		std::getline(cin, username);
		remove_spaces(username);
		cout << "Password: ";
		password.assign(take_password_from_user());
		cout << "Description: ";
		std::getline(cin, description);
	}
	else if (argc == 4) // user enter: pm add [service] [username]
	{
		service = argv[2];
		username = argv[3];
		cout << "Password: ";
		password.assign(take_password_from_user());
		cout << "Description: ";
		std::getline(cin, description);
	}
}

void query_credentials(PasswordManager &app, int argc, char *argv[])
{
	if (argc == 2) // user enter: pm find
	{
		vector<Account> accounts = app.find_all_credentials();
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found.");
		}
		else
		{
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			Account account = accounts[choice];
			print_find_result(account.get_username(), app.decrypt_password(account.get_password()));
		}
	}
	else if (argc == 3) // user enter: pm find [service]
	{
		string service = argv[2];
		transform_to_lower_case(service);
		vector<Account> accounts = app.find_credentials_of_service(service);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service '" + service + "'");
		}
		else
		{
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			Account account = accounts[choice];
			print_find_result(account.get_username(), app.decrypt_password(account.get_password()));
		}
	}
	else if (argc == 4) // user enter: pm find [service] [username]
	{
		string service = argv[2];
		string username = argv[3];
		transform_to_lower_case(service);
		vector<Account> accounts = app.find_one_credential_of_service_and_username(service, username);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service '" + service + "' with username '" + username + "'");
		}
		else if (accounts.size() > 1)
		{
			throw std::runtime_error("Multiple accounts found for service'" + service + "' with username '" + username + "'");
		}
		else if (accounts.size() == 1)
		{
			Account account = accounts[0];
			print_find_result(account.get_username(), app.decrypt_password(account.get_password()));
		}
	}
}

void get_account_info_to_edit(PasswordManager &app, string &service, string &username, string &new_password, const int &argc, char *argv[])
{
	if (argc == 2) // user enter: pm edit -> list all accounts and choose one to edit
	{
		vector<Account> accounts = app.find_all_credentials();
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found.");
		}
		else
		{
			cout << endl;
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			cout << "New password: ";
			cin.ignore();
			new_password.assign(take_password_from_user());

			if (choice < 0 || choice >= accounts.size())
			{
				throw std::runtime_error("Invalid choice.");
			}

			service = accounts[choice].get_service();
			username = accounts[choice].get_username();
		}
	}
	else if (argc == 3) // user enter: pm edit [service]
	{
		service = argv[2];
		vector<Account> accounts = app.find_credentials_of_service(service);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service '" + service + "'");
		}

		else
		{
			cout << endl;
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			cout << "New password: ";
			cin.ignore();
			new_password.assign(take_password_from_user());
			username = accounts[choice].get_username();
		}
	}
	else if (argc == 4) // user enter: pm edit [service] [username]
	{
		string service = argv[2];
		string username = argv[3];
		vector<Account> accounts = app.find_one_credential_of_service_and_username(service, username);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service '" + service + "' and username '" + username + "'");
		}
		else if (accounts.size() > 1)
		{
			throw std::runtime_error("Multiple accounts found for service '" + service + "' and username '" + username + "'");
		}
		else if (accounts.size() == 1)
		{
			cout << "New password: ";
			new_password.assign(take_password_from_user());
		}
	}
}

void get_account_info_to_remove(PasswordManager &app, string &service, string &username, const int &argc, char *argv[])
{
	if (argc == 2) // user enter: pm remove
	{
		vector<Account> accounts = app.find_all_credentials();
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found.");
		}
		else
		{
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			service = accounts[choice].get_service();
			username = accounts[choice].get_username();
		}
	}
	else if (argc == 3) // user enter: pm remove [service]
	{
		service = argv[2];
		vector<Account> accounts = app.find_credentials_of_service(service);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service'" + service + "'");
		}
		else
		{
			print_accounts(accounts);
			int choice = get_account_index_from_user_input(accounts);
			username = accounts[choice].get_username();
		}
	}
	else if (argc == 4) // user enter: pm remove [service] [username]
	{
		service = argv[2];
		username = argv[3];
		vector<Account> accounts = app.find_one_credential_of_service_and_username(service, username);
		if (accounts.empty())
		{
			throw std::runtime_error("No accounts found for service '" + service + "' and username '" + username + "'");
		}
		else if (accounts.size() > 1)
		{
			throw std::runtime_error("Multiple accounts found for service '" + service + "' and username '" + username + "'");
		}
	}
}

void remove_spaces(string &str)
{
	str.erase(std::find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), str.end());

	std::replace(str.begin(), str.end(), ' ', '-');
}

void ask_for_sure()
{
	cout << "Are you sure you want to proceed ? [y/n]: ";
	string choice;
	cin >> choice;
	if (!(choice == "y" || choice == "Y"))
	{
		throw std::runtime_error("Process has been cancelled by user.");
	}
}
