
#ifndef CALENDAR_H
#define CALENDAR_H

#include <boost/date_time.hpp>
namespace bg = boost::gregorian;

#include <string>
#include <vector>

#include "crops.h"

class CalendarEvent;

class Calendar
{
public:
    Calendar(const Crops &crops);
    std::vector< std::unique_ptr< CalendarEvent > > GetEvents(bg::date start=bg::date(bg::neg_infin),
                                                               bg::date end=bg::date(bg::pos_infin));

private:
    const Crops& _crops;

};

class CalendarEvent
{
public:
    virtual bg::date GetDate() = 0;
    virtual std::string GetTitle() = 0;

};

class CustomCalendarEvent: public CalendarEvent
{
public:
    CustomCalendarEvent(bg::date date, std::string title);
    bg::date GetDate() override;
    std::string GetTitle() override;

private:
    bg::date _date;
    std::string _title;
};

class CropCalendarEvent: public CalendarEvent
{
public:
    CropCalendarEvent(const Crop& crop, Crop::DateSel date_sel);
    bg::date GetDate() override;
    std::string GetTitle() override;

private:
    const Crop& _crop;
    Crop::DateSel _date_sel;
};

#endif
