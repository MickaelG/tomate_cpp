
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
    virtual std::vector< std::unique_ptr< CalendarEvent > > GetEvents(bg::date start=bg::date(bg::neg_infin),
                                                                      bg::date end=bg::date(bg::pos_infin)) const = 0;

};

class CropsCalendar: public Calendar
{
public:
    CropsCalendar(const Crops& crops);
    std::vector< std::unique_ptr< CalendarEvent > > GetEvents(bg::date start=bg::date(bg::neg_infin),
                                                              bg::date end=bg::date(bg::pos_infin)) const override;

private:
    const Crops& _crops;

};

class CalendarEvent
{
public:
    virtual bg::date GetStartDate() const = 0;
    virtual bg::date GetEndDate() const;
    virtual std::string GetTitle() const = 0;

};

class CustomCalendarEvent: public CalendarEvent
{
public:
    CustomCalendarEvent(bg::date date, std::string title);
    bg::date GetStartDate() const override;
    std::string GetTitle() const override;

private:
    bg::date _date;
    std::string _title;
};

class CropCalendarEvent: public CalendarEvent
{
public:
    CropCalendarEvent(const Crop& crop, Crop::DateSel date_sel);
    bg::date GetStartDate() const override;
    std::string GetTitle() const override;

private:
    const Crop& _crop;
    Crop::DateSel _date_sel;
};

#endif
