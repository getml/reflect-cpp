#ifndef RFL_INTERNAL_ERROR_HPP_
#define RFL_INTERNAL_ERROR_HPP_

#include <string>

namespace rfl::internal {

/// Defines the error class to be returned when something went wrong.
/// This is used throughout reflect-cpp for error handling.
class Error {
 public:
  /// Default constructor - creates an empty error.
  Error() = default;

  /// Constructs an error with a message (copy).
  /// @param _what The error message describing what went wrong
  Error(const std::string& _what) : what_(_what) {}

  /// Constructs an error with a message (move).
  /// @param _what The error message describing what went wrong (will be moved)
  Error(std::string&& _what) : what_(std::move(_what)) {}

  /// Destructor.
  ~Error() = default;

  /// Copy constructor.
  /// @param e The error to copy from
  Error(const Error& e) = default;

  /// Move constructor.
  /// @param e The error to move from
  Error(Error&& e) noexcept = default;

  /// Copy assignment operator.
  /// @param _other The error to copy from
  /// @return Reference to this error
  Error& operator=(const Error& _other) = default;

  /// Move assignment operator.
  /// @param _other The error to move from
  /// @return Reference to this error
  Error& operator=(Error&& _other) noexcept = default;

  /// Returns the error message, equivalent to .what() in std::exception.
  const std::string& what() const& { return what_; }
  /// Moves the error message out of Error object and leaves what_ in a moved
  /// from state
  std::string what() && { return std::move(what_); }

 private:
  /// Documents what went wrong
  std::string what_;
};

}  // namespace rfl::internal

#endif
