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
#include "volumemountpoint.h"

 // VCL File System Library
#include "../readers/volumefilereader.h"

namespace Vcl { namespace FileSystem
{
	VolumeMountPoint::VolumeMountPoint(std::string name, path mount_path, path volume_path)
	: MountPoint{ std::move(name), std::move(mount_path) }
	, _volumePath{ volume_path }
	{
	}

	std::shared_ptr<FileReader> VolumeMountPoint::createReader(const path& file_name)
	{
		return std::make_shared<VolumeFileReader>(file_name, convertToVolumePath(file_name));
	}

	bool VolumeMountPoint::exists(const path& entry) const
	{
		// Check if the file exists on disk
		return std::experimental::filesystem::exists(convertToVolumePath(entry));
	}

	VolumeMountPoint::path VolumeMountPoint::convertToVolumePath(const path& virtual_path) const
	{
		// Remove the mount path from the entry
		path rel_path = virtual_path.string().substr(mountPath().string().length());

		// Append it to the volume path
		auto abs_path = _volumePath / rel_path;

		return std::move(abs_path);
	}
}}
