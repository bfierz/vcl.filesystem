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
#include "volumefilereader.h"

// Windows
#include <windows.h>

namespace Vcl { namespace FileSystem
{
	uint64_t GetFileSize(std::wstring const &path)
	{ 
		WIN32_FIND_DATAW data;
		HANDLE h = FindFirstFileW(path.c_str(), &data);
		if (h == INVALID_HANDLE_VALUE)
			return -1;

		FindClose(h);

		return data.nFileSizeLow | (__int64) data.nFileSizeHigh << 32;
	}

	VolumeFileReader::VolumeFileReader(path virtual_path, path volume_path)
	: FileReader(virtual_path)
	, _volumePath(std::move(volume_path))
	{
		_size = GetFileSize(_volumePath.wstring());

		_file.open(_volumePath.c_str(), std::ios::binary);
	}

	void VolumeFileReader::seek(const uint64_t pos)
	{
		_file.seekg(pos);
		_curr_pos = pos;
	}

	uint64_t VolumeFileReader::read(void* buf, const uint64_t buffer_size)
	{
		auto left_to_read = size() - pos();
		auto read_bytes = std::min<uint64_t>(buffer_size, left_to_read);
		_file.read(static_cast<char*>(buf), read_bytes);

		_curr_pos += read_bytes;
		return read_bytes;
	}

	bool VolumeFileReader::eof() const
	{
		return _file.eof();
	}

	uint64_t VolumeFileReader::size() const
	{
		return _size;
	}

	uint64_t VolumeFileReader::pos() const
	{
		return _curr_pos;
	}
}}
