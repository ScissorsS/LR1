#include <iostream>
#include <cstdlib>
#include <fstream>
#include <any>
#include <map>
#include <vector>

using namespace std;



class Json {
public:
    // Конструктор из строки, содержащей Json-данные.
    Json(const std::string& s): str(s) {}

    // Метод возвращает true, если данный экземпляр содержит в себе JSON-массив. Иначе false.
    bool is_array() const
    {
        if (str[position] != '[')
            return false;
        else return true;
    }
   	// Метод возвращает true, если данный экземпляр содержит в себе JSON-объект. Иначе false.
    bool is_object() const
    {
        if(str[position] != '{')
            return false;
        else return true;
    }

    // Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-массивом, генерируется исключение.
    std::any& operator[](const std::string& key)
    {
        auto it = parsed.find(key);
        //try(Json)
        return it->second;
    }

    // Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
    // Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
    // Если экземпляр является JSON-объектом, генерируется исключение.
    std::any& operator[](int index)
    {
        auto it = parsed.begin();
        advance(it, index);
        //try(Json)
        return it->second;
    }

private:
    string findKey (const string& s)
    {
        int start, finish;
        string key;
        start = s.find("\"", position);

        finish = s.find("\"", start+1);
        key.assign(s, start+1, finish-(start+1));
        position = finish;
        return key;
    }

    any findMean (const string& s)
    {
        string ss;
        any mean;
        position = s.find(":", position);

        if (s[position+1] == ' ')
            position += 2;

            if (s[position] == '\"')
            {
                ss.assign(s, position + 1, (s.find("\"", position + 1) - (position + 1)));
                position = s.find("\"", position+1);
                position++;
                return ss;
            }

            if ((s[position] >= '1') && (s[position] <= '9'))
            {
                do
                {
                    ss += s.at(position);
                    position++;
                }
                while (s[position] != ',');
                return atoi(ss.c_str());
            }

            if (is_object())
            {
                ss.assign(s, position, s.size()-position);
                position = s.find("}", position) + 1;
                return Json::parse(ss);
            }

            if (s[position] == 't')
            {
                return true;
            }

            if (s[position] == 'f')
            {
                return false;
            }

            if (is_array())
            {
                vector <string> numbers;
                numbers.push_back("");
                for (int i = 0; s[position] != ']';)
                {
                    if ((s[position] == '[') || (s[position] == '\t') || (s[position] == '\n'))
                    {
                        position++;
                    }
                    else if ((s[position] >= '1') && (s[position] <= '9'))
                    {
                        numbers[i] = s[position];
                        position++;
                    }
                    else if (s[position] == ',')
                    {
                        position++;
                        i++;
                        numbers.push_back("");
                    }
                    cout << ")" << numbers[i] << endl;
                }
                position++;
                return numbers;
            }
            return mean;
    }

public:
    // Метод возвращает объект класса Json из строки, содержащей Json-данные.
    static Json parse(const std::string& s)
    {
        Json object(s);
        object.position = 0;
        string ss = s;
        while(s[object.position] != '}')
        {
            string key;
            any mean;
            key = object.findKey(s);
            mean = object.findMean(s);
            object.parsed[key] = mean;

            while ((s[object.position] == '\t') || (s[object.position] == '\n') || (s[object.position] == ','))
                object.position++;
        }
        return object;
    }

    // Метод возвращает объекта класса Json из файла, содержащего Json-данные в текстовом формате.
    static Json parseFile(const std::string& path_to_file)
    {
        ifstream fin(path_to_file);
        string stringToParse("");
        while(!fin.eof())
        {
            string buf;
            getline(fin, buf);
            stringToParse += buf;
            stringToParse += '\n';
        }
        return Json::parse(stringToParse);
    }

    int position;
    std::string str;
    std::map <std::string, std::any> parsed;
};

std::string fileToString (const std::string &pathToFile)
{
    ifstream fin(pathToFile);
    string files("");
    while(!fin.eof())
    {
        string buf;
        getline(fin, buf);
        files += buf;
        files += '\n';
    }
    return files;
}

int main()
{
    string str = fileToString("JSON.txt");
    Json j = Json::parse(str);
    return 0;
}
