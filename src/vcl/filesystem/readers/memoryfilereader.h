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
#pragma once

// VCL configuration
#include <vcl/config/global.h>

// C++ standard library
#include <fstream>

// VCL File System Library
#include "../util/memoryfile.h"
#include "../filereader.h"

namespace Vcl { namespace FileSystem
{
	class MemoryFileReader : public FileReader
	{
	public:
		MemoryFileReader(path virtual_path, std::shared_ptr<Util::MemoryFile> file);

		void     seek(const uint64_t pos) override;
		uint64_t read(void* buf, const uint64_t size) override;

		bool     eof() const override;
		uint64_t size() const override;
		uint64_t pos() const override;

	private:
		//! Memory file resource
		std::shared_ptr<Util::MemoryFile> _file;

		//! Size of the entire file
		uint64_t _size{ 0 };

		//! Current position in the file buffer
		uint64_t _curr_pos{ 0 };
	};
}}
