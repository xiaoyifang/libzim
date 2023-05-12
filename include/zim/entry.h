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

#ifndef ZIM_ENTRY_H
#define ZIM_ENTRY_H

#include "zim.h"

#include <string>
#include <memory>

namespace zim
{
  class Item;
  class Dirent;
  class FileImpl;

  /**
   * An entry in an `Archive`.
   *
   * All `Entry`'s methods are threadsafe.
   */
  class Entry
  {
    public:
      LIBZIM_API explicit Entry(std::shared_ptr<FileImpl> file_, entry_index_type idx_);

      LIBZIM_API bool isRedirect() const;
      LIBZIM_API std::string getTitle() const;
      LIBZIM_API std::string getPath() const;

      /** Get the item associated to the entry.
       *
       * An item is associated only if the entry is not a redirect.
       * For convenience, if follow is true, return the item associated to the targeted entry.
       *
       * @param follow True if the redirection is resolved before getting the item. (false by default)
       * @return The Item associated to the entry.
       * @exception InvalidType if the entry is a redirection and follow is false.
       */
      LIBZIM_API Item getItem(bool follow=false) const;

      /** Get the item associated to the target entry.
       *
       * If there is a chain of redirection, the whole chain is resolved
       * and the item associted to the last entry is returned.
       *
       * @return the Item associated with the targeted entry.
       * @exception InvalidType if the entry is not a redirection.
       */
      LIBZIM_API Item getRedirect() const;

      /** Get the Entry targeted by the entry.
       *
       * @return The entry directly targeted by this redirect entry.
       * @exception InvalidEntry if the entry is not a redirection.
       */
      LIBZIM_API Entry getRedirectEntry() const;

      /** Get the index of the Entry targeted by the entry.
       *
       * @return The index of the entry directly targeted by this redirect
       *         entry.
       * @exception InvalidEntry if the entry is not a redirection.
       */
      LIBZIM_API entry_index_type getRedirectEntryIndex() const;

      LIBZIM_API entry_index_type getIndex() const   { return m_idx; }

    private:
      std::shared_ptr<FileImpl> m_file;
      entry_index_type m_idx;
      std::shared_ptr<const Dirent> m_dirent;
  };

}

#endif // ZIM_ENTRY_H

