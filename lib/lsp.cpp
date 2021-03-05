#include "lsp.hpp"
#include <sstream>
#include <fstream>

namespace sempr {

std::string RuleChecker::Error::toString() const
{
    std::stringstream ss;
    ss << line << ":" <<  start << "-" << end << " -- " << description;
    return ss.str();
}

RuleChecker::RuleChecker()
{
    core_.loadPlugins();

    // remove default-rules
    auto rules = core_.rules();
    for (auto rule : rules)
    {
        core_.removeRule(rule->id());
    }
}

std::string RuleChecker::lastDOT() const
{
    return lastDOT_;
}


std::vector<std::string> RuleChecker::listConditions() const
{
    return const_cast<Core&>(core_).parser().listAvailableConditions();
}

std::vector<std::string> RuleChecker::listEffects() const
{
    return const_cast<Core&>(core_).parser().listAvailableEffects();
}


std::vector<RuleChecker::Error> RuleChecker::getErrors(const std::string& rules)
{
    std::vector<RuleChecker::Error> errs;

    std::vector<size_t> parsed;

    try {
        parsed = core_.addRules(rules);
//        core_.reasoner().performInference(1000);

        lastDOT_ = core_.reasoner().net().toDot();
//        std::ofstream("lsp_last.dot") << lastDOT_;

    } catch (rete::ParserExceptionLocalized& e) {
        RuleChecker::Error err;

        err.line = e.row();
        err.start = e.colStart();
        err.end = e.colEnd();
        err.description = e.what();

        errs.push_back(err);

        std::cout << err.toString() << std::endl;
    } catch (rete::NoBuilderException& e) {
        RuleChecker::Error err;

        auto rule = e.getRule();
        auto part = e.getPart();
        auto startRule = std::search(rules.begin(), rules.end(), rule.begin(), rule.end());
        auto endRule = startRule + e.getRule().size();
        auto startErr = std::search(startRule, endRule, part.begin(), part.end());
        auto row = std::count(rules.begin(), startErr, '\n');
        auto rStartErr = rules.rbegin() + (rules.size() - std::distance(rules.begin(), startErr));
        auto rLastNL = std::find(rStartErr, rules.rend(), '\n');
        auto col = std::distance(rStartErr, rLastNL);

        err.line = row;
        err.start = col;
        err.end = col + e.getPart().size();
        err.description = "'" + e.unknown() + "' unknown. Did you mean '" + e.bestCandidate() + "'?";

        errs.push_back(err);

//    } catch (rete::NodeBuilderException& e) {
    } catch (rete::RuleConstructionException& e) {
        RuleChecker::Error err;

        auto rule = e.getRule();
        auto part = e.getPart();
        auto startRule = std::search(rules.begin(), rules.end(), rule.begin(), rule.end());
        auto endRule = startRule + e.getRule().size();
        auto startErr = std::search(startRule, endRule, part.begin(), part.end());
        auto row = std::count(rules.begin(), startErr, '\n');
        auto rStartErr = rules.rbegin() + (rules.size() - std::distance(rules.begin(), startErr));
        auto rLastNL = std::find(rStartErr, rules.rend(), '\n');
        auto col = std::distance(rStartErr, rLastNL);

        err.line = row;
        err.start = col;
        err.end = col + e.getPart().size();
        err.description = e.getDetail() + " -- at: " + e.getPart();

        errs.push_back(err);

    } catch (std::exception& e) {
        std::cerr << "uncaught exception: " << e.what() << std::endl;

        RuleChecker::Error err;
        err.line = 0;
        err.start = 0;
        err.end = 0;
        err.description = std::string("uncaught exception: \n") + e.what();

        errs.push_back(err);
    }

    // cleanup
    for (auto rule : parsed)
    {
        core_.removeRule(rule);
    }
    core_.reasoner().performInference();

    // return result
    return errs;
}

}
