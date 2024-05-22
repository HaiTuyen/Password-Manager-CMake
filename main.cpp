#include <iostream>
#include <string>
#include <iomanip>
#include "PasswordManager.h"

using std::cin;
using std::cout;
using std::endl;

std::string take_password_from_user();
void print_accounts(const vector<Account>& accounts);
bool login(PasswordManager& app);
bool login_with_system_variables(PasswordManager& app);
void transform_to_lower_case(string& str);

int main(int argc, char* argv[])
{
	PasswordManager app;
	string tabs = "\t";
	string user, master_password;

	if (argc == 1)
	{
		cout << "Password Manager is a simple tool that allows you to store and retrieve passwords securely." << endl << endl;
		cout << "Usage:" << endl << "  pm [command]" << endl << endl;
		cout << "Available commands: " << endl;
		cout << "  find  " << tabs << "Find a password" << endl;
		cout << "  add   " << tabs << "Add a password" << endl;
		cout << "  edit  " << tabs << "Edit a password" << endl;
		cout << "  remove" << tabs << "Remove a password" << endl;
		cout << "  reset " << tabs << "Reset the master password" << endl;
		cout << "  gen   " << tabs << "Generate a password" << endl << endl;
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
					cout << endl;
					cout << "------------------" << endl;
					cout << "USING FIND COMMAND" << endl;
					cout << "------------------" << endl;
					if (argc == 2) // user enter: pm find
					{
						vector<Account> accounts = app.find_all_credentials();
						//cout << endl;
						//cout << "Finding all credentials from database" << endl;
						if (accounts.empty())
						{
							//cout << endl << "[INFO] No accounts found." << endl;
							throw std::runtime_error("No accounts found.");
						}
						else
						{
							cout << endl;
							print_accounts(accounts);
							cout << endl << "-> Choose an account to view: ";
							int choice;
							cin >> choice;
							if (choice < 0 || choice >= accounts.size())
							{
								cout << "[FAIL] Invalid choice." << endl;
							}
							else
							{
								Account account = accounts[choice];
								cout << endl;
								cout << "--> Username: " << account.get_username() << endl;
								cout << "--> Password: " << app.decrypt_password(account.get_password()) << endl;
							}

						}
					}
					else if (argc == 3) // user enter: pm find [service]
					{
						string service = argv[2];
						transform_to_lower_case(service);
						vector<Account> accounts = app.find_credentials_of_service(service);
						//cout << endl;
						//cout << "Finding credentials for service '" << service << "' from database" << endl;
						if (accounts.empty())
						{
							//cout << "[INFO] No account found for service '" << service << "'" << endl;
							throw std::runtime_error("No accounts found for service '" + service + "'");
						}
						else
						{
							cout << endl;
							print_accounts(accounts);
							cout << endl << "-> Choose an account to view: ";
							int choice;
							cin >> choice;
							if (choice < 0 || choice >= accounts.size())
							{
								cout << "[FAIL] Invalid choice." << endl;
							}
							else
							{
								Account account = accounts[choice];
								cout << endl;
								cout << "--> Username: " << account.get_username() << endl;
								cout << "--> Password: " << app.decrypt_password(account.get_password()) << endl;
							}
						}
					}
					else if (argc == 4) // user enter: pm find [service] [username]
					{
						string service = argv[2];
						string username = argv[3];
						transform_to_lower_case(service);
						vector<Account> accounts = app.find_one_credential_of_service_and_username(service, username);
						//cout << endl;
						//cout << "Finding credentials for service '" << service << "' with username '" << username << "' from database" << endl;
						if (accounts.empty())
						{
							//cout << "[INFO] No account found for service '" << service << "' with username '" << username << "'" << endl;
							throw std::runtime_error("No accounts found for service '" + service + "' with username '" + username + "'");
						}
						else if (accounts.size() > 1)
						{
							//cout << "[FAIL] Multiple accounts found for service'" << service << "' with username '" << username << "'" << endl;
							throw std::runtime_error("Multiple accounts found for service'" + service + "' with username '" + username + "'");
						}
						else if (accounts.size() == 1)
						{
							Account account = accounts[0];
							cout << endl;
							cout << "--> Username: " << account.get_username() << endl;
							cout << "--> Password: " << app.decrypt_password(account.get_password()) << endl;
						}

					}
				}

				else if (command == "add")
				{
					cout << endl;
					cout << "-----------------" << endl;
					cout << "USING ADD COMMAND" << endl;
					cout << "-----------------" << endl;

					string service, username, password, description;
					if (argc == 2) // user enter: pm add
					{
						cout << endl;
						cout << "-> Service: ";
						std::getline(cin, service);

						cout << "-> Username: ";
						cin >> username;
						cout << "-> Password: ";
						cin.ignore();
						password.assign(take_password_from_user());
						cout << "-> Description: ";
						std::getline(cin, description);

					}
					else if (argc == 3) // user enter: pm add [service]
					{
						service = argv[2];

						cout << endl;
						cout << "-> Username: ";
						cin >> username;
						cout << "-> Password: ";
						cin.ignore();
						password.assign(take_password_from_user());
						cout << "-> Description: ";
						std::getline(cin, description);

					}
					else if (argc == 4) // user enter: pm add [service] [username]
					{
						service = argv[2];
						username = argv[3];

						cout << endl;
						cout << "-> Password: ";
						password.assign(take_password_from_user());
						cout << "-> Description: ";
						std::getline(cin, description);
					}
					transform_to_lower_case(service);
					app.add_credential(service, username, password, description);
					cout << endl << "[SUCCESS] Credential added successfully." << endl;
				}

				else if (command == "edit")
				{
					cout << endl;
					cout << "------------------" << endl;
					cout << "USING EDIT COMMAND" << endl;
					cout << "------------------" << endl;

					string service, username, new_password;
					if (argc == 2) // user enter: pm edit -> list all accounts and choose one to edit
					{
						vector<Account> accounts = app.find_all_credentials();
						if (accounts.empty())
						{
							cout << endl << "[INFO] No accounts found." << endl;
							throw std::runtime_error("No accounts found.");
						}
						else
						{
							cout << endl;
							print_accounts(accounts);
							cout << endl << "-> Choose an account to edit: ";
							int choice;
							cin >> choice;
							cout << "-> New password: ";
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
							//cout << "[INFO] No account found for '" << service << "'" << endl;
							throw std::runtime_error("No accounts found for service '" + service + "'");
						}

						else
						{
							cout << endl;
							print_accounts(accounts);
							cout << endl << "-> Choose an account to edit: ";
							int choice;
							cin >> choice;
							cout << "-> New password: ";
							cin.ignore();
							new_password.assign(take_password_from_user());

							if (choice < 0 || choice >= accounts.size())
							{
								throw std::runtime_error("Invalid choice.");
							}

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
							//cout << "[INFO] No account found for '" << service << "' and '" << username << "'" << endl;
							throw std::runtime_error("No accounts found for service '" + service + "' and username '" + username + "'");
						}
						else if (accounts.size() > 1)
						{
							//cout << "[FAIL] Multiple accounts found for '" << service << "' and '" << username << "'" << endl;
							throw std::runtime_error("Multiple accounts found for service '" + service + "' and username '" + username + "'");
						}
						else if (accounts.size() == 1)
						{
							cout << endl;
							cout << "-> New password: ";
							new_password.assign(take_password_from_user());
						}
					}
					transform_to_lower_case(service);
					app.edit_credential(service, username, new_password);
					cout << endl << "[SUCCESS] Credential edited successfully." << endl;
				}

				else if (command == "remove")
				{
					cout << endl;
					cout << "--------------------" << endl;
					cout << "USING REMOVE COMMAND" << endl;
					cout << "--------------------" << endl;

					string service, username;
					if (argc == 2) // user enter: pm remove
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
							cout << endl << "-> Choose an account to remove: ";
							int choice;
							cin >> choice;
							if (choice < 0 || choice >= accounts.size())
							{
								throw std::runtime_error("Invalid choice.");
							}
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
							cout << endl;
							print_accounts(accounts);
							cout << endl << "-> Choose an account to remove: ";
							int choice;
							cin >> choice;
							if (choice < 0 || choice >= accounts.size())
							{
								throw std::runtime_error("Invalid choice.");
							}
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
						/*else if (accounts.size() == 1)
						{
							app.remove_credential(service, username);
						}*/
					}
					transform_to_lower_case(service);
					app.remove_credential(service, username);
					cout << endl << "[SUCCESS] Credential removed successfully." << endl;
				}

				else if (command == "reset")
				{
					cout << endl;
					cout << "-------------------" << endl;
					cout << "USING RESET COMMAND" << endl;
					cout << "-------------------" << endl;
					cout << endl;
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
					cout << endl;
					cout << "-----------------" << endl;
					cout << "USING GEN COMMAND" << endl;
					cout << "-----------------" << endl;

					cout << endl << "[SUCCESS] Generated password: " << app.generate_random_password() << endl;
				}

				else
				{
					throw std::runtime_error("Invalid command.");
				}
			}
		}
		catch (std::exception& e)
		{
			cout << endl << "[ERROR] " << e.what() << endl;
		}
	}
	return 0;
}


