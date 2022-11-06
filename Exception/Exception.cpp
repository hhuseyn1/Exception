#include <iostream>
#include <windows.h>
#include<vector>
#include<string>

using namespace std;

class Exception :public exception {
protected:
	string _message;
	int _line;
	const char* _time;
public:
	Exception(const char* message = "Exception",const char* time = "",int line = 0): exception(message)
	{
		_time = time;
		_line = line;


		_message = "Message: " + (string)exception::what()
			+ "\nTime: " + _time
			+ "\nLine: " + to_string(line);
	}


	int line() const { return _line; }
	const char* time() const { return _time; }
	const char* message() const { return exception::what(); }


	const char* what() const override
	{
		return _message.c_str();;
	}

};
class DatabaseException : public Exception {

public:
	DatabaseException()
	{
		throw Exception("DatabaseException", __TIME__, __LINE__);
	}
};
class InvalidArgumentException :public Exception {
public:
	InvalidArgumentException()
	{
		throw Exception("InvalidArgumentException", __TIME__, __LINE__);
	}
};


class User {
	static int staticId;
	int id;
	string username;
	string password;
	string name;
	string surname;

public:

	User(string username, string password, string name, string surname)
		:username(username), password(password), name(name), surname(surname)
	{
		id = ++staticId;
	}
	string getUsername() { return username; }
	string getPassword() { return password; }
	string getName() { return name; }
	string getSurname() { return surname; }
	int getID() { return id; }

	friend ostream& operator<<(ostream& output, const User& u);
	bool operator==(const User& other)
	{
		if (id == other.id && username == other.username && password == other.password && name == other.name && surname == other.surname)
			return true;
		return false;
	}
};

int User::staticId = 0;
ostream& operator<<(ostream& output, const User& u)
{
	cout << "Id: " << u.id << endl;
	cout << "Username: " << u.username << endl;
	cout << "Password: ";
	for (size_t i = 0; i < u.password.size(); i++)
	{
		cout << "*";
	}
	cout << endl;
	cout << "Name: " << u.name << endl;
	cout << "Surname: " << u.surname << endl;
	return output;
}

class Database {
	vector<User>users;
public:
	void addUser(const User& user)
	{
		users.push_back(user);
	}

	User& getUserByUsername(string username)
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].getUsername() == username)
				return users[i];
		}
	}
	void updateUser(User& olduser, const User& newuser)
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i] == olduser)
				users[i] = newuser;

		}
	}

	User* getUsers()
	{
		return users.data();
	}

	int getSize()
	{
		return users.size();
	}

	void hashUserDataById(const int& id)
	{
		for (size_t i = 0; i < users.size(); i++)
		{
			if (users[i].getID() == id)
			{
				users[i].getPassword() = to_string(stoi(users[i].getPassword()) ^ 15);
				return;
			}
		}
		throw exception("There is no such user");
	}


};



class Registration {
	Database _database;
public:
	Registration(const Database& database) {
		_database = database;
	}

	bool signIn(string username, string password)
	{
		for (size_t i = 0; i < _database.getSize(); i++)
		{
			if (_database.getUsers()[i].getUsername() == username && _database.getUsers()[i].getPassword() == password)
				return true;

		}
		throw exception("Wrong password or username");
	}

	Database getDatabase() { return _database; }

	void signUp(User user) {

		if (user.getUsername().length() < 6 || (user.getUsername()[0] >= 97 && user.getUsername()[0] <= 122) || user.getPassword().length() < 6 ||
			user.getName().length() < 3 || user.getSurname().length() < 4)
			throw InvalidArgumentException();
		for (size_t i = 0; i < _database.getSize(); i++)
		{
			if (_database.getUsers()[i].getUsername() == user.getUsername())
				throw DatabaseException();
		}
		_database.addUser(user);
	}
};



void main() {
	Database db;
	Registration Reg(db);
	User user("Huseyn123", "huseyn123", "Huseyn", "Hemidov");
	try
	{
		Reg.signUp(user);
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}

	try
	{
		Reg.signIn("Huseyn123", "huseyn123") ? cout << "Welcome " << user.getName() << " !!!" << endl : cout << "";
	}
	catch (exception& ex)
	{
		cout << ex.what() << endl;
	}


	for (size_t i = 0; i < Reg.getDatabase().getSize(); i++)
	{
		cout << Reg.getDatabase().getUsers()[i] << endl;
	}
	system("pause");
}
