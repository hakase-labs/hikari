#include <hikari/core/util/HashedString.hpp>
#include <functional>

namespace hikari {

   std::ostream& operator<<(std::ostream& message, const HashedString& string) {
     message << string.getString();
     return message;
   }

   HashedString::HashedString(const std::string& string)
     : hash(0)
     , string(string) {
     std::hash<std::string> hashingFunction;
     hash = hashingFunction(string);
   }

   HashedString::~HashedString() { }

   bool HashedString::operator<(const HashedString& other) const {
     return (getHash() < other.getHash());
   }

   bool HashedString::operator>(const HashedString& other) const {
     return (getHash() > other.getHash());
   }

   bool HashedString::operator==(const HashedString& other) const {
     return (getHash() == other.getHash());
   }

   bool HashedString::operator!=(const HashedString& other) const {
     return (getHash() != other.getHash());
   }

   const std::size_t HashedString::getHash() const {
     return hash;
   }

   const std::string& HashedString::getString() const {
     return string;
   }

} // hikari
