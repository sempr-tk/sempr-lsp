#ifndef SEMPR_LSP_HPP_
#define SEMPR_LSP_HPP_

#include <string>
#include <vector>

#include <sempr/sempr.hpp>

namespace sempr {

/**
    The only purpose of this class is to provide a context in which we can
    try to parse some rules and report any errors.
*/
class RuleChecker {
public:

    struct Error {
        int line;
        int start, end;
        std::string description;
        std::string toString() const;
    };

    RuleChecker();
    std::vector<Error> getErrors(const std::string& rules);
    std::string lastDOT() const;


private:
    Core core_;
    std::string lastDOT_;
};


}


#endif /* include guard: SEMPR_LSP_HPP_ */
