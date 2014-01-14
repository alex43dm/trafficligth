#ifndef WAY_H
#define WAY_H

#define on true
#define off false

typedef enum
{
    People,
    Tranport
} WayTypes;

class Way
{
public:

    int id;
    WayTypes type;
    bool state;

    Way();
    virtual ~Way();
protected:
private:
};

#endif // WAY_H
