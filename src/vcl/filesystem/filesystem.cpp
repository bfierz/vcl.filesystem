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
#include "filesystem.h"

// C++ Standard Library
#include <numeric>

namespace Vcl { namespace FileSystem
{
	void FileSystem::addMountPoint(path prefix, std::unique_ptr<MountPoint> mp)
	{
		// Store the mount point
		_mountPoints.emplace_back(std::move(mp));
	}

	bool FileSystem::exists(const path& entry)
	{
		auto mp = findMountPoint(entry);
		if (mp)
		{
			return mp->exists(entry);
		}
		else
		{
			return false;
		}
	}

	const MountPoint* FileSystem::findMountPoint(path entry)
	{
		// Only consider the directory of the path to search for the mount point
		entry.remove_filename();

		const MountPoint* init{ nullptr };
		auto mp = std::accumulate(_mountPoints.begin(), _mountPoints.end(), init, [&entry](const MountPoint* a, const std::unique_ptr<MountPoint>& b) -> const MountPoint*
		{
			if ((!a || a->mountPath().string().length() < b->mountPath().string().length()) && 
			    b->mountPath().compare(entry) < 0)
				return b.get();
			else
				return a;
		});

		return mp;
	}
}}
