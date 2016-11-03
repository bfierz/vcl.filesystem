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
#include <memory>
#include <unordered_map>

// Forward declaration
class ZipArchive;
class ZipArchiveEntry;

namespace std
{
	template<>
	struct hash<std::experimental::filesystem::path>
	{
		size_t operator()(const std::experimental::filesystem::path& x) const
		{
			return std::experimental::filesystem::hash_value(x);
		}
	};
}

namespace Vcl { namespace FileSystem { namespace Util
{
	class ArchivePathIterator : public std::unordered_map<std::experimental::filesystem::path, std::shared_ptr<ZipArchiveEntry>>::const_iterator
	{
		using path = std::experimental::filesystem::path;
		using ArchiveIterator = std::unordered_map<path, std::shared_ptr<ZipArchiveEntry>>::const_iterator;

	public:
		ArchivePathIterator(ArchiveIterator s) : ArchiveIterator{ s } {};
		path* const operator->() { return (path* const)&(ArchiveIterator::operator->()->first); }
		const path operator*() { return ArchiveIterator::operator*().first; }
	};

	class Archive
	{
	protected:
		using path = std::experimental::filesystem::path;

	public:
		Archive(path zip_file);

		/*!
		 *	\brief Check if file exists in the archive
		 *	\param entry Entry to be checked
		 *	\returns true if 'entry' is found in the archive
		 */
		bool entryExists(const path& entry) const;

		ArchivePathIterator beginPaths() const { return{ _entries.cbegin() }; }
		ArchivePathIterator endPaths() const { return{ _entries.cend() }; }

	private:
		void enumerateFiles();

	private:
		//! Path to the on volume archive
		path _file;

		//! Implementation of archive
		std::shared_ptr<ZipArchive> _archive;

		//! Cache of files in the archive
		std::unordered_map<path, std::shared_ptr<ZipArchiveEntry>> _entries;
	};
}}}
