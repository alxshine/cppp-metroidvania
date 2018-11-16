#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

namespace game {
class GameException : public std::exception {
  protected:
	std::string message;

  public:
	GameException(std::string s) : message(s){};
	const char *what() const noexcept
	{
		return message.c_str();
	}
};

class ResourceNotFoundException : public GameException {
  public:
	ResourceNotFoundException(std::string s) : GameException(s) {}
};
} // namespace game
#endif /* EXCEPTIONS_H */
