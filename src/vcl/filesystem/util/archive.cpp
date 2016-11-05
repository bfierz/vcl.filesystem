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
#include "archive.h"

// ZipLib
#include <ZipLib/ZipFile.h>

namespace Vcl { namespace FileSystem { namespace Util
{
	Archive::Archive(path zip_file)
	: _file{ zip_file }
	{
		_archive = ZipFile::Open(_file.string());

		if (_archive)
			enumerateFiles();
	}

	bool Archive::entryExists(const path& entry) const 
	{
		std::string str;
		if (entry.has_root_directory())
			str = entry.string().substr(1);
		else
			str = entry.string();

		return _entries.find(str) != _entries.end();
	}

	std::shared_ptr<ZipArchiveEntry> Archive::entry(const path& entry) const
	{
		std::string str;
		if (entry.has_root_directory())
			str = entry.string().substr(1);
		else
			str = entry.string();

		return _entries.find(str)->second;
	}

	void Archive::enumerateFiles()
	{
		// Adds all the entries to an internal list
		auto nr_entries = _archive->GetEntriesCount();
		for (decltype(nr_entries) e = 0; e < nr_entries; ++e)
		{
			auto entry = _archive->GetEntry(static_cast<int>(e));
			_entries.emplace(entry->GetFullName(), entry);
		}
	}
}}}
