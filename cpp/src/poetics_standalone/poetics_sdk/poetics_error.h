#ifndef POETICS_ERROR_H
#define POETICS_ERROR_H
#include <string>
using std::string;
#include <optional>
using std::optional;

namespace poetics
{
    class Error
    {
    private:
        optional<string> _message;
    public:
        Error() { _message = std::nullopt; }
        Error(string message) { _message = message; }
        optional<string> getMessage() { return _message; }
    };
}
#endif