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
#include "memoryfilewriter.h"

namespace Vcl { namespace FileSystem
{
	MemoryFileWriter::MemoryFileWriter(path virtual_path, std::shared_ptr<Util::MemoryFile> file)
	: FileWriter(virtual_path)
	, _file(std::move(file))
	{
	}

	void MemoryFileWriter::seek(const uint64_t pos)
	{
		_curr_pos = pos;
	}

	void MemoryFileWriter::write(void* buf, const uint64_t buffer_size)
	{
		_file->write(_curr_pos, buf, buffer_size);
		_curr_pos += buffer_size;
	}

	uint64_t MemoryFileWriter::pos() const
	{
		return _curr_pos;
	}
}}
