/*
 * Copyright (C) 2020 Matthieu Gautier <mgautier@kymeria.fr>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
 *
 */

#ifndef ZIM_ERROR_H
#define ZIM_ERROR_H

#include "zim.h"

#include <stdexcept>
#include <sstream>
#include <typeinfo>

namespace zim
{
  class ZimFileFormatError : public std::runtime_error
  {
    public:
      LIBZIM_API explicit ZimFileFormatError(const std::string& msg)
        : std::runtime_error(msg)
        { }
  };

  class InvalidType: public std::logic_error
  {
    public:
      LIBZIM_API explicit InvalidType(const std::string& msg)
        : std::logic_error(msg)
      {}
  };

  class EntryNotFound : public std::runtime_error
  {
    public:
      LIBZIM_API explicit EntryNotFound(const std::string& msg)
       : std::runtime_error(msg)
      {}
  };

  /* Exception thrown by the Creator in case of error.
   *
   * Most exceptions actually thrown are inheriting this exception.
   */
  class CreatorError : public std::runtime_error
  {
    public:
      LIBZIM_API explicit CreatorError(const std::string& message)
       : std::runtime_error(message)
      {}
  };

   /* Exception thrown when a entry cannot be added to the Creator.*/
  class InvalidEntry : public CreatorError
  {
    public:
      LIBZIM_API explicit InvalidEntry(const std::string& message)
       : CreatorError(message)
      {}
  };

  /* Exception thrown if a incoherence in the user implementation has been detected.
   *
   * Users need to implement interfaces such as:
   * - ContentProvider
   * - IndexData
   * - Item
   *
   * If a incoherence has been detected in those implementations a
   * `IncoherentImplementationError` will be thrown.
   */
  class IncoherentImplementationError : public CreatorError
  {
    public:
      LIBZIM_API explicit IncoherentImplementationError(const std::string& message)
       : CreatorError(message)
      {}
  };

  /* Exception thrown in the main thread when another exception has been
   * thrown in another worker thread.
   *
   * Creator uses different worker threads to do background work.
   * If an exception is thrown in one of this threads, it is catched and
   * "rethrown" in the main thread as soon as possible with a `AsyncError`.
   *
   * AsyncError contains the original exception. You can rethrow the original
   * exception using `rethrow`:
   *
   * ```
   * try {
   *   creator->addStuff(...);
   * } catch (const zim::AsyncError& e) {
   *   // An exception has been thrown in a worker thread
   *   try {
   *     e.rethrow();
   *   } catch (const std::exception& original_exception) {
   *     // original_exception is the exception thrown in the worker thread
   *     ...
   *   }
   * }
   * ```
   */
  class AsyncError : public CreatorError
  {
    public:
      LIBZIM_API explicit AsyncError(const std::exception_ptr exception)
       : CreatorError(buildErrorMessage(exception)),
         m_exception(exception)
      {}

      LIBZIM_API [[noreturn]] void rethrow() const {
        std::rethrow_exception(m_exception);
      }

    private: // data
      std::exception_ptr m_exception;

    private: // function
      static std::string buildErrorMessage(const std::exception_ptr exception) {
        try {
          std::rethrow_exception(exception);
        } catch (const std::exception& e) {
          std::stringstream ss;
          ss << "Asynchronous error: ";
          ss << typeid(e).name() << std::endl;
          ss << e.what();
          return ss.str();
        } catch (...) {
          return "Unknown asynchronous exception";
        }
      }
  };

  /* Exception thrown when the creator is in error state.
   *
   * If the creator is in error state (mostly because a AsyncError has already
   * being thrown), any call to any method on it will thrown a `CreatorStateError`.
   */
  class CreatorStateError : public CreatorError
  {
    public:
      LIBZIM_API explicit CreatorStateError()
       : CreatorError("Creator is in error state.")
      {}
  };
}

#endif // ZIM_ERROR_H

