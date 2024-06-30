#include <iostream>
#include <fstream>
#include <string>
#include <map>

class BankAccount
{
	long long sum_{};

public:
	BankAccount(int sum = 0)
		: sum_(sum) {}

	void deposit(long long sum) { sum_ += sum; }
	void withdraw(long long sum) { sum_ -= sum; }
	long long  balance() const { return sum_; }
	void income(double p) { if (sum_ > 0) sum_ *= (1 + p * 0.01); }
	void transfer(BankAccount& person, long long sum)
	{
		sum_ -= sum;
		person.sum_ += sum;
	}
};

static void deposit(std::map<std::string, BankAccount>& bank, std::fstream& in)
{
	std::string name;
	long long sum;
	in >> name >> sum;

	if (!bank.count(name))
		bank[name] = BankAccount(sum);
	else
		bank[name].deposit(sum);
}

static void withdraw(std::map<std::string, BankAccount>& bank, std::fstream& in)
{
	std::string name;
	long long sum;
	in >> name >> sum;

	if (!bank.count(name))
		bank[name] = BankAccount(-sum);
	else
		bank[name].withdraw(sum);
}

static void balance(std::map<std::string, BankAccount>& bank, std::fstream& in)
{
	std::string name;
	in >> name;

	static int count{ 0 };
	if (count != 0)
		std::cout << std::endl;

	if (!bank.count(name))
		std::cout << "ERROR";
	else
		std::cout << bank[name].balance() ;
	++count;
}

static void transfer(std::map<std::string, BankAccount>& bank, std::fstream& in)
{
	std::string name1, name2;
	long long sum;
	in >> name1 >> name2 >> sum;

	if (!bank.count(name1))
		bank[name1] = BankAccount();
	if (!bank.count(name2))
		bank[name2] = BankAccount();
	bank[name1].transfer(bank[name2], sum);
}

static void income(std::map<std::string, BankAccount>& bank, std::fstream& in)
{
	double p;
	in >> p;
	for (auto& i : bank)
		i.second.income(p);
}

static void write(std::map<std::string, BankAccount>& bank)
{
	std::fstream in("input.txt");

	if (in.is_open())
	{
		std::string operation;
		while (in >> operation)
		{
			if (operation == "DEPOSIT")
				deposit(bank, in);
			else if (operation == "WITHDRAW")
				withdraw(bank, in);
			else if (operation == "BALANCE")
				balance(bank, in);
			else if (operation == "TRANSFER")
				transfer(bank, in);
			else if (operation == "INCOME")
				income(bank, in);
			//std::cout << '\n';
		}
	}

	in.close();
}

int main()
{
	std::map<std::string, BankAccount> bank;
	write(bank);
	return 0;
}