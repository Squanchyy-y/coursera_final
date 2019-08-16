#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <map>
#include <set>
#include <exception>
using namespace std;

class Date { // создаем обьект для удобного хранения даты в общей базе и сортировки
public:
	Date (const string& s) {
		correctDate(s);
	}

	int GetYear () const {
		return year;
	}

	int GetMonth () const {
		return month;
	}

	int GetDay () const {
		return day;
	}

private:
	int year;
	int month;
	int day;
	void correctDate (const string& date_str) { // преобразование строки в обьект Date
		stringstream date_stream(date_str);
		bool flag = true;

		flag = flag && (date_stream >> year); 
		flag = flag && (date_stream.peek() == '-');
		date_stream.ignore(1);

		flag = flag && (date_stream >> month);
		flag = flag && (date_stream.peek() == '-');
		date_stream.ignore(1);

		flag = flag && (date_stream >> day);
		flag = flag && date_stream.eof();

		/*если был нарушен порядок ввода опускаем флаг, на формат года ограничений
		установлено не было, далее провепка формата месяца и дня согласно ТЗ*/

		if (!flag) {
			throw runtime_error("Wrong date format: " + date_str);
		}
		if (month < 1 || month > 12) {
			throw runtime_error("Month value is invalid: " + to_string(month));
		}
		if (day < 1 || day > 31) {
			throw runtime_error("Day value is invalid: " + to_string(day));
		}
	}
};


bool operator<(const Date& lhs, const Date& rhs) { //определяем для Date < , что бы использовать в map
	if (lhs.GetYear() != rhs.GetYear()) {
		return lhs.GetYear() < rhs.GetYear();
	} else if (lhs.GetMonth() != rhs.GetMonth()) {
		return lhs.GetMonth() < rhs.GetMonth();
	} else {
		return lhs.GetDay() < rhs.GetDay();
	}
}

class Database {
public:
	void AddEvent (const Date& date, const string& event) {
		event_list[date].insert(event); //добавляем введенное событие к указанной дате
	}

	/*удаление даты*/
	bool DeleteEvent (const Date& date, const string& event) {
		if (event_list.count(date) > 0 && event_list.at(date).count(event) > 0) {
			event_list.at(date).erase(event);// если существует дата и событие, удаляем
			return true; // и возвраем истину
		}
		return false; // ложь если события или даты не существует
	}
	
	/*удаление даты и всех существующих для нее событий*/
	int DeleteDate (const Date& date) {
		if (event_list.count(date) > 0) { // если дата была добавлена ранее
			int size_event = event_list.at(date).size(); // сохраняем колличество событий
			event_list.erase(date); // удаляем все
			return size_event; // возвращаем число удаленных событий
		}
		return 0; // если даты нет, возвращаем 0 событий, было упомянуто в условии
	}
	
	/*вывод всех событий определенной даты*/
	set<string> Find (const Date& date) const {
		set<string> s;
		if (event_list.count(date) > 0) { // проверяем есть ли такая дата
			s = event_list.at(date); // если есть возвращаем ее множество событий
		}
		return s;
	}

	/*обходим базу и печатаем все даты и события, не стал перегружать << ибо пока требуется 
	лишь в одном месте*/
	void Print () const {
		for (auto a : event_list) {
			for (auto b : a.second) {
				cout << setfill('0') << setw(4) << a.first.GetYear() << "-";
				cout << setfill('0') << setw(2) << a.first.GetMonth() << "-";
				cout << setfill('0') << setw(2) << a.first.GetDay() << " ";
				cout << b << endl;
			}
		}
	}

private:
	map<Date, set<string>> event_list;
};

int main () {
	/*заводим обьект базы и пользуемся уже созданными методами согласно условию задачи*/
	Database db;
	string s;

	while (getline(cin, s)) {
		try {
			stringstream stream (s);
			string command;
			stream >> command;

			if (command == "") continue;

			if (command == "Add") {
				string data_str, event;
				stream >> data_str >> event;
				Date date (data_str);
				db.AddEvent(date, event);
			} else if (command == "Del") {
				string data_str, event;
				stream >> data_str;
				Date date (data_str);

				if (!stream.eof()) {
					stream >> event;

					if (db.DeleteEvent (date, event)) {
						cout << "Deleted successfully" << endl;
					} else {
						cout << "Event not found" << endl;
					}
				} else {
					cout << "Deleted " << db.DeleteDate(date) <<" events" <<endl;
				}
			} else if (command == "Find") {
				string date_str;
				stream >> date_str;
				Date date(date_str);
				set<string> s = db.Find(date);

				for (auto item : s) {
					cout << item << endl;
				}

			} else if (command == "Print") {
				db.Print();
			} else {
				throw runtime_error ("Unknown command: " + command);
			}
		} catch (runtime_error& e) {
			cout << e.what() << endl;
		}
	}
	return 0;
}
