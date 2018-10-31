#ifndef SDLEXCEPTION_H
#define SDLEXCEPTION_H
#include <exception>
#include <string>
class SdlException : public std::exception {
  private:
	std::string message;

  public:
	SdlException(std::string s) : message(s) {}
	const char *what() const throw()
	{
		return message.c_str();
	}
};

#endif /* SDLEXCEPTION_H */
