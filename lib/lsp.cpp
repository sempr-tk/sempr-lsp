#include "lsp.hpp"

namespace sempr {

RuleChecker::RuleChecker()
{
    core_.loadPlugins();
}

std::vector<RuleChecker::Error> RuleChecker::getErrors(const std::string& rules)
{
    std::vector<RuleChecker::Error> errs;

    try {
        auto parsed = core_.addRules(rules);
        for (auto rule : parsed)
        {
            core_.removeRule(rule);
        }
    } catch (rete::ParserException& e) {
        RuleChecker::Error err;

        // TODO: exceptions thrown by rete do not contain better information yet
        err.line = 0;
        err.start = 0;
        err.end = 0;

        err.description = e.what();
        errs.push_back(err);
    }

    return errs;
}

}
