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

// C++ Standard Library
#include <filesystem>
#include <list>
#include <memory>

namespace Vcl { namespace FileSystem { namespace Util
{
	struct MemoryPage
	{
		uint8_t Memory[512];
	};

	class MemoryFile
	{
	protected:
		using path = std::experimental::filesystem::path;
		
	public:
		MemoryFile(path rel_path);

		size_t read(size_t offset, void* buffer, size_t size);
		void write(size_t offset, void* buffer, size_t size);

	public: // Properties

		//! \returns the path of the file relative to the mount point
		const path& relativePath() const { return _relPath; }

	private:
		//! Relative path of the memory file
		path _relPath;

		//! Size of the content
		size_t _size{ 0 };

		//! List of memory blobs making the memory file
		std::vector<std::unique_ptr<MemoryPage>> _pages;
	};
}}}
