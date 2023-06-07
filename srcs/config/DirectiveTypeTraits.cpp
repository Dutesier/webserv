#include "config/DirectiveTypeTraits.hpp"

namespace config {

DirectiveTypeTraitsBase::DirectiveTypeTraitsBase(std::string const& name)
    : m_name(name) {}

DirectiveTypeTraitsBase::~DirectiveTypeTraitsBase(void) {}

bool DirectiveTypeTraitsBase::operator==(
    DirectiveTypeTraitsBase const& rhs) const {
    LOG_D("Here");
    LOG_D("name: " << m_name << " rhs: " << rhs.m_name);
    return (m_name == rhs.m_name);
}

std::string const DirectiveTypeTraitsBase::getName(void) { return (m_name); }

} // namespace config