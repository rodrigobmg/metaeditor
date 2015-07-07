#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include "observer.h"

namespace Data {
namespace Patterns {

class Subject
{
public:
    Subject();


    void Attach(const Observer* observer);
    void Dettach(const Observer* observer);
    void Notify();

private:
    std::vector<const Observer*> _observers;
};

}}
#endif // SUBJECT_H
