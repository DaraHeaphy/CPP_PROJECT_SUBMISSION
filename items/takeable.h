#ifndef TAKEABLE_H
#define TAKEABLE_H

// abstract class
class Takeable
{
public:
    Takeable();
    // pure virtual function
    virtual void take() = 0;
};

#endif // TAKEABLE_H
