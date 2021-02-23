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
    };

    RuleChecker();
    std::vector<Error> getErrors(const std::string& rules);


private:
    Core core_;
};


}


#endif /* include guard: SEMPR_LSP_HPP_ */
