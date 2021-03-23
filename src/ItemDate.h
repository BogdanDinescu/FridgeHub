#include <iostream>
#include <vector>

class ItemDate {
public:
    int day, month, year;
    ItemDate() {}
    ItemDate(int d,int m, int y) {
        day = d;
        month = m;
        year = y;
    }
    static ItemDate stringToItemDate(std::string s)
    {
        const char delim = '/'; 
        std::vector<std::string> out;
        tokenize(s, delim, out);
        ItemDate date;
        date.day = stoi(out[0]);
        date.month = stoi(out[1]);
        date.year = stoi(out[2]);
        return date;
    }
    int getDay() const{
        return this->day;
    }
    int getMonth() const{
        return this->month;
    }
    int getYear() const{
        return this->year;
    }
    bool operator<(const ItemDate &x)
    {
        if (this->year < x.getYear())
        {
            return true;
        }
        else if (this->year > x.getYear())
        {
            return false;
        }
        
        if (this->month < x.getMonth()) {
            return true;
        } 
        else if (this->month > x.getMonth())
        {
            return false;
        } 
        
        if (this->day < x.getDay()) {
            return true;
        }
        else if (this->day > x.getDay())
        {
            return false;
        }
        
        return false;
    }
    bool operator>(ItemDate &x)
    {
        if (this->year > x.getYear())
        {
            return true;
        }
        else if (this->year < x.getYear())
        {
            return false;
        }
        
        if (this->month > x.getMonth()) {
            return true;
        } 
        else if (this->month < x.getMonth())
        {
            return false;
        } 
        
        if (this->day > x.getDay()) {
            return true;
        }
        else if (this->day < x.getDay())
        {
            return false;
        }
        
        return false;
    }
    std::string getString()
    {
        return std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year); 
    }
private:
    static void tokenize(std::string const &str, const char delim,
                std::vector<std::string> &out)
    {
        size_t start;
        size_t end = 0;
    
        while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
        {
            end = str.find(delim, start);
            out.push_back(str.substr(start, end - start));
        }
    }
}; 