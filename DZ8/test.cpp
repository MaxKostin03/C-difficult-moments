#include "pch.h"
#include <iostream>
#include <string>
#include <tuple>
#include <optional>
#include <iomanip>
#include <fstream>
#include <vector>
#include <tuple>
#include <algorithm>

using namespace std;
//---------------------------1----------------------------

struct Person
{

    Person();

    Person(const string surname, const string name, optional<string> patronymic);

    Person(const string surname, const string name);

    friend ostream& operator <<(ostream& out, const Person& person);

    friend bool operator == (const Person& lhs, const Person& rhs);

    friend bool operator <(const Person& lhs, const Person& rhs);

    string surname_;

    string name_;

    optional<string> patronymic_;
};

Person::Person() : surname_(""), name_(""), patronymic_("") {}

Person::Person(const string surname, const string name, optional<string> patronymic) : surname_(surname), name_(name), patronymic_(patronymic) {}

Person::Person(const string surname, const string name) : Person::Person(surname, name, "") {}

ostream& operator<<(ostream& out, const Person& person)
{
    out << setw(15) << person.surname_ << setw(12) << person.name_ << " ";

    if (person.patronymic_)
        out << setw(17) << person.patronymic_.value();
    else out << setw(18);


    return out;
}

bool operator==(const Person& lhs, const Person& rhs)
{
    return tie(lhs.name_, lhs.surname_, lhs.patronymic_) == tie(rhs.name_, rhs.surname_, rhs.patronymic_);
}

bool operator<(const Person& lhs, const Person& rhs)
{
    return tie(lhs.name_, lhs.surname_, lhs.patronymic_) < tie(rhs.name_, rhs.surname_, rhs.patronymic_);
}


//---------------------------2----------------------------

struct PhoneNumber
{
    PhoneNumber();

    PhoneNumber(int countryCode, int cityCode, string number, optional<int> additionalNumber);

    PhoneNumber(int countryCode, int cityCode, string number);

    friend ostream& operator << (ostream& out, PhoneNumber pn);

    int countryCode_;

    int cityCode_;

    string number_;

    optional<int> additionalNumber_;
};

PhoneNumber::PhoneNumber() : countryCode_(-1), cityCode_(-1), number_(""), additionalNumber_(nullopt) {}

PhoneNumber::PhoneNumber(int countryCode, int cityCode,
    string number, optional<int> additionalNumber) : countryCode_(countryCode), cityCode_(cityCode), number_(number), additionalNumber_(additionalNumber) {}

PhoneNumber::PhoneNumber(int countryCode, int cityCode, string number) : PhoneNumber::PhoneNumber(countryCode, cityCode, number, nullopt) {}

ostream& operator<<(ostream& out, PhoneNumber pn)
{
    if (pn.countryCode_ < 0)
    {
        out << "NO NUMBER";
        return out;
    }

    out << setw(3) << "+" << pn.countryCode_ << "(" << pn.cityCode_ << ")" << pn.number_ << " ";
    if (pn.additionalNumber_)
        out << pn.additionalNumber_.value();
    return out;
}


//---------------------------3----------------------------

class PhoneBook
{
private:

    vector<pair<Person, PhoneNumber>> phoneBook_;

public:

    PhoneBook(ifstream& file) {
        if (file.is_open())
        {
            string tmpName, tmpSurname, tmpPatronymic,
                tmpNumber, tmpCountryCode, tmpCityCode, tmpAdditionalNumber;
            optional<string> patronymic;
            optional<int>additionalNumber;
            pair<Person, PhoneNumber> tmpPair;

            while (!file.eof())
            {
                getline(file, tmpSurname, '|');
                getline(file, tmpName, '|');
                getline(file, tmpPatronymic, '|');

                if (tmpPatronymic == "-")
                    patronymic = nullopt;
                else
                    patronymic = tmpPatronymic;

                tmpPair.first = Person(tmpSurname, tmpName, patronymic);

                getline(file, tmpCountryCode, '|');
                getline(file, tmpCityCode, '|');
                getline(file, tmpNumber, '|');
                getline(file, tmpAdditionalNumber, '\n');

                if (tmpAdditionalNumber == "-")
                    additionalNumber = nullopt;
                else
                    additionalNumber = stoi(tmpAdditionalNumber);

                tmpPair.second = PhoneNumber(stoi(tmpCountryCode), stoi(tmpCityCode),
                    tmpNumber, additionalNumber);

                phoneBook_.push_back(tmpPair);
            }
        }
    };

    friend ostream& operator <<(ostream& out, const PhoneBook& pb);

    void SortByName();

    void SortByPhone();

    pair<string, PhoneNumber> GetPhoneNumber(string surname);

    void ChangePhoneNumber(const Person& person, const PhoneNumber& newNumber);


};

ostream& operator<<(ostream& out, const PhoneBook& pb)
{
    for (const auto& i : pb.phoneBook_)
    {
        out << i.first << " " << i.second << endl;
    }
    return out;
}

