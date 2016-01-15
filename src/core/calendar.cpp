
#include "calendar.h"

#include <memory>

using namespace std;

class EventIterator
{
public:
    EventIterator();
    const CalendarEvent& operator*() const;
    const CalendarEvent& operator->() const;
    EventIterator& operator++();
    bool equals(const EventIterator& rhs) const;

private:
};

inline bool operator==(const EventIterator& lhs, const EventIterator& rhs)
{
    return lhs.equals(rhs);
}

inline bool operator!=(const EventIterator& lhs, const EventIterator& rhs)
{
    return !lhs.equals(rhs);
}

EventIterator::EventIterator()
{
}

const CalendarEvent& EventIterator::operator*() const
{
    //TODO
}

const CalendarEvent& EventIterator::operator->() const
{
    //TODO
}

EventIterator& EventIterator::operator++()
{
    //TODO: increment
    return *this;
}

bool EventIterator::equals(const EventIterator &rhs) const
{
    //TODO
}

///////////////////////////////////////////////////////////////////////////////


CropsCalendar::CropsCalendar(const Crops& crops) : _crops(crops)
{
}

vector< unique_ptr< CalendarEvent > >
CropsCalendar::GetEvents(bg::date start, bg::date end) const
{
    std::vector< std::unique_ptr< CalendarEvent > > result;
    bg::date_period period(start, end + bg::date_duration(1));
    for (auto& crop: _crops) {
        for (Crop::DateSel date_sel: {Crop::DateSel::START, Crop::DateSel::END,
                                      Crop::DateSel::PLANNED_START, Crop::DateSel::PLANNED_END} ) {
            bg::date date = crop.get_date(date_sel);
            if (!date.is_special() && period.contains(date)) {
                result.push_back(unique_ptr<CalendarEvent>(new CropCalendarEvent(crop, date_sel)));
            }
        }
    }
    return result;
}

bg::date CalendarEvent::GetEndDate() const
{
    return GetStartDate();
}

CustomCalendarEvent::CustomCalendarEvent(bg::date date, std::string title) :
    _date(date), _title(title)
{
}

bg::date CustomCalendarEvent::GetStartDate() const
{
    return _date;
}

string CustomCalendarEvent::GetTitle() const
{
    return _title;
}

CropCalendarEvent::CropCalendarEvent(const Crop& crop, Crop::DateSel date_sel) :
    _crop(crop), _date_sel(date_sel)
{
}

bg::date CropCalendarEvent::GetStartDate() const
{
    return _crop.get_date(_date_sel);
}

string CropCalendarEvent::GetTitle() const
{
    return _crop.get_repr();
}
