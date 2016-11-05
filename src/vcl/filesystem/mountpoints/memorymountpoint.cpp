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
#include "memorymountpoint.h"

 // VCL File System Library
#include "../readers/memoryfilereader.h"
#include "../writers/memoryfilewriter.h"

namespace Vcl { namespace FileSystem
{
	MemoryMountPoint::MemoryMountPoint(std::string name, path mount_path)
	: MountPoint(std::move(name), std::move(mount_path))
	{

	}

	std::shared_ptr<FileReader> MemoryMountPoint::createReader(const path& entry)
	{
		auto file = findMemoryFile(entry);
		if (file)
		{
			return std::make_shared<MemoryFileReader>(file->relativePath(), std::move(file));
		}
		else
		{
			return{};
		}
	}
	std::shared_ptr<FileWriter> MemoryMountPoint::createWriter(const path& entry)
	{
		auto file = findMemoryFile(entry);
		if (file)
		{
			return std::make_shared<MemoryFileWriter>(file->relativePath(), std::move(file));
		}
		else
		{
			path rel_path = relativePath(entry);
			_files.emplace_back(std::make_shared<Util::MemoryFile>(rel_path));
			return std::make_shared<MemoryFileWriter>(rel_path, _files.back());
		}
	}
	bool MemoryMountPoint::exists(const path& entry) const
	{
		return static_cast<bool>(findMemoryFile(entry));
	}

	std::shared_ptr<Util::MemoryFile> MemoryMountPoint::findMemoryFile(const path& filename) const
	{
		// Remove the mount path from the entry
		path rel_path = relativePath(filename);

		auto file_it = std::find_if(_files.begin(), _files.end(), [&rel_path](const std::shared_ptr<Util::MemoryFile>& file)
		{
			return file->relativePath() == rel_path;
		});

		if (file_it != _files.end())
			return *file_it;
		else
			return{};
	}
}}

