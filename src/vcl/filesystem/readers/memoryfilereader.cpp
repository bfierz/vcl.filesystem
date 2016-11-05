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
#include "memoryfilereader.h"

namespace Vcl { namespace FileSystem
{
	MemoryFileReader::MemoryFileReader(path virtual_path, std::shared_ptr<Util::MemoryFile> file)
	: FileReader(virtual_path)
	, _file(std::move(file))
	{
	}

	void MemoryFileReader::seek(const uint64_t pos)
	{
		_curr_pos = pos;
	}

	uint64_t MemoryFileReader::read(void* buf, const uint64_t buffer_size)
	{
		auto bytes_read = _file->read(_curr_pos, buf, buffer_size);
		_curr_pos += bytes_read;

		return bytes_read;
	}

	bool MemoryFileReader::eof() const
	{
		return _curr_pos >= _size;
	}

	uint64_t MemoryFileReader::size() const
	{
		return _size;
	}

	uint64_t MemoryFileReader::pos() const
	{
		return _curr_pos;
	}
}}
