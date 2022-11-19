#pragma once

namespace Pebble {

enum DayOfWeek
{
    Sunday,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

class TimeSpan
{
public:
    TimeSpan(int seconds) : TimeSpan(0, 0, seconds) { } 
    TimeSpan(int hours, int minutes, int seconds) : TimeSpan(0, hours, minutes, seconds) { }
    TimeSpan(int days, int hours, int minutes, int seconds) {
        seconds_ = seconds % 60;
        minutes += (seconds / 60);
        minutes_ = minutes % 60;
        hours += (minutes / 60);
        hours_ = hours % 24;
        days_ = days + (hours / 24);
    }
    inline int Days() { return days_; }
    inline int Hours() { return hours_; }
    inline int Minutes() { return minutes_; }
    inline int Seconds() { return seconds_; }
protected:
    int days_;
    int hours_;
    int minutes_;
    int seconds_;
};

class DateTime
{
public:
    DateTime(time_t timestamp) : date_(*localtime(&timestamp))
    {
    }
    
    DateTime(const tm& tm) : date_(tm)
    {
    }
    
    DateTime(int year, int month, int day) : DateTime(year, month, day, 0, 0, 0)
    {
    }
    
    DateTime(int year, int month, int day, int hour, int minute, int second)
    {
        date_.tm_year = year-1900;
        date_.tm_mon = month-1;
        date_.tm_mday = day;
        date_.tm_hour = hour;
        date_.tm_min = minute;
        date_.tm_sec = second;
        Normalize();    
    }
    
    operator const ::tm*() const { return &date_; }
    
    inline int GetDay() const { return date_.tm_mday; }
    inline int GetMonth() const { return date_.tm_mon; }
    inline int GetYear() const { return date_.tm_year+1900; }
    inline int GetHour() const { return date_.tm_hour; }
    inline int GetMinute() const { return date_.tm_min; }
    inline int GetSecond() const { return date_.tm_sec; }
    inline DayOfWeek GetDayOfWeek() const { return (DayOfWeek)date_.tm_wday; }
    inline time_t GetTimestamp() const { return mktime(const_cast<tm*>(&date_)); }
    
    inline DateTime AddYears(int years) const { return DateTime(GetYear()+years, GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond()); }
    inline DateTime AddMonths(int months) const { return DateTime(GetYear(), GetMonth()+months, GetDay(), GetHour(), GetMinute(), GetSecond()); }
    inline DateTime AddDays(int days) const { return DateTime(GetYear(), GetMonth(), GetDay()+days, GetHour(), GetMinute(), GetSecond()); }
    inline DateTime AddHours(int hours) const { return DateTime(GetYear(), GetMonth(), GetDay(), GetHour()+hours, GetMinute(), GetSecond()); }
    inline DateTime AddMinutes(int minutes) const { return DateTime(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute()+minutes, GetSecond()); }
    inline DateTime AddSeconds(int seconds) const { return DateTime(GetYear(), GetMonth(), GetDay(), GetHour(), GetMinute(), GetSecond()+seconds); }
    
    inline size_t Format(char* s, size_t max, const char* fmt) const { return strftime(s, max, fmt, &date_); } 
    
    static inline DateTime Now() { return DateTime(time(NULL)); }
    
protected:
    inline void Normalize()
    {
        time_t timestamp = GetTimestamp();
        date_ = *localtime(&timestamp);
    }

    tm date_;
};

}