#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "lsp.hpp"

namespace  py = pybind11;


PYBIND11_MODULE(semprlsp, m)
{
    py::class_<::sempr::RuleChecker::Error>(m, "RuleError")
        .def(py::init<>())
        .def_readonly("line", &::sempr::RuleChecker::Error::line)
        .def_readonly("start", &::sempr::RuleChecker::Error::start)
        .def_readonly("end", &::sempr::RuleChecker::Error::end)
        .def_readonly("description", &::sempr::RuleChecker::Error::description)
        .def("__str__", &::sempr::RuleChecker::Error::toString);


    py::class_<::sempr::RuleChecker>(m, "RuleChecker")
        .def(py::init<>())
        .def("getErrors", &::sempr::RuleChecker::getErrors)
        .def("lastDOT", &::sempr::RuleChecker::lastDOT)
        .def("listConditions", &::sempr::RuleChecker::listConditions)
        .def("listEffects", &::sempr::RuleChecker::listEffects);
}

