/*
 * This file is part of the Visual Computing Library (VCL) release under the
 * MIT license.
 *
 * Copyright (c) 2014-2016 Basil Fierz
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "memoryfile.h"

// C++ standard library
#include <algorithm>

namespace Vcl { namespace FileSystem { namespace Util
{
	MemoryFile::MemoryFile(path rel_path)
	: _relPath(rel_path)
	{
	}

	size_t MemoryFile::read(size_t offset, void* buffer, size_t size)
	{
		// Check validity of offset
		if (offset >= _size)
			return 0;

		// Determine the start page
		auto page_idx = offset / size;
		auto page_offset = offset - size * page_idx;

		size_t read_bytes = 0;
		while (read_bytes < size && page_idx < _pages.size())
		{
			// Bytes to read in this page
			size_t bytes_to_read = sizeof(MemoryPage::Memory) - page_offset;
			bytes_to_read = std::min(bytes_to_read, size - read_bytes);

			// Copy the content
			memcpy(static_cast<uint8_t*>(buffer) + read_bytes, _pages[page_idx]->Memory + page_offset, bytes_to_read);

			read_bytes += bytes_to_read;
			page_offset = 0;
			page_idx++;
		}

		return read_bytes;
	}

	void MemoryFile::write(size_t offset, void* buffer, size_t size)
	{
		// Allocate additional pages if necessary
		if (offset + size >= _size)
		{
			_size = offset + size;

			auto nr_pages = (_size + (sizeof(MemoryPage::Memory) - 1)) / sizeof(MemoryPage::Memory);
			if (nr_pages > _pages.size())
			{
				// Current page count
				auto cnt = _pages.size();
				_pages.resize(nr_pages);

				std::generate(_pages.begin() + cnt, _pages.end(), []()
				{
					return std::make_unique<MemoryPage>();
				});
			}
		}

		// Determine the start page
		auto page_idx = offset / size;
		auto page_offset = offset - size * page_idx;

		size_t written_bytes = 0;
		while (written_bytes < size)
		{
			// Bytes to read in this page
			size_t bytes_to_write = sizeof(MemoryPage::Memory) - page_offset;
			bytes_to_write = std::min(bytes_to_write, size - written_bytes);

			// Copy the content
			memcpy(_pages[page_idx]->Memory + page_offset, static_cast<uint8_t*>(buffer) + written_bytes, bytes_to_write);

			written_bytes += bytes_to_write;
			page_offset = 0;
			page_idx++;
		}
	}
}}}