void PhoneBook::SortByName()
{
    sort(phoneBook_.begin(), phoneBook_.end(),
        [](const auto& lhs, const auto& rhs)
        {
            if (lhs.first.surname_ != rhs.first.surname_)
                return lhs.first.surname_ < rhs.first.surname_;
            else if (lhs.first.name_ != rhs.first.name_)
                return lhs.first.name_ < rhs.first.name_;
            else
                return lhs.first.patronymic_ < rhs.first.patronymic_;
        });
}

void PhoneBook::SortByPhone()
{
    sort(phoneBook_.begin(), phoneBook_.end(), [](const auto& lhs, const auto& rhs)
        {
            if (lhs.second.countryCode_ != rhs.second.countryCode_)
                return lhs.second.countryCode_ < rhs.second.countryCode_;
            else if (lhs.second.cityCode_ != rhs.second.cityCode_)
                return lhs.second.cityCode_ < rhs.second.cityCode_;
            else if (lhs.second.number_ != rhs.second.number_)
                return lhs.second.number_ < rhs.second.number_;
            else
                return lhs.second.additionalNumber_ < rhs.second.additionalNumber_;
        });
}

pair<string, PhoneNumber> PhoneBook::GetPhoneNumber(string surname)
{
    vector<PhoneNumber> phoneNumbers;
    for_each(phoneBook_.begin(), phoneBook_.end(),
        [&phoneNumbers, &surname](const auto& value)
        {
            string tmpSurname = value.first.surname_;
            transform(surname.begin(), surname.end(),
                surname.begin(), [](unsigned char c)
                {
                    return tolower(c);
                });

            transform(tmpSurname.begin(), tmpSurname.end(),
                tmpSurname.begin(), [](unsigned char c)
                {
                    return tolower(c);
                });

            if (tmpSurname == surname)
            {
                phoneNumbers.push_back(value.second);
            }
        });

    if (phoneNumbers.size() == 0)
        return { "Not found", PhoneNumber() };
    else if (phoneNumbers.size() == 1)
        return { "", phoneNumbers.at(0) };
    else
        return { "Found more than one",
                phoneNumbers.at(rand() % (phoneNumbers.size() - 1)) };
}

void PhoneBook::ChangePhoneNumber(const Person& person, const PhoneNumber& newNumber)
{
    auto result = find_if(phoneBook_.begin(), phoneBook_.end(),
        [&person](const auto& value)
        {
            return value.first == person;
        });

    if (result != phoneBook_.end())
    {
        result->second = newNumber;
        return;
    }
    else { return; }
}

class PhoneBookTest : public testing::Test
{
protected:
    void SetUp() override
    {
        ifstream file("D:\\PhoneBook.txt");
        book = new PhoneBook(file);
    }
    void TearDown() override
    {
        delete book;
    }
    PhoneBook* book;
};


TEST_F(PhoneBookTest, SortByName)
{
    book->SortByName();
    stringstream ss;
    ss << *book;
    std::string str;
    str = ss.str();
    size_t count = 0;
    for (auto iter : str)
    {
        if (iter == ' ')
        {
            count++;
            continue;
        }
        break;
    }
    str = str.substr(count, (sizeof("Aleksandrov") / sizeof(char)) - 1);
    ASSERT_TRUE(str == "Aleksandrov");
}
TEST_F(PhoneBookTest, SortByPhone)
{
    book->SortByPhone();
    stringstream ss;
    ss << *book;
    string str;
    str = ss.str();
    size_t count = 0;
    for (auto iter : str)
    {
        if (iter == ' ')
        {
            count++;
            continue;
        }
        break;
    }
    str = str.substr(count, (sizeof("Solovev") / sizeof(char)) - 1);
    ASSERT_TRUE(str == "Solovev");
}

int main()
{
    ifstream file("D:\\PhoneBook.txt", ios_base::in);

    PhoneBook pb(file);

    cout << pb << "\n\n";

    pb.SortByName();

    cout << "==================== SortByName() ====================\n\n";

    cout << pb << "\n\n";

    pb.SortByPhone();

    cout << "==================== SortByPhone() ====================\n\n";

    cout << pb << "\n\n";

    auto numberUpperCaseSurname = pb.GetPhoneNumber("ILIN");
    auto numberLowerCaseSurname = pb.GetPhoneNumber("ilin");
    auto numberCombinedSurname = pb.GetPhoneNumber("Ilin");
    auto numberNoMatch = pb.GetPhoneNumber("abcabcabc");

    cout << "==================== GetPhoneNumber() =================\n\n";

    cout << numberUpperCaseSurname.first << " " << numberUpperCaseSurname.second << "\n\n";
    cout << numberLowerCaseSurname.first << " " << numberLowerCaseSurname.second << "\n\n";
    cout << numberCombinedSurname.first << " " << numberCombinedSurname.second << "\n\n";
    cout << numberNoMatch.first << " " << numberNoMatch.second << "\n\n";

    Person person("Ilin", "Petr", "Artemovich");

    PhoneNumber pn(1, 2, "345678", 12);

    pb.ChangePhoneNumber(person, pn);



    pb.SortByPhone();

    cout << "==================== ChangePhoneNumber() ====================\n\n";

    cout << pb << "\n\n";

    cout << "==================== Test ===================================\n\n";


    testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