std::string take_password_from_user()
{
	// HANDLE hStdInput = GetStdHandle(STD_INPUT_HANDLE);
	// DWORD mode = 0;

	// // Create a restore point Mode, is know 503 
	// GetConsoleMode(hStdInput, &mode);

	// // Enable echo input, set to 499 
	// SetConsoleMode(hStdInput, mode & (~ENABLE_ECHO_INPUT));

	// Take input 
	string ipt;
	getline(cin, ipt);

	// Otherwise next cout will print into the same line 
	cout << endl;

	// Restore the mode 
	//SetConsoleMode(hStdInput, mode);
	return ipt;
}


void print_accounts(const vector<Account>& accounts)
{
	size_t i = 0;
	for (Account account : accounts)
	{
		string index = "[" + std::to_string(i) + "]";
		string service = "[" + account.get_service() + "]";
		string username = account.get_username();
		string description = account.get_description();
		cout << std::left << std::setfill('.') << std::setw(10) << index << std::left << std::setw(20) << service << std::setw(50) << username << description << endl;
		i++;
	}
}

bool login(PasswordManager& app)
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


bool login_with_system_variables(PasswordManager& app)
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
			cout << "[SUCCESS] Okay, welcome back!" << endl;
			return true;
		}
	}
	else
	{
		//cout << "[FAIL] Master password and username cannot be empty." << endl;
		throw std::runtime_error("Master password and username cannot be empty.");
	}
	return false;
}

void transform_to_lower_case(string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}
