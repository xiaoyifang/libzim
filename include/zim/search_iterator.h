/*
 * Copyright (C) 2021 Maneesh P M <manu.pm55@gmail.com>
 * Copyright (C) 2020 Matthieu Gautier <mgautier@kymeria.fr>
 * Copyright (C) 2006 Tommi Maekitalo
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

#ifndef ZIM_SEARCH_ITERATOR_H
#define ZIM_SEARCH_ITERATOR_H

#include <memory>
#include <iterator>
#include "entry.h"
#include "archive.h"
#include "uuid.h"

namespace zim
{
class SearchResultSet;

class SearchIterator : public std::iterator<std::bidirectional_iterator_tag, Entry>
{
    friend class zim::SearchResultSet;
    public:
        LIBZIM_API SearchIterator();
        LIBZIM_API SearchIterator(const SearchIterator& it);
        LIBZIM_API SearchIterator& operator=(const SearchIterator& it);
        LIBZIM_API SearchIterator(SearchIterator&& it);
        LIBZIM_API SearchIterator& operator=(SearchIterator&& it);
        LIBZIM_API ~SearchIterator();

        LIBZIM_API bool operator== (const SearchIterator& it) const;
        LIBZIM_API bool operator!= (const SearchIterator& it) const;

        LIBZIM_API SearchIterator& operator++();
        LIBZIM_API SearchIterator operator++(int);
        LIBZIM_API SearchIterator& operator--();
        LIBZIM_API SearchIterator operator--(int);

        LIBZIM_API std::string getPath() const;
        LIBZIM_API std::string getTitle() const;
        LIBZIM_API int getScore() const;
        LIBZIM_API std::string getSnippet() const;
        LIBZIM_API int getWordCount() const;
        LIBZIM_API DEPRECATED int getSize() const;
        LIBZIM_API int getFileIndex() const;
        LIBZIM_API Uuid getZimId() const;
        LIBZIM_API reference operator*() const;
        LIBZIM_API pointer operator->() const;

#ifdef ZIM_PRIVATE
        LIBZIM_API std::string getDbData() const;
#endif

    private:
        struct InternalData;
        std::unique_ptr<InternalData> internal;
        SearchIterator(InternalData* internal_data);

        bool isEnd() const;
};

} // namespace zim

#endif // ZIM_SEARCH_ITERATOR_H
