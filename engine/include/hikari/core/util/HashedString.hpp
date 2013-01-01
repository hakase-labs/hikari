#ifndef HIKARI_CORE_UTIL_HASHEDSTRING
#define HIKARI_CORE_UTIL_HASHEDSTRING

#include <hikari/core/Platform.hpp>
#include <cstddef>
#include <string>

namespace hikari {

   class HashedString;

   std::ostream& operator<<(std::ostream& message, const HashedString& string);

   class HIKARI_API HashedString {
   private:
     HashedString();
     std::size_t hash;
     std::string string;

   public:
     explicit HashedString(const std::string& string);
     ~HashedString();

     bool operator<(const HashedString& other) const;
     bool operator>(const HashedString& other) const;
     bool operator==(const HashedString& other) const;
     bool operator!=(const HashedString& other) const;
     const std::size_t getHash() const;
     const std::string& getString() const;
   };
}

#endif // HIKARI_CORE_UTIL_HASHEDSTRING
