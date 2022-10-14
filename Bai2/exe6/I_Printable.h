#ifndef _I_PRINTABLE_H_
#define _I_PRINTABLE_H
#include <ostream>

class I_Printable {
    friend std::ostream& operator<<(std::ostream&, const I_Printable&obj);
public:
    virtual void print (std::ostream& out) const = 0;
    virtual ~I_Printable() = default;
};

#endif